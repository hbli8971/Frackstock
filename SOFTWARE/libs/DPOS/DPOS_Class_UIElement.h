#include <stdint.h>
#ifndef _DPOS_CLASS_UIELEMENT_H
#define _DPOS_CLASS_UIELEMENT_H
#include <Arduino.h>
#include <Adafruit_GFX.h>

enum DPOS_UIElement_Type {
  DPOS_ELEMENT_TYPE_NONE,
  DPOS_ELEMENT_TYPE_TEXTBODY,
  DPOS_ELEMENT_TYPE_MENU,
  DPOS_ELEMENT_TYPE_SCROLL_INDICATOR,
  DPOS_ELEMENT_TYPE_TAP_INDICATOR,
  DPOS_ELEMENT_TYPE_ANI_FRAME,
  DPOS_ELEMENT_TYPE_ANI_TRANSITION,
  DPOS_ELEMENT_TYPE_PAGE,
  DPOS_ELEMENT_TYPE_CREATIVE_CANVAS
};

class DPOS_UIElement {

protected:
  int32_t posX, posY;
  uint32_t fieldWidth, fieldHeight;
  GFXcanvas1 canvas;

  
  void debugPrintCanvas();

public:

  DPOS_UIElement(uint32_t w, uint32_t h);
  ~DPOS_UIElement();

  //position
  void setPos(int32_t x, int32_t y);
  void setPosX(int32_t x);
  void setPosY(int32_t y);
  int32_t getPosX();
  int32_t getPosY();

  //dimensions
  void setDimensions(uint32_t width, uint32_t height);
  void setWidth(uint32_t width);
  void setHeight(uint32_t height);
  uint32_t getWidth();
  uint32_t getHeight();

  uint8_t *getCanvasPtr() const {
    return this->canvas.getBuffer();
  }

  bool getCanvasPixel(int16_t x, int16_t y);

  //must be implemented in derived class!
  virtual void draw() = 0;
  virtual void update() {
    this->draw();
  }

  virtual DPOS_UIElement_Type getElementType() {
    return DPOS_ELEMENT_TYPE_NONE;
  }

  virtual uint32_t getElementBit() {
    return 0;
  }
};



#endif