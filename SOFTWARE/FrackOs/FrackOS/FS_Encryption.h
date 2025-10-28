#ifndef _FS_ENCRYPTION_H
#define _FS_ENCRYPTION_H
#include <Arduino.h>
/* only used during the flashing process*/
void initEncryption();
void createFullKey(const char* deviceId);
void freeFullKey();
bool encryptJsonString(String& buffer);
bool decryptJsonString(String& buffer);
char* getFullKey();

#endif