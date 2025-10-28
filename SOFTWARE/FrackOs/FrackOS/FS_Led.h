#include <sys/_stdint.h>
#ifndef _FS_LED_H
#define _FS_LED_H
#include "config.h"

#define FS_LED_COUNT 5

#define FS_LED_NO_CLEAR (1 << 0)

typedef enum {
  ledModeNone,
  ledModeStatic,
  ledModeBreathe,
  ledModeBreatheExtended,
  ledModeToggle,
  ledModeRotatingPoint
} ledModes_t;

typedef struct {
  uint32_t color1;
  uint32_t color2;
  uint32_t duration;
  ledModes_t mode;
  int16_t repetitions;
  uint16_t misc;
} ledInfo_t;


void initLED();

void FS_SetLed(ledInfo_t* led);
void FS_SetLed(uint32_t color1, uint32_t color2, uint32_t duration, ledModes_t mode, int16_t repetitions, uint16_t misc);

void FS_LED_OFF();
void FS_LED_NORMAL_MODE();

void LedTask(void* pvParameters);

#endif