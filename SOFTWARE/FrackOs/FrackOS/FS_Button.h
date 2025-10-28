#ifndef _FS_BUTTON_H
#define _FS_BUTTON_H
#include <stdint.h>
#include <Arduino.h>

#define FS_BUTTON_DEFAULT_DEBOUNCE_TIME 50

enum {
  NO_ACTION,
  SINGLE_PRESS,
  DOUBLE_PRESS,
  TRIPPLE_PRESS,
  QUADRUPLE_PRESS,
  MAX_PRESSES,
  LONG_PRESS,
  TOTAL_ACTIONS
} ButtonActions;


typedef struct {
  uint32_t timer = 0;
  uint32_t duration = 0;
  volatile uint32_t istTimeLast = 0;
  uint32_t timerRelease = 0;
  uint32_t durationRelease = 0;
  uint32_t timerAction = 0;

  uint8_t debounceTime = FS_BUTTON_DEFAULT_DEBOUNCE_TIME;
  uint8_t pin = 0;
  uint8_t action = 0;
  uint8_t counterAction = 0;

  volatile bool isrFlag = false;
  bool defaultValue = false;
  bool state = false;
  bool stateOld = false;
  const char* name;
} Button_t;


extern Button_t btnHead;
extern Button_t btnProg;

void initButtons();
uint8_t getButtonAction(Button_t* btn);
uint32_t getButtonDuration(Button_t* btn);
bool isButtonDurationBiggerThan(Button_t* btn, uint32_t time);
bool isButtonAction(Button_t* btn, uint8_t act);
bool isButtonActionR(Button_t* btn, uint8_t act);
void resetDuration(Button_t* btn);
void resetAction(Button_t* btn);
void resetButtons();



void buttonLoop();
void buttonActionLoop();

#endif
