#include <sys/_stdint.h>
#ifndef _DPOS_CLASS_TAP_INDICATOR_H
#define _DPOS_CLASS_TAP_INDICATOR_H

#include <Arduino.h>
#include "DPOS_Class_UIElement.h"
#include "DPOS_cfg.h"



class DPOS_TapIndicator : public DPOS_UIElement {
private:
  uint8_t counter;
  uint8_t style;
  int16_t maxCounterDisplayValue;

public:
  DPOS_TapIndicator();
  ~DPOS_TapIndicator();

  void setCounter(uint8_t count);
  uint8_t getCounter();
  void setStyle(uint8_t newStyle);

  void draw() override;
  void update() override;

  DPOS_UIElement_Type getElementType() override {
    return DPOS_ELEMENT_TYPE_TAP_INDICATOR;
  }
  uint32_t getElementBit() override {
    return DPOS_ELEMENT_BIT_TAP_INDICATOR;
  }
};



#endif