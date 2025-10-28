#include "esp_wifi_types_generic.h"
#include "WiFiGeneric.h"
#include "esp32-hal.h"
#include "WiFiType.h"
#include <sys/_stdint.h>
#include <stdlib.h>
#include "FS_Network.h"

#include <ArduinoJson.h>
#include <hbli_Logging.h>

#include "wifi_credentials.h"

#include "FS_memCtrl.h"

#include "FS_Led.h"

#include "FS_MenuDefinition.h"

#define FS_NETWORK_EVT_NETWORK_MODE (1 << 0)
#define FS_NETWORK_EVT_TERMINATE_NETWORK (1 << 1)
#define FS_NETWORK_EVT_TERMINATION_DONE (1 << 2)
#define FS_NETWORK_EVT_NETWORK_MODE_SWITCHED (1 << 3)

#define MQTT_LAST_WILL_TOPIC(x) (String(x) + '/' + MSG_DISCONNECT).c_str()

#define MQTT_SUBSCRIBE_QOS 2
#define MQTT_PUBLISH_QOS 2

// WiFiClientSecure espClient;
WiFiClient espClient;
MQTTClient client(512, 256);
JsonDocument jdocNet;

static const char* tag = "net-task";

SemaphoreHandle_t xNetworkMutex;  //access to wifiInfo_t
EventGroupHandle_t xNetworkEventGroup;


static volatile bool networkTaskLoop = true;
static uint32_t _timerNetwork = 0;
static uint32_t _timerNetworkConnectionCheck = 0;
static uint8_t wifiReconnectionTries = 0;
static uint8_t mqttReconnectionTries = 0;
static bool wifiConnected, wifiConnectedOld = false;


// static char payload[MQTT_BUFFER_SIZE];             //deprecated, will be removed later
char usrList[MQTT_MAX_USERS][MAX_FULL_NAME_LENGTH];  //user list (global)
const char* usrPtrs[MQTT_MAX_USERS];                 //pointer array to users in list
static uint8_t usrListAmount;                        //the amount of users in the received list
static uint8_t msgType = 0;                          //holds message type for async response
static String _tempRssiStr;                          //used in con-check
static const char* _tempRssiCStr;                    //used in con-check
static char tempBeerStr[4];                          //used when requesting beer counter




// data from network_t, gets copied on initNetwork
static const char* _ssid;
static const char* _pw;
static const char* _mqtt_userName;
static const char* _mqtt_pw;
static const char* _mqtt_ip;
static const char* _mqtt_identifier;
static int _mqtt_port;

void messageReceivedCallback(String& topic, String& payload);
bool mqttSendMessage(const char* topic);
bool mqttSendMessage(const char* topic, const char* msg);


void FS_Network_Lock_wifiInfo() {
  xSemaphoreTake(xNetworkMutex, portMAX_DELAY);
}

void FS_Network_Release_wifiInfo() {
  xSemaphoreGive(xNetworkMutex);
}

// wifiInfo interface
void FS_Network_SetRSSI(int32_t rssi) {
  FS_Network_Lock_wifiInfo();
  net.RSSI = rssi;
  FS_Network_Release_wifiInfo();
}

int32_t FS_Network_GetRSSI() {
  int32_t ret;
  FS_Network_Lock_wifiInfo();
  ret = net.RSSI;
  FS_Network_Release_wifiInfo();
  return ret;
}

void FS_Network_SetIpAddr(String ip) {
  FS_Network_Lock_wifiInfo();
  net.IpAddress = ip;
  FS_Network_Release_wifiInfo();
}

String FS_Network_GetIpAddr() {
  String ret;
  FS_Network_Lock_wifiInfo();
  ret = net.IpAddress;
  FS_Network_Release_wifiInfo();
  return ret;
}

void FS_Network_SetMacAddr(String mac) {
  FS_Network_Lock_wifiInfo();
  net.MacAddress = mac;
  FS_Network_Release_wifiInfo();
}

String FS_Network_GetMacAddr() {
  String ret;
  FS_Network_Lock_wifiInfo();
  ret = net.MacAddress;
  FS_Network_Release_wifiInfo();
  return ret;
}

void FS_Network_SetWiFiStatus(bool status) {
  FS_Network_Lock_wifiInfo();
  net.wifiConnected = status;
  FS_Network_Release_wifiInfo();
}

bool FS_Network_GetWiFiStatus() {
  bool ret;
  FS_Network_Lock_wifiInfo();
  ret = net.wifiConnected;
  FS_Network_Release_wifiInfo();
  return ret;
}

void FS_Network_SetMqttStatus(bool status) {
  FS_Network_Lock_wifiInfo();
  net.mqttConnected = status;
  FS_Network_Release_wifiInfo();
}

bool FS_Network_GetMqttStatus() {
  bool ret;
  FS_Network_Lock_wifiInfo();
  ret = net.mqttConnected;
  FS_Network_Release_wifiInfo();
  return ret;
}

void FS_Network_SetUsrListFlag() {
  FS_Network_Lock_wifiInfo();
  net.userListUpdated = true;
  FS_Network_Release_wifiInfo();
}

void FS_Network_ResetUsrListFlag() {
  FS_Network_Lock_wifiInfo();
  net.userListUpdated = false;
  FS_Network_Release_wifiInfo();
}

bool FS_Network_GetUsrListFlag() {
  bool res;
  FS_Network_Lock_wifiInfo();
  res = net.userListUpdated;
  if (res) {
    net.userListUpdated = false;
  }
  FS_Network_Release_wifiInfo();
  return res;
}

void FS_Network_SetMiscInfo(uint8_t val) {
  FS_Network_Lock_wifiInfo();
  net.misc = val;
  FS_Network_Release_wifiInfo();
}

uint8_t FS_Network_GetMiscInfo() {
  uint8_t res;
  FS_Network_Lock_wifiInfo();
  res = net.misc;
  FS_Network_Release_wifiInfo();
  return res;
}

uint8_t FS_Network_GetUserListAmount() {
  return usrListAmount;
}

//pingInfo interface:

//new ping flag
void FS_Network_SetNewPingFlag() {
  FS_Network_Lock_wifiInfo();
  net.PingInfo.newPingReceived = true;
  FS_Network_Release_wifiInfo();
}

void FS_Network_ClearNewPingFlag() {
  FS_Network_Lock_wifiInfo();
  net.PingInfo.newPingReceived = false;
  FS_Network_Release_wifiInfo();
}

bool FS_Network_GetNewPingFlag() {
  bool res;
  FS_Network_Lock_wifiInfo();
  res = net.PingInfo.newPingReceived;
  FS_Network_Release_wifiInfo();
  return res;
}

//Source
void FS_Network_SetPingSource(String Psource) {
  FS_Network_Lock_wifiInfo();
  net.PingInfo.source = Psource;
  FS_Network_Release_wifiInfo();
}

String FS_Network_GetPingSource() {
  String res;
  FS_Network_Lock_wifiInfo();
  res = net.PingInfo.source;
  FS_Network_Release_wifiInfo();
  return res;
}

void FS_Network_Start() {
  if (!(xEventGroupGetBits(xNetworkEventGroup) & FS_NETWORK_EVT_NETWORK_MODE)) {
    xEventGroupSetBits(xNetworkEventGroup, FS_NETWORK_EVT_NETWORK_MODE | FS_NETWORK_EVT_NETWORK_MODE_SWITCHED);
  }
}

void FS_Network_stop() {
  if (xEventGroupGetBits(xNetworkEventGroup) & FS_NETWORK_EVT_NETWORK_MODE) {
    xEventGroupClearBits(xNetworkEventGroup, FS_NETWORK_EVT_NETWORK_MODE);
    xEventGroupSetBits(xNetworkEventGroup, FS_NETWORK_EVT_NETWORK_MODE_SWITCHED);
  }
}

void setupMQTT() {

  client.setWill(MQTT_LAST_WILL_TOPIC(_mqtt_identifier), "client disconnected", false, 1);
  client.begin(_mqtt_ip, _mqtt_port, espClient);  //set server connection
  client.onMessage(messageReceivedCallback);      //set callback
}

void connectToWiFi() {
  WiFi.begin(_ssid, _pw);
}

void connectToMQTT() {
  if (client.connect(_mqtt_identifier, _mqtt_userName, _mqtt_pw)) {
    mqttSendMessage(MSG_USR_JOINED, String(mem.FirstName + "," + mem.LastName).c_str());

    if (client.subscribe(MQTT_PUBLIC_TOPIC_SUBSCRIBE, MQTT_SUBSCRIBE_QOS)) {
      LOGI(tag, F("Subscribed to "), MQTT_PUBLIC_TOPIC_SUBSCRIBE);
    } else {
      LOGE(tag, F("Failed to subscribe to"), MQTT_PUBLIC_TOPIC_SUBSCRIBE);
    }

    String _tempTopic = String(_mqtt_identifier) + "/#";

    if (client.subscribe(_tempTopic, MQTT_SUBSCRIBE_QOS)) {
      LOGI(tag, F("Subscribed to "), _tempTopic);
    } else {
      LOGE(tag, F("Failed to subscribe to"), _tempTopic);
    }

  } else {
    LOGE(tag, F("MQTT-Connection failed"), F("retrying"));
    FS_Network_SetMqttStatus(false);
  }
}

void checkConnections() {
  wifiConnectedOld = wifiConnected;
  if (WiFi.status() != WL_CONNECTED) {
    FS_Network_SetWiFiStatus(false);
    FS_Network_SetMqttStatus(false);
    wifiConnected = false;
    wifiReconnectionTries++;
    connectToWiFi();
    LOGI(tag, F("connecting to WiFi"));
  } else if (!client.connected()) {
    FS_Network_SetMqttStatus(false);
    FS_Network_SetWiFiStatus(true);
    wifiConnected = true;
    wifiReconnectionTries = 0;
    mqttReconnectionTries++;
    connectToMQTT();
    LOGI(tag, F("connecting to MQTT"));
  } else {
    wifiReconnectionTries = 0;
    mqttReconnectionTries = 0;
    FS_Network_SetMqttStatus(true);
    FS_Network_SetWiFiStatus(true);
  }

  if (wifiConnected != wifiConnectedOld) {
    if (wifiConnected) {
      FS_Network_SetIpAddr(String(WiFi.localIP()));
    } else {
      FS_Network_SetIpAddr(String("0.0.0.0"));
    }
  }
}

void NetworkTask(void* pvParameters) {
  network_t* config = (network_t*)pvParameters;

  _ssid = config->ssid;
  _pw = config->pw;
  _mqtt_userName = config->mqtt_userName;
  _mqtt_pw = config->mqtt_pw;
  _mqtt_ip = config->mqtt_ip;
  _mqtt_identifier = config->mqtt_identifier;
  _mqtt_port = config->mqtt_port;


  LOGI(tag, "ssid", _ssid);
  LOGI(tag, "password", WC_SHOW_PW ? _pw : "** HIDDEN **");
  LOGI(tag, "username", _mqtt_userName);
  LOGI(tag, "mqtt password", _mqtt_pw);
  LOGI(tag, "ip", _mqtt_ip);
  LOGI(tag, "id", _mqtt_identifier);
  LOGI(tag, "port", _mqtt_port);

  setupMQTT();

  if (mem.DeviceMode == 1) {
    /*INITIAL WIFI CONNECTION*/
    connectToWiFi();
    _timerNetworkConnectionCheck = millis();
    while (WiFi.status() != WL_CONNECTED) {

      if (millis() - _timerNetworkConnectionCheck >= TIME_CHECK_CONNECTIONS) {
        _timerNetworkConnectionCheck = millis();
        connectToWiFi();
      }
      vTaskDelay(100 / portTICK_PERIOD_MS);
    }
    /*INITIAL WIFI CONNECTION*/
    wifiConnected = true;
    wifiConnectedOld = true;
    FS_Network_SetIpAddr(String(WiFi.localIP()));

    connectToMQTT();  //initial MQTT-Connection

  } else {
    FS_Network_stop();
    WiFi.mode(WIFI_OFF);
  }

  // connectToWiFi();
  // _timerNetworkConnectionCheck = millis();
  // while (WiFi.status() != WL_CONNECTED) {

  //   if (millis() - _timerNetworkConnectionCheck >= TIME_CHECK_CONNECTIONS) {
  //     _timerNetworkConnectionCheck = millis();
  //     connectToWiFi();
  //   }
  //   vTaskDelay(100 / portTICK_PERIOD_MS);
  // }
  // /*INITIAL WIFI CONNECTION*/

  // connectToMQTT();  //initial MQTT-Connection

  while (networkTaskLoop) {
    _timerNetwork = millis();

    EventBits_t uxBits = xEventGroupWaitBits(xNetworkEventGroup, FS_NETWORK_EVT_NETWORK_MODE, pdFALSE, pdFALSE, 0);

    if (uxBits & FS_NETWORK_EVT_NETWORK_MODE) {

      if (uxBits & FS_NETWORK_EVT_NETWORK_MODE_SWITCHED) {
        xEventGroupClearBits(xNetworkEventGroup, FS_NETWORK_EVT_NETWORK_MODE_SWITCHED);
        WiFi.mode(WIFI_STA);
      }

      if (_timerNetwork - _timerNetworkConnectionCheck >= TIME_CHECK_CONNECTIONS) {
        checkConnections();
        FS_Network_SetRSSI(WiFi.RSSI());
        _timerNetworkConnectionCheck = _timerNetwork;
      }

      //handle async response (MQTT)
      if (msgType != msgType_NONE) {
        switch (msgType) {
          case msgType_ConCheck:
            _tempRssiStr = String(WiFi.RSSI());
            _tempRssiCStr = _tempRssiStr.c_str();
            mqttSendMessage(MSG_CON_CHECK_RETURN, _tempRssiCStr);
            break;
          case msgType_BeerCtn:
            itoa(MemGetBeerCounter(), tempBeerStr, 10);
            mqttSendMessage(MSG_RECEIVED, tempBeerStr);
            break;
          case msgType_LoginFailed:
            mqttSendMessage(MSG_CFG_CONFIRM, "Login failed");
            break;
          case msgType_LoginSuccess:
            mqttSendMessage(MSG_CFG_CONFIRM, "Login successful");
            break;
          case msgType_OperationFailed:
            mqttSendMessage(MSG_CFG_CONFIRM, "Operation Failed");
            break;
          case msgType_CfgNoRights:
            mqttSendMessage(MSG_CFG_CONFIRM, "No Admin Rights!");
            break;
          case msgType_Ident_set:
            mqttSendMessage(MSG_CFG_CONFIRM, "Identifier set");
            break;
          case msgType_FName_set:
            mqttSendMessage(MSG_CFG_CONFIRM, "First Name set");
            break;
          case msgType_LName_set:
            mqttSendMessage(MSG_CFG_CONFIRM, "Last Name set");
            break;
          case msgType_ClrBeer_set:
            mqttSendMessage(MSG_CFG_CONFIRM, "Color Beer set");
            break;
          case msgType_ClrBeerInc_set:
            mqttSendMessage(MSG_CFG_CONFIRM, "Color Beer increment set");
            break;
          case msgType_ClrPing_set:
            mqttSendMessage(MSG_CFG_CONFIRM, "Color Ping set");
            break;
          case msgType_DevMode_set:
            mqttSendMessage(MSG_CFG_CONFIRM, "Device Mode set");
            break;
          case msgType_BatCalib_set:
            mqttSendMessage(MSG_CFG_CONFIRM, "Battery Calibration set");
            break;
          case msgType_BeerCtn_set:
            mqttSendMessage(MSG_CFG_CONFIRM, "Beer Counter set");
            break;
          case msgType_TapScale_set:
            mqttSendMessage(MSG_CFG_CONFIRM, "Tap-Scale set");
            break;
          case msgType_TapThresh_set:
            mqttSendMessage(MSG_CFG_CONFIRM, "Tap-Threshhold set");
            break;
          default:
            mqttSendMessage(MSG_CFG_CONFIRM, "error in msg-Type");
            break;
        }
        msgType = msgType_NONE;
      }

      if (FS_Network_GetMqttStatus()) {
        client.loop();
      }
      vTaskDelay(25 / portTICK_PERIOD_MS);
    } else {
      if (uxBits & FS_NETWORK_EVT_NETWORK_MODE_SWITCHED) {
        xEventGroupClearBits(xNetworkEventGroup, FS_NETWORK_EVT_NETWORK_MODE_SWITCHED);
        client.disconnect();
        WiFi.disconnect();
        WiFi.mode(WIFI_OFF);
      }
      vTaskDelay(500 / portTICK_PERIOD_MS);
    }
  }
}

void initNetwork(network_t* config) {
  xNetworkMutex = xSemaphoreCreateMutex();
  xNetworkEventGroup = xEventGroupCreate();
  xTaskCreate(NetworkTask, "Network-task", 4096, (void*)config, 4, NULL);
  FS_Network_Start();
  FS_Network_SetMacAddr(WiFi.macAddress());
}

bool mqttSendMessage(const char* topic) {
  String FullTopic = String(_mqtt_identifier) + "/" + String(topic);
  return client.publish(FullTopic, String(MSG_DEFAULT), false, MQTT_PUBLISH_QOS);
}

bool mqttSendMessage(const char* topic, const char* msg) {
  String FullTopic = String(_mqtt_identifier) + "/" + String(topic);  // clientID/topic
  return client.publish(FullTopic, String(msg), false, MQTT_PUBLISH_QOS);
}

bool checkTopic(String topic, String subStr) {
  bool res;
  (topic.indexOf(subStr) != -1) ? res = true : res = false;
  return res;
}

void processUserList() {
  // Extract the number of users
  size_t usrAmount = jdocNet["UsrCtn"].as<size_t>();

  usrListAmount = usrAmount;
  LOGI(tag, F("userCount is"), usrListAmount);
  // Check for array bounds
  if (usrAmount > MQTT_MAX_USERS) {
    LOGE(tag, F("process user list"), F("too many users"));
    usrAmount = MQTT_MAX_USERS;  // Limiting to the maximum size
  }

  // Clear the usrList array before populating it
  memset(usrList, 0, sizeof(usrList));

  // Iterate over the U1, U2, ..., Un keys and extract user names
  for (size_t i = 1; i <= usrAmount; i++) {
    char key[4];  // Assuming maximum number of users is less than 100
    snprintf(key, sizeof(key), "U%u", i);
    const char* fullName = jdocNet[key];

    LOGV("Extracted user name for key ", key, fullName);

    if (fullName != nullptr) {
      // Calculate the actual length of the full name (capping it at FULL_NAME_LENGTH - 1)
      size_t nameLength = strnlen(fullName, MAX_FULL_NAME_LENGTH - 1);

      // Copy the full name into usrList, ensuring null termination
      strncpy(usrList[i - 1], fullName, nameLength);

      // Place the null terminator immediately after the last character
      usrList[i - 1][nameLength] = '\0';  // Proper null termination

      // Set pointer to the user name
      usrPtrs[i - 1] = usrList[i - 1];
    }
  }

  FS_Network_Lock_wifiInfo();
  net.userListUpdated = true;
  FS_Network_Release_wifiInfo();
}

void messageReceivedCallback(String& topic, String& payload) {
  const char* payloadChar = payload.c_str();
  int msgIndex = -1;

  int slashIndex = topic.indexOf('/');
  if (slashIndex != -1 && topic.length() >= slashIndex + 4) {
    String extractedNumbers = topic.substring(slashIndex + 1, slashIndex + 4);
    msgIndex = extractedNumbers.toInt();
    LOGI(tag, F("Topic identifier is"), msgIndex);
  } else {
    LOGE(tag, F("Invalid Topic structure"));
  }

  if (msgIndex < 300) {
    return;
  }

  DeserializationError error = deserializeJson(jdocNet, payloadChar);

  if (error) {
    LOGE(tag, F("mqtt callback"), F("error on deserialization"));
    Serial.print("deserializeJson() failed: ");
    Serial.println(error.c_str());
  }

  String tempString;
  const char* _clr1Str;
  const char* _clr2Str;
  uint32_t tempU32_1;
  uint32_t tempU32_2;
  uint8_t tempByte;
  uint16_t tempU16;

  switch (msgIndex) {
    case CODE_MSG_USR_LIST:
      processUserList();
      break;
    case CODE_MSG_TIME_UPDATE:
      break;
    case CODE_MSG_PING_ALL:
      tempString = String(jdocNet["source"].as<const char*>());  //read source
      FS_Network_SetPingSource(tempString);                      //set source
      FS_Network_SetMiscInfo(PUBLIC_PING_RECEIVED);              //set pingtype
      FS_Network_SetNewPingFlag();                               //indicate new ping received
      MM.goTo(&PingReceived);                                    //change menu
      break;
    case CODE_MSG_PING_USR:
      tempString = String(jdocNet["source"].as<const char*>());  //read source
      FS_Network_SetPingSource(tempString);                      //set source
      FS_Network_SetMiscInfo(PRIVATE_PING_RECEIVED);             //set PingType
      FS_Network_SetNewPingFlag();                               //indicate new ping received
      MM.goTo(&PingReceived);                                    //change menu
      break;
    case CODE_MSG_BEER_CTN:
      msgType = msgType_BeerCtn;
      break;
    case CODE_MSG_SYNC_MODE:
      sys.flag |= SYS_FLAG_SYNC_MODE;
      if (!DEVICE_MASTER) {
        MM.goTo(&SyncModeScreen);
      }
      break;
    case CODE_MSG_SYNC_RELEASE:
      FS_SetLed(COLOR_BLACK, COLOR_BLACK, 1, ledModeStatic, 1, 0);
      sys.flag &= ~(SYS_FLAG_SYNC_MODE);
      MM.goBackToRoot();
      break;
    case CODE_MSG_LED_CONTROL:
      _clr1Str = jdocNet["color1"];
      _clr2Str = jdocNet["color2"];
      tempU32_1 = strtoul(&_clr1Str[1], NULL, 16);
      tempU32_2 = strtoul(&_clr2Str[1], NULL, 16);
      if (LOGIC_INVERSION) {
        tempU32_1 = ~tempU32_1;
        tempU32_2 = ~tempU32_2;
      }
      FS_SetLed(tempU32_1, tempU32_2, jdocNet["duration"], jdocNet["mode"], -10, FS_LED_NO_CLEAR);
      break;
    case CODE_MSG_CON_CHECK:
      msgType_ConCheck;
      break;
    case CODE_CFG_CLIENT_IDENT:
      if (sys.AdminPrivileg) {
        tempString = String(jdocNet["cfg"].as<const char*>());

        if (MemSetIdentifier(tempString)) {
          msgType = msgType_Ident_set;
        } else {
          msgType = msgType_OperationFailed;
        }

      } else {
        LOGE(tag, F("setting Identifier"), F("no admin rights"));
        msgType = msgType_CfgNoRights;
      }
      break;
    case CODE_CFG_USR_FNAME:
      if (sys.AdminPrivileg) {
        tempString = String(jdocNet["cfg"].as<const char*>());

        if (MemSetFirstName(tempString)) {
          msgType = msgType_FName_set;
        } else {
          msgType = msgType_OperationFailed;
        }

      } else {
        LOGE(tag, F("setting First name"), F("no admin rights"));
        msgType = msgType_CfgNoRights;
      }
      break;
    case CODE_CFG_USR_LNAME:
      if (sys.AdminPrivileg) {
        tempString = String(jdocNet["cfg"].as<const char*>());

        if (MemSetLastName(tempString)) {
          msgType = msgType_LName_set;
        } else {
          msgType = msgType_OperationFailed;
        }

      } else {
        LOGE(tag, F("setting Last Name"), F("no admin rights"));
        msgType = msgType_CfgNoRights;
      }
      break;
    case CODE_CFG_CLR_BEER:
      if (sys.AdminPrivileg) {
        tempU32_1 = jdocNet["cfg"];

        if (LOGIC_INVERSION) {
          tempU32_1 = ~tempU32_1;
        }

        if (MemSetColorBeer(tempU32_1)) {
          msgType = msgType_ClrBeer_set;
          LOGI(tag, F("new color is"), tempU32_1);
        } else {
          msgType = msgType_OperationFailed;
        }

      } else {
        LOGE(tag, F("setting Color Beer"), F("no admin rights"));
        msgType = msgType_CfgNoRights;
      }
      break;
    case CODE_CFG_CLR_BEER_INC:
      if (sys.AdminPrivileg) {
        tempU32_1 = jdocNet["cfg"];
        if (LOGIC_INVERSION) {
          tempU32_1 = ~tempU32_1;
        }

        if (MemSetColorBeerAdd(tempU32_1)) {
          msgType = msgType_ClrBeerInc_set;
          LOGI(tag, F("new color is"), tempU32_1);
        } else {
          msgType = msgType_OperationFailed;
        }

      } else {
        LOGE(tag, F("setting Color Beer Add"), F("no admin rights"));
        msgType = msgType_CfgNoRights;
      }
      break;
    case CODE_CFG_CLR_PING:
      if (sys.AdminPrivileg) {
        tempU32_1 = jdocNet["cfg"];

        if (LOGIC_INVERSION) {
          tempU32_1 = ~tempU32_1;
        }

        if (MemSetColorPing(tempU32_1)) {
          msgType = msgType_ClrPing_set;
          LOGI(tag, F("new color is"), tempU32_1);
        } else {
          msgType = msgType_OperationFailed;
        }

      } else {
        LOGE(tag, F("setting Color Ping"), F("no admin rights"));
        msgType = msgType_CfgNoRights;
      }
      break;
    case CODE_CFG_BEER_COUNT:
      if (sys.AdminPrivileg) {
        tempByte = jdocNet["cfg"];
        if (MemSetBeerCounter(tempByte)) {
          msgType = msgType_BeerCtn_set;
        } else {
          msgType = msgType_OperationFailed;
        }

      } else {
        LOGE(tag, F("setting beer counter"), F("no admin rights"));
        msgType = msgType_CfgNoRights;
      }
      break;
    case CODE_CFG_DEV_MODE:
      if (sys.AdminPrivileg) {
        tempByte = jdocNet["cfg"];

        if (MemSetDeviceMode(tempByte)) {
          msgType = msgType_DevMode_set;
        } else {
          msgType = msgType_OperationFailed;
        }

      } else {
        LOGE(tag, F("setting Device Mode"), F("no admin rights"));
        msgType = msgType_CfgNoRights;
      }
      break;
    case CODE_CFG_BAT_CALIB:
      if (sys.AdminPrivileg) {
        tempU16 = jdocNet["cfg"];

        if (MemSetBatCalib(tempU16)) {
          msgType = msgType_BatCalib_set;
        } else {
          msgType = msgType_OperationFailed;
        }

      } else {
        LOGE(tag, F("setting Bat-Calib"), F("no admin rights"));
        msgType = msgType_CfgNoRights;
      }
      break;
    case CODE_ADMIN_LOGIN:
      sys.AdminPrivileg = false;
      if (jdocNet["user"] == MQTT_ADMIN_USER) {
        if (jdocNet["pw"] == MQTT_ADMIN_PASSWORD) {
          sys.AdminPrivileg = true;
        }
      }
      if (sys.AdminPrivileg) {
        msgType = msgType_LoginSuccess;
      } else {
        msgType = msgType_LoginFailed;
      }
      break;
    case CODE_CFG_TAP_SCALE:
      if (sys.AdminPrivileg) {
        tempByte = jdocNet["cfg"];

        if (MemSetTapRange(tempByte)) {
          msgType = msgType_TapScale_set;
        } else {
          msgType = msgType_OperationFailed;
        }

      } else {
        LOGE(tag, F("setting Tap-Scale"), F("no admin rights"));
        msgType = msgType_CfgNoRights;
      }
      break;
    case CODE_CFG_TAP_THRESH:
      if (sys.AdminPrivileg) {
        tempByte = jdocNet["cfg"];

        if (MemSetTapThreshhold(tempByte)) {
          msgType = msgType_TapThresh_set;
        } else {
          msgType = msgType_OperationFailed;
        }

      } else {
        LOGE(tag, F("setting Tap-Threshhold"), F("no admin rights"));
        msgType = msgType_CfgNoRights;
      }
      break;
  }
  LOGI(tag, topic, payload);
}