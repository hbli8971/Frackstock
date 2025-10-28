#include "FS_Led.h"
#include "freertos/projdefs.h"
#include "esp32-hal.h"
#include <sys/_stdint.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "freertos/event_groups.h"
#include <Adafruit_NeoPixel.h>
#include <hbli_Logging.h>

#define FS_EVT_LED_POWERMODE (1 << 0)
#define FS_EVT_NEW_DATA (1 << 1)
#define FS_EVT_LED_DONE (1 << 2)


#define STEP_MAX 200
#define STEP_HALVE (STEP_MAX / 2)


#if (LOGIC_INVERSION == true)
#define FS_NUM_LED (FS_LED_COUNT + 1)
#else
#define FS_NUM_LED FS_LED_COUNT
#endif


Adafruit_NeoPixel strip = Adafruit_NeoPixel(FS_NUM_LED, FS_PIN_WS2812, NEO_GRB + NEO_KHZ800);

static const char* tag = "LED-Task";

EventGroupHandle_t xLedEventGroup;
SemaphoreHandle_t xLedMutex;

static volatile ledInfo_t ledDataBuffer;
static volatile bool _flagClearAfterwards;


void FS_LED_LOCK_DATA_BUFFER() {
  xSemaphoreTake(xLedMutex, portMAX_DELAY);
}

void FS_LED_RELEASE_DATA_BUFFER() {
  xSemaphoreGive(xLedMutex);
}

void FS_LED_SET_LED_DONE() {
  xEventGroupSetBits(xLedEventGroup, FS_EVT_LED_DONE);
}

void FS_LED_CLEAR_LED_DONE() {
  xEventGroupClearBitsFromISR(xLedEventGroup, FS_EVT_LED_DONE);
}

bool FS_GET_LED_DONE_FLAG() {
  EventBits_t uxBitsTemp = xEventGroupWaitBits(xLedEventGroup, FS_EVT_LED_DONE, pdFALSE, pdFALSE, 0);
  return (uxBitsTemp & FS_EVT_LED_DONE) ? true : false;
}

void FS_LED_OFF() {
  xEventGroupClearBits(xLedEventGroup, FS_EVT_LED_POWERMODE);
}

void FS_LED_NORMAL_MODE() {
  xEventGroupSetBits(xLedEventGroup, FS_EVT_LED_POWERMODE);
}




void FS_SetLed(uint32_t color1, uint32_t color2, uint32_t duration, ledModes_t mode, int16_t repetitions, uint16_t misc) {
  FS_LED_LOCK_DATA_BUFFER();

  ledDataBuffer.color1 = color1;
  ledDataBuffer.color2 = color2;
  ledDataBuffer.duration = duration;
  ledDataBuffer.mode = mode;
  ledDataBuffer.repetitions = repetitions;
  ledDataBuffer.misc = misc;
  FS_LED_RELEASE_DATA_BUFFER();
  xEventGroupSetBits(xLedEventGroup, FS_EVT_NEW_DATA);
}

void FS_SetLed(ledInfo_t* led) {
  FS_LED_LOCK_DATA_BUFFER();
  ledDataBuffer.color1 = led->color1;
  ledDataBuffer.color2 = led->color2;
  ledDataBuffer.duration = led->duration;
  ledDataBuffer.mode = led->mode;
  ledDataBuffer.repetitions = led->repetitions;
  ledDataBuffer.misc = led->misc;
  FS_LED_RELEASE_DATA_BUFFER();
  xEventGroupSetBits(xLedEventGroup, FS_EVT_NEW_DATA);
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void set_all_led(uint32_t color) {
  for (int i = 0; i < FS_NUM_LED; i++) {
    strip.setPixelColor(i, color);
  }
  strip.show();
}

void breathe(uint32_t color_start, uint32_t color_end, uint8_t step) {

  if (step > 100) step = 100;

  // Extract the RGB components from the starting color
  uint8_t start_r = (color_start >> 16) & 0xFF;
  uint8_t start_g = (color_start >> 8) & 0xFF;
  uint8_t start_b = color_start & 0xFF;
  // Extract the RGB components from the ending color
  uint8_t end_r = (color_end >> 16) & 0xFF;
  uint8_t end_g = (color_end >> 8) & 0xFF;
  uint8_t end_b = color_end & 0xFF;
  // Calculate the interpolated RGB components
  uint8_t new_r = start_r + (end_r - start_r) * step / 100;
  uint8_t new_g = start_g + (end_g - start_g) * step / 100;
  uint8_t new_b = start_b + (end_b - start_b) * step / 100;

  // Combine the new RGB components into a single color
  uint32_t new_color = (new_r << 16) | (new_g << 8) | new_b;

  set_all_led(new_color);
}

void breathe_animation(uint32_t color, uint8_t step) {
  if (step > STEP_HALVE) {
    breathe(color, COLOR_BLACK, step - STEP_HALVE);
  } else {
    breathe(COLOR_BLACK, color, step);
  }
}

void breathe_animation_extended(uint32_t color1, uint32_t color2, uint8_t step) {
  if (step > STEP_HALVE) {
    breathe(color2, color1, step - STEP_HALVE);
  } else {
    breathe(color1, color2, step);
  }
}

void initLED() {
  xLedEventGroup = xEventGroupCreate();
  xLedMutex = xSemaphoreCreateMutex();
  strip.begin();
  set_all_led(COLOR_BLACK);
  FS_LED_NORMAL_MODE();
  xTaskCreate(LedTask, "LED-task", 4096, NULL, 1, NULL);
}

void LedTask(void* pvParameters) {
  uint32_t _color1 = COLOR_BLACK;
  uint32_t _color2 = COLOR_BLACK;
  uint32_t _duration = 0;
  ledModes_t _mode = ledModeNone;
  int16_t _repetitions = 0;
  uint16_t _misc = 0;
  uint32_t _timerLedTaskStart = 0;
  uint32_t _timerLedTaskInternal = 0;
  uint8_t _step;
  bool _flagShowLed;


  while (1) {

    //get time
    _timerLedTaskInternal = millis();

    //get eventGroup
    EventBits_t uxBits = xEventGroupWaitBits(xLedEventGroup, FS_EVT_LED_POWERMODE | FS_EVT_NEW_DATA, pdFALSE, pdFALSE, 0);

    if (uxBits & FS_EVT_LED_POWERMODE) {

      // read new LED-Data if available
      if (uxBits & FS_EVT_NEW_DATA) {
        FS_LED_LOCK_DATA_BUFFER();                              //lock buffer
        _color1 = ledDataBuffer.color1;                         //read data
        _color2 = ledDataBuffer.color2;                         //read data
        _duration = ledDataBuffer.duration;                     //read data
        _mode = ledDataBuffer.mode;                             //read data
        _repetitions = ledDataBuffer.repetitions;               //read data
        _misc = ledDataBuffer.misc;                             //read data
        xEventGroupClearBits(xLedEventGroup, FS_EVT_NEW_DATA);  //clear data-rdy flag
        FS_LED_RELEASE_DATA_BUFFER();                           //release buffer
        _timerLedTaskStart = _timerLedTaskInternal;
        _step = 0;
      }

      if (_repetitions != 0) {
        if (_timerLedTaskInternal - _timerLedTaskStart > _duration) {
          _timerLedTaskStart = _timerLedTaskInternal;
          if (_repetitions > 0) {
            _repetitions--;
          }
          if (_repetitions == 0) {
            _flagShowLed = false;
            FS_LED_SET_LED_DONE();
            FS_LED_LOCK_DATA_BUFFER();
            if ((_misc & FS_LED_NO_CLEAR) == 0) {
              set_all_led(COLOR_BLACK);
            }
            FS_LED_RELEASE_DATA_BUFFER();
          }
        } else {
          _step = (_timerLedTaskInternal - _timerLedTaskStart) / (_duration / STEP_MAX);
          switch (_mode) {
            case ledModeNone:
              break;
            case ledModeStatic:
              set_all_led(_color1);
              break;
            case ledModeBreathe:
              breathe_animation(_color1, _step);
              break;
            case ledModeBreatheExtended:
              breathe_animation_extended(_color1, _color2, _step);
              break;
            case ledModeToggle:
              if (_step >= STEP_HALVE) {
                set_all_led(_color2);
              } else {
                set_all_led(_color1);
              }
              break;
            case ledModeRotatingPoint:
              for (int i = 0; i < FS_NUM_LED; i++) {
                if ((_step / FS_LED_COUNT) == i) {
                  strip.setPixelColor(i, _color1);
                } else {
                  strip.setPixelColor(i, _color2);
                }
              }
              break;
            default:
              LOGE(tag, F("sw-df"), F("ERROR: 897"));
              break;
          }
        }
      }
      vTaskDelay(50 / portTICK_PERIOD_MS);
    } else {
      vTaskDelay(500 / portTICK_PERIOD_MS);  //give more time to other tasks if led's unused
    }
  }
}