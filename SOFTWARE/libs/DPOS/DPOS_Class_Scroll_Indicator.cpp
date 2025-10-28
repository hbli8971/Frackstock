#include "DPOS_Class_Scroll_Indicator.h"
#include "hbli_Logging.h"

static const char *tag = "Scroll-Indicator";

DPOS_ScrollIndicator::DPOS_ScrollIndicator(uint32_t width, uint32_t height)
    : DPOS_UIElement(width, height), totalSteps(0), currentStep(0),
      vertical(true) {}

DPOS_ScrollIndicator::~DPOS_ScrollIndicator() {}

void DPOS_ScrollIndicator::setNumberOfSteps(uint16_t steps) {
  this->totalSteps = steps;
}

void DPOS_ScrollIndicator::setCurrentStep(uint16_t step) {
  this->currentStep = step;
}

void DPOS_ScrollIndicator::draw() {

  uint8_t radius;
  uint16_t scrollIndicator;

  if (this->vertical) {
    radius = this->fieldWidth / 2;
  } else {
    radius = this->fieldHeight / 2;
  }

  if(this->vertical){
    scrollIndicator = map(this->currentStep, 0, this->totalSteps - 1,
                                 radius, this->fieldHeight - radius - 1);
  } else {
    scrollIndicator = map(this->currentStep, 0, this->totalSteps - 1,
                                 radius, this->fieldWidth - radius - 1);
  }

 
  this->canvas.fillScreen(0);
  if(this->vertical){
    this->canvas.drawFastVLine(radius, 0, this->fieldHeight, 1);
  } else {
    this->canvas.drawFastVLine(radius, 0, this->fieldWidth, 1);
  }
  
  this->canvas.fillCircle(radius, scrollIndicator, radius, 1);
}
void DPOS_ScrollIndicator::update() { this->draw(); }

void DPOS_ScrollIndicator::setHorizontal() { this->vertical = false; }

void DPOS_ScrollIndicator::setVertical() { this->vertical = true; }