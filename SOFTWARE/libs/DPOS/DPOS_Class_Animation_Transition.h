#ifndef _DPOS_CLASS_ANIMATION_TRANSITION_H
#define _DPOS_CLASS_ANIMATION_TRANSITION_H
#include <Arduino.h>
#include "DPOS_Class_UIElement.h"
#include "DPOS_Class_Page.h"

enum DPOSAniTransModes {
  DPOS_ANITRANS_CIRCLE,
  DPOS_ANITRANS_LINE_TOPDOWN,
  DPOS_ANITRANS_LINE_LEFT2RIGHT,
  DPOS_ANITRANS_LINE_RIGHT2LEFT,
  DPOS_ANITRANS_LINE_DIAGN
};


class DPOS_Animation_Transition : public DPOS_UIElement {
private:
  DPOS_Page* startPage;
  DPOS_Page* endPage;
  int16_t radiusInternal;
  uint8_t currentStep;
  uint8_t misc;
  

  void _aniTransConditionalDraw(bool condition, int16_t _x_, int16_t _y_);

public:

  DPOS_Animation_Transition();
  ~DPOS_Animation_Transition();

  void setStartPage(DPOS_Page* start);
  void setEndPage(DPOS_Page* end);
  void setTransitionMode(uint8_t mode);
  void startTransition();
  bool getTransitionDone(bool resetFlag);


  void draw() override;
  void update() override;

  DPOS_UIElement_Type getElementType() override {
    return DPOS_ELEMENT_TYPE_ANI_TRANSITION;
  }
  uint32_t getElementBit() override {
    return DPOS_ELEMENT_BIT_ANI_TRANS;
  }
};

#endif