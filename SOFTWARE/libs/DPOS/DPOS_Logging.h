#ifndef _DPOS_LOGGING_H
#define _DPOS_LOGGING_H

#include "Arduino.h"
#include "Print.h"

#define DPOS_LOG_LEVEL_ERROR 1
#define DPOS_LOG_LEVEL_INFO 2
#define DPOS_LOG_LEVEL_VERBOSE 3
#define DPOS_LOG_LEVEL_DEBUG 4

// Set the current log output level
#ifndef DPOS_LOG_OUTPUT_LEVEL
#define DPOS_LOG_OUTPUT_LEVEL DPOS_LOG_LEVEL_INFO
#endif

// Internal static functions to hide logPrint from user access
namespace {
// Static logPrint functions to be used only within this header
static void logPrint(const char* tag, const __FlashStringHelper* value) {
  Serial.print(tag);
  Serial.print(" : ");
  Serial.println(value);
}

static void logPrint(const char* tag, const __FlashStringHelper* value1, const __FlashStringHelper* value2) {
  Serial.print(tag);
  Serial.print(" : ");
  Serial.print(value1);
  Serial.print(", ");
  Serial.println(value2);
}

// Template function for general log print
template<typename T>
static void logPrint(const char* tag, T value) {
  Serial.print(tag);
  Serial.print(" : ");
  Serial.println(value);
}

template<typename T, typename V>
static void logPrint(const char* tag, T value, V value2) {
  Serial.print(tag);
  Serial.print(" : ");
  Serial.print(value);
  Serial.print(" , ");
  Serial.println(value2);
}
}


//////////////////// DPOS_LOG_LEVEL_ERROR ////////////////////
template<typename T>
void LOGE(const char* tag, T value) {
  if (DPOS_LOG_OUTPUT_LEVEL >= DPOS_LOG_LEVEL_ERROR) {
    Serial.print("[ERROR] ");
    logPrint(tag, value);
  }
}

inline void LOGE(const char* tag, const __FlashStringHelper* value) {
  if (DPOS_LOG_OUTPUT_LEVEL >= DPOS_LOG_LEVEL_ERROR) {
    Serial.print("[ERROR] ");
    logPrint(tag, value);
  }
}

template<typename T, typename V>
void LOGE(const char* tag, T value1, V value2) {
  if (DPOS_LOG_OUTPUT_LEVEL >= DPOS_LOG_LEVEL_ERROR) {
    Serial.print("[ERROR] ");
    logPrint(tag, value1, value2);
  }
}

template<typename T>
void LOGE(const char* tag, T value1, const __FlashStringHelper* value2) {
  if (DPOS_LOG_OUTPUT_LEVEL >= DPOS_LOG_LEVEL_ERROR) {
    Serial.print("[ERROR] ");
    logPrint(tag, value1, value2);
  }
}

inline void LOGE(const char* tag, const __FlashStringHelper* value1, const __FlashStringHelper* value2) {
  if (DPOS_LOG_OUTPUT_LEVEL >= DPOS_LOG_LEVEL_ERROR) {
    Serial.print("[ERROR] ");
    logPrint(tag, value1, value2);
  }
}

template<typename V>
void LOGE(const char* tag, const __FlashStringHelper* value1, V value2) {
  if (DPOS_LOG_OUTPUT_LEVEL >= DPOS_LOG_LEVEL_ERROR) {
    Serial.print("[ERROR] ");
    logPrint(tag, value1, value2);
  }
}


//////////////////// DPOS_LOG_LEVEL_INFO ////////////////////
template<typename T>
void LOGI(const char* tag, T value) {
  if (DPOS_LOG_OUTPUT_LEVEL >= DPOS_LOG_LEVEL_INFO) {
    Serial.print("[INFO] ");
    logPrint(tag, value);
  }
}

inline void LOGI(const char* tag, const __FlashStringHelper* value) {
  if (DPOS_LOG_OUTPUT_LEVEL >= DPOS_LOG_LEVEL_INFO) {
    Serial.print("[INFO] ");
    logPrint(tag, value);
  }
}

template<typename T, typename V>
void LOGI(const char* tag, T value1, V value2) {
  if (DPOS_LOG_OUTPUT_LEVEL >= DPOS_LOG_LEVEL_INFO) {
    Serial.print("[INFO] ");
    logPrint(tag, value1, value2);
  }
}

template<typename T>
void LOGI(const char* tag, T value1, const __FlashStringHelper* value2) {
  if (DPOS_LOG_OUTPUT_LEVEL >= DPOS_LOG_LEVEL_INFO) {
    Serial.print("[INFO] ");
    logPrint(tag, value1, value2);
  }
}

inline void LOGI(const char* tag, const __FlashStringHelper* value1, const __FlashStringHelper* value2) {
  if (DPOS_LOG_OUTPUT_LEVEL >= DPOS_LOG_LEVEL_INFO) {
    Serial.print("[INFO] ");
    logPrint(tag, value1, value2);
  }
}

template<typename V>
void LOGI(const char* tag, const __FlashStringHelper* value1, V value2) {
  if (DPOS_LOG_OUTPUT_LEVEL >= DPOS_LOG_LEVEL_INFO) {
    Serial.print("[INFO] ");
    logPrint(tag, value1, value2);
  }
}

//////////////////// DPOS_LOG_LEVEL_VERBOSE ////////////////////
template<typename T>
void LOGV(const char* tag, T value) {
  if (DPOS_LOG_OUTPUT_LEVEL == DPOS_LOG_LEVEL_VERBOSE) {
    Serial.print("[VERBOSE] ");
    logPrint(tag, value);
  }
}

inline void LOGV(const char* tag, const __FlashStringHelper* value) {
  if (DPOS_LOG_OUTPUT_LEVEL == DPOS_LOG_LEVEL_VERBOSE) {
    Serial.print("[VERBOSE] ");
    logPrint(tag, value);
  }
}

template<typename T, typename V>
void LOGV(const char* tag, T value1, V value2) {
  if (DPOS_LOG_OUTPUT_LEVEL == DPOS_LOG_LEVEL_VERBOSE) {
    Serial.print("[VERBOSE] ");
    logPrint(tag, value1, value2);
  }
}

template<typename T>
void LOGV(const char* tag, T value1, const __FlashStringHelper* value2) {
  if (DPOS_LOG_OUTPUT_LEVEL == DPOS_LOG_LEVEL_VERBOSE) {
    Serial.print("[VERBOSE] ");
    logPrint(tag, value1, value2);
  }
}

inline void LOGV(const char* tag, const __FlashStringHelper* value1, const __FlashStringHelper* value2) {
  if (DPOS_LOG_OUTPUT_LEVEL == DPOS_LOG_LEVEL_VERBOSE) {
    Serial.print("[VERBOSE] ");
    logPrint(tag, value1, value2);
  }
}

template<typename V>
void LOGV(const char* tag, const __FlashStringHelper* value1, V value2) {
  if (DPOS_LOG_OUTPUT_LEVEL == DPOS_LOG_LEVEL_VERBOSE) {
    Serial.print("[VERBOSE] ");
    logPrint(tag, value1, value2);
  }
}

//////////////////// DPOS_LOG_LEVEL_DEBUG ////////////////////
template<typename T>
void LOGG(const char* tag, T value) {
  if (DPOS_LOG_OUTPUT_LEVEL == DPOS_LOG_LEVEL_DEBUG) {
    Serial.print("[DEBUG] ");
    logPrint(tag, value);
  }
}

inline void LOGG(const char* tag, const __FlashStringHelper* value) {
  if (DPOS_LOG_OUTPUT_LEVEL == DPOS_LOG_LEVEL_DEBUG) {
    Serial.print("[DEBUG] ");
    logPrint(tag, value);
  }
}

template<typename T, typename V>
void LOGG(const char* tag, T value1, V value2) {
  if (DPOS_LOG_OUTPUT_LEVEL == DPOS_LOG_LEVEL_DEBUG) {
    Serial.print("[DEBUG] ");
    logPrint(tag, value1, value2);
  }
}

template<typename T>
void LOGG(const char* tag, T value1, const __FlashStringHelper* value2) {
  if (DPOS_LOG_OUTPUT_LEVEL == DPOS_LOG_LEVEL_DEBUG) {
    Serial.print("[DEBUG] ");
    logPrint(tag, value1, value2);
  }
}

inline void LOGG(const char* tag, const __FlashStringHelper* value1, const __FlashStringHelper* value2) {
  if (DPOS_LOG_OUTPUT_LEVEL == DPOS_LOG_LEVEL_DEBUG) {
    Serial.print("[DEBUG] ");
    logPrint(tag, value1, value2);
  }
}

template<typename V>
void LOGG(const char* tag, const __FlashStringHelper* value1, V value2) {
  if (DPOS_LOG_OUTPUT_LEVEL == DPOS_LOG_LEVEL_DEBUG) {
    Serial.print("[DEBUG] ");
    logPrint(tag, value1, value2);
  }
}

#endif  // _DPOS_LOGGING_H
