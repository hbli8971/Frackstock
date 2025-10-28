#include "DPOS_Class_UIElement.h"

DPOS_UIElement::DPOS_UIElement(uint32_t w, uint32_t h)
  : posX(0),
    posY(0),
    fieldWidth(w),
    fieldHeight(h),
    canvas(w, h) {
}

DPOS_UIElement::~DPOS_UIElement() {
}


void DPOS_UIElement::setPos(int32_t x, int32_t y) {
  this->setPosX(x);
  this->setPosY(y);
}

void DPOS_UIElement::setPosX(int32_t x) {
  this->posX = x;
}

void DPOS_UIElement::setPosY(int32_t y) {
  this->posY = y;
}

int32_t DPOS_UIElement::getPosX() {
  return this->posX;
}

int32_t DPOS_UIElement::getPosY() {
  return this->posY;
}


//dimensions
void DPOS_UIElement::setDimensions(uint32_t width, uint32_t height) {
  this->setWidth(width);
  this->setHeight(height);
}

void DPOS_UIElement::setWidth(uint32_t width) {
  this->fieldWidth = width;
}

void DPOS_UIElement::setHeight(uint32_t height) {
  this->fieldHeight = height;
}

uint32_t DPOS_UIElement::getWidth() {
  return this->fieldWidth;
}

uint32_t DPOS_UIElement::getHeight() {
  return this->fieldHeight;
}

void DPOS_UIElement::debugPrintCanvas() {
  uint8_t* bufferPtr = this->canvas.getBuffer();

  if (!bufferPtr) {
    // LOGE("debugPrint", "empty buffer");
    Serial.println("no buf");
    return;
  }


  for (int16_t y = 0; y < this->fieldHeight; y++) {
    if (y < 10) {
      Serial.print(" ");
    }
    Serial.print(y);
    Serial.print("|");
    for (int16_t x = 0; x < this->fieldWidth; x++) {

      bool pixel = this->canvas.getPixel(x, y);

      pixel ? Serial.print("#") : Serial.print(" ");
    }
    Serial.print("|");
    Serial.println();
  }
}

bool DPOS_UIElement::getCanvasPixel(int16_t x, int16_t y) {
  return this->canvas.getPixel(x, y);
}