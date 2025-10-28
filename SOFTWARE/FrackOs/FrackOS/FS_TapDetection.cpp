#include "FS_TapDetection.h"
#include <sys/_stdint.h>
#include "esp32-hal.h"
#include <Wire.h>
#include <Adafruit_LIS3DH.h>
#include <hbli_Logging.h>
#include "FS_DPOS_Definitions.h"
#include "FS_memCtrl.h"

// #define LIS3DH_TAP_THRESHOLD      80
#define LIS3DH_TAP_THRESHOLD 20
#define LIS3DH_TAP_MODE 2
#define LIS3DH_TIME_LIMIT 10
#define LIS3DH_TIME_LATENCY 20
#define LIS3DH_TIME_WINDOW 255

#define LIS3DH_TAP_DEBOUNCE 250
#define LIS3DH_TAP_INTERVAL 1000


Adafruit_LIS3DH lis = Adafruit_LIS3DH();

static const char* tag = "TapDetect";

void lis3dh_set_range(uint8_t range);

bool TapsDetected(uint8_t tap_amount) {
  if (tap_amount == sys.tapCount) {
    sys.tapCount = 0;
    tap.setCounter(0);
    return true;
  }
  return false;
}

bool TapsDetectedMoreThan(uint8_t tap_amount) {
  if (tap_amount < sys.tapCount) {
    sys.tapCount = 0;
    tap.setCounter(0);
    return true;
  }
  return false;
}


//private functions
void writeRegister(uint8_t reg, uint8_t value) {
  Wire.beginTransmission(I2C_ADDR_LIS3DH);
  Wire.write(reg);
  Wire.write(value);
  Wire.endTransmission();
}

uint8_t readRegister(uint8_t reg) {
  uint8_t value;

  // Begin I2C transmission to the LIS3DH
  Wire.beginTransmission(I2C_ADDR_LIS3DH);
  // Write the register address to be read
  Wire.write(reg);
  // End the transmission to the LIS3DH
  Wire.endTransmission();

  // Request 1 byte of data from the LIS3DH
  Wire.requestFrom(I2C_ADDR_LIS3DH, 1);
  // Wait until the byte is received
  while (Wire.available() == 0)
    ;
  // Read the byte from the I2C bus
  value = Wire.read();

  // Return the read value
  return value;
}

void initTapDetection(Lis3dh_t* init) {
  if (!lis.begin(I2C_ADDR_LIS3DH)) {  // Default I2C address
    LOGE(tag, F("LIS3DH not found!"));
  }
  LOGI(tag, F("LIS3DH found!"));

  lis.setDataRate(LIS3DH_DATARATE_LOWPOWER_5KHZ);  // Example: 200Hz data rate
  lis3dh_set_range(init->range);

  lis.setClick(1, init->threshhold, init->timeLim, init->timeLatency, init->window);

  writeRegister(LIS3DH_REG_CLICKCFG, 0x04);  // enable interrupt single click on y axis
  writeRegister(LIS3DH_REG_CTRL2, 0x01);     // enable filter in int 1
  writeRegister(LIS3DH_REG_CTRL3, 0x80);     // Turn on INT1 click
  writeRegister(LIS3DH_REG_CTRL5, 0x08);
  writeRegister(LIS3DH_REG_CLICKTHS, readRegister(LIS3DH_REG_CLICKTHS) | 0x80);

  pinMode(FS_PIN_LIS_INT_1, INPUT);
  pinMode(FS_PIN_LIS_INT_2, INPUT);
  uint8_t garbage = resetTapInterrupt();
  attachInterrupt(digitalPinToInterrupt(FS_PIN_LIS_INT_1), tapDetectedISR, RISING);  // Set the interrupt
}

uint8_t resetTapInterrupt() {
  return readRegister(LIS3DH_REG_CLICKSRC);
}

void tapDetectedISR() {
  tapDetect.flagTapInterrupt = true;
  tapDetect.timerTapDetectedLast = millis();
}

void lis3dh_set_range(uint8_t range) {
  switch (range) {
    case 0:
      lis.setRange(LIS3DH_RANGE_2_G);
      break;
    case 1:
      lis.setRange(LIS3DH_RANGE_4_G);
      break;
    case 2:
      lis.setRange(LIS3DH_RANGE_8_G);
      break;
    case 3:
      lis.setRange(LIS3DH_RANGE_16_G);
      break;
    default:
      LOGE(tag, F("invalid input"));
      break;
  }
}

void lis3dh_set_thresh(uint8_t thresh) {
  lis.setClick(1, thresh, lisCfg.timeLim, lisCfg.timeLatency, lisCfg.window);
}

void lis3dh_set_time_lim(uint8_t lim) {
  lis.setClick(1, lisCfg.threshhold, lim, lisCfg.timeLatency, lisCfg.window);
}

void lis3dh_set_time_latency(uint8_t latency) {
  lis.setClick(1, lisCfg.threshhold, lisCfg.timeLim, latency, lisCfg.window);
}

void lis3dh_set_window(uint8_t window) {
  lis.setClick(1, lisCfg.threshhold, lisCfg.timeLim, lisCfg.timeLatency, window);
}

void lis3dh_loadDefaultConfig(Lis3dh_t* data) {
  data->range = LIS3DH_RANGE_4_G;
  data->threshhold = LIS3DH_TAP_THRESHOLD;
  data->timeLim = LIS3DH_TIME_LIMIT;
  data->timeLatency = LIS3DH_TIME_LATENCY;
  data->window = LIS3DH_TIME_WINDOW;
}
