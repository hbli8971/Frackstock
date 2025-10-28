#ifndef _FS_NETWORK_H
#define _FS_NETWORK_H

#include <WiFi.h>
// #include <WiFiClientSecure.h>
#include <MQTT.h>
#include "WString.h"
#include "config.h"
#include "FS_Datatypes.h"

enum wifiInfoDescriptor {
  NO_MEANING,
  PUBLIC_PING_RECEIVED,
  PRIVATE_PING_RECEIVED
};


extern wifiInfo_t net;
extern MemInfo_t mem;
extern SystemInfo_t sys;

extern char usrList[MQTT_MAX_USERS][MAX_FULL_NAME_LENGTH];
extern const char* usrPtrs[MQTT_MAX_USERS];

void initNetwork(network_t* config);
void FS_Network_Start();
void FS_Network_stop();

bool mqttSendMessage(const char* topic);
bool mqttSendMessage(const char* topic, const char* msg);

void FS_Network_SetNewPingFlag();
void FS_Network_ClearNewPingFlag();
bool FS_Network_GetNewPingFlag();
void FS_Network_SetPingSource(String Psource);
String FS_Network_GetPingSource();
void FS_Network_SetRSSI(int32_t rssi);
int32_t FS_Network_GetRSSI();
void FS_Network_SetIpAddr(String ip);
String FS_Network_GetIpAddr();
void FS_Network_SetMacAddr(String mac);
String FS_Network_GetMacAddr();
void FS_Network_SetWiFiStatus(bool status);
bool FS_Network_GetWiFiStatus();
void FS_Network_SetMqttStatus(bool status);
bool FS_Network_GetMqttStatus();
void FS_Network_SetUsrListFlag();
void FS_Network_ResetUsrListFlag();
bool FS_Network_GetUsrListFlag();
void FS_Network_SetMiscInfo(uint8_t val);
uint8_t FS_Network_GetMiscInfo();
uint8_t FS_Network_GetUserListAmount();








#endif