#include "DPOS_Class_Tap_Indicator.h"
#include "hbli_Logging.h"




static const char* tag = "Tap-Indicator";

enum DPOS_TapIndicatorStyle {
  DEFAULT_NO_BG,
  DEFAULT_W_BG,
  RECT_ROUND_BG,
  RECT_ROUND_BORDER_BG
};


DPOS_TapIndicator::DPOS_TapIndicator()
  : DPOS_UIElement(DPOS_TAP_INDICATOR_WIDTH, DPOS_TAP_INDICATOR_HEIGHT),
    counter(0),
    style(RECT_ROUND_BORDER_BG),
    maxCounterDisplayValue(DPOS_TAP_INDICATOR_MAX_INDICATOR_VALUE) {
}

DPOS_TapIndicator::~DPOS_TapIndicator() {
}

void DPOS_TapIndicator::setCounter(uint8_t count) {
  this->counter = count;
}

uint8_t DPOS_TapIndicator::getCounter() {
  return this->counter;
}

void DPOS_TapIndicator::setStyle(uint8_t newStyle) {
  switch (newStyle) {
    case DEFAULT_NO_BG:
    case DEFAULT_W_BG:
    case RECT_ROUND_BG:
    case RECT_ROUND_BORDER_BG:
      this->style = newStyle;
      break;
    default:
      LOGE(tag, "setStyle", "invalid Style selected");
      break;
  }
}

void DPOS_TapIndicator::draw() {
  bool txtColor;

  if (DPOS_TAP_INDICATOR_HIDE_ON_0) {
    if (this->counter == 0) {
      this->canvas.fillScreen(0);
      return;
    }
  }


  this->canvas.fillScreen(this->style == DEFAULT_W_BG ? 1 : 0);

  switch (this->style) {
    case DEFAULT_NO_BG:
      txtColor = true;
      break;
    case DEFAULT_W_BG:
      txtColor = false;
      break;
    case RECT_ROUND_BG:
      txtColor = false;
      this->canvas.fillCircle((this->fieldWidth / 2) - 1, this->fieldHeight / 2, DPOS_TAP_INDICATOR_RADIUS, 1);
      break;
    case RECT_ROUND_BORDER_BG:
      txtColor = true;
      this->canvas.drawCircle((this->fieldWidth / 2) - 1, this->fieldHeight / 2, DPOS_TAP_INDICATOR_RADIUS, 1);
      break;
    default:
      LOGE(tag, "int-Draw", "ERROR:897");
      break;
  }

  this->canvas.setCursor((this->fieldWidth / 2) - (CHAR_WIDTH_SMALL / 2), (this->fieldHeight / 2) - (CHAR_HEIGHT_SMALL / 2) + 1);

  if (this->maxCounterDisplayValue == -1) {
    this->canvas.print(this->counter % 10);
  } else {
    if (this->counter > this->maxCounterDisplayValue) {
      this->canvas.print(this->maxCounterDisplayValue);
    } else {
      this->canvas.print(this->counter);
    }
  }
}
void DPOS_TapIndicator::update() {
  this->draw();
}