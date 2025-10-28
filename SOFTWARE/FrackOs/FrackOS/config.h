#ifndef FS_CONFIG_H
#define FS_CONFIG_H
#include <sys/_stdint.h>
#include "color_table.h"
#include <Arduino.h>

/*  CHANGE HERE AFTER CODE-CHANGE   */
#define SW_VERSION (uint16_t)1
#define LOGIC_INVERSION true  //<- version 1.x
/*  CHANGE HERE AFTER CODE-CHANGE   */

#define RESET_EEPROM_ON_START_SECTION_FAIL true

// #define DEVICE_MASTER true

#define DPOS_LOG_LEVEL_NONE 0
#define DPOS_LOG_LEVEL_INFO 1
#define DPOS_LOG_LEVEL_VERBOSE 2
#define DPOS_LOG_LEVEL_DEBUG 3

#define DPOS_LOG_OUTPUT_LEVEL DPOS_LOG_LEVEL_INFO


#define IDENTIFIER_LENGTH 8

#define MAX_FIRST_NAME_LENGTH 32
#define MAX_LAST_NAME_LENGTH 32
#define MAX_FULL_NAME_LENGTH (MAX_FIRST_NAME_LENGTH + MAX_LAST_NAME_LENGTH + 2)

#define MAX_SSID_LENGTH 32
#define MAX_PASSWORD_LENGTH 32

#define SERIAL_BAUD_RATE 115200
#define RX_BUFFER_SIZE 512
#define TX_BUFFER_SIZE 512




/*  READ TAP DETECTION  */
#define TIME_TAP_DEBOUNCE 400
#define TIME_TAP_INTERVAL 3 * TIME_TAP_DEBOUNCE


// definitions for FS_MenuDefinitions funcitons
#define DEFAULT_TIMEOUT_TIME 5000

#define BEER_TIME_LIGHTING 1000
#define BEER_REPETITIONS 2
#define BEER_TIMEOUT DEFAULT_TIMEOUT_TIME

#define PINGMENU_TIMEOUT 3 * DEFAULT_TIMEOUT_TIME

#define SETTINGS_TIMEOUT 3 * DEFAULT_TIMEOUT_TIME
#define SETTINGS_DEVICEMODE_TIMEOUT 2 * DEFAULT_TIMEOUT_TIME

#define SETTINGS_TAPCONFIG_TIMEOUT 5 * DEFAULT_TIMEOUT_TIME

#define SETTINGS_RESTART_TIMEOUT 2 * DEFAULT_TIMEOUT_TIME

#define SETTINGS_BATTERY_VOLTAGE_UPDATE 500
#define SETTINGS_BATTERY_TIMEOUT 6 * DEFAULT_TIMEOUT_TIME

#define PING_FAILED_TIMEOUT DEFAULT_TIMEOUT_TIME
#define PING_SUCCESS_TIMEOUT DEFAULT_TIMEOUT_TIME
#define PING_LED_REPETITIONS_STATUS 3
#define PING_LED_DURATION_STATUS 1000

#define PING_USERLIST_WAIT_TIMEOUT DEFAULT_TIMEOUT_TIME
#define PING_USERLIST_TIMEOUT 30000

#define PING_RECEIVED_TIMEOUT 7500






#define PING_REPETITIONS 5
#define PING_DURATION 1000




// #define MQTT_IDENT_LENGTH 8

/*  *** PIN DEFINITIONS ***  */
#define FS_PIN_LED1 22
#define FS_PIN_LED2 23
#define FS_PIN_BTN_PROG 9
#define FS_PIN_BTN_HEAD 18
#define FS_PIN_LIS_INT_1 20
#define FS_PIN_LIS_INT_2 19
#define FS_PIN_WS2812 15

//define the trigger button, default is "FS_PIN_BTN_HEAD"
#define TRIGGER_BUTTON FS_PIN_BTN_HEAD
#define TRIGGER_BUTTON_DEFAULT_VALUE 0
#define TRIGGER_BUTTON_DEBOUNCE 25

//I2C
#define FS_PIN_I2C_SDA 6
#define FS_PIN_I2C_SCL 7
#define I2C_ADDR_LIS3DH 0x18
// #define I2C_ADDR_SSD1306 0x3C  // <- is defined in DPOS

// *** POWER-Settings ***
// if voltage is above name, it will register as the name-as %
// example if voltage is above BAT_V_100 the percentage is 100
#define BAT_V_100 4.1
#define BAT_V_75 3.9
#define BAT_V_50 3.7
#define BAT_V_25 3.2

//buton handle time def
#define BUTTON_TIME_SETTINGS 1000  //min held down time








// screen / display
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
// #define DISPLAY_BUFFER_SIZE SCREEN_WIDTH* SCREEN_HEIGHT




//Network definitions
#define TIME_CHECK_CONNECTIONS 5000
#define SCAN_FOR_SSID 30000

#define MQTT_BUFFER_SIZE 2048

#define MQTT_MAX_USERS 64

#define NETWORK_WIFI_CONNECTION_TRIES 5

#define MQTT_PUBLIC_TOPIC "Fracknet"
#define MQTT_PUBLIC_TOPIC_SUBSCRIBE MQTT_PUBLIC_TOPIC "/#"

/*
  MESSAGE RANGES
  0XX - reserved
  1XX - client 2 server requests
  2XX - client 2 server messages
  3XX - server 2 client response
  4XX - server 2 client messages
  5XX - 
  6XX - 
  7XX - 
  8XX - config
  9XX - ADMIN
*/
enum message_range_designator {
  CODE_REQ_PING_ALL = 100,
  CODE_REQ_PING_USR,
  CODE_REQ_USR_LIST,
  CODE_REQ_SYNC_MODE,
  CODE_REQ_SYNC_RELEASE,
  CODE_REQ_SYNC_SET_MODE,
  CODE_REQ_TIME_UPDATE,
  CODE_REQ_BEER_TAP,

  CODE_MSG_USR_JOINED = 200,
  CODE_MSG_LED_ANIMATION,
  CODE_MSG_CON_CHECK_RETURN,
  CODE_MSG_CFG_CONFIRM,
  CODE_MSG_RECEIVED,
  CODE_MSG_DISCONNECT,

  CODE_MSG_USR_LIST = 300,
  CODE_MSG_TIME_UPDATE,

  CODE_MSG_PING_ALL = 400,
  CODE_MSG_PING_USR,
  CODE_MSG_BEER_CTN,
  CODE_MSG_SYNC_MODE,
  CODE_MSG_SYNC_RELEASE,
  CODE_MSG_LED_CONTROL,
  CODE_MSG_CON_CHECK,

  CODE_CFG_CLIENT_IDENT = 800,
  CODE_CFG_USR_FNAME,
  CODE_CFG_USR_LNAME,
  CODE_CFG_CLR_BEER,
  CODE_CFG_CLR_BEER_INC,
  CODE_CFG_CLR_PING,
  CODE_CFG_BEER_COUNT,
  CODE_CFG_DEV_MODE,
  CODE_CFG_BAT_CALIB,
  CODE_CFG_TAP_SCALE,
  CODE_CFG_TAP_THRESH,

  CODE_ADMIN_LOGIN = 900,
};

//1XX - client 2 server requests
#define REQ_PING_ALL "100PingAllC2S"              //requests a Ping to all devices on MQTT_PUBLIC_TOPIC
#define REQ_PING_USR "101PingUsrC2S"              //requests a Ping to a specific device on their own MQTT_CLIENT_IDENTIFIER
#define REQ_USR_LIST "102ReqUsrListC2S"           //requests the active user list from the server
#define REQ_SYNC_MODE "103ReqSyncModeC2S"         //requests the synchronisation of all clients
#define REQ_SYNC_RELEASE "104ReqSyncReleaseC2S"   //requests / indicates to stop sync mode
#define REQ_SYNC_SET_MODE "105ReqSyncSetModeC2S"  //requests a syncMode
#define REQ_TIME_UPDATE "106ReqTimeUpdateC2ST"    //requests the current time
#define REQ_BEER_TAP "107ReqBeerTapC2S"

//2XX - client 2 server messages
#define MSG_USR_JOINED "200joinedSrv"             //on first connection to server
#define MSG_LED_ANIMATION "201LEDAnimation"       //select animation for sync mode
#define MSG_CON_CHECK_RETURN "202ConnectionGood"  //con-check return
#define MSG_CFG_CONFIRM "203configReceived"       //config command received
#define MSG_RECEIVED "204msgReceived"             //message received
#define MSG_DISCONNECT "205Disconnected"

//3XX - server 2 client response
#define MSG_USR_LIST "300UsrListS2C"     //indicates that the user list has been sent back
#define MSG_TIME_UPDATE "301TimeUpdate"  //indicates time update message

//4XX - server 2 client messages
#define MSG_PING_ALL "400PingAllS2C"        //indicates a received Ping from MQTT_PUBLIC_TOPIC
#define MSG_PING_USR "401PingUsrS2C"        //indicates a received Ping from another client
#define MSG_BEER_CTN "402BeerCountS2C"      //indicates to send the current beer count to the server
#define MSG_SYNC_MODE "403SyncMode"         //indicates to sync all devices
#define MSG_SYNC_RELEASE "404SyncRelease"   //indicates the end of sync mode
#define MSG_LED_CONTROL "405LEDSet"         //indicates an incoming LED message
#define MSG_CON_CHECK "406CheckConnection"  //indicates connection check

//8XX - config
#define CFG_CLIENT_IDENT "800CfgClientIdent"
#define CFG_USR_FNAME "801CfgUserFName"
#define CFG_USR_LNAME "802CfgUserLName"
#define CFG_CLR_BEER "803CfgClrBeer"
#define CFG_CLR_BEER_INC "804CfgClrBeerInc"
#define CFG_CLR_PING "805CfgClrPing"
#define CFG_BEER_COUNT "806CfgBeerCount"
#define CFG_DEV_MODE "807CfgDevMode"
#define CFG_BAT_CALIB "808CfgBatCalib"
#define CFG_TAP_SCALE "809CfgTapScale"
#define CFG_TAP_THRESH "810CfgTapThresh"

//9XX - ADMIN
#define ADMIN_LOGIN "900AdminLogin"

// MQTT-Messages
#define MSG_DEFAULT "\\(.__.)/"





enum MQTTMessageDesignator {
  msgType_NONE,
  msgType_ConCheck,
  msgType_BeerCtn,
  msgType_LoginFailed,
  msgType_LoginSuccess,
  msgType_CfgNoRights,
  msgType_OperationFailed,
  msgType_BeerCtn_set,
  msgType_Ident_set,
  msgType_FName_set,
  msgType_LName_set,
  msgType_ClrBeer_set,
  msgType_ClrBeerInc_set,
  msgType_ClrPing_set,
  msgType_DevMode_set,
  msgType_BatCalib_set,
  msgType_TapScale_set,
  msgType_TapThresh_set
};


//misc

#define ANIMATION_IDENT_EGG 1
#define ANIMATION_IDENT_BEER 2


#if (LOGIC_INVERSION == true)
#define COLOR_WHITE ~WHITE
#define COLOR_BLACK ~BLACK
#define COLOR_RED ~RED
#define COLOR_GREEN ~GREEN
#define COLOR_BLUE ~BLUE
#define COLOR_CYAN ~CYAN
#define COLOR_YELLOW ~YELLOW
#define COLOR_DARK_YELLOW ~DARK_YELLOW
#define COLOR_LIGHT_GREEN ~LIGHT_GREEN
#define COLOR_DARK_BLUE ~DARK_BLUE
#define COLOR_ORANGE ~ORANGE
#define COLOR_PURPLE ~PURPLE
#define COLOR_PINK ~PINK
#define COLOR_BROWN ~BROWN
#define COLOR_GREY ~GREY
#define COLOR_LIGHT_BLUE ~LIGHT_BLUE
#define COLOR_DARK_GREEN ~DARK_GREEN
#define COLOR_MAGENTA ~MAGENTA
#define COLOR_LIME ~LIME
#define COLOR_MAROON ~MAROON
#define COLOR_OLIVE ~OLIVE
#define COLOR_NAVY ~NAVY
#define COLOR_TEAL ~TEAL
#define COLOR_SILVER ~SILVER
#define COLOR_GOLD ~GOLD
#define COLOR_LIGHT_CYAN ~LIGHT_CYAN
#define COLOR_LIGHT_YELLOW ~LIGHT_YELLOW
#define COLOR_LIGHT_GREY ~LIGHT_GREY
#define COLOR_DARK_RED ~DARK_RED
#define COLOR_DARK_ORANGE ~DARK_ORANGE
#define COLOR_VIOLET ~VIOLET
#define COLOR_INDIGO ~INDIGO
#define COLOR_TURQUOISE ~TURQUOISE
#define COLOR_PEACH ~PEACH
#define COLOR_MINT ~MINT
#define COLOR_LAVENDER ~LAVENDER
#define COLOR_CORAL ~CORAL
#define COLOR_SALMON ~SALMON
#else
#define COLOR_WHITE WHITE
#define COLOR_BLACK BLACK
#define COLOR_RED RED
#define COLOR_GREEN GREEN
#define COLOR_BLUE BLUE
#define COLOR_CYAN CYAN
#define COLOR_YELLOW YELLOW
#define COLOR_DARK_YELLOW DARK_YELLOW
#define COLOR_LIGHT_GREEN LIGHT_GREEN
#define COLOR_DARK_BLUE DARK_BLUE
#define COLOR_ORANGE ORANGE
#define COLOR_PURPLE PURPLE
#define COLOR_PINK PINK
#define COLOR_BROWN BROWN
#define COLOR_GREY GREY
#define COLOR_LIGHT_BLUE LIGHT_BLUE
#define COLOR_DARK_GREEN DARK_GREEN
#define COLOR_MAGENTA MAGENTA
#define COLOR_LIME LIME
#define COLOR_MAROON MAROON
#define COLOR_OLIVE OLIVE
#define COLOR_NAVY NAVY
#define COLOR_TEAL TEAL
#define COLOR_SILVER SILVER
#define COLOR_GOLD GOLD
#define COLOR_LIGHT_CYAN LIGHT_CYAN
#define COLOR_LIGHT_YELLOW LIGHT_YELLOW
#define COLOR_LIGHT_GREY LIGHT_GREY
#define COLOR_DARK_RED DARK_RED
#define COLOR_DARK_ORANGE DARK_ORANGE
#define COLOR_VIOLET VIOLET
#define COLOR_INDIGO INDIGO
#define COLOR_TURQUOISE TURQUOISE
#define COLOR_PEACH PEACH
#define COLOR_MINT MINT
#define COLOR_LAVENDER LAVENDER
#define COLOR_CORAL CORAL
#define COLOR_SALMON SALMON
#endif


/* SystemInfo_t -> flag meaning */

#define COLOR_PING_ALL COLOR_BLUE



extern const char* EntriesSettingsMenu[];
extern const char* LEDModeList[];
extern const char* pingMenuEntries[];
extern const char* EntriesSettingsDevMode[];
extern const char* EntriesSettingsTapConfig[];
extern const char* EntriesSettingsNetInfo[];




/*    SCREEN SETTINGS     */
#define SS_BEER_TBODY_W SCREEN_WIDTH / 2  //64
#define SS_BEER_TBODY_H SCREEN_HEIGHT     //64
#define SS_BEER_TBODY_X SCREEN_WIDTH / 2  //64
#define SS_BEER_TBODY_Y 0                 //0

#define SS_BEER_ANI_W SCREEN_WIDTH / 2  //64
#define SS_BEER_ANI_H SCREEN_HEIGHT     //64
#define SS_BEER_ANI_X 0                 //0
#define SS_BEER_ANI_Y 0                 //0

#endif