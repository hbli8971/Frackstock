#include <string.h>
#include "FS_Encryption.h"
#include <ArduinoJson.h>
#include "wifi_credentials.h"
#include <AESLib.h>
#include <hbli_Logging.h>

AESLib aesLib;  
const size_t maxJsonSize = 512;

char encryptedData[maxJsonSize * 2];
char decryptedData[maxJsonSize];
char* fullKey = nullptr;  // Pointer to store the full key
byte aes_iv[16] = { IV_0, IV_1, IV_2, IV_3, IV_4, IV_5, IV_6, IV_7, IV_8, IV_9, IV_10, IV_11, IV_12, IV_13, IV_14, IV_15 };
byte aes_iv_copy[16];

void initEncryption() {
  aesLib.set_paddingmode(paddingMode::CMS);
}

// Function to generate the full key from the base key and the device ID
void createFullKey(const char* deviceId) {
  if (fullKey != nullptr) {
    free(fullKey);  // Free previously allocated memory if any
  }
  fullKey = (char*)malloc(17 * sizeof(char));  // Allocate memory for the full key, including null terminator
  if (fullKey != nullptr) {
    memcpy(fullKey, baseKey, 8);       // Copy base key (from wifi_credentials.h)
    memcpy(fullKey + 8, deviceId, 8);  // Append the device ID to form full key
    fullKey[16] = '\0';                // Null-terminate the full key
  }
}

void freeFullKey() {
  if (fullKey != nullptr) {
    free(fullKey);
    fullKey = nullptr;
  }
}

// Function to encrypt the received JSON string (modifies the input string)
bool encryptJsonString(String& buffer) {
  if (fullKey == nullptr) {
    return false;  // Full key has not been generated
  }
  memcpy(aes_iv_copy, aes_iv, sizeof(aes_iv));
  uint16_t input_length = buffer.length();
  uint16_t cipherLength = aesLib.encrypt64((byte*)buffer.c_str(), input_length, encryptedData, (byte*)fullKey, 128, aes_iv_copy);

  if (cipherLength > 0) {
    encryptedData[cipherLength] = '\0';  // Null-terminate the encrypted data
    buffer = String(encryptedData);      // Set the encrypted data to the same buffer
    return true;                         // Encryption successful
  } else {
    return false;  // Encryption failed
  }
}

// Function to decrypt the received JSON string (modifies the input string)
bool decryptJsonString(String& buffer) {
  if (fullKey == nullptr) {
    return false;  // Full key has not been generated
  }
  uint16_t input_length = buffer.length();
  memset(decryptedData, 0, sizeof(decryptedData));
  memcpy(aes_iv_copy, aes_iv, sizeof(aes_iv));
  uint16_t plainLength = aesLib.decrypt64((char*)buffer.c_str(), input_length, (byte*)decryptedData, (byte*)fullKey, 128, aes_iv_copy);

  if (plainLength > 0) {
    decryptedData[plainLength] = '\0';      // Null-terminate the decrypted data
    buffer = String((char*)decryptedData);  // Set the decrypted data to the same buffer
    return true;                            // Decryption successful
  } else {
    return false;  // Decryption failed
  }
}

char* getFullKey() {
  return fullKey;
}
