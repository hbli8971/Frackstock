#include <string.h>
#include "FS_Flasher.h"
#include "esp_system.h"
#include "esp32-hal.h"
#include "FS_memCtrl.h"
#include <ArduinoJson.h>
#include "FS_Encryption.h"
#include "wifi_credentials.h"
#include <hbli_Logging.h>

#define FSF_TIMEOUT_MAX 10000

#define FSF_STARTING_MESSAGE "ESP-RDY"
#define FSF_ENDING_MESSAGE "ESP-DONE"

#define FSF_JSON_IDENTIFIER "id"
#define FSF_JSON_FNAME "fName"
#define FSF_JSON_LNAME "lName"
#define FSF_JSON_SSID "ssid"
#define FSF_JSON_PASSWD "passwd"
#define FSF_JSON_CLR_BEER "clrBeer"
#define FSF_JSON_CLR_BEER_ADD "clrBeerAdd"
#define FSF_JSON_CLR_PING "clrPing"
#define FSF_JSON_VERSION "ver"
#define FSF_JSON_DEVICEMODE "devMode"

#define FSF_JSON_NO_STR "no"
#define FSF_JSON_NO_NR (uint32_t)0xFFFFFFFF

static const char* tag = "flasher";

static const char* errMsg = "ERROR while reading Json-String";
static String strJson = "";
static char jsonBuffer[1024];
static size_t bufferIndex = 0;
static JsonDocument jdoc;

static const char* tempStrChar;
static String tempStr;
static uint32_t tempClr;
static uint16_t temp_nr;
static uint8_t tempU8;
static String identifier_temp;





void FSF_SendStartingMessage() {
  Serial.println(FSF_STARTING_MESSAGE);
}

void FSF_WaitForJsonString() {
  memset(jsonBuffer, 0, sizeof(jsonBuffer));
  bufferIndex = 0;
  bool loopFlag = true;

  while (loopFlag) {
    if (Serial.available() > 0) {
      char incomingChar = Serial.read();

      if (incomingChar == '\n') {
        break;  // Stop reading when a newline is detected
      } else {
        // Ensure we don't overflow the buffer
        if (bufferIndex < 1023) {
          jsonBuffer[bufferIndex++] = incomingChar;
        } else {
          // Handle overflow (could set an error flag or handle otherwise)
          break;
        }
      }
    }
  }
}


void messageConverter(const char* msg, const char* alt) {
  tempStrChar = jdoc[msg] | alt;
  tempStr = String(tempStrChar);
}

void FSF_PerformFlashingRoutine() {
  initEncryption();

  FSF_SendStartingMessage();  //send rdy-message

  FSF_WaitForJsonString();  //wait for first json string

  strJson = String(jsonBuffer);

  DeserializationError error = deserializeJson(jdoc, strJson);

  if (error) {
    Serial.print(F("deserializeJson() returned "));
    Serial.println(error.c_str());
    Serial.println(F("restart Flashing process!"));
    Serial.println(F("Restarting in 3 seconds"));
    delay(3000);
    esp_restart();
    return;
  }

  identifier_temp = String(jdoc[FSF_JSON_IDENTIFIER].as<const char*>());

  //call function to create full key, and save it
  createFullKey(identifier_temp.c_str());

  //encrypt received json string with fullKey
  encryptJsonString(strJson);

  //send json string back to pc
  Serial.println(strJson);

  FSF_WaitForJsonString();  //wait for second json string

  strJson = String(jsonBuffer);

  // decrypt received json string
  if (!decryptJsonString(strJson)) {
    LOGE(tag, "decrypt json string");
  }

  //deserialize
  error = deserializeJson(jdoc, strJson);

  if (error) {
    Serial.print(F("deserializeJson() returned "));
    Serial.println(error.c_str());
    Serial.println(F("restart Flashing process!"));
    Serial.println(F("Restarting in 3 seconds"));
    delay(3000);
    esp_restart();
    return;
  }

  /* IDENTIFIER */
  messageConverter(FSF_JSON_IDENTIFIER, FSF_JSON_NO_STR);
  if (tempStr == FSF_JSON_NO_STR) {
    LOGE(tag, errMsg, "identifier");
  }
  if (!MemSetIdentifier(tempStr)) LOGE(tag, errMsg, "setting id");


  ////////////////////////////////////////////////////////////////////////////////////////

  /* FIRST NAME */
  messageConverter(FSF_JSON_FNAME, FSF_JSON_NO_STR);
  if (tempStr == FSF_JSON_NO_STR) {
    LOGE(tag, errMsg, "first name");
  }

  if (!MemSetFirstName(tempStr)) LOGE(tag, errMsg, "first name");
  ////////////////////////////////////////////////////////////////////////////////////////

  /* LAST NAME */
  messageConverter(FSF_JSON_LNAME, FSF_JSON_NO_STR);
  if (tempStr == FSF_JSON_NO_STR) {
    LOGE(tag, errMsg, "last name");
  }

  if (!MemSetLastName(tempStr)) LOGE(tag, errMsg, "last name");
  ////////////////////////////////////////////////////////////////////////////////////////

  /* WIFI SSID */
  messageConverter(FSF_JSON_SSID, FSF_JSON_NO_STR);
  if (tempStr == FSF_JSON_NO_STR) {
    LOGE(tag, errMsg, "ssid");
  }

  if (!MemSetSSID(tempStr)) LOGE(tag, errMsg, "ssid");
  ////////////////////////////////////////////////////////////////////////////////////////

  /* WIFI PASSWORD */
  messageConverter(FSF_JSON_PASSWD, FSF_JSON_NO_STR);
  if (tempStr == FSF_JSON_NO_STR) {
    LOGE(tag, errMsg, "password");
  }

  if (!MemSetPassword(tempStr)) LOGE(tag, errMsg, "password");
  ////////////////////////////////////////////////////////////////////////////////////////

  // clr1 = strtoul(color1Str, NULL, 10);

  /* BEER COLOR */
  tempClr = jdoc[FSF_JSON_CLR_BEER] | FSF_JSON_NO_NR;
  if (tempClr == FSF_JSON_NO_NR) {
    LOGE(tag, errMsg, "color-beer");
  }

  if (!MemSetColorBeer(tempClr)) LOGE(tag, errMsg, "color-beer");
  ////////////////////////////////////////////////////////////////////////////////////////

  // clr1 = strtoul(color1Str, NULL, 10);

  /* BEER COLOR INVREMENT */
  tempClr = jdoc[FSF_JSON_CLR_BEER_ADD] | FSF_JSON_NO_NR;
  if (tempClr == FSF_JSON_NO_NR) {
    LOGE(tag, errMsg, "color-beer add");
  }

  if (!MemSetColorBeerAdd(tempClr)) LOGE(tag, errMsg, "color-beer add");
  ////////////////////////////////////////////////////////////////////////////////////////



  /* COLOR PING */
  tempClr = jdoc[FSF_JSON_CLR_PING] | FSF_JSON_NO_NR;
  if (tempClr == FSF_JSON_NO_NR) {
    LOGE(tag, errMsg, "color-ping");
  }
  if (!MemSetColorPing(tempClr)) LOGE(tag, errMsg, "color-ping");

  ////////////////////////////////////////////////////////////////////////////////////////

  /* SOFTWARE VERSION */
  temp_nr = jdoc[FSF_JSON_CLR_PING] | FSF_JSON_NO_NR;
  if (tempClr == FSF_JSON_NO_NR) {
    LOGE(tag, errMsg, "version");
  }

  if (!MemSetVersion(temp_nr)) LOGE(tag, errMsg, "version");
  ////////////////////////////////////////////////////////////////////////////////////////

  /* DEVICE MODE */
  tempU8 = jdoc[FSF_JSON_DEVICEMODE] | FSF_JSON_NO_NR;
  if (tempClr == FSF_JSON_NO_NR) {
    LOGE(tag, errMsg, "device mode");
  }

  if (!MemSetDeviceMode(tempU8)) LOGE(tag, errMsg, "device mode");


  Serial.println(FSF_ENDING_MESSAGE);
  freeFullKey();
}