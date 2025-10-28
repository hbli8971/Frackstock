#include "EEPROM.h"
#include <sys/_stdint.h>
#include "FS_memCtrl.h"
#include "wifi_credentials.h"
#include "DPOS_Logging.h"
#include "config.h"

static const char* tag = "EEPROM";
static const char* tl = "too long";

void initEEPROM() {
  LOGI(tag, "EEPROM-status", EEPROM.begin(MEM_EEPROM_SIZE));
}

void MemPrintAddresses() {
  LOGI(tag, F("MEM_ADDR_MEM_CHECK"), MEM_ADDR_MEM_CHECK);
  LOGI(tag, F("MEM_ADDR_IDENTIFIER"), MEM_ADDR_IDENTIFIER);
  LOGI(tag, F("MEM_ADDR_BEERCOUNTER"), MEM_ADDR_BEERCOUNTER);
  LOGI(tag, F("MEM_ADDR_FIRST_NAME"), MEM_ADDR_FIRST_NAME);
  LOGI(tag, F("MEM_ADDR_LAST_NAME"), MEM_ADDR_LAST_NAME);
  LOGI(tag, F("MEM_ADDR_SSID"), MEM_ADDR_SSID);
  LOGI(tag, F("MEM_ADDR_PASSWORD"), MEM_ADDR_PASSWORD);
  LOGI(tag, F("MEM_ADDR_COLOR_BEER"), MEM_ADDR_COLOR_BEER);
  LOGI(tag, F("MEM_ADDR_COLOR_BEER_ADD"), MEM_ADDR_COLOR_BEER_ADD);
  LOGI(tag, F("MEM_ADDR_COLOR_PING"), MEM_ADDR_COLOR_PING);
  LOGI(tag, F("MEM_ADDR_BAT_CALIB"), MEM_ADDR_BAT_CALIB);
  LOGI(tag, F("MEM_ADDR_VERSION"), MEM_ADDR_VERSION);
  LOGI(tag, F("MEM_ADDR_DEVICEMODE"), MEM_ADDR_DEVICEMODE);
  LOGI(tag, F("MEM_ADDR_TAP_RANGE"), MEM_ADDR_TAP_RANGE);
  LOGI(tag, F("MEM_ADDR_TAP_THRESH"), MEM_ADDR_TAP_THRESH);
  LOGI(tag, F("MEM_ADDR_TAP_TIME_LIM"), MEM_ADDR_TAP_TIME_LIM);
  LOGI(tag, F("MEM_ADDR_TAP_TIME_LATENCY"), MEM_ADDR_TAP_TIME_LATENCY);
  LOGI(tag, F("MEM_ADDR_TAP_WINDOW"), MEM_ADDR_TAP_WINDOW);
}

bool writeByteToEEPROM(int address, uint8_t value) {
  if (address < 0 || address >= MEM_EEPROM_SIZE) {
    return false;
  }
  EEPROM.write(address, value);
  return EEPROM.commit();
}

uint8_t readByteFromEEPROM(int address) {
  return EEPROM.read(address);
}
///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////



String MemGetIdentifier() {
  return EEPROM.readString(MEM_ADDR_IDENTIFIER);
}

bool MemSetIdentifier(String ident) {
  if (ident.length() > IDENTIFIER_LENGTH) {
    LOGE(tag, F("setting Identifier"), tl);
    return false;
  }
  EEPROM.writeString(MEM_ADDR_IDENTIFIER, ident);
  return EEPROM.commit();
}

uint8_t MemGetBeerCounter() {
  return EEPROM.readByte(MEM_ADDR_BEERCOUNTER);
}

bool MemSetBeerCounter(uint8_t val) {
  EEPROM.writeByte(MEM_ADDR_BEERCOUNTER, val);
  return EEPROM.commit();
}

String MemGetFirstName() {
  return EEPROM.readString(MEM_ADDR_FIRST_NAME);
}

bool MemSetFirstName(String fName) {
  if (fName.length() > MAX_FIRST_NAME_LENGTH) {
    LOGE(tag, F("setting First Name"), tl);
    return false;
  }

  EEPROM.writeString(MEM_ADDR_FIRST_NAME, fName);
  return EEPROM.commit();
}

String MemGetLastName() {
  return EEPROM.readString(MEM_ADDR_LAST_NAME);
}

bool MemSetLastName(String lName) {
  if (lName.length() > MAX_LAST_NAME_LENGTH) {
    LOGE(tag, F("setting Last Name"), tl);
    return false;
  }

  EEPROM.writeString(MEM_ADDR_LAST_NAME, lName);
  return EEPROM.commit();
}

String MemGetSSID() {
  return EEPROM.readString(MEM_ADDR_SSID);
}

bool MemSetSSID(String ssid) {
  if (ssid.length() > MAX_SSID_LENGTH) {
    LOGE(tag, F("setting SSID"), tl);
    return false;
  }
  EEPROM.writeString(MEM_ADDR_SSID, ssid);
  return EEPROM.commit();
}

String MemGetPassword() {
  return EEPROM.readString(MEM_ADDR_PASSWORD);
}

bool MemSetPassword(String passwd) {
  if (passwd.length() > MAX_PASSWORD_LENGTH) {
    LOGE(tag, "setting Password", tl);
    return false;
  }
  EEPROM.writeString(MEM_ADDR_PASSWORD, passwd);
  return EEPROM.commit();
}

uint32_t MemGetColorBeer() {
  return EEPROM.readULong(MEM_ADDR_COLOR_BEER);
}

bool MemSetColorBeer(uint32_t color) {
  EEPROM.writeULong(MEM_ADDR_COLOR_BEER, color);
  return EEPROM.commit();
}

uint32_t MemGetColorBeerAdd() {
  return EEPROM.readULong(MEM_ADDR_COLOR_BEER_ADD);
}

bool MemSetColorBeerAdd(uint32_t color) {
  EEPROM.writeULong(MEM_ADDR_COLOR_BEER_ADD, color);
  return EEPROM.commit();
}

uint32_t MemGetColorPing() {
  return EEPROM.readULong(MEM_ADDR_COLOR_PING);
}

bool MemSetColorPing(uint32_t color) {
  EEPROM.writeULong(MEM_ADDR_COLOR_PING, color);
  return EEPROM.commit();
}

int16_t MemGetBatCalib() {
  return EEPROM.readShort(MEM_ADDR_BAT_CALIB);
}

bool MemSetBatCalib(int16_t calib) {
  EEPROM.writeShort(MEM_ADDR_BAT_CALIB, calib);
  return EEPROM.commit();
}

uint16_t MemGetVersion() {
  return EEPROM.readUShort(MEM_ADDR_VERSION);
}

bool MemSetVersion(uint16_t vers) {
  EEPROM.writeUShort(MEM_ADDR_VERSION, vers);
  return EEPROM.commit();
}

uint8_t MemGetDeviceMode() {
  return EEPROM.readByte(MEM_ADDR_DEVICEMODE);
}

bool MemSetDeviceMode(uint8_t mode) {
  EEPROM.writeByte(MEM_ADDR_DEVICEMODE, mode);
  return EEPROM.commit();
}

uint8_t MemGetTapRange() {
  return EEPROM.readByte(MEM_ADDR_TAP_RANGE);
}

bool MemSetTapRange(uint8_t range) {
  if (range > 3) {
    return false;
  }
  EEPROM.writeByte(MEM_ADDR_TAP_RANGE, range);
  return EEPROM.commit();
}

uint8_t MemGetTapThreshhold() {
  return EEPROM.readByte(MEM_ADDR_TAP_THRESH);
}

bool MemSetTapThreshhold(uint8_t thresh) {
  EEPROM.writeByte(MEM_ADDR_TAP_THRESH, thresh);
  return EEPROM.commit();
}

uint8_t MemGetTapTimeLim() {
  return EEPROM.readByte(MEM_ADDR_TAP_TIME_LIM);
}

bool MemSetTapTimeLim(uint8_t timeLim) {
  EEPROM.writeByte(MEM_ADDR_TAP_TIME_LIM, timeLim);
  return EEPROM.commit();
}

uint8_t MemGetTapTimeLatency() {
  return EEPROM.readByte(MEM_ADDR_TAP_TIME_LATENCY);
}

bool MemSetTapTimeLatency(uint8_t latency) {
  EEPROM.writeByte(MEM_ADDR_TAP_TIME_LATENCY, latency);
  return EEPROM.commit();
}

uint8_t MemGetTapWindow() {
  return EEPROM.readByte(MEM_ADDR_TAP_WINDOW);
}

bool MemSetTapWindow(uint8_t window) {
  EEPROM.writeByte(MEM_ADDR_TAP_WINDOW, window);
  return EEPROM.commit();
}

bool MemSaveTapConfig(Lis3dh_t* cfg) {
  bool res = true;
  res &= MemSetTapRange(cfg->range);
  res &= MemSetTapThreshhold(cfg->threshhold);
  res &= MemSetTapTimeLim(cfg->timeLim);
  res &= MemSetTapTimeLatency(cfg->timeLatency);
  res &= MemSetTapWindow(cfg->window);
  return res;
}


bool MemResetEEPROM() {
  for (int i = 0; i < MEM_EEPROM_SIZE; i++) {
    EEPROM.writeByte(i, EEPROM_DEFAULT_VAL);
  }
  return EEPROM.commit();
}

bool MemGetStartSectionCheck(bool resetEEPROM = false) {
  bool res = true;
  uint8_t checkByte = EEPROM.readByte(MEM_ADDR_MEM_CHECK);
  if (checkByte != MEM_EEPROM_SECTION_CHECK_1) {
    LOGI(tag, F("StartSection-Check"), F("section 1"));
    res = false;
  }
  checkByte = EEPROM.readByte(MEM_ADDR_MEM_CHECK + 1);
  if (checkByte != MEM_EEPROM_SECTION_CHECK_2) {
    LOGI(tag, F("StartSection-Check"), F("section 2"));
    res = false;
  }
  checkByte = EEPROM.readByte(MEM_ADDR_MEM_CHECK + 2);
  if (checkByte != MEM_EEPROM_SECTION_CHECK_3) {
    LOGI(tag, F("StartSection-Check"), F("section 3"));
    res = false;
  }
  checkByte = EEPROM.readByte(MEM_ADDR_MEM_CHECK + 3);
  if (checkByte != MEM_EEPROM_SECTION_CHECK_4) {
    LOGI(tag, F("StartSection-Check"), F("section 4"));
    res = false;
  }

  if (res) {
    LOGI(tag, F("StartSection-Check"), F("Success!"));
  } else {
    LOGI(tag, F("StartSection-Check"), F("Failed!"));
    if (resetEEPROM) {
      LOGI(tag, F("Resetting EEPROM"));
      if (MemResetEEPROM()) {
        LOGI(tag, F("Reset done"), F("Success"));
      } else {
        LOGI(tag, F("Reset done"), F("Failed"));
      }
    }
  }
  return res;
}

bool MemSetStartSectionCheck() {
  EEPROM.writeByte(MEM_ADDR_MEM_CHECK, MEM_EEPROM_SECTION_CHECK_1);
  EEPROM.writeByte(MEM_ADDR_MEM_CHECK + 1, MEM_EEPROM_SECTION_CHECK_2);
  EEPROM.writeByte(MEM_ADDR_MEM_CHECK + 2, MEM_EEPROM_SECTION_CHECK_3);
  EEPROM.writeByte(MEM_ADDR_MEM_CHECK + 3, MEM_EEPROM_SECTION_CHECK_4);
  return EEPROM.commit();
}

bool MemGetSWVersionCheck() {
  if (MemGetVersion() == SW_VERSION) {
    return true;
  } else {
    LOGI(tag, F("Software-version missmatch detected"));
    return false;
  }
}

bool MemSetSWVersionCheck() {
  MemSetVersion(SW_VERSION);
  return EEPROM.commit();
}

bool MemGetMemCheck() {
  bool res_st = MemGetStartSectionCheck(RESET_EEPROM_ON_START_SECTION_FAIL);
  bool res_sw = MemGetSWVersionCheck();

  if (res_st && res_sw) {
    LOGI(tag, F("Memory-check complete"), F("Success!"));
    return true;
  } else {
    LOGI(tag, F("Memory-check complete"), F("Failed!"));
    return false;
  }
}

bool MemSetMemCheck() {
  MemSetStartSectionCheck();
}
