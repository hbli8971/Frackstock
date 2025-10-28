#include "hal/ledc_ll.h"
#include "esp_system.h"
#include "esp32-hal.h"
#include "FS_MenuDefinition.h"
#include "FS_DPOS_Definitions.h"
#include "FS_TapDetection.h"
#include "FS_Network.h"
#include "FS_adc.h"
#include "FS_Button.h"

#include <hbli_Logging.h>

#include "FS_memCtrl.h"

static const char* tag = "FS_MenuDef";

static uint32_t timerGeneral;

MenuEntry rootIdle(F("rootIdle"), ID_ROOTIDLE);
MenuEntry BeerMenu(F("BeerMenu"), ID_BEERMENU);
MenuEntry PingMenu(F("PingMenu"), ID_PINGMENU);

MenuEntry PingMenu_Public(F("PingMenu_Public"), ID_PINGMENU_PUBLIC);
MenuEntry PingMenu_Private(F("PingMenu_Private"), ID_PINGMENU_PRIVATE);
MenuEntry PingMenu_Ping_Failed(F("PingMenu_Ping_Failed"), ID_PINGMENU_PING_FAILED);
MenuEntry PingMenu_Ping_Success(F("PingMenu_Ping_Success"), ID_PINGMENU_PING_SUCCESS);
MenuEntry PingMenu_Userlist(F("PingMenu_Userlist"), ID_PINGMENU_USERLIST);

MenuEntry Settings(F("Settings"), ID_SETTINGS);
MenuEntry Settings_DeviceMode(F("Settings_DeviceMode"), ID_SETTINGS_DEVICE_MODE);
MenuEntry Settings_TapConfig(F("Settings_TapConfig"), ID_SETTINGS_TAP_CONFIG);

MenuEntry Settings_TapConfig_Range(F("TapConfig_Range"), ID_SETTINGS_TAP_CONFIG_RANGE);
MenuEntry Settings_TapConfig_Threshhold(F("TapConfig_Threshhold"), ID_SETTINGS_TAP_CONFIG_THRESHHOLD);
MenuEntry Settings_TapConfig_TimeLim(F("TapConfig_TimeLim"), ID_SETTINGS_TAP_CONFIG_TIME_LIM);
MenuEntry Settings_TapConfig_TimeLatency(F("TapConfig_TimeLatency"), ID_SETTINGS_TAP_CONFIG_TIME_LATENCY);
MenuEntry Settings_TapConfig_Window(F("TapConfig_Window"), ID_SETTINGS_TAP_CONFIG_WINDOW);

MenuEntry Settings_NetworkInfo(F("Settings_NetworkInfo"), ID_SETTINGS_NETWORK_INFO);
MenuEntry Settings_BatteryInfo(F("Settings_BatteryInfo"), ID_SETTINGS_BATTERY_INFO);
MenuEntry Settings_RestartDevice(F("Settings_RestartDevice"), ID_SETTINGS_RESTART_DEVICE);
MenuEntry Settings_Credits(F("Settings_Credits"), ID_SETTINGS_CREDITS);
MenuEntry Settings_SyncMode(F("Settings_SyncMode"), ID_SETTINGS_SYNC_MODE);

MenuEntry SyncModeScreen(F("SyncModeScreen"), ID_SYNC_MODE_SCREEN);
MenuEntry AdminSection(F("AdminSection"), ID_ADMIN_SECTION);

MenuEntry EGGO(F("EGGO"), ID_EGGO);

MenuEntry Boot(F("BootScreen"), ID_BOOT);
MenuEntry PingReceived(F("Ping_Received"), ID_PING_RECEIVED);

// MANAGER ************** ______________ **************
MenuManager MM(&rootIdle);
// MANAGER ************** ______________ **************

void setupStructureRoot() {
  rootIdle.addSubMenu(&BeerMenu);
  rootIdle.addSubMenu(&PingMenu);
  rootIdle.addSubMenu(&Settings);
  rootIdle.addSubMenu(&SyncModeScreen);
  rootIdle.addSubMenu(&AdminSection);
}

void setupStructurePingMenu() {
  PingMenu.addSubMenu(&PingMenu_Public);
  PingMenu.addSubMenu(&PingMenu_Private);
}

void setupStructurePingMenu_Private() {
  PingMenu_Private.addSubMenu(&PingMenu_Userlist);
}

void setupStructureSettings() {
  Settings.addSubMenu(&Settings_DeviceMode);
  Settings.addSubMenu(&Settings_TapConfig);
  Settings.addSubMenu(&Settings_NetworkInfo);
  Settings.addSubMenu(&Settings_BatteryInfo);
  Settings.addSubMenu(&Settings_RestartDevice);
  Settings.addSubMenu(&Settings_Credits);
  Settings.addSubMenu(&Settings_SyncMode);
}

void setupstructureTapConfig() {
  Settings_TapConfig.addSubMenu(&Settings_TapConfig_Range);
  Settings_TapConfig.addSubMenu(&Settings_TapConfig_Threshhold);
  Settings_TapConfig.addSubMenu(&Settings_TapConfig_TimeLim);
  Settings_TapConfig.addSubMenu(&Settings_TapConfig_TimeLatency);
  Settings_TapConfig.addSubMenu(&Settings_TapConfig_Window);
}

void updateTimer() {
  timerGeneral = millis();
  sys.currentTime = millis();
}

void initMenuStructureTree() {
  setupStructureRoot();
  setupStructurePingMenu();
  setupStructurePingMenu_Private();
  setupStructureSettings();
  setupstructureTapConfig();
}

void initOnEntryFunctions() {
  rootIdle.onEnter = []() {
    //no action
  };

  BeerMenu.onEnter = []() {
    //update beer counter
    uint8_t digits = 1;
    uint8_t beerCtn = MemGetBeerCounter();
    if (beerCtn >= 100) {
      digits = 3;
    } else if (beerCtn >= 10) {
      digits = 2;
    }
    beerMenu_tBody.setDynamicValue(beerCtn, digits, 0);
    BeerMenuPage.draw();  //rebder page
    led.color1 = mem.ColorBeer;
    led.mode = ledModeStatic;
    led.duration = 1;
    led.repetitions = 1;
    led.misc = FS_LED_NO_CLEAR;
    FS_SetLed(&led);
    DPOS.setEndPage(BeerMenu.getID());
    DPOS.startTransition();
    DPOS.waitForTransition();
    updateTimer();
  };

  PingMenu.onEnter = []() {
    DPOS.setEndPage(PingMenu.getID());
    DPOS.startTransition();
    DPOS.waitForTransition();
    updateTimer();
  };

  PingMenu_Public.onEnter = []() {
    if (net.mqttConnected) {
      mqttSendMessage(mem.Identifier.c_str(), REQ_PING_ALL);
      MM.goTo(&PingMenu_Ping_Success);
    } else {
      MM.goTo(&PingMenu_Ping_Failed);
    }
  };

  PingMenu_Private.onEnter = []() {
    if (net.mqttConnected) {
      mqttSendMessage(REQ_USR_LIST);

      MM.goToSubmenu(0);
    } else {
      MM.goTo(&PingMenu_Ping_Failed);
    }
  };

  PingMenu_Ping_Failed.onEnter = []() {
    PingFailed_tBody.setScrollPosition(0);
    PingFailed_tBody.setScrollDelay(20);
    led.color1 = COLOR_RED;
    led.mode = ledModeBreathe;
    led.duration = PING_LED_DURATION_STATUS;
    led.repetitions = PING_LED_REPETITIONS_STATUS;
    led.misc = 0;
    FS_SetLed(&led);
    updateTimer();
  };

  PingMenu_Ping_Success.onEnter = []() {
    PingSuccess_tBody.setScrollPosition(0);
    PingSuccess_tBody.setScrollDelay(20);
    led.color1 = COLOR_GREEN;
    led.mode = ledModeBreathe;
    led.duration = PING_LED_DURATION_STATUS;
    led.repetitions = PING_LED_REPETITIONS_STATUS;
    led.misc = 0;
    FS_SetLed(&led);
    updateTimer();
  };

  PingMenu_Userlist.onEnter = []() {
    DPOS.setEndPage(PingMenu_Userlist.getID());
    const char* entryNoUsr[] = { "list requested " };
    PingMenu_UserList_menu.setEntries(entryNoUsr, 1);
    PingMenu_UserlistPage.draw();
    DPOS.startTransition();
    DPOS.waitForTransition();
    updateTimer();
  };

  Settings.onEnter = []() {
    DPOS.setEndPage(Settings.getID());
    DPOS.startTransition();
    DPOS.waitForTransition();
    updateTimer();
  };

  Settings_DeviceMode.onEnter = []() {
    DPOS.setEndPage(Settings_DeviceMode.getID());
    DPOS.setTransitionMode(DPOS_ANITRANS_LINE_TOPDOWN);
    DPOS.startTransition();
    DPOS.waitForTransition();
    DPOS.setTransitionMode(DPOS_ANITRANS_CIRCLE);
    updateTimer();
  };

  Settings_TapConfig.onEnter = []() {
    LOGI(tag, "entered settings on enter");
    DPOS.setEndPage(Settings_TapConfig.getID());
    DPOS.setTransitionMode(DPOS_ANITRANS_LINE_TOPDOWN);
    DPOS.startTransition();
    DPOS.waitForTransition();
    DPOS.setTransitionMode(DPOS_ANITRANS_CIRCLE);
    updateTimer();
    led.color1 = COLOR_GREEN;
    led.color2 = COLOR_BLACK;
    led.duration = 1000;
    led.repetitions = 1;
    led.mode = ledModeBreatheExtended;
  };

  Settings_TapConfig_Range.onEnter = []() {
    if (mem.TapRange == 0) {
      TapConfigRange_tBody.setDynamicValue(2, 1, 0);
    } else if (mem.TapRange == 1) {
      TapConfigRange_tBody.setDynamicValue(4, 1, 0);
    } else if (mem.TapRange == 2) {
      TapConfigRange_tBody.setDynamicValue(8, 1, 0);
    } else {
      TapConfigRange_tBody.setDynamicValue(16, 2, 0);
    }
    TapConfigRange_tBody.draw();
    DPOS.setEndPage(Settings_TapConfig_Range.getID());
    DPOS.setTransitionMode(DPOS_ANITRANS_LINE_TOPDOWN);
    DPOS.startTransition();
    DPOS.waitForTransition();
    DPOS.setTransitionMode(DPOS_ANITRANS_CIRCLE);
    updateTimer();
  };

  Settings_TapConfig_Threshhold.onEnter = []() {
    TapConfigThreshhold_tBody.setDynamicValue(mem.TapThresh, 3, 0);
    TapConfigThreshhold_tBody.draw();
    DPOS.setEndPage(Settings_TapConfig_Threshhold.getID());
    DPOS.setTransitionMode(DPOS_ANITRANS_LINE_TOPDOWN);
    DPOS.startTransition();
    DPOS.waitForTransition();
    DPOS.setTransitionMode(DPOS_ANITRANS_CIRCLE);
    updateTimer();
  };

  Settings_TapConfig_TimeLim.onEnter = []() {
    TapConfigTimeLim_tBody.setDynamicValue(mem.TapTimeLim, 3, 0);
    TapConfigTimeLim_tBody.draw();
    DPOS.setEndPage(Settings_TapConfig_TimeLim.getID());
    DPOS.setTransitionMode(DPOS_ANITRANS_LINE_TOPDOWN);
    DPOS.startTransition();
    DPOS.waitForTransition();
    DPOS.setTransitionMode(DPOS_ANITRANS_CIRCLE);
    updateTimer();
  };

  Settings_TapConfig_TimeLatency.onEnter = []() {
    TapConfigTimeLatency_tBody.setDynamicValue(mem.TapTimeLatency, 3, 0);
    TapConfigTimeLatency_tBody.draw();
    DPOS.setEndPage(Settings_TapConfig_TimeLatency.getID());
    DPOS.setTransitionMode(DPOS_ANITRANS_LINE_TOPDOWN);
    DPOS.startTransition();
    DPOS.waitForTransition();
    DPOS.setTransitionMode(DPOS_ANITRANS_CIRCLE);
    updateTimer();
  };

  Settings_TapConfig_Window.onEnter = []() {
    TapConfigWindow_tBody.setDynamicValue(mem.TapWindow, 3, 0);
    TapConfigWindow_tBody.draw();
    DPOS.setEndPage(Settings_TapConfig_Window.getID());
    DPOS.setTransitionMode(DPOS_ANITRANS_LINE_TOPDOWN);
    DPOS.startTransition();
    DPOS.waitForTransition();
    DPOS.setTransitionMode(DPOS_ANITRANS_CIRCLE);
    updateTimer();
  };

  Settings_NetworkInfo.onEnter = []() {
    char tempStrChar[20];  // Ensure the buffer is large enough to hold the IP address
    strncpy(tempStrChar, net.IpAddress.c_str(), sizeof(tempStrChar));
    LOGI(tag, "tempchar string is", tempStrChar);
    // strncpy(tempStrChar, net.IpAddress.c_str(), sizeof(tempStrChar));
    // Settings_NetworkInfo_ipAddr.setDynamicText(tempStrChar);
    Settings_NetworkInfo_RSSI.setDynamicValue(net.RSSI, 2, 0);

    DPOS.setEndPage(Settings_NetworkInfo.getID());
    DPOS.setTransitionMode(DPOS_ANITRANS_LINE_TOPDOWN);
    DPOS.startTransition();
    DPOS.waitForTransition();
    DPOS.setTransitionMode(DPOS_ANITRANS_CIRCLE);
    updateTimer();
  };

  Settings_BatteryInfo.onEnter = []() {
    DPOS.setEndPage(Settings_BatteryInfo.getID());
    DPOS.setTransitionMode(DPOS_ANITRANS_LINE_TOPDOWN);
    DPOS.startTransition();
    DPOS.waitForTransition();
    DPOS.setTransitionMode(DPOS_ANITRANS_CIRCLE);
    updateTimer();
  };

  Settings_RestartDevice.onEnter = []() {
    DPOS.setEndPage(Settings_RestartDevice.getID());
    DPOS.setTransitionMode(DPOS_ANITRANS_LINE_TOPDOWN);
    DPOS.startTransition();
    DPOS.waitForTransition();
    DPOS.setTransitionMode(DPOS_ANITRANS_CIRCLE);
    updateTimer();
  };

  Settings_Credits.onEnter = []() {
    DPOS.setEndPage(Settings_Credits.getID());
    DPOS.setTransitionMode(DPOS_ANITRANS_LINE_TOPDOWN);
    DPOS.startTransition();
    DPOS.waitForTransition();
    DPOS.setTransitionMode(DPOS_ANITRANS_CIRCLE);
    updateTimer();
  };

  Settings_SyncMode.onEnter = []() {
    updateTimer();
    DPOS.setEndPage(Settings_SyncMode.getID());
    DPOS.setTransitionMode(DPOS_ANITRANS_LINE_TOPDOWN);
    DPOS.startTransition();
    DPOS.waitForTransition();
    DPOS.setTransitionMode(DPOS_ANITRANS_CIRCLE);
    updateTimer();
  };

  SyncModeScreen.onEnter = []() {

  };

  AdminSection.onEnter = []() {

  };

  EGGO.onEnter = []() {
    eggy_ani.setRepetitions(3);
    eggy_ani.startAnimation();
    eggy_ani.setCurrentFrame(0);
  };

  Boot.onEnter = []() {
    DPOS.setEndPage(Boot.getID());
    DPOS.startTransition();
    DPOS.waitForTransition();
    boot_ani.setRepetitions(2);
    boot_ani.startAnimation();
    updateTimer();
  };

  PingReceived.onEnter = []() {
    PingReceived_Source.setDynamicText(FS_Network_GetPingSource().c_str());
    PingReceivedPage.draw();

    uint8_t tempMisc = FS_Network_GetMiscInfo();

    if (tempMisc == PUBLIC_PING_RECEIVED) {
      led.color1 = COLOR_PING_ALL;
    } else if (tempMisc == PRIVATE_PING_RECEIVED) {
      led.color1 = mem.ColorPing;
    }
    led.mode = ledModeBreathe;
    led.duration = 1000;
    led.repetitions = 3;
    led.misc = 0;
    FS_SetLed(&led);
    updateTimer();
  };
}

void initOnLoopFunctions() {
  rootIdle.onLoop = []() {
    if (TapsDetected(1)) {
      MM.goToSubmenu(BEERMENU_ROOT);
    } else if (TapsDetectedMoreThan(1)) {
      MM.goToSubmenu(PINGMENU_ROOT);
    }
  };

  BeerMenu.onLoop = []() {
    if (TapsDetected(1)) {
      timerGeneral = sys.currentTime;
    } else if (TapsDetected(2)) {
      LOGI(tag, "adding beer");
      //update counter on screen
      uint8_t digits = 1;
      uint8_t beerCtn = (MemGetBeerCounter() + 1);
      if (beerCtn >= 100) {
        digits = 3;
      } else if (beerCtn >= 10) {
        digits = 2;
      }
      if (!MemSetBeerCounter(beerCtn)) {
        LOGE(tag, "setting beer counter");
      }
      beerMenu_tBody.setDynamicValue(beerCtn, digits, 0);
      beerMenu_tBody.draw();

      //start animation
      beerMenu_animation.setCurrentFrame(0);
      beerMenu_animation.setRepetitions(2);
      beerMenu_animation.startAnimation();


      //set led animation
      led.color1 = mem.ColorBeer;
      led.color2 = mem.ColorBeerIncrement;
      led.duration = BEER_TIME_LIGHTING;
      led.repetitions = BEER_REPETITIONS;
      led.mode = ledModeBreatheExtended;
      led.misc = FS_LED_NO_CLEAR;
      FS_SetLed(&led);


      timerGeneral = sys.currentTime;
    } else if (TapsDetectedMoreThan(2)) {
      timerGeneral = sys.currentTime;
    }
    if ((sys.currentTime - timerGeneral >= BEER_TIMEOUT) && (sys.tapCountTemp <= 0)) {
      MM.goBackToRoot();
    }
  };

  PingMenu.onLoop = []() {
    if (TapsDetected(1)) {
      pingMenu_menu.next();
      pingMenu_menu.draw();
      updateTimer();
    } else if (TapsDetected(2)) {
      MM.goToSubmenu(pingMenu_menu.select());
    } else if (TapsDetectedMoreThan(2)) {
      updateTimer();
    }
    if ((sys.currentTime - timerGeneral >= PINGMENU_TIMEOUT) && (sys.tapCountTemp <= 0)) {
      MM.goBackToRoot();
    }
  };

  PingMenu_Public.onLoop = []() {
    //no action
  };

  PingMenu_Private.onLoop = []() {
    //no action
  };

  PingMenu_Ping_Failed.onLoop = []() {
    if (sys.currentTime - timerGeneral >= PING_FAILED_TIMEOUT) {
      MM.goBackToRoot();
    }
  };

  PingMenu_Ping_Success.onLoop = []() {
    if (sys.currentTime - timerGeneral >= PING_SUCCESS_TIMEOUT) {
      MM.goBackToRoot();
    }
  };

  PingMenu_Userlist.onLoop = []() {
    static bool _checkFlag = false;

    if (FS_Network_GetUsrListFlag()) {
      if (FS_Network_GetUserListAmount() > 0) {
        PingMenu_UserList_menu.setEntries(usrPtrs, FS_Network_GetUserListAmount());
      } else {
        const char* entryNoUsr[] = { "no one Online :( " };
        PingMenu_UserList_menu.setEntries(entryNoUsr, 1);
      }
      PingMenu_UserlistPage.draw();
      _checkFlag = true;
    }

    if (_checkFlag) {
      if (TapsDetected(1) || isButtonActionR(&btnHead, SINGLE_PRESS)) {
        PingMenu_UserList_menu.next();
        PingMenu_UserlistPage.draw();
        updateTimer();
      } else if (TapsDetected(2) || isButtonActionR(&btnHead, DOUBLE_PRESS)) {
        PingMenu_UserList_menu.prev();
        PingMenu_UserlistPage.draw();
        updateTimer();
      } else if (TapsDetected(3) || isButtonActionR(&btnHead, LONG_PRESS)) {
        if (net.mqttConnected) {
          mqttSendMessage(REQ_PING_USR, usrPtrs[PingMenu_UserList_menu.select()]);
          MM.goTo(&PingMenu_Ping_Success);
        } else {
          MM.goTo(&PingMenu_Ping_Failed);
        }

      } else if (TapsDetectedMoreThan(3)) {
        updateTimer();
      }
    }





    if (!_checkFlag && (sys.currentTime - timerGeneral >= PING_USERLIST_WAIT_TIMEOUT)) {
      MM.goBackToRoot();
    }

    if (_checkFlag && (sys.currentTime - timerGeneral >= PING_USERLIST_TIMEOUT) && (sys.tapCountTemp == 0)) {
      MM.goBackToRoot();
    }
  };

  Settings.onLoop = []() {
    if (TapsDetected(1) || isButtonActionR(&btnHead, SINGLE_PRESS)) {
      Settings_menu.next();
      SettingsPage.draw();
      updateTimer();
    } else if (TapsDetected(2) || isButtonActionR(&btnHead, DOUBLE_PRESS)) {
      Settings_menu.prev();
      SettingsPage.draw();
      updateTimer();
    } else if (TapsDetected(3) || isButtonActionR(&btnHead, LONG_PRESS)) {
      MM.goToSubmenu(Settings_menu.select());
    } else if (TapsDetectedMoreThan(3)) {
      updateTimer();
    }
    if (((sys.currentTime - timerGeneral) >= SETTINGS_TIMEOUT) && (sys.tapCountTemp <= 0)) {
      MM.goBackToRoot();
    }
  };

  Settings_DeviceMode.onLoop = []() {
    if (TapsDetected(1)) {
      Settings_DeviceMode_menu.next();
      Settings_DeviceModePage.draw();
      updateTimer();
    } else if (TapsDetected(2)) {
      if (Settings_DeviceMode_menu.select()) {
        //offline mode
        //only do something, if network mode before
        if (mem.DeviceMode) {
          MemSetDeviceMode(0);
          FS_Network_stop();
          mem.DeviceMode = 0;
        }
      } else {
        //online mode
        if (mem.DeviceMode == 0) {
          MemSetDeviceMode(1);
          FS_Network_Start();
          mem.DeviceMode = 1;
        }
      }
      MM.goBackToRoot();
    } else if (TapsDetectedMoreThan(2)) {
      updateTimer();
    }

    if ((sys.currentTime - timerGeneral >= SETTINGS_DEVICEMODE_TIMEOUT) && (sys.tapCountTemp == 0)) {
      MM.goBackToRoot();
    }
  };

  Settings_TapConfig.onLoop = []() {
    if (TapsDetected(1) || isButtonActionR(&btnHead, SINGLE_PRESS)) {
      Settings_TapConfig_menu.next();
      Settings_TapConfig_menu.draw();
      updateTimer();
    } else if (TapsDetected(2) || isButtonActionR(&btnHead, DOUBLE_PRESS)) {
      Settings_TapConfig_menu.prev();
      Settings_TapConfig_menu.draw();
      updateTimer();
    } else if (TapsDetected(3) || isButtonActionR(&btnHead, LONG_PRESS)) {
      MM.goToSubmenu(Settings_TapConfig_menu.select());
      updateTimer();
    } else if (TapsDetectedMoreThan(3)) {

      updateTimer();
    }

    if ((sys.currentTime - timerGeneral >= SETTINGS_TAPCONFIG_TIMEOUT) && (sys.tapCountTemp == 0)) {
      MM.goBackToRoot();
    }
  };

  Settings_TapConfig_Range.onLoop = []() {
    if (TapsDetected(1) || isButtonActionR(&btnHead, SINGLE_PRESS)) {
      if (mem.TapRange < 3) {
        mem.TapRange++;

        TapConfigRange_tBody.setDynamicValue(pow(2, mem.TapRange + 1), (mem.TapRange == 3) ? 2 : 1, 0);
        TapConfigRange_tBody.draw();
      }

      updateTimer();
    } else if (TapsDetected(2) || isButtonActionR(&btnHead, DOUBLE_PRESS)) {
      if (mem.TapRange > 0) {
        mem.TapRange--;
        TapConfigRange_tBody.setDynamicValue(pow(2, mem.TapRange + 1), (mem.TapRange == 3) ? 2 : 1, 0);
        TapConfigRange_tBody.draw();
      }
      updateTimer();
    } else if (TapsDetected(3) || isButtonActionR(&btnHead, LONG_PRESS)) {
      lis3dh_set_range(mem.TapRange);
      if (MemSetTapRange(mem.TapRange)) {
        FS_SetLed(&led);
        MM.goTo(&Settings_TapConfig);
      }

    } else if (TapsDetectedMoreThan(3)) {
      updateTimer();
    }
    if ((sys.currentTime - timerGeneral >= SETTINGS_TAPCONFIG_TIMEOUT) && (sys.tapCountTemp == 0)) {
      MM.goBackToRoot();
    }
  };

  Settings_TapConfig_Threshhold.onLoop = []() {
    if (TapsDetected(1) || isButtonActionR(&btnHead, SINGLE_PRESS)) {
      if (mem.TapThresh <= 255) {
        mem.TapThresh += 5;
        TapConfigThreshhold_tBody.setDynamicValue(mem.TapThresh, 3, 0);
        TapConfigThreshhold_tBody.draw();
      }
      updateTimer();
    } else if (TapsDetected(2) || isButtonActionR(&btnHead, DOUBLE_PRESS)) {
      if (mem.TapThresh >= 5) {
        mem.TapThresh -= 5;
        TapConfigThreshhold_tBody.setDynamicValue(mem.TapThresh, 3, 0);
        TapConfigThreshhold_tBody.draw();
      }
      updateTimer();
    } else if (TapsDetected(3) || isButtonActionR(&btnHead, LONG_PRESS)) {
      lis3dh_set_thresh(mem.TapThresh);
      if (MemSetTapThreshhold(mem.TapThresh)) {
        FS_SetLed(&led);
        MM.goTo(&Settings_TapConfig);
      }
    } else if (TapsDetectedMoreThan(3)) {
      updateTimer();
    }
    if ((sys.currentTime - timerGeneral >= SETTINGS_TAPCONFIG_TIMEOUT) && (sys.tapCountTemp == 0)) {
      MM.goBackToRoot();
    }
  };

  Settings_TapConfig_TimeLim.onLoop = []() {
    if (TapsDetected(1) || isButtonActionR(&btnHead, SINGLE_PRESS)) {
      if (mem.TapTimeLim <= 255) {
        mem.TapTimeLim += 5;
        TapConfigTimeLim_tBody.setDynamicValue(mem.TapTimeLim, 3, 0);
        TapConfigTimeLim_tBody.draw();
      }
      updateTimer();
    } else if (TapsDetected(2) || isButtonActionR(&btnHead, DOUBLE_PRESS)) {
      if (mem.TapTimeLim >= 5) {
        mem.TapTimeLim -= 5;
        TapConfigTimeLim_tBody.setDynamicValue(mem.TapTimeLim, 3, 0);
        TapConfigTimeLim_tBody.draw();
      }
      updateTimer();
    } else if (TapsDetected(3) || isButtonActionR(&btnHead, LONG_PRESS)) {
      lis3dh_set_time_lim(mem.TapTimeLim);
      if (MemSetTapTimeLim(mem.TapTimeLim)) {
        FS_SetLed(&led);
        MM.goTo(&Settings_TapConfig);
      }
    } else if (TapsDetectedMoreThan(3)) {
    }
    if ((sys.currentTime - timerGeneral >= SETTINGS_TAPCONFIG_TIMEOUT) && (sys.tapCountTemp == 0)) {
      MM.goBackToRoot();
    }
  };

  Settings_TapConfig_TimeLatency.onLoop = []() {
    if (TapsDetected(1) || isButtonActionR(&btnHead, SINGLE_PRESS)) {
      if (mem.TapTimeLatency <= 255) {
        mem.TapTimeLatency += 5;
        TapConfigTimeLatency_tBody.setDynamicValue(mem.TapTimeLatency, 3, 0);
        TapConfigTimeLatency_tBody.draw();
      }
      updateTimer();
    } else if (TapsDetected(2) || isButtonActionR(&btnHead, DOUBLE_PRESS)) {
      if (mem.TapTimeLatency >= 5) {
        mem.TapTimeLatency -= 5;
        TapConfigTimeLatency_tBody.setDynamicValue(mem.TapTimeLatency, 3, 0);
        TapConfigTimeLatency_tBody.draw();
      }
      updateTimer();
    } else if (TapsDetected(3) || isButtonActionR(&btnHead, LONG_PRESS)) {
      lis3dh_set_time_latency(mem.TapTimeLatency);
      if (MemSetTapTimeLatency(mem.TapTimeLatency)) {
        FS_SetLed(&led);
        MM.goTo(&Settings_TapConfig);
      }
    } else if (TapsDetectedMoreThan(3)) {
      updateTimer();
    }
    if ((sys.currentTime - timerGeneral >= SETTINGS_TAPCONFIG_TIMEOUT) && (sys.tapCountTemp == 0)) {
      MM.goBackToRoot();
    }
  };

  Settings_TapConfig_Window.onLoop = []() {
    if (TapsDetected(1) || isButtonActionR(&btnHead, SINGLE_PRESS)) {
      if (mem.TapWindow <= 255) {
        mem.TapWindow += 5;
        TapConfigWindow_tBody.setDynamicValue(mem.TapWindow, 3, 0);
        TapConfigWindow_tBody.draw();
      }
      updateTimer();
    } else if (TapsDetected(2) || isButtonActionR(&btnHead, DOUBLE_PRESS)) {
      if (mem.TapWindow >= 5) {
        mem.TapWindow -= 5;
        TapConfigWindow_tBody.setDynamicValue(mem.TapWindow, 3, 0);
        TapConfigWindow_tBody.draw();
      }
      updateTimer();
    } else if (TapsDetected(3) || isButtonActionR(&btnHead, LONG_PRESS)) {
      lis3dh_set_window(mem.TapWindow);
      if (MemSetTapWindow(mem.TapWindow)) {
        FS_SetLed(&led);
        MM.goTo(&Settings_TapConfig);
      }
    } else if (TapsDetectedMoreThan(3)) {
      updateTimer();
    }
    if ((sys.currentTime - timerGeneral >= SETTINGS_TAPCONFIG_TIMEOUT) && (sys.tapCountTemp == 0)) {
      MM.goBackToRoot();
    }
  };

  Settings_NetworkInfo.onLoop = []() {

  };

  Settings_BatteryInfo.onLoop = []() {
    if (sys.currentTime - timerGeneral >= SETTINGS_BATTERY_VOLTAGE_UPDATE) {
      updateTimer();
      BatInfo_value_tBody.setDynamicValue(getBatteryVoltageRaw(), 3, 2);
      Settings_BatteryInfoPage.draw();
    }
    if (TapsDetected(1)) {
      updateTimer();
    } else if (TapsDetectedMoreThan(1)) {
      MM.goBackToRoot();
    }

    if (sys.currentTime - timerGeneral >= SETTINGS_BATTERY_TIMEOUT) {
      MM.goBackToRoot();
    }
  };

  Settings_RestartDevice.onLoop = []() {
    if (TapsDetected(2)) {
      esp_restart();
    }

    if ((sys.currentTime - timerGeneral >= SETTINGS_RESTART_TIMEOUT) && (sys.tapCountTemp == 0)) {
      MM.goBackToRoot();
    }
  };

  Settings_Credits.onLoop = []() {

  };

  Settings_SyncMode.onLoop = []() {
    if (TapsDetected(1) || isButtonActionR(&btnHead, SINGLE_PRESS)) {
      SyncMode_menu.next();
      Settings_SyncModePage.draw();
      updateTimer();
    } else if (TapsDetected(2) || isButtonActionR(&btnHead, DOUBLE_PRESS)) {
      //send syncmode request
      if (net.mqttConnected) {
        mqttSendMessage(REQ_SYNC_MODE);
      } else {
        // error
      }


    } else if (TapsDetected(3) || isButtonActionR(&btnHead, LONG_PRESS)) {
      //send command
    }
  };

  SyncModeScreen.onLoop = []() {

  };

  AdminSection.onLoop = []() {

  };

  EGGO.onLoop = []() {
    if (eggy_ani.getAnimationDone()) {
      MM.goBackToRoot();
    }
  };

  Boot.onLoop = []() {
    if (boot_ani.getAnimationDone()) {
      LOGI(tag, "going back to root");
      MM.goBackToRoot();
    }
  };

  PingReceived.onLoop = []() {
    if (sys.currentTime - timerGeneral >= PING_RECEIVED_TIMEOUT) {
      MM.goBackToRoot();
    }
  };
}

void initOnExitFunctions() {
  rootIdle.onExit = []() {

  };

  BeerMenu.onExit = []() {
    led.color1 = COLOR_BLACK;
    led.mode = ledModeStatic;
    led.misc = 0;
    // led.repetitions = 1;
    FS_SetLed(&led);
  };

  PingMenu.onExit = []() {
    pingMenu_menu.resetCursor();
    pingMenu_menu.draw();
  };

  PingMenu_Public.onExit = []() {

  };

  PingMenu_Private.onExit = []() {

  };

  PingMenu_Ping_Failed.onExit = []() {

  };

  PingMenu_Ping_Success.onExit = []() {

  };

  PingMenu_Userlist.onExit = []() {
    PingMenu_UserList_menu.resetCursor();
  };

  Settings.onExit = []() {
    Settings_menu.resetCursor();
    Settings_menu.draw();
  };

  Settings_DeviceMode.onExit = []() {
    Settings_DeviceMode_menu.resetCursor();
    Settings_DeviceMode_menu.draw();
  };

  Settings_TapConfig.onExit = []() {

  };

  Settings_NetworkInfo.onExit = []() {

  };

  Settings_BatteryInfo.onExit = []() {

  };

  Settings_RestartDevice.onExit = []() {

  };

  Settings_Credits.onExit = []() {

  };

  Settings_SyncMode.onExit = []() {

  };

  SyncModeScreen.onExit = []() {

  };

  AdminSection.onExit = []() {

  };

  EGGO.onExit = []() {

  };
  PingReceived.onExit = []() {
    PingReceived_text.setScrollPosition(0);
    PingReceived_text.setScrollDelay(20);
    PingReceived_text.setScrollDirection(1);
    PingReceived_Source.setScrollPosition(0);
    PingReceived_Source.setScrollDelay(20);
    PingReceived_Source.setScrollDirection(1);
    PingReceivedPage.draw();
  };
}



void initMenuStructure() {
  initOnEntryFunctions();
  initOnLoopFunctions();
  initOnExitFunctions();
  initMenuStructureTree();
}
/*
rootIdle              
BeerMenu              
PingMenu              
PingMenu_Public       
PingMenu_Private      
PingMenu_Ping_Failed  
PingMenu_Ping_Success 
PingMenu_Userlist     
Settings              
Settings_DeviceMode   
Settings_NetworkInfo  
Settings_BatteryInfo  
Settings_RestartDevice
Settings_Credits      
Settings_SyncMode     
SyncModeScreen        
AdminSection          
*/
