#include <stdint.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "freertos/event_groups.h"

#include <DPOS.h>
#include <Wire.h>
#include <EEPROM.h>
#include <ArduinoJson.h>

#include <hbli_EMM.h>
#include <hbli_Logging.h>

#include "FS_MenuDefinition.h"
#include "FS_DPOS_Definitions.h"
#include "FS_Led.h"
#include "FS_memCtrl.h"
#include "FS_Flasher.h"
#include "FS_TapDetection.h"
#include "FS_Network.h"
#include "FS_adc.h"
#include "FS_Button.h"
#include "FS_Datatypes.h"

#include "config.h"
#include "wifi_credentials.h"

static const char* tag = "main";

network_t netInitData;
wifiInfo_t net;
TapDetect_t tapDetect;
ledInfo_t led;
SystemInfo_t sys;
MemInfo_t mem;
Lis3dh_t lisCfg;

void setup() {
  Serial.begin(SERIAL_BAUD_RATE);
  Serial.setRxBufferSize(RX_BUFFER_SIZE);
  Serial.setTxBufferSize(TX_BUFFER_SIZE);
  Wire.begin(FS_PIN_I2C_SDA, FS_PIN_I2C_SCL);

  initButtons();

  digitalWrite(FS_PIN_LED1, LOW);
  digitalWrite(FS_PIN_LED2, LOW);

  initEEPROM();

  initMenuStructure();
  DPOS_init();
  init_DPOS_elements();
  initLED();

  sys.flag = 0;
  delay(2000);

  /*  *** FLASHING PROCESS ***  */

  if (WC_FORCE_WRITE_EEPROM) {
    uint8_t res;
    res += MemSetIdentifier("spulejan");
    res += MemSetFirstName("Jan");
    res += MemSetLastName("Spuler");
    res += MemSetSSID(WIFI_SSID);
    res += MemSetPassword(WIFI_PASSWORD);
    res += MemSetColorBeer(COLOR_DARK_YELLOW);
    res += MemSetColorBeerAdd(COLOR_YELLOW);
    res += MemSetColorPing(COLOR_ORANGE);
    res += MemSetBatCalib(BAT_CALIB_DEFAULT);
    res += MemSetDeviceMode(1);
    res += MemSetTapScale(1);
    res += MemSetTapThreshhold(20);
    if (res >= 0) {
      LOGE(tag, F("force-write to eeprom"), F("Failed at some point"));
    }
  }

  /*  GET DATA FROM EEPROM  */
  if (WC_ENGINEERING_MODE) {
    mem.Identifier = "spulejan";
    mem.FirstName = "Jan";
    mem.LastName = "Spuler";
    mem.wifiCred.SSID = WIFI_SSID;
    mem.wifiCred.Password = WIFI_PASSWORD;
    mem.ColorBeer = COLOR_BROWN;
    mem.ColorBeerIncrement = COLOR_CYAN;
    mem.ColorPing = COLOR_ORANGE;
    mem.BatCalib = BAT_CALIB_DEFAULT;
    mem.DeviceVersion = SW_VERSION;
    mem.DeviceMode = 1;
    mem.TapRange = 1;
    mem.TapThresh = 20;
    mem.TapTimeLim = 10;
    mem.TapTimeLatency = 20;
    mem.TapWindow = 255;
    lis3dh_loadDefaultConfig(&lisCfg);
  } else if (WC_CHECK_EEPROM) {
    if (MemGetMemCheck()) {
      //a pass means: no new SW-Version detected, and Memory-check-Sequence is correct
      //procceed to read data from memory

      LOGI(tag, "mem check completed");

      /* READ EEPROM TO SET MemInfo_t*/
      mem.Identifier = MemGetIdentifier();
      mem.FirstName = MemGetFirstName();
      mem.LastName = MemGetLastName();
      if (WC_FORCE_USE_DEFAULT_CRED) {
        mem.wifiCred.SSID = WIFI_SSID;
        mem.wifiCred.Password = WIFI_PASSWORD;
      } else {
        mem.wifiCred.SSID = MemGetSSID();
        mem.wifiCred.Password = MemGetPassword();
      }
      mem.ColorBeer = MemGetColorBeer();
      mem.ColorBeerIncrement = MemGetColorBeerAdd();
      mem.ColorPing = MemGetColorPing();
      mem.BatCalib = MemGetBatCalib();
      mem.DeviceVersion = MemGetVersion();
      mem.DeviceMode = MemGetDeviceMode();
      mem.TapRange = MemGetTapRange();
      mem.TapThresh = MemGetTapThreshhold();
      mem.TapTimeLim = MemGetTapTimeLim();
      mem.TapTimeLatency = MemGetTapTimeLatency();
      mem.TapWindow = MemGetTapWindow();
      lisCfg.range = mem.TapRange;
      lisCfg.threshhold = mem.TapThresh;
      lisCfg.timeLim = mem.TapTimeLim;
      lisCfg.timeLatency = mem.TapTimeLatency;
      lisCfg.window = mem.TapWindow;


      /* READ EEPROM TO SET MemInfo_t*/
    } else {
      //memory check failed:
      //failed means: either or both the SW-Version or Memory-check-Sequence are incorrect

      // initiate the Flashing process:
      FSF_PerformFlashingRoutine();
    }
  }

  /*  PRINT DATA FROM EEPROM  */
  if (WC_PRINT_MEM_DATA) {
    LOGI(tag, F("Identifier"), mem.Identifier);
    LOGI(tag, F("FirstName"), mem.FirstName);
    LOGI(tag, F("LastName"), mem.LastName);
    LOGI(tag, F("wifiCred.SSID"), mem.wifiCred.SSID);
    if (WC_SHOW_PW) {
      LOGI(tag, F("wifiCred.Password"), mem.wifiCred.Password);
    } else {
      LOGI(tag, F("wifiCred.Password"), F("**HIDDEN**"));
    }
    LOGI(tag, F("ColorBeer"), String(mem.ColorBeer, HEX));
    LOGI(tag, F("ColorBeerIncrement"), String(mem.ColorBeerIncrement, HEX));
    LOGI(tag, F("ColorPing"), String(mem.ColorPing, HEX));
    LOGI(tag, F("BatCalib"), mem.BatCalib);
    LOGI(tag, F("DeviceVersion"), mem.DeviceVersion);
    LOGI(tag, F("DeviceMode"), mem.DeviceMode);
    LOGI(tag, F("Tap-Range"), mem.TapRange);
    LOGI(tag, F("Tap-Threshhold"), mem.TapThresh);
    LOGI(tag, F("Tap-TimeLimit"), mem.TapTimeLim);
    LOGI(tag, F("Tap-TimeLatency"), mem.TapTimeLatency);
    LOGI(tag, F("Tap-Window"), mem.TapWindow);
  }
  /*  *** FLASHING PROCESS ***  */

  initTapDetection(&lisCfg);

  netInitData.ssid = mem.wifiCred.SSID.c_str();
  netInitData.pw = mem.wifiCred.Password.c_str();
  netInitData.mqtt_userName = MQTT_USER_NAME;
  netInitData.mqtt_pw = MQTT_PASSWORD;
  netInitData.mqtt_ip = MQTT_IP_ADDR;
  netInitData.mqtt_identifier = mem.Identifier.c_str();
  netInitData.mqtt_port = MQTT_PORT;
  initNetwork(&netInitData);

  if (mem.DeviceMode == 0) {
    FS_Network_stop();
  }

  /* FOR TESTING ONLY, REMOVE LATER!!!  */
  // FS_Network_stop();

  initAdc(mem.BatCalib);



  tapDetect.flagTapEnabled = true;

  DPOS.setActivePage(ROOTIDLE_PAGE);
  DPOS.setRenderModePage();
  DPOS_StartDPManager();
  DPOS.draw();
  MM.goTo(&Boot);
  // MemPrintAddresses();
}

void loop() {
  sys.currentTime = millis();

  /*  HANDLE BUTTON  */
  //quadruple press to enter settings
  if (isButtonActionR(&btnHead, QUADRUPLE_PRESS)) {
    if (!sys.flag & SYS_FLAG_SYNC_MODE) {
      MM.goTo(&Settings);
    }
  }
  //tripple press to toggle tap-detection
  if (isButtonActionR(&btnHead, TRIPPLE_PRESS)) {
    led.mode = ledModeBreatheExtended;
    led.duration = 1000;
    led.repetitions = 2;
    led.misc = 0;
    if (tapDetect.flagTapEnabled) {
      tapDetect.flagTapEnabled = false;
      led.color1 = COLOR_RED;
      led.color2 = COLOR_ORANGE;
      FS_SetLed(&led);
      // FSSetLed(COLOR_RED, COLOR_ORANGE, ledModeBreatheExtended, 1000, 2);
    } else {
      tapDetect.flagTapEnabled = true;
      led.color1 = COLOR_GREEN;
      led.color2 = COLOR_CYAN;
      FS_SetLed(&led);
      // FSSetLed(COLOR_GREEN, COLOR_CYAN, ledModeBreatheExtended, 1000, 2);
    }
  }
  // :P if u know u know
  if (isButtonDurationBiggerThan(&btnHead, WINNER_WINNER_CHICKEN_DINNER)) {
    if (!sys.flag & SYS_FLAG_SYNC_MODE) {
      MM.goTo(&EGGO);
    }
  }

  /*  READ TAP DETECTION  */
  if (tapDetect.flagTapEnabled) {
    if ((sys.currentTime - tapDetect.timerTapDetectedLast <= TIME_TAP_INTERVAL) && (tapDetect.flagTapInterrupt)) {
      tapDetect.flagTapInterrupt = false;            //reset flag
      tapDetect.timerTapDebounce = sys.currentTime;  //update debounce timer
      sys.tapCountTemp++;                            //increment temp tap counter
    }

    if ((sys.currentTime - tapDetect.timerTapDetectedLast >= TIME_TAP_INTERVAL) && (sys.tapCount == 0)) {
      sys.tapCount = sys.tapCountTemp;  //update tap count
      sys.tapCountTemp = 0;             //reset temp counter
    }

    if (sys.currentTime - tapDetect.timerTapDebounce >= TIME_TAP_DEBOUNCE) {
      resetTapInterrupt();
    }

    if (sys.tapCountTemp != 0) {
      if (sys.tapCountTemp > sys.tapCountTempOld) {
        LOGI(tag, "TAP-COUNT (temp)", sys.tapCountTemp);
        tap.setCounter(sys.tapCountTemp);
      }
    }
    sys.tapCountTempOld = sys.tapCountTemp;
  }  // end tap detection


  sys.currentMenuIdOld = sys.currentMenuId;
  sys.currentMenuId = MM.getCurrentMenu()->getID();

  if (sys.currentMenuId != sys.currentMenuIdOld) {
    // LOGI(tag, "setting new page to", sys.currentMenuId);
    DPOS.setActivePage(sys.currentMenuId);
  }
  MM.loop();



  /*  TAP-SAFEGUARD  */
  if (sys.tapCount != 0) {
    LOGE(tag, F("TAP-SAFEGUARD"));
    sys.tapCount = 0;
    tap.setCounter(0);
  }
  /*  BUTTON-SAFEGUARD  */
  resetButtons();  //buttons safeGuard
  delay(100);
}
