#include "FS_DPOS_Definitions.h"
#include "config.h"
#include "wifi_credentials.h"
#include "animation.h"
#include "FS_Network.h"


DPOS_Page rootIdlePage(ROOTIDLE_PAGE);
DPOS_Page BeerMenuPage(BEERMENU_PAGE);
DPOS_Page PingMenuPage(PINGMENU_PAGE);
DPOS_Page PingMenu_PublicPage(PINGMENU_PUBLIC_PAGE);
DPOS_Page PingMenu_PrivatePage(PINGMENU_PRIVATE_PAGE);
DPOS_Page PingMenu_Ping_FailedPage(PINGMENU_PING_FAILED_PAGE);
DPOS_Page PingMenu_Ping_SuccessPage(PINGMENU_PING_SUCCESS_PAGE);
DPOS_Page PingMenu_UserlistPage(PINGMENU_USERLIST_PAGE);
DPOS_Page SettingsPage(SETTINGS_PAGE);
DPOS_Page Settings_DeviceModePage(SETTINGS_DEVICEMODE_PAGE);
DPOS_Page Settings_TapConfigPage(SETTINGS_TAPCONFIG_PAGE);
DPOS_Page Settings_TapConfig_RangePage(SETTINGS_TAPCONFIG_RANGE_PAGE);
DPOS_Page Settings_TapConfig_ThreshholdPage(SETTINGS_TAPCONFIG_THRESHHOLD_PAGE);
DPOS_Page Settings_TapConfig_TimeLimPage(SETTINGS_TAPCONFIG_TIMELIM_PAGE);
DPOS_Page Settings_TapConfig_TimeLatencyPage(SETTINGS_TAPCONFIG_TIMELATENCY_PAGE);
DPOS_Page Settings_TapConfig_WindowPage(SETTINGS_TAPCONFIG_WINDOW_PAGE);
DPOS_Page Settings_NetworkInfoPage(SETTINGS_NETWORKINFO_PAGE);
DPOS_Page Settings_BatteryInfoPage(SETTINGS_BATTERYINFO_PAGE);
DPOS_Page Settings_RestartDevicePage(SETTINGS_RESTARTDEVICE_PAGE);
DPOS_Page Settings_CreditsPage(SETTINGS_CREDITS_PAGE);
DPOS_Page Settings_SyncModePage(SETTINGS_SYNCMODE_PAGE);
DPOS_Page SyncModeScreenPage(SYNCMODE_SCREEN_PAGE);
DPOS_Page AdminSectionPage(ADMIN_SECTION_PAGE);
DPOS_Page EggyPage(EGGY_PAGE);
DPOS_Page BootPage(BOOT_PAGE);
DPOS_Page PingReceivedPage(PINGRECEIVED_PAGE);



//"global" tap indicator
DPOS_TapIndicator tap;

//Beermenu
DPOS_TextBody beerMenu_tBody(SS_BEER_TBODY_X, SS_BEER_TBODY_Y, SS_BEER_TBODY_W, SS_BEER_TBODY_H);
DPOS_Animation_Frame beerMenu_animation(SS_BEER_ANI_W, SS_BEER_ANI_H);

//PingMenu
DPOS_Menu pingMenu_menu;

//Ping failed
DPOS_TextBody PingFailed_tBody(0, (SCREEN_HEIGHT - 30) / 2, SCREEN_WIDTH, SCREEN_HEIGHT - 30);

//Ping success
DPOS_TextBody PingSuccess_tBody(0, (SCREEN_HEIGHT - 30) / 2, SCREEN_WIDTH, SCREEN_HEIGHT - 30);

//Ping userlist
DPOS_Menu PingMenu_UserList_menu;

//Settings Menu
DPOS_Menu Settings_menu;

//DeviceMode Menu
DPOS_Menu Settings_DeviceMode_menu;

DPOS_Menu Settings_TapConfig_menu;

DPOS_TextBody TapConfigRange_tBody(0, 20, SCREEN_WIDTH, 20);
DPOS_ScrollIndicator TapConfigRange_scroll(SCREEN_WIDTH, 10);

DPOS_TextBody TapConfigThreshhold_tBody(0, 20, SCREEN_WIDTH, 20);
DPOS_ScrollIndicator TapConfigThreshhold_scroll(SCREEN_WIDTH, 10);

DPOS_TextBody TapConfigTimeLim_tBody(0, 20, SCREEN_WIDTH, 20);
DPOS_ScrollIndicator TapConfigTimeLim_scroll(SCREEN_WIDTH, 10);

DPOS_TextBody TapConfigTimeLatency_tBody(0, 20, SCREEN_WIDTH, 20);
DPOS_ScrollIndicator TapConfigTimeLatency_scroll(SCREEN_WIDTH, 10);

DPOS_TextBody TapConfigWindow_tBody(0, 20, SCREEN_WIDTH, 20);
DPOS_ScrollIndicator TapConfigWindow_scroll(SCREEN_WIDTH, 10);



//NetworkInfo

DPOS_TextBody Settings_NetworkInfo_Title(16, 0, SCREEN_WIDTH - 21, 18);

DPOS_TextBody Settings_NetworkInfo_ipAddr_text(0, 20, 35, 12);
DPOS_TextBody Settings_NetworkInfo_macAddr_text(0, 32, 35, 12);
DPOS_TextBody Settings_NetworkInfo_RSSI_text(0, 44, 35, 12);

DPOS_TextBody Settings_NetworkInfo_ipAddr(40, 20, SCREEN_WIDTH - 40, 12);
DPOS_TextBody Settings_NetworkInfo_macAddr(40, 32, SCREEN_WIDTH - 40, 12);
DPOS_TextBody Settings_NetworkInfo_RSSI(40, 44, SCREEN_WIDTH - 40, 12);


//BatteryInfo
DPOS_TextBody BatInfo_text_tBody(0, 15, SCREEN_WIDTH, 25);
DPOS_TextBody BatInfo_value_tBody(0, SCREEN_HEIGHT - 20, SCREEN_WIDTH, 25);

//resart device
DPOS_TextBody RestartDevice_tBody(0, (SCREEN_HEIGHT - 30) / 2, SCREEN_WIDTH, (SCREEN_HEIGHT - 30));

//credits
DPOS_TextBody Credits_tBody(0, (SCREEN_HEIGHT - 30) / 2, SCREEN_WIDTH, (SCREEN_HEIGHT - 30));

//Settings syncmode page
DPOS_Menu SyncMode_menu;

//syncMode screen
DPOS_Menu SyncMode_InfoScreen;

//eggy ani
DPOS_Animation_Frame eggy_ani(ANI_FRAME_WIDTH, ANI_FRAME_HEIGHT);

//boot page
DPOS_Animation_Frame boot_ani(ANI_FRAME_WIDTH, ANI_FRAME_HEIGHT);

//ping received
DPOS_TextBody PingReceived_text(0, 0, DPOS_SCREEN_WIDTH, 30);
DPOS_TextBody PingReceived_Source(0, 34, DPOS_SCREEN_WIDTH, 30);

//
void setup_Tapindicator() {
  tap.setStyle(RECT_ROUND_BORDER_BG);
  tap.setCounter(0);
}
//
void setup_rootIdlePage() {
  rootIdlePage.addElement(&tap);
};
//
void setup_BeerMenuPage() {
  beerMenu_tBody.setTextSize(TEXT_SIZE_LARGE);
  beerMenu_tBody.setAlignment(CENTER);
  beerMenu_tBody.setStyle(RECT_ROUND_BG);
  beerMenu_tBody.setStyleSetting(8);
  beerMenu_tBody.setTextColor(0);
  beerMenu_tBody.setText(DPOS_VALUE_MARKER);

  beerMenu_animation.setPos(SS_BEER_ANI_X, SS_BEER_ANI_Y);
  beerMenu_animation.addFrame(frame_00);
  beerMenu_animation.addFrame(frame_01);
  beerMenu_animation.addFrame(frame_02);
  beerMenu_animation.addFrame(frame_03);
  beerMenu_animation.addFrame(frame_04);
  beerMenu_animation.addFrame(frame_05);
  beerMenu_animation.addFrame(frame_06);
  beerMenu_animation.addFrame(frame_07);
  beerMenu_animation.addFrame(frame_08);
  beerMenu_animation.addFrame(frame_09);
  beerMenu_animation.addFrame(frame_10);
  beerMenu_animation.addFrame(frame_11);
  beerMenu_animation.addFrame(frame_12);
  beerMenu_animation.addFrame(frame_13);
  beerMenu_animation.addFrame(frame_14);
  beerMenu_animation.addFrame(frame_15);
  beerMenu_animation.addFrame(frame_16);
  beerMenu_animation.addFrame(frame_17);
  beerMenu_animation.addFrame(frame_18);
  beerMenu_animation.addFrame(frame_19);
  beerMenu_animation.addFrame(frame_20);
  beerMenu_animation.addFrame(frame_21);
  beerMenu_animation.addFrame(frame_22);
  beerMenu_animation.addFrame(frame_23);
  beerMenu_animation.addFrame(frame_24);
  beerMenu_animation.addFrame(frame_25);
  beerMenu_animation.stopAnimation();
  beerMenu_animation.draw();

  BeerMenuPage.addElement(&beerMenu_tBody);
  BeerMenuPage.addElement(&beerMenu_animation);
  BeerMenuPage.addElement(&tap);
};
//
void setup_PingMenuPage() {
  pingMenu_menu.setTitle("Ping Menu");
  pingMenu_menu.setEntries(pingMenuEntries, 2);
  pingMenu_menu.setTypeMenu();
  pingMenu_menu.draw();

  PingMenuPage.addElement(&pingMenu_menu);
  PingMenuPage.addElement(&tap);
};

//unused
void setup_PingMenu_PublicPage(){
  //
};
//unused
void setup_PingMenu_PrivatePage(){
  //
};
//
void setup_PingMenu_Ping_FailedPage() {
  PingFailed_tBody.setTextSize(TEXT_SIZE_MEDIUM);
  PingFailed_tBody.setText("Ping Failed");
  PingFailed_tBody.setAlignment(CENTER);
  PingFailed_tBody.setStyle(RECT_ROUND_BORDER_BG);
  PingFailed_tBody.setStyleSetting(5);
  PingFailed_tBody.setTextOffsetX(3);
  PingFailed_tBody.setTextColor(1);
  PingFailed_tBody.draw();
  PingMenu_Ping_FailedPage.addElement(&PingFailed_tBody);
};
//
void setup_PingMenu_Ping_SuccessPage() {
  PingSuccess_tBody.setTextSize(TEXT_SIZE_MEDIUM);
  PingSuccess_tBody.setText("Ping Sent");
  PingSuccess_tBody.setAlignment(CENTER);
  PingSuccess_tBody.setStyle(RECT_ROUND_BORDER_BG);
  PingSuccess_tBody.setStyleSetting(5);
  PingSuccess_tBody.setTextOffsetX(3);
  PingSuccess_tBody.setTextColor(1);
  PingSuccess_tBody.draw();
  PingMenu_Ping_SuccessPage.addElement(&PingSuccess_tBody);
};
//
void setup_PingMenu_UserlistPage() {
  PingMenu_UserList_menu.setTypeMenu();
  PingMenu_UserList_menu.setTitle("Select User");


  PingMenu_UserlistPage.addElement(&PingMenu_UserList_menu);
  PingMenu_UserlistPage.addElement(&tap);
};
//
void setup_SettingsPage() {
  Settings_menu.setTitle("Settings");
  Settings_menu.setEntries(EntriesSettingsMenu, ENTRIES_SETTINGS_MENU_NUM);
  Settings_menu.setTypeMenu();
  Settings_menu.draw();  //initial render of menu
  SettingsPage.addElement(&Settings_menu);
  SettingsPage.addElement(&tap);
};
//
void setup_Settings_DeviceModePage() {
  Settings_DeviceMode_menu.setTitle("Device Mode");
  Settings_DeviceMode_menu.setEntries(EntriesSettingsDevMode, 2);
  Settings_DeviceMode_menu.setTypeMenu();
  Settings_DeviceMode_menu.draw();

  Settings_DeviceModePage.addElement(&Settings_DeviceMode_menu);
  Settings_DeviceModePage.addElement(&tap);
};

void setup_Settings_TapConfigPage() {
  Settings_TapConfig_menu.setTitle("Tap Config");
  Settings_TapConfig_menu.setTypeMenu();
  Settings_TapConfig_menu.setEntries(EntriesSettingsTapConfig, 5);
  Settings_TapConfig_menu.draw();

  Settings_TapConfigPage.addElement(&Settings_TapConfig_menu);
  Settings_TapConfigPage.addElement(&tap);
}

void setup_Settings_TapConfig_allPages() {

  TapConfigRange_tBody.setText("{{value}}G");
  TapConfigThreshhold_tBody.setText("{{value}}");
  TapConfigTimeLim_tBody.setText("{{value}}");
  TapConfigTimeLatency_tBody.setText("{{value}}");
  TapConfigWindow_tBody.setText("{{value}}");

  TapConfigRange_tBody.setTextSize(TEXT_SIZE_MEDIUM);
  TapConfigThreshhold_tBody.setTextSize(TEXT_SIZE_MEDIUM);
  TapConfigTimeLim_tBody.setTextSize(TEXT_SIZE_MEDIUM);
  TapConfigTimeLatency_tBody.setTextSize(TEXT_SIZE_MEDIUM);
  TapConfigWindow_tBody.setTextSize(TEXT_SIZE_MEDIUM);

  TapConfigRange_tBody.setAlignment(CENTER);
  TapConfigThreshhold_tBody.setAlignment(CENTER);
  TapConfigTimeLim_tBody.setAlignment(CENTER);
  TapConfigTimeLatency_tBody.setAlignment(CENTER);
  TapConfigWindow_tBody.setAlignment(CENTER);

  TapConfigRange_tBody.setTextOffsetX(2);
  TapConfigThreshhold_tBody.setTextOffsetX(2);
  TapConfigTimeLim_tBody.setTextOffsetX(2);
  TapConfigTimeLatency_tBody.setTextOffsetX(2);
  TapConfigWindow_tBody.setTextOffsetX(2);

  TapConfigRange_tBody.setTextColor(1);
  TapConfigThreshhold_tBody.setTextColor(1);
  TapConfigTimeLim_tBody.setTextColor(1);
  TapConfigTimeLatency_tBody.setTextColor(1);
  TapConfigWindow_tBody.setTextColor(1);

  Settings_TapConfig_RangePage.addElement(&TapConfigRange_tBody);
  Settings_TapConfig_ThreshholdPage.addElement(&TapConfigThreshhold_tBody);
  Settings_TapConfig_TimeLimPage.addElement(&TapConfigTimeLim_tBody);
  Settings_TapConfig_TimeLatencyPage.addElement(&TapConfigTimeLatency_tBody);
  Settings_TapConfig_WindowPage.addElement(&TapConfigWindow_tBody);


  TapConfigRange_scroll.setHorizontal();
  TapConfigThreshhold_scroll.setHorizontal();
  TapConfigTimeLim_scroll.setHorizontal();
  TapConfigTimeLatency_scroll.setHorizontal();
  TapConfigWindow_scroll.setHorizontal();

  TapConfigRange_scroll.setNumberOfSteps(4);
  TapConfigThreshhold_scroll.setNumberOfSteps(25);
  TapConfigTimeLim_scroll.setNumberOfSteps(25);
  TapConfigTimeLatency_scroll.setNumberOfSteps(25);
  TapConfigWindow_scroll.setNumberOfSteps(25);


  TapConfigRange_scroll.setCurrentStep(0);
  TapConfigThreshhold_scroll.setCurrentStep(0);
  TapConfigTimeLim_scroll.setCurrentStep(0);
  TapConfigTimeLatency_scroll.setCurrentStep(0);
  TapConfigWindow_scroll.setCurrentStep(0);

  Settings_TapConfig_RangePage.addElement(&TapConfigRange_scroll);
  Settings_TapConfig_ThreshholdPage.addElement(&TapConfigThreshhold_scroll);
  Settings_TapConfig_TimeLimPage.addElement(&TapConfigTimeLim_scroll);
  Settings_TapConfig_TimeLatencyPage.addElement(&TapConfigTimeLatency_scroll);
  Settings_TapConfig_WindowPage.addElement(&TapConfigWindow_scroll);

  Settings_TapConfig_RangePage.addElement(&tap);
  Settings_TapConfig_ThreshholdPage.addElement(&tap);
  Settings_TapConfig_TimeLimPage.addElement(&tap);
  Settings_TapConfig_TimeLatencyPage.addElement(&tap);
  Settings_TapConfig_WindowPage.addElement(&tap);
}

void setup_Settings_NetworkInfoPage() {

  Settings_NetworkInfo_Title.setTextSize(TEXT_SIZE_MEDIUM);
  Settings_NetworkInfo_Title.setAlignment(CENTER);
  Settings_NetworkInfo_Title.setStyle(RECT_ROUND_BORDER_BG);
  Settings_NetworkInfo_Title.setStyleSetting(4);
  Settings_NetworkInfo_Title.setTextColor(1);
  Settings_NetworkInfo_Title.setTextOffsetX(3);
  Settings_NetworkInfo_Title.setText("Network Info");

  Settings_NetworkInfo_ipAddr_text.setTextSize(TEXT_SIZE_SMALL);
  Settings_NetworkInfo_ipAddr_text.setAlignment(CENTER_LEFT);
  Settings_NetworkInfo_ipAddr_text.setStyle(RECT_ROUND_BORDER_BG);
  Settings_NetworkInfo_ipAddr_text.setStyleSetting(3);
  Settings_NetworkInfo_ipAddr_text.setTextColor(1);
  Settings_NetworkInfo_ipAddr_text.setTextOffsetX(2);
  Settings_NetworkInfo_ipAddr_text.setText("IP:");

  Settings_NetworkInfo_macAddr_text.setTextSize(TEXT_SIZE_SMALL);
  Settings_NetworkInfo_macAddr_text.setAlignment(CENTER_LEFT);
  Settings_NetworkInfo_macAddr_text.setStyle(RECT_ROUND_BORDER_BG);
  Settings_NetworkInfo_macAddr_text.setStyleSetting(3);
  Settings_NetworkInfo_macAddr_text.setTextColor(1);
  Settings_NetworkInfo_macAddr_text.setTextOffsetX(2);
  Settings_NetworkInfo_macAddr_text.setText("MAC:");

  Settings_NetworkInfo_RSSI_text.setTextSize(TEXT_SIZE_SMALL);
  Settings_NetworkInfo_RSSI_text.setAlignment(CENTER_LEFT);
  Settings_NetworkInfo_RSSI_text.setStyle(RECT_ROUND_BORDER_BG);
  Settings_NetworkInfo_RSSI_text.setStyleSetting(3);
  Settings_NetworkInfo_RSSI_text.setTextColor(1);
  Settings_NetworkInfo_RSSI_text.setTextOffsetX(2);
  Settings_NetworkInfo_RSSI_text.setText("RSSI:");

  Settings_NetworkInfo_macAddr.setTextSize(TEXT_SIZE_SMALL);
  Settings_NetworkInfo_macAddr.setAlignment(CENTER_LEFT);
  Settings_NetworkInfo_macAddr.setStyle(RECT_ROUND_BORDER_BG);
  Settings_NetworkInfo_macAddr.setStyleSetting(3);
  Settings_NetworkInfo_macAddr.setTextColor(1);
  Settings_NetworkInfo_macAddr.setTextOffsetX(2);
  Settings_NetworkInfo_macAddr.setText(WiFi.macAddress().c_str());

  //dynamic entires ________________________________________________________________
  Settings_NetworkInfo_ipAddr.setTextSize(TEXT_SIZE_SMALL);
  Settings_NetworkInfo_ipAddr.setAlignment(CENTER_LEFT);
  Settings_NetworkInfo_ipAddr.setStyle(RECT_ROUND_BORDER_BG);
  Settings_NetworkInfo_ipAddr.setStyleSetting(3);
  Settings_NetworkInfo_ipAddr.setTextColor(1);
  Settings_NetworkInfo_ipAddr.setTextOffsetX(2);
  Settings_NetworkInfo_ipAddr.setText(DPOS_TEXT_MARKER);

  Settings_NetworkInfo_RSSI.setTextSize(TEXT_SIZE_SMALL);
  Settings_NetworkInfo_RSSI.setAlignment(CENTER_LEFT);
  Settings_NetworkInfo_RSSI.setStyle(RECT_ROUND_BORDER_BG);
  Settings_NetworkInfo_RSSI.setStyleSetting(3);
  Settings_NetworkInfo_RSSI.setTextColor(1);
  Settings_NetworkInfo_RSSI.setTextOffsetX(2);
  Settings_NetworkInfo_RSSI.setText(DPOS_VALUE_MARKER);



  Settings_NetworkInfoPage.addElement(&Settings_NetworkInfo_Title);
  Settings_NetworkInfoPage.addElement(&Settings_NetworkInfo_ipAddr_text);
  Settings_NetworkInfoPage.addElement(&Settings_NetworkInfo_macAddr_text);
  Settings_NetworkInfoPage.addElement(&Settings_NetworkInfo_RSSI_text);
  Settings_NetworkInfoPage.addElement(&Settings_NetworkInfo_ipAddr);
  Settings_NetworkInfoPage.addElement(&Settings_NetworkInfo_macAddr);
  Settings_NetworkInfoPage.addElement(&Settings_NetworkInfo_RSSI);
  Settings_NetworkInfoPage.draw();
  Settings_NetworkInfoPage.addElement(&tap);
};

void setup_Settings_BatteryInfoPage() {
  BatInfo_text_tBody.setTextSize(TEXT_SIZE_MEDIUM);
  BatInfo_text_tBody.setText("Voltage:");
  BatInfo_text_tBody.setAlignment(CENTER);
  BatInfo_text_tBody.draw();


  BatInfo_value_tBody.setTextSize(TEXT_SIZE_MEDIUM);
  BatInfo_value_tBody.setText(DPOS_VALUE_MARKER);
  BatInfo_value_tBody.setAlignment(CENTER);
  BatInfo_value_tBody.setDynamicValue(0, 3, 2);
  BatInfo_value_tBody.draw();

  Settings_BatteryInfoPage.addElement(&BatInfo_value_tBody);
  Settings_BatteryInfoPage.addElement(&BatInfo_text_tBody);
  Settings_BatteryInfoPage.addElement(&tap);
};
//
void setup_Settings_RestartDevicePage() {
  RestartDevice_tBody.setTextSize(TEXT_SIZE_MEDIUM);
  RestartDevice_tBody.setText("Restart device? Tap 2 times!");
  RestartDevice_tBody.setAlignment(CENTER);
  RestartDevice_tBody.setStyle(RECT_ROUND_BORDER_BG);
  RestartDevice_tBody.setTextColor(1);
  RestartDevice_tBody.setScrollSpeed(2);
  RestartDevice_tBody.setTextOffsetX(3);
  RestartDevice_tBody.draw();


  Settings_RestartDevicePage.addElement(&RestartDevice_tBody);
  Settings_RestartDevicePage.addElement(&tap);
};
//
void setup_Settings_CreditsPage() {
  Credits_tBody.setTextSize(TEXT_SIZE_MEDIUM);
  Credits_tBody.setText("created by Jan Spuler");
  Credits_tBody.setAlignment(CENTER);
  Credits_tBody.setScrollSpeed(2);


  Settings_CreditsPage.addElement(&Credits_tBody);
  Settings_CreditsPage.addElement(&tap);
};
//
void setup_Settings_SyncModePage() {
  SyncMode_menu.setTitle("Sync Mode");
  SyncMode_menu.setEntries(LEDModeList, 5);
  SyncMode_menu.setTypeMenu();
  SyncMode_menu.draw();
  Settings_SyncModePage.addElement(&SyncMode_menu);
  Settings_SyncModePage.addElement(&tap);
};
//
void setup_SyncModeScreenPage() {
  SyncMode_InfoScreen.setTypeInfo();
  SyncMode_InfoScreen.setTitle("Sync-Mode");

  SyncModeScreenPage.addElement(&SyncMode_InfoScreen);
};

//unused for now
void setup_AdminSectionPage(){

};
//
void setup_Eggy_Page() {
  eggy_ani.addFrame(frame_00_e);
  eggy_ani.addFrame(frame_30_e);
  eggy_ani.addFrame(frame_31_e);
  eggy_ani.addFrame(frame_32_e);
  eggy_ani.addFrame(frame_33_e);
  eggy_ani.addFrame(frame_34_e);
  eggy_ani.addFrame(frame_35_e);
  eggy_ani.addFrame(frame_00_e);
  eggy_ani.addFrame(frame_01_e);
  eggy_ani.addFrame(frame_02_e);
  eggy_ani.addFrame(frame_03_e);
  eggy_ani.addFrame(frame_04_e);
  eggy_ani.addFrame(frame_05_e);
  eggy_ani.addFrame(frame_06_e);
  eggy_ani.addFrame(frame_07_e);
  eggy_ani.addFrame(frame_08_e);
  eggy_ani.addFrame(frame_09_e);
  eggy_ani.addFrame(frame_10_e);
  eggy_ani.addFrame(frame_11_e);
  eggy_ani.addFrame(frame_12_e);
  eggy_ani.addFrame(frame_13_e);
  eggy_ani.addFrame(frame_14_e);
  eggy_ani.addFrame(frame_15_e);
  eggy_ani.addFrame(frame_16_e);
  eggy_ani.addFrame(frame_17_e);
  eggy_ani.addFrame(frame_18_e);
  eggy_ani.addFrame(frame_19_e);
  eggy_ani.addFrame(frame_20_e);
  eggy_ani.addFrame(frame_21_e);
  eggy_ani.addFrame(frame_22_e);
  eggy_ani.addFrame(frame_23_e);
  eggy_ani.addFrame(frame_24_e);
  eggy_ani.addFrame(frame_25_e);
  eggy_ani.addFrame(frame_26_e);
  eggy_ani.addFrame(frame_27_e);
  eggy_ani.addFrame(frame_28_e);
  eggy_ani.addFrame(frame_29_e);
  eggy_ani.setPos(40, 0);
  eggy_ani.draw();

  EggyPage.addElement(&eggy_ani);
}

void setup_Boot_Screen() {
  boot_ani.addFrame(frame_00_b);
  boot_ani.addFrame(frame_01_b);
  boot_ani.addFrame(frame_02_b);
  boot_ani.addFrame(frame_03_b);
  boot_ani.addFrame(frame_04_b);
  boot_ani.addFrame(frame_05_b);
  boot_ani.addFrame(frame_06_b);
  boot_ani.addFrame(frame_07_b);
  boot_ani.addFrame(frame_08_b);
  boot_ani.addFrame(frame_09_b);
  boot_ani.addFrame(frame_10_b);
  boot_ani.addFrame(frame_11_b);
  boot_ani.addFrame(frame_12_b);
  boot_ani.addFrame(frame_13_b);
  boot_ani.addFrame(frame_14_b);
  boot_ani.addFrame(frame_15_b);
  boot_ani.addFrame(frame_16_b);
  boot_ani.addFrame(frame_17_b);
  boot_ani.addFrame(frame_18_b);
  boot_ani.addFrame(frame_19_b);
  boot_ani.addFrame(frame_20_b);
  boot_ani.addFrame(frame_21_b);
  boot_ani.addFrame(frame_22_b);
  boot_ani.addFrame(frame_23_b);
  boot_ani.addFrame(frame_24_b);
  boot_ani.addFrame(frame_25_b);
  boot_ani.addFrame(frame_26_b);
  boot_ani.addFrame(frame_27_b);
  boot_ani.addFrame(frame_28_b);
  boot_ani.addFrame(frame_29_b);
  boot_ani.addFrame(frame_30_b);
  boot_ani.addFrame(frame_31_b);
  boot_ani.addFrame(frame_32_b);
  boot_ani.addFrame(frame_33_b);
  boot_ani.addFrame(frame_34_b);
  boot_ani.addFrame(frame_35_b);
  boot_ani.addFrame(frame_36_b);
  boot_ani.addFrame(frame_37_b);
  boot_ani.addFrame(frame_38_b);
  boot_ani.addFrame(frame_39_b);
  boot_ani.setPos(32, 0);
  boot_ani.draw();

  BootPage.addElement(&boot_ani);
}

void setup_PingReceivedPage() {
  PingReceived_text.setTextSize(TEXT_SIZE_MEDIUM);
  PingReceived_text.setStyle(DEFAULT_NO_BG);
  PingReceived_text.setTextOffsetX(3);
  PingReceived_text.setText("Ping received from:");
  PingReceived_text.setScrollSpeed(2);
  PingReceived_text.draw();

  PingReceived_Source.setTextSize(TEXT_SIZE_MEDIUM);
  PingReceived_Source.setStyle(RECT_ROUND_BG);
  PingReceived_Source.setStyleSetting(4);
  PingReceived_Source.setTextColor(0);
  PingReceived_Source.setText(DPOS_TEXT_MARKER);
  PingReceived_Source.setTextOffsetX(3);
  PingReceived_Source.setDynamicText("not Found!");
  PingReceived_Source.setAlignment(CENTER);
  PingReceived_Source.draw();

  PingReceivedPage.addElement(&PingReceived_text);
  PingReceivedPage.addElement(&PingReceived_Source);
}

void addPagesToDPOS() {
  DPOS.addPage(&rootIdlePage);
  DPOS.addPage(&BeerMenuPage);
  DPOS.addPage(&PingMenuPage);

  DPOS.addPage(&PingMenu_PublicPage);
  DPOS.addPage(&PingMenu_PrivatePage);
  DPOS.addPage(&PingMenu_Ping_FailedPage);
  DPOS.addPage(&PingMenu_Ping_SuccessPage);
  DPOS.addPage(&PingMenu_UserlistPage);

  DPOS.addPage(&SettingsPage);
  DPOS.addPage(&Settings_DeviceModePage);
  DPOS.addPage(&Settings_TapConfigPage);
  DPOS.addPage(&Settings_TapConfig_RangePage);
  DPOS.addPage(&Settings_TapConfig_ThreshholdPage);
  DPOS.addPage(&Settings_TapConfig_TimeLimPage);
  DPOS.addPage(&Settings_TapConfig_TimeLatencyPage);
  DPOS.addPage(&Settings_TapConfig_WindowPage);
  DPOS.addPage(&Settings_NetworkInfoPage);
  DPOS.addPage(&Settings_BatteryInfoPage);
  DPOS.addPage(&Settings_RestartDevicePage);
  DPOS.addPage(&Settings_CreditsPage);
  DPOS.addPage(&Settings_SyncModePage);

  DPOS.addPage(&SyncModeScreenPage);
  DPOS.addPage(&AdminSectionPage);
  DPOS.addPage(&EggyPage);
  DPOS.addPage(&BootPage);
  DPOS.addPage(&PingReceivedPage);
}

void init_DPOS_elements() {
  setup_rootIdlePage();
  setup_Tapindicator();
  setup_BeerMenuPage();
  setup_PingMenuPage();
  setup_PingMenu_Ping_FailedPage();
  setup_PingMenu_Ping_SuccessPage();
  setup_PingMenu_PublicPage();
  setup_PingMenu_PrivatePage();
  setup_PingMenu_UserlistPage();
  setup_SettingsPage();
  setup_Settings_DeviceModePage();
  setup_Settings_TapConfigPage();
  setup_Settings_TapConfig_allPages();
  setup_Settings_BatteryInfoPage();
  setup_Settings_RestartDevicePage();
  setup_Settings_CreditsPage();
  setup_Settings_SyncModePage();
  setup_SyncModeScreenPage();
  setup_AdminSectionPage();
  setup_Eggy_Page();
  setup_Boot_Screen();
  setup_PingReceivedPage();
  addPagesToDPOS();
};
/*
rootIdlePage
BeerMenuPage
PingMenuPage
PingMenu_PublicPage
PingMenu_PrivatePage
PingMenu_Ping_FailedPage
PingMenu_Ping_SuccessPage
PingMenu_UserlistPage
SettingsPage
Settings_DeviceModePage
Settings_NetworkInfoPage
Settings_BatteryInfoPage
Settings_RestartDevicePage
Settings_CreditsPage
Settings_SyncModePage
SyncModeScreenPage
AdminSectionPage
*/