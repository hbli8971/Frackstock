#ifndef _FS_MENUDEFINITION_H
#define _FS_MENUDEFINITION_H

#include <hbli_EMM.h>
#include "config.h"
#include "FS_Led.h"
#include "FS_Datatypes.h"

enum MenuEntryID {
  ID_ROOTIDLE = 1,
  ID_BEERMENU,
  ID_PINGMENU,
  ID_PINGMENU_PUBLIC,
  ID_PINGMENU_PRIVATE,
  ID_PINGMENU_PING_FAILED,
  ID_PINGMENU_PING_SUCCESS,
  ID_PINGMENU_USERLIST,
  ID_SETTINGS,
  ID_SETTINGS_DEVICE_MODE,
  ID_SETTINGS_TAP_CONFIG,
  ID_SETTINGS_TAP_CONFIG_RANGE,
  ID_SETTINGS_TAP_CONFIG_THRESHHOLD,
  ID_SETTINGS_TAP_CONFIG_TIME_LIM,
  ID_SETTINGS_TAP_CONFIG_TIME_LATENCY,
  ID_SETTINGS_TAP_CONFIG_WINDOW,
  ID_SETTINGS_NETWORK_INFO,
  ID_SETTINGS_BATTERY_INFO,
  ID_SETTINGS_RESTART_DEVICE,
  ID_SETTINGS_CREDITS,
  ID_SETTINGS_SYNC_MODE,
  ID_SYNC_MODE_SCREEN,
  ID_ADMIN_SECTION,
  ID_EGGO,
  ID_BOOT,
  ID_PING_RECEIVED
};

enum MenuID_ROOT {
  BEERMENU_ROOT,
  PINGMENU_ROOT,
  SETTINGS_ROOT,
  SYNCMODESCREEN_ROOT,
  ADMINSECTION_ROOT
};

enum MenuID_PING_MENU {
  PINGMENU_PUBLIC_PING,
  PINGMENU_PRIVATE_PING
};

enum MenuID_PING_MENU_PRIVATE {
  PINGMENU_PRIVATE_PING_PRIVATE
};

enum MenuID_SETTINGS {
  DEVICEMODE_SETTINGS,
  TAPCONFIG_SETTINGS,
  NETWORKINFO_SETTINGS,
  BATTERYINFO_SETTINGS,
  RESTARTDEVICE_SETTINGS,
  CREDITS_SETTINGS,
  SYNCMODE_SETTINGS
};

enum MenuID_SETTINGS_TAPCONFIG {
  RANGE_TAPCONFIG,
  THRESHHOLD_TAPCONFIG,
  TIME_LIM_TAPCONFIG,
  TIME_LATENCY_TAPCONFIG,
  WINDOW_TAPCONFIG
};



extern MenuEntry rootIdle;
extern MenuEntry BeerMenu;
extern MenuEntry PingMenu;
extern MenuEntry PingMenu_Public;
extern MenuEntry PingMenu_Private;
extern MenuEntry PingMenu_Ping_Failed;
extern MenuEntry PingMenu_Ping_Success;
extern MenuEntry PingMenu_Userlist;
extern MenuEntry Settings;
extern MenuEntry Settings_DeviceMode;
extern MenuEntry Settings_TapConfig;
extern MenuEntry Settings_TapConfig_Range;
extern MenuEntry Settings_TapConfig_Threshhold;
extern MenuEntry Settings_TapConfig_TimeLim;
extern MenuEntry Settings_TapConfig_TimeLatency;
extern MenuEntry Settings_TapConfig_Window;
extern MenuEntry Settings_NetworkInfo;
extern MenuEntry Settings_BatteryInfo;
extern MenuEntry Settings_RestartDevice;
extern MenuEntry Settings_Credits;
extern MenuEntry Settings_SyncMode;
extern MenuEntry SyncModeScreen;
extern MenuEntry AdminSection;
extern MenuEntry EGGO;
extern MenuEntry Boot;
extern MenuEntry PingReceived;




extern MenuManager MM;
extern SystemInfo_t sys;
extern ledInfo_t led;
extern MemInfo_t mem;
extern Lis3dh_t lisCfg;

void initMenuStructure();

#endif