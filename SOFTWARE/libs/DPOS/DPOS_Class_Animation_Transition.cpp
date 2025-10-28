#include "DPOS_Class_Animation_Transition.h"
#include "hbli_Logging.h"

#define DPOS_ANIMATION_TRANSITION_START_BIT (1 << 7)
#define DPOS_ANIMATION_TRANSITION_DONE_BIT (1 << 6)

#define DPOS_ANIMATION_TRANSITION_MODE_MASK 0x0F

#define DPOS_ANIMATION_TRANSITION_START_BUFFER 0
#define DPOS_ANIMATION_TRANSITION_END_BUFFER 1
#define DPOS_ANIMATION_TRANSITION_STEPS 10
#define DPOS_ANIMATION_TRANSITION_MAX_RADIUS 73


static const char* tag = "ani-Transition";





DPOS_Animation_Transition::DPOS_Animation_Transition()
  : DPOS_UIElement(DPOS_SCREEN_WIDTH, DPOS_SCREEN_HEIGHT),
    startPage(nullptr),
    endPage(nullptr),
    currentStep(0),
    misc(0),
    radiusInternal(0) {
}


DPOS_Animation_Transition::~DPOS_Animation_Transition() {
}


void DPOS_Animation_Transition::setStartPage(DPOS_Page* start) {
  this->startPage = start;
}

void DPOS_Animation_Transition::setEndPage(DPOS_Page* end) {
  this->endPage = end;
  LOGI(tag, "endPage-ID is", end->getPageID());
}

void DPOS_Animation_Transition::setTransitionMode(uint8_t mode) {
  switch (mode) {
    case DPOS_ANITRANS_CIRCLE:
    case DPOS_ANITRANS_LINE_TOPDOWN:
    case DPOS_ANITRANS_LINE_LEFT2RIGHT:
    case DPOS_ANITRANS_LINE_RIGHT2LEFT:
    case DPOS_ANITRANS_LINE_DIAGN:
      this->misc &= ~(DPOS_ANIMATION_TRANSITION_MODE_MASK);
      this->misc |= (mode & DPOS_ANIMATION_TRANSITION_MODE_MASK);
      break;

    default:
      LOGE(tag, F("setTransitionMode"), F("invalid mode"));
      break;
  }
}

void DPOS_Animation_Transition::startTransition() {
  this->currentStep = 0;
  this->misc |= DPOS_ANIMATION_TRANSITION_START_BIT;
}

bool DPOS_Animation_Transition::getTransitionDone(bool resetFlag) {
  bool result = this->misc & DPOS_ANIMATION_TRANSITION_DONE_BIT;
  if (resetFlag) {
    if (result) {
      this->misc &= ~(DPOS_ANIMATION_TRANSITION_DONE_BIT);
    }
  }
  return result;
}

void DPOS_Animation_Transition::_aniTransConditionalDraw(bool condition, int16_t _x_, int16_t _y_) {
  if (condition) {
    this->canvas.drawPixel(_x_, _y_, this->startPage->getCanvasPixel(_x_, _y_));
  } else {
    this->canvas.drawPixel(_x_, _y_, this->endPage->getCanvasPixel(_x_, _y_));
  }
}

void DPOS_Animation_Transition::draw() {
  // LOGI(tag, "mode is", this->misc & DPOS_ANIMATION_TRANSITION_MODE_MASK);
  switch (this->misc & DPOS_ANIMATION_TRANSITION_MODE_MASK) {
    case DPOS_ANITRANS_CIRCLE:
      // Define the bounding box for the circle to limit the number of iterations
      this->radiusInternal = (this->currentStep * DPOS_ANIMATION_TRANSITION_MAX_RADIUS) / DPOS_ANIMATION_TRANSITION_STEPS;

      for (int16_t y = 0; y < DPOS_SCREEN_HEIGHT; y++) {
        int _dy = y - DPOS_SCREEN_HEIGHT / 2;

        for (int16_t x = 0; x < DPOS_SCREEN_WIDTH; x++) {
          int byteIndex = x + (y / 8) * DPOS_SCREEN_WIDTH;
          int bitIndex = y % 8;
          int _dx = x - DPOS_SCREEN_WIDTH / 2;

          _aniTransConditionalDraw((pow(_dx, 2) + pow(_dy, 2)) >= pow(this->radiusInternal, 2), x, y);
        }
      }
      // Draw the circle border
      this->canvas.drawCircle(DPOS_SCREEN_WIDTH / 2, DPOS_SCREEN_HEIGHT / 2, this->radiusInternal, 1);
      break;

    case DPOS_ANITRANS_LINE_TOPDOWN:

      for (int16_t y = 0; y < DPOS_SCREEN_HEIGHT; y++) {
        for (int16_t x = 0; x < DPOS_SCREEN_WIDTH; x++) {
          _aniTransConditionalDraw(y > (this->currentStep * (DPOS_SCREEN_HEIGHT / DPOS_ANIMATION_TRANSITION_STEPS)), x, y);
        }
      }
      this->canvas.drawFastHLine(0, (this->currentStep * (DPOS_SCREEN_HEIGHT / DPOS_ANIMATION_TRANSITION_STEPS)), DPOS_SCREEN_WIDTH, 1);
      break;

    case DPOS_ANITRANS_LINE_RIGHT2LEFT:
      for (int16_t y = 0; y < DPOS_SCREEN_HEIGHT; y++) {
        for (int16_t x = 0; x < DPOS_SCREEN_WIDTH; x++) {
          _aniTransConditionalDraw(x > (DPOS_SCREEN_WIDTH - (this->currentStep * (DPOS_SCREEN_WIDTH / DPOS_ANIMATION_TRANSITION_STEPS))), x, y);
        }
      }
      this->canvas.drawFastVLine((DPOS_SCREEN_WIDTH - (this->currentStep * (DPOS_SCREEN_WIDTH / DPOS_ANIMATION_TRANSITION_STEPS))), 0, DPOS_SCREEN_HEIGHT, 1);
      break;

    case DPOS_ANITRANS_LINE_LEFT2RIGHT:
      for (int16_t y = 0; y < DPOS_SCREEN_HEIGHT; y++) {
        for (int16_t x = 0; x < DPOS_SCREEN_WIDTH; x++) {
          _aniTransConditionalDraw(x > (this->currentStep * (DPOS_SCREEN_WIDTH / DPOS_ANIMATION_TRANSITION_STEPS)), x, y);
        }
      }
      this->canvas.drawFastVLine(((this->currentStep * (DPOS_SCREEN_WIDTH / DPOS_ANIMATION_TRANSITION_STEPS))), 0, DPOS_SCREEN_HEIGHT, 1);
      break;
    case DPOS_ANITRANS_LINE_DIAGN:
      for (int16_t y = 0; y < DPOS_SCREEN_HEIGHT; y++) {
        for (int16_t x = 0; x < DPOS_SCREEN_WIDTH; x++) {
          _aniTransConditionalDraw((x + y) > (this->currentStep * ((DPOS_SCREEN_WIDTH + DPOS_SCREEN_HEIGHT) / DPOS_ANIMATION_TRANSITION_STEPS)), x, y);
        }
      }      
      break;
  }
}

void DPOS_Animation_Transition::update() {
  if (misc & DPOS_ANIMATION_TRANSITION_START_BIT) {
    if (this->currentStep < DPOS_ANIMATION_TRANSITION_STEPS) {
      this->draw();
      this->currentStep++;
    } else {
      this->misc &= ~(DPOS_ANIMATION_TRANSITION_START_BIT);  //stop animation
      this->misc |= DPOS_ANIMATION_TRANSITION_DONE_BIT;
      this->currentStep = 0;
    }
  }
}
