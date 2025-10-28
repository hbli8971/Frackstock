#ifndef WIFI_CREDENTIALS_H
#define WIFI_CREDENTIALS_H

#define WIFI_SSID "--REDACTED--"
#define WIFI_PASSWORD "--REDACTED--"

const char baseKey[8] = { 'R', 'E', 'D', 'A', 'C', 'T', 'E', 'D' };

#define IV_0 0xDE
#define IV_1 0xAD
#define IV_2 0xBE
#define IV_3 0xEF
#define IV_4 0xB0
#define IV_5 0x0B
#define IV_6 0x01
#define IV_7 0x23
#define IV_8 0x45
#define IV_9 0x67
#define IV_10 0x89
#define IV_11 0xAB
#define IV_12 0xCD
#define IV_13 0xEF
#define IV_14 0x89
#define IV_15 0x71



#define MQTT_USER_NAME "Fracknet"
#define MQTT_PASSWORD "100Frackwoche?"
#define MQTT_IP_ADDR "192.168.1.140"
#define MQTT_PORT 1883

#define MQTT_ADMIN_USER "--REDACTED--"
#define MQTT_ADMIN_PASSWORD "--REDACTED--"

#define BAT_CALIB_DEFAULT 1000


#define MEM_EEPROM_SECTION_CHECK_1 0xDE
#define MEM_EEPROM_SECTION_CHECK_2 0xAD
#define MEM_EEPROM_SECTION_CHECK_3 0xBE
#define MEM_EEPROM_SECTION_CHECK_4 0xEF



#define BOSSMODE
#define DEVICE_MASTER true

//new flags
#define WC_CHECK_EEPROM true
#define WC_ENGINEERING_MODE true
#define WC_PRINT_MEM_DATA true
#define WC_SHOW_PW false
#define WC_FORCE_USE_DEFAULT_CRED true
#define WC_FORCE_WRITE_EEPROM false





// #define WC_OVERWRITE_EEPROM false

#ifdef BOSSMODE
#define ENTRIES_SETTINGS_MENU_NUM 7
#else
#define ENTRIES_SETTINGS_MENU_NUM 6
#endif


#define WINNER_WINNER_CHICKEN_DINNER 5 * 1000
#endif