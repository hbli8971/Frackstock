#ifndef _FS_DATATYPES_H
#define _FS_DATATYPES_H

#include <Arduino.h>

/* SystemInfo-Flag  */
#define SYS_FLAG_SYNC_MODE (1 << 0)




/* network types  */
//carries information about a received ping
typedef struct {
  volatile bool newPingReceived;
  uint8_t padding1;
  uint16_t padding2;
  String source;
} pingInfo_t;

//carries information about the network-related stuff
typedef struct {
  int32_t RSSI;
  String IpAddress;
  String MacAddress;
  pingInfo_t PingInfo;
  volatile bool wifiConnected;
  volatile bool mqttConnected;
  volatile bool userListUpdated;
  volatile uint8_t misc;
} wifiInfo_t;

//read the name... u can guess
typedef struct {
  String SSID;
  String Password;
} WifiCredentials_t;

//for init of the network, names should suffice
typedef struct {
  const char* ssid;
  const char* pw;
  const char* mqtt_userName;
  const char* mqtt_pw;
  const char* mqtt_ip;
  const char* mqtt_identifier;
  int mqtt_port;
} network_t;
///////////////////////////////////////////////////////////////////////////////////////////

typedef struct {
  String Identifier;
  WifiCredentials_t wifiCred;
  String FirstName;
  String LastName;
  uint32_t ColorBeer;
  uint32_t ColorBeerIncrement;
  uint32_t ColorPing;
  uint16_t BatCalib;
  uint16_t DeviceVersion;
  uint8_t DeviceMode;
  uint8_t TapRange;
  uint8_t TapThresh;
  uint8_t TapTimeLim;
  uint8_t TapTimeLatency;
  uint8_t TapWindow;
  //+2 Padding :/
} MemInfo_t;

typedef struct {
  uint32_t timerTapDetectedLast;
  uint32_t timerTapDebounce;
  volatile bool flagTapInterrupt;
  volatile bool flagTapEnabled;
  //+2 padding :(
} TapDetect_t;

typedef struct {
  volatile uint8_t tapCount;
  volatile uint8_t tapCountTemp;
  volatile uint8_t tapCountTempOld;
  volatile uint8_t flag;

  volatile uint32_t currentTime;

  volatile uint16_t currentMenuId;
  volatile uint16_t currentMenuIdOld;

  volatile bool AdminPrivileg;
  //+3 padding :(
} SystemInfo_t;

typedef struct {
  uint8_t range;
  uint8_t threshhold;
  uint8_t timeLim;
  uint8_t timeLatency;
  uint8_t window;
} Lis3dh_t;

#endif