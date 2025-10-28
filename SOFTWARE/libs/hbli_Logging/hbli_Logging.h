#ifndef _HLBI_LOGGING_H
#define _HLBI_LOGGING_H

#include "WString.h"
#include "Arduino.h"
#include "Print.h"


enum HBLI_LOG_LEVEL {
  HBLI_LOG_LEVEL_ERROR = 1,
  HBLI_LOG_LEVEL_INFO,
  HBLI_LOG_LEVEL_VERBOSE,
  HBLI_LOG_LEVEL_DEBUG
};

// Set the current log output level
#ifndef HBLI_LOG_OUTPUT_LEVEL
#define HBLI_LOG_OUTPUT_LEVEL HBLI_LOG_LEVEL_INFO
#endif

const char errorLabel[] PROGMEM = "[ERROR] ";
const char infoLabel[] PROGMEM = "[INFO] ";
const char verboseLabel[] PROGMEM = "[VERBOSE] ";
const char debugLabel[] PROGMEM = "[DEBUG] ";

const char separator1[] PROGMEM = " : ";
const char separator2[] PROGMEM = ", ";



// Internal static functions to hide logPrint from user access
namespace {
// Static logPrint functions to be used only within this header
static void logPrint(const char* tag, const __FlashStringHelper* value) {
  Serial.print(tag);
  Serial.print((FPSTR(separator1)));
  Serial.println(value);
}

static void logPrint(const char* tag, const __FlashStringHelper* value1, const __FlashStringHelper* value2) {
  Serial.print(tag);
  Serial.print(FPSTR(separator1));
  Serial.print(value1);
  Serial.print(FPSTR(separator2));
  Serial.println(value2);
}

// Template function for general log print
template<typename T>
static void logPrint(const char* tag, T value) {
  Serial.print(tag);
  Serial.print(FPSTR(separator1));
  Serial.println(value);
}

template<typename T, typename V>
static void logPrint(const char* tag, T value, V value2) {
  Serial.print(tag);
  Serial.print(FPSTR(separator1));
  Serial.print(value);
  Serial.print(FPSTR(separator2));
  Serial.println(value2);
}
}



//////////////////// HBLI_LOG_LEVEL_ERROR ////////////////////
template<typename T>
void LOGE(const char* tag, T value) {
  if (HBLI_LOG_OUTPUT_LEVEL >= HBLI_LOG_LEVEL_ERROR) {
    Serial.print(FPSTR(errorLabel));
    logPrint(tag, value);
  }
}

inline void LOGE(const char* tag, const __FlashStringHelper* value) {
  if (HBLI_LOG_OUTPUT_LEVEL >= HBLI_LOG_LEVEL_ERROR) {
    Serial.print(FPSTR(errorLabel));
    logPrint(tag, value);
  }
}

template<typename T, typename V>
void LOGE(const char* tag, T value1, V value2) {
  if (HBLI_LOG_OUTPUT_LEVEL >= HBLI_LOG_LEVEL_ERROR) {
    Serial.print(FPSTR(errorLabel));
    logPrint(tag, value1, value2);
  }
}

template<typename T>
void LOGE(const char* tag, T value1, const __FlashStringHelper* value2) {
  if (HBLI_LOG_OUTPUT_LEVEL >= HBLI_LOG_LEVEL_ERROR) {
    Serial.print(FPSTR(errorLabel));
    logPrint(tag, value1, value2);
  }
}

inline void LOGE(const char* tag, const __FlashStringHelper* value1, const __FlashStringHelper* value2) {
  if (HBLI_LOG_OUTPUT_LEVEL >= HBLI_LOG_LEVEL_ERROR) {
    Serial.print(FPSTR(errorLabel));
    logPrint(tag, value1, value2);
  }
}

template<typename V>
void LOGE(const char* tag, const __FlashStringHelper* value1, V value2) {
  if (HBLI_LOG_OUTPUT_LEVEL >= HBLI_LOG_LEVEL_ERROR) {
    Serial.print(FPSTR(errorLabel));
    logPrint(tag, value1, value2);
  }
}


//////////////////// HBLI_LOG_LEVEL_INFO ////////////////////
template<typename T>
void LOGI(const char* tag, T value) {
  if (HBLI_LOG_OUTPUT_LEVEL >= HBLI_LOG_LEVEL_INFO) {
    Serial.print(FPSTR(infoLabel));
    logPrint(tag, value);
  }
}

inline void LOGI(const char* tag, const __FlashStringHelper* value) {
  if (HBLI_LOG_OUTPUT_LEVEL >= HBLI_LOG_LEVEL_INFO) {
    Serial.print(FPSTR(infoLabel));
    logPrint(tag, value);
  }
}

template<typename T, typename V>
void LOGI(const char* tag, T value1, V value2) {
  if (HBLI_LOG_OUTPUT_LEVEL >= HBLI_LOG_LEVEL_INFO) {
    Serial.print(FPSTR(infoLabel));
    logPrint(tag, value1, value2);
  }
}

template<typename T>
void LOGI(const char* tag, T value1, const __FlashStringHelper* value2) {
  if (HBLI_LOG_OUTPUT_LEVEL >= HBLI_LOG_LEVEL_INFO) {
    Serial.print(FPSTR(infoLabel));
    logPrint(tag, value1, value2);
  }
}

inline void LOGI(const char* tag, const __FlashStringHelper* value1, const __FlashStringHelper* value2) {
  if (HBLI_LOG_OUTPUT_LEVEL >= HBLI_LOG_LEVEL_INFO) {
    Serial.print(FPSTR(infoLabel));
    logPrint(tag, value1, value2);
  }
}

template<typename V>
void LOGI(const char* tag, const __FlashStringHelper* value1, V value2) {
  if (HBLI_LOG_OUTPUT_LEVEL >= HBLI_LOG_LEVEL_INFO) {
    Serial.print(FPSTR(infoLabel));
    logPrint(tag, value1, value2);
  }
}

//////////////////// HBLI_LOG_LEVEL_VERBOSE ////////////////////
template<typename T>
void LOGV(const char* tag, T value) {
  if (HBLI_LOG_OUTPUT_LEVEL == HBLI_LOG_LEVEL_VERBOSE) {
    Serial.print(FPSTR(verboseLabel));
    logPrint(tag, value);
  }
}

inline void LOGV(const char* tag, const __FlashStringHelper* value) {
  if (HBLI_LOG_OUTPUT_LEVEL == HBLI_LOG_LEVEL_VERBOSE) {
    Serial.print(FPSTR(verboseLabel));
    logPrint(tag, value);
  }
}

template<typename T, typename V>
void LOGV(const char* tag, T value1, V value2) {
  if (HBLI_LOG_OUTPUT_LEVEL == HBLI_LOG_LEVEL_VERBOSE) {
    Serial.print(FPSTR(verboseLabel));
    logPrint(tag, value1, value2);
  }
}

template<typename T>
void LOGV(const char* tag, T value1, const __FlashStringHelper* value2) {
  if (HBLI_LOG_OUTPUT_LEVEL == HBLI_LOG_LEVEL_VERBOSE) {
    Serial.print(FPSTR(verboseLabel));
    logPrint(tag, value1, value2);
  }
}

inline void LOGV(const char* tag, const __FlashStringHelper* value1, const __FlashStringHelper* value2) {
  if (HBLI_LOG_OUTPUT_LEVEL == HBLI_LOG_LEVEL_VERBOSE) {
    Serial.print(FPSTR(verboseLabel));
    logPrint(tag, value1, value2);
  }
}

template<typename V>
void LOGV(const char* tag, const __FlashStringHelper* value1, V value2) {
  if (HBLI_LOG_OUTPUT_LEVEL == HBLI_LOG_LEVEL_VERBOSE) {
    Serial.print(FPSTR(verboseLabel));
    logPrint(tag, value1, value2);
  }
}

//////////////////// HBLI_LOG_LEVEL_DEBUG ////////////////////
template<typename T>
void LOGG(const char* tag, T value) {
  if (HBLI_LOG_OUTPUT_LEVEL == HBLI_LOG_LEVEL_DEBUG) {
    Serial.print(FPSTR(debugLabel));
    logPrint(tag, value);
  }
}

inline void LOGG(const char* tag, const __FlashStringHelper* value) {
  if (HBLI_LOG_OUTPUT_LEVEL == HBLI_LOG_LEVEL_DEBUG) {
    Serial.print(FPSTR(debugLabel));
    logPrint(tag, value);
  }
}

template<typename T, typename V>
void LOGG(const char* tag, T value1, V value2) {
  if (HBLI_LOG_OUTPUT_LEVEL == HBLI_LOG_LEVEL_DEBUG) {
    Serial.print(FPSTR(debugLabel));
    logPrint(tag, value1, value2);
  }
}

template<typename T>
void LOGG(const char* tag, T value1, const __FlashStringHelper* value2) {
  if (HBLI_LOG_OUTPUT_LEVEL == HBLI_LOG_LEVEL_DEBUG) {
    Serial.print(FPSTR(debugLabel));
    logPrint(tag, value1, value2);
  }
}

inline void LOGG(const char* tag, const __FlashStringHelper* value1, const __FlashStringHelper* value2) {
  if (HBLI_LOG_OUTPUT_LEVEL == HBLI_LOG_LEVEL_DEBUG) {
    Serial.print(FPSTR(debugLabel));
    logPrint(tag, value1, value2);
  }
}

template<typename V>
void LOGG(const char* tag, const __FlashStringHelper* value1, V value2) {
  if (HBLI_LOG_OUTPUT_LEVEL == HBLI_LOG_LEVEL_DEBUG) {
    Serial.print(FPSTR(debugLabel));
    logPrint(tag, value1, value2);
  }
}

#endif