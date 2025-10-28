#ifndef FS_MEMCTRL_H
#define FS_MEMCTRL_H
#include <stdint.h>
#include <EEPROM.h>
#include "config.h"
#include "FS_Datatypes.h"

#define MEM_EEPROM_SIZE 256
#define EEPROM_DEFAULT_VAL 0x00

#define MEM_ADDR_SIZE_MEM_CHECK 4
#define MEM_ADDR_SIZE_IDENTIFIER IDENTIFIER_LENGTH + 1  //+1 for \0
#define MEM_ADDR_SIZE_BEERCOUNTER 1
#define MEM_ADDR_SIZE_FIRST_NAME MAX_FIRST_NAME_LENGTH
#define MEM_ADDR_SIZE_LAST_NAME MAX_LAST_NAME_LENGTH
#define MEM_ADDR_SIZE_SSID MAX_SSID_LENGTH
#define MEM_ADDR_SIZE_PASSWORD MAX_PASSWORD_LENGTH
#define MEM_ADDR_SIZE_COLOR_BEER 4
#define MEM_ADDR_SIZE_COLOR_BEER_ADD 4
#define MEM_ADDR_SIZE_COLOR_PING 4
#define MEM_ADDR_SIZE_BAT_CALIB 2
#define MEM_ADDR_SIZE_VERSION 2
#define MEM_ADDR_SIZE_DEVICEMODE 1
#define MEM_ADDR_SIZE_TAP_RANGE 1
#define MEM_ADDR_SIZE_TAP_THRESH 1
#define MEM_ADDR_SIZE_TAP_TIME_LIM 1    
#define MEM_ADDR_SIZE_TAP_TIME_LATENCY 1
#define MEM_ADDR_SIZE_TAP_WINDOW 1

//////////////////////////////////////////////////////////////////////////////////////////////

#define MEM_ADDR_MEM_CHECK          0
#define MEM_ADDR_IDENTIFIER         MEM_ADDR_MEM_CHECK + MEM_ADDR_SIZE_MEM_CHECK
#define MEM_ADDR_BEERCOUNTER        MEM_ADDR_IDENTIFIER + MEM_ADDR_SIZE_IDENTIFIER
#define MEM_ADDR_FIRST_NAME         MEM_ADDR_BEERCOUNTER + MEM_ADDR_SIZE_BEERCOUNTER
#define MEM_ADDR_LAST_NAME          MEM_ADDR_FIRST_NAME + MEM_ADDR_SIZE_FIRST_NAME
#define MEM_ADDR_SSID               MEM_ADDR_LAST_NAME + MEM_ADDR_SIZE_LAST_NAME
#define MEM_ADDR_PASSWORD           MEM_ADDR_SSID + MEM_ADDR_SIZE_SSID
#define MEM_ADDR_COLOR_BEER         MEM_ADDR_PASSWORD + MEM_ADDR_SIZE_PASSWORD
#define MEM_ADDR_COLOR_BEER_ADD     MEM_ADDR_COLOR_BEER + MEM_ADDR_SIZE_COLOR_BEER
#define MEM_ADDR_COLOR_PING         MEM_ADDR_COLOR_BEER_ADD + MEM_ADDR_SIZE_COLOR_BEER_ADD
#define MEM_ADDR_BAT_CALIB          MEM_ADDR_COLOR_PING + MEM_ADDR_SIZE_COLOR_PING
#define MEM_ADDR_VERSION            MEM_ADDR_BAT_CALIB + MEM_ADDR_SIZE_BAT_CALIB
#define MEM_ADDR_DEVICEMODE         MEM_ADDR_VERSION + MEM_ADDR_SIZE_VERSION
#define MEM_ADDR_TAP_RANGE          MEM_ADDR_DEVICEMODE + MEM_ADDR_SIZE_DEVICEMODE
#define MEM_ADDR_TAP_THRESH         MEM_ADDR_TAP_RANGE + MEM_ADDR_SIZE_TAP_RANGE
#define MEM_ADDR_TAP_TIME_LIM       MEM_ADDR_TAP_THRESH + MEM_ADDR_SIZE_TAP_THRESH
#define MEM_ADDR_TAP_TIME_LATENCY   MEM_ADDR_TAP_TIME_LIM + MEM_ADDR_SIZE_TAP_TIME_LIM
#define MEM_ADDR_TAP_WINDOW         MEM_ADDR_TAP_TIME_LATENCY + MEM_ADDR_SIZE_TAP_TIME_LATENCY

void initEEPROM();

void MemPrintAddresses();




String MemGetIdentifier();
bool MemSetIdentifier(String ident);

uint8_t MemGetBeerCounter();
bool MemSetBeerCounter(uint8_t val);

String MemGetFirstName();
bool MemSetFirstName(String fName);

String MemGetLastName();
bool MemSetLastName(String lName);

String MemGetSSID();
bool MemSetSSID(String ssid);

String MemGetPassword();
bool MemSetPassword(String passwd);

uint32_t MemGetColorBeer();
bool MemSetColorBeer(uint32_t color);

uint32_t MemGetColorBeerAdd();
bool MemSetColorBeerAdd(uint32_t color);

uint32_t MemGetColorPing();
bool MemSetColorPing(uint32_t color);

int16_t MemGetBatCalib();
bool MemSetBatCalib(int16_t calib);

uint16_t MemGetVersion();
bool MemSetVersion(uint16_t vers);

uint8_t MemGetDeviceMode();
bool MemSetDeviceMode(uint8_t mode);

uint8_t MemGetTapScale();
bool MemSetTapScale(uint8_t scale);


uint8_t MemGetTapRange();
bool MemSetTapRange (uint8_t range);

uint8_t MemGetTapThreshhold();
bool MemSetTapThreshhold(uint8_t thresh);

uint8_t MemGetTapTimeLim();
bool MemSetTapTimeLim (uint8_t timeLim);

uint8_t MemGetTapTimeLatency();
bool MemSetTapTimeLatency (uint8_t latency);

uint8_t MemGetTapWindow();
bool MemSetTapWindow (uint8_t window);

bool MemSaveTapConfig(Lis3dh_t *cfg);

bool MemGetStartSectionCheck(bool resetEEPROM);
bool MemSetStartSectionCheck();

bool MemGetSWVersionCheck();
bool MemSetSWVersionCheck();

bool MemResetEEPROM();

bool MemGetMemCheck();
bool MemSetMemCheck();

#endif