#ifndef _DPOS_CLASS_CREATIVE_CANVAS_H
#define _DPOS_CLASS_CREATIVE_CANVAS_H
#include <Arduino.h>
#include "DPOS_Class_UIElement.h"
#include "DPOS_cfg.h"



class DPOS_CustomCanvas : public DPOS_UIElement {
private:
protected:
public:
  DPOS_CustomCanvas(uint16_t w, uint16_t h);
  ~DPOS_CustomCanvas();

  void draw() override {};

  GFXcanvas1& getCanvas() {
    return this->canvas;
  }
  DPOS_UIElement_Type getElementType() override {
    return DPOS_ELEMENT_TYPE_CREATIVE_CANVAS;
  }
  uint32_t getElementBit() override {
    return DPOS_ELEMENT_BIT_CREATIVE_CANVAS;
  }
};


#endif