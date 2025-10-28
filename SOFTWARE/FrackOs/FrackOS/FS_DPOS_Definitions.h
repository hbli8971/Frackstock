#ifndef _FS_DPOS_DEFINITIONS_H
#define _FS_DPOS_DEFINITIONS_H
#include <DPOS.h>


enum PageIndicator {
  ROOTIDLE_PAGE = 1,
  BEERMENU_PAGE,
  PINGMENU_PAGE,
  PINGMENU_PUBLIC_PAGE,
  PINGMENU_PRIVATE_PAGE,
  PINGMENU_PING_FAILED_PAGE,
  PINGMENU_PING_SUCCESS_PAGE,
  PINGMENU_USERLIST_PAGE,
  SETTINGS_PAGE,
  SETTINGS_DEVICEMODE_PAGE,
  SETTINGS_TAPCONFIG_PAGE,
  SETTINGS_TAPCONFIG_RANGE_PAGE,
  SETTINGS_TAPCONFIG_THRESHHOLD_PAGE,
  SETTINGS_TAPCONFIG_TIMELIM_PAGE,
  SETTINGS_TAPCONFIG_TIMELATENCY_PAGE,
  SETTINGS_TAPCONFIG_WINDOW_PAGE,
  SETTINGS_NETWORKINFO_PAGE,
  SETTINGS_BATTERYINFO_PAGE,
  SETTINGS_RESTARTDEVICE_PAGE,
  SETTINGS_CREDITS_PAGE,
  SETTINGS_SYNCMODE_PAGE,
  SYNCMODE_SCREEN_PAGE,
  ADMIN_SECTION_PAGE,
  EGGY_PAGE,
  BOOT_PAGE,
  PINGRECEIVED_PAGE
};



extern DPOS_Page rootIdlePage;
extern DPOS_Page BeerMenuPage;
extern DPOS_Page PingMenuPage;
extern DPOS_Page PingMenu_PublicPage;
extern DPOS_Page PingMenu_PrivatePage;
extern DPOS_Page PingMenu_Ping_FailedPage;
extern DPOS_Page PingMenu_Ping_SuccessPage;
extern DPOS_Page PingMenu_UserlistPage;
extern DPOS_Page SettingsPage;
extern DPOS_Page Settings_DeviceModePage;
extern DPOS_Page Settings_NetworkInfoPage;
extern DPOS_Page Settings_BatteryInfoPage;
extern DPOS_Page Settings_RestartDevicePage;
extern DPOS_Page Settings_CreditsPage;
extern DPOS_Page Settings_SyncModePage;
extern DPOS_Page SyncModeScreenPage;
extern DPOS_Page AdminSectionPage;
extern DPOS_Page EggyPage;
extern DPOS_Page BootPage;
extern DPOS_Page PingReceivedPage;


//"global" tap indicator
extern DPOS_TapIndicator tap;

//Beermenu
extern DPOS_TextBody beerMenu_tBody;
extern DPOS_Animation_Frame beerMenu_animation;

//PingMenu
extern DPOS_Menu pingMenu_menu;

//Ping failed
extern DPOS_TextBody PingFailed_tBody;

//Ping success
extern DPOS_TextBody PingSuccess_tBody;

//Ping userlist
extern DPOS_Menu PingMenu_UserList_menu;

//Settings Menu
extern DPOS_Menu Settings_menu;

//DeviceMode Menu
extern DPOS_Menu Settings_DeviceMode_menu;

extern DPOS_Menu Settings_TapConfig_menu;


extern DPOS_TextBody TapConfigRange_tBody;
extern DPOS_ScrollIndicator TapConfigRange_scroll;
extern DPOS_TextBody TapConfigThreshhold_tBody;
extern DPOS_ScrollIndicator TapConfigThreshhold_scroll;
extern DPOS_TextBody TapConfigTimeLim_tBody;
extern DPOS_ScrollIndicator TapConfigTimeLim_scroll;
extern DPOS_TextBody TapConfigTimeLatency_tBody;
extern DPOS_ScrollIndicator TapConfigTimeLatency_scroll;
extern DPOS_TextBody TapConfigWindow_tBody;
extern DPOS_ScrollIndicator TapConfigWindow_scroll;

//NetworkInfo
// extern DPOS_Menu Settings_NetworkInfo_menu;

extern DPOS_TextBody Settings_NetworkInfo_Title;
extern DPOS_TextBody Settings_NetworkInfo_ipAddr_text;
extern DPOS_TextBody Settings_NetworkInfo_macAddr_text;
extern DPOS_TextBody Settings_NetworkInfo_RSSI_text;
extern DPOS_TextBody Settings_NetworkInfo_ipAddr;
extern DPOS_TextBody Settings_NetworkInfo_macAddr;
extern DPOS_TextBody Settings_NetworkInfo_RSSI;

//BatteryInfo
extern DPOS_TextBody BatInfo_text_tBody;
extern DPOS_TextBody BatInfo_value_tBody;

//resart device
extern DPOS_TextBody RestartDevice_tBody;

//credits
extern DPOS_TextBody Credits_tBody;

//Settings syncmode page
extern DPOS_Menu SyncMode_menu;

//syncMode screen
extern DPOS_Menu SyncMode_InfoScreen;

//eggy ani
extern DPOS_Animation_Frame eggy_ani;

extern DPOS_Animation_Frame boot_ani;

extern DPOS_TextBody PingReceived_text;
extern DPOS_TextBody PingReceived_Source;




void init_DPOS_elements();

#endif