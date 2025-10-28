#include <stdint.h>
#include "esp32-hal.h"
#include "esp32-hal-gpio.h"
#include "FS_Button.h"
#include "config.h"

#include <hbli_Logging.h>

#define FS_BUTTON_NUM_BUTTONS 2
#define MAX_TIME_LOOP2LOOP 10
#define TIME_SINGLE_PRESS 200  //[ms]
#define TIME_LONG_PRESS 500
#define TIME_RELEASE_ACTION 400
// #define TIME_HOLD 1000

// #define LONG_PRESS_MAGIC 69

static const char* tag = "Button";
SemaphoreHandle_t xButtonMutex;
Button_t btnHead;
Button_t btnProg;
Button_t* buttons[FS_BUTTON_NUM_BUTTONS] = { &btnHead, &btnProg };

void ISR_Button_Head();
void ISR_Button_Prog();
void buttonLoop();
void buttonActionLoop();

void LockButtons() {
  xSemaphoreTake(xButtonMutex, portMAX_DELAY);
}

void ReleaseButtons() {
  xSemaphoreGive(xButtonMutex);
}

uint32_t getButtonDuration(Button_t* btn) {
  uint32_t res;
  LockButtons();
  res = btn->duration;
  ReleaseButtons();
  return res;
}

uint8_t getButtonAction(Button_t* btn) {
  uint8_t res;
  LockButtons();
  res = btn->action;
  ReleaseButtons();
  return res;
}

bool isButtonDurationBiggerThan(Button_t* btn, uint32_t time) {
  bool res = false;
  LockButtons();
  if (btn->duration >= time) {
    res = true;
    btn->duration = 0;
  }
  ReleaseButtons();
  return res;
}

bool isButtonAction(Button_t* btn, uint8_t act) {
  if (act == MAX_PRESSES || act >= TOTAL_ACTIONS) {
    LOGE(tag, F("invalid input"));
    return false;
  }
  bool res;
  LockButtons();
  btn->action == act ? res = true : res = false;
  ReleaseButtons();
  return res;
}

//resets action if true
bool isButtonActionR(Button_t* btn, uint8_t act) {
  if (act == MAX_PRESSES || act >= TOTAL_ACTIONS) {
    LOGE(tag, F("invalid input"));
    return false;
  }
  bool res = false;
  LockButtons();
  if (btn->action == act) {
    LOGI(tag, "action is", act);
    res = true;
    btn->action = NO_ACTION;
  }
  ReleaseButtons();
  return res;
}

void ButtonTask(void* pvParameters) {
  while (1) {
    LockButtons();
    buttonLoop();
    buttonActionLoop();
    ReleaseButtons();
    vTaskDelay(25 / portTICK_PERIOD_MS);
  }
}

void initButtons() {
  btnHead.pin = FS_PIN_BTN_HEAD;
  btnHead.defaultValue = false;
  btnHead.name = "Head";

  btnProg.pin = FS_PIN_BTN_PROG;
  btnProg.defaultValue = true;
  btnProg.name = "Prog";

  pinMode(btnHead.pin, INPUT);
  pinMode(btnProg.pin, INPUT);

  attachInterrupt(digitalPinToInterrupt(btnHead.pin), ISR_Button_Head, CHANGE);
  attachInterrupt(digitalPinToInterrupt(btnProg.pin), ISR_Button_Prog, CHANGE);

  xButtonMutex = xSemaphoreCreateMutex();

  xTaskCreate(ButtonTask, "Button-task", 1024, NULL, 1, NULL);
}

void buttonLoop() {
  for (int i = 0; i < FS_BUTTON_NUM_BUTTONS; i++) {
    Button_t* btn = buttons[i];

    if (btn->duration != 0) continue;

    if (btn->isrFlag) {                         //only if isrFlag is set
      btn->state = digitalRead(btn->pin);       //read current state
      if (btn->state != btn->stateOld) {        //only do if state changed
        btn->stateOld = btn->state;             //set old state
        if (btn->state != btn->defaultValue) {  //check if pressed
          btn->timer = millis();                //get current time
          // LOGI(tag, btn->name, F("pressed"));                   //indicate in console
          btn->durationRelease = millis() - btn->timerRelease;  //calculate releaseTimer
        } else {                                                //
          btn->duration = millis() - btn->timer;                //calculate duration
          // LOGI(tag, btn->name, F("released"));                  //indicate in console
          btn->timerRelease = millis();  //get current time for release timer
        }                                //
      }                                  //
      btn->isrFlag = false;              //reset isrFlag
    }
  }
}

void buttonActionLoop() {
  for (int i = 0; i < FS_BUTTON_NUM_BUTTONS; i++) {
    Button_t* btn = buttons[i];
    uint32_t time = millis();

    if (btn->action != NO_ACTION) continue;


    if (btn->state == btn->defaultValue) {
      //button is currently released

      //if button was pressed less than TIME_SINGLE_PRESS and the press happend less that MAX_TIME_LOOP2LOOP ago
      if ((btn->duration <= TIME_SINGLE_PRESS) && (time - btn->timerRelease < MAX_TIME_LOOP2LOOP)) {
        if (btn->counterAction < (MAX_PRESSES - 1)) {
          btn->counterAction++;
          btn->timerAction = time;
        }
      } else if (btn->duration >= TIME_LONG_PRESS) {
        btn->action = LONG_PRESS;
      }
    }
    // LOGI(tag, btn->name, btn->action);
    if ((time - btn->timerAction >= TIME_RELEASE_ACTION) && (btn->action == NO_ACTION)) {
      btn->action = btn->counterAction;
      btn->counterAction = 0;
    }
  }
}

void resetDuration(Button_t* btn) {
  LockButtons();
  btn->duration = 0;
  ReleaseButtons();
}

void resetAction(Button_t* btn) {
  LockButtons();
  btn->action = 0;
  ReleaseButtons();
}

void resetButtons() {
  for (int i = 0; i < FS_BUTTON_NUM_BUTTONS; i++) {
    Button_t* btn = buttons[i];
    resetAction(btn);
    resetDuration(btn);
  }
}

void ISR_Button_Head() {
  uint32_t currentTime = millis();
  if (currentTime - btnHead.istTimeLast >= btnHead.debounceTime) {
    btnHead.istTimeLast = currentTime;
    btnHead.isrFlag = true;
  }
}

void ISR_Button_Prog() {
  uint32_t currentTime = millis();
  if (currentTime - btnProg.istTimeLast >= btnProg.debounceTime) {
    btnProg.istTimeLast = currentTime;
    btnProg.isrFlag = true;
  }
}
