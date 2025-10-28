#ifndef _DPOS_CLASS_SCROLL_INDICATOR_H
#define _DPOS_CLASS_SCROLL_INDICATOR_H

#include <Arduino.h>
#include "DPOS_Class_UIElement.h"
#include "DPOS_cfg.h"

class DPOS_ScrollIndicator : public DPOS_UIElement {
private:
  uint16_t totalSteps;
  uint16_t currentStep;
  bool vertical;
public:

  DPOS_ScrollIndicator(uint32_t width, uint32_t height);
  ~DPOS_ScrollIndicator();

  void setNumberOfSteps(uint16_t steps);
  void setCurrentStep(uint16_t step);

  void setHorizontal();
  void setVertical();

  void draw() override;
  void update() override;

  DPOS_UIElement_Type getElementType() override {
    return DPOS_ELEMENT_TYPE_SCROLL_INDICATOR;
  }
  uint32_t getElementBit() override {
    return DPOS_ELEMENT_BIT_SCROLL_INDICATOR;
  }
};

#endif