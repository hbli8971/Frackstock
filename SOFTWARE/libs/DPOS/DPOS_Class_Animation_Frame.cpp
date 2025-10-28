#include "esp32-hal.h"
#include "DPOS_Class_Animation_Frame.h"
#include "hbli_Logging.h"

static const char* tag = "Ani-Frame";

#define LOOP_ANIMATION (1 << 15)
#define START_ANIMATION (1 << 14)
#define ANIMAITON_DONE (1 << 13)
#define REPETITIONS_ANIMATION 0x00FF

DPOS_Animation_Frame::DPOS_Animation_Frame(uint16_t width, uint16_t height)
  : DPOS_UIElement(width, height),
    totalFrames(0),
    currentFrame(0),
    lastUpdate(0),
    frameDuration(0),
    misc(0) {
}

DPOS_Animation_Frame::~DPOS_Animation_Frame() {
  this->frames.clear();
}

void DPOS_Animation_Frame::addFrame(const uint8_t* frame) {
  this->frames.push_back(frame);
  this->totalFrames++;
}

void DPOS_Animation_Frame::setRepetitions(int8_t rep) {
  this->misc &= ~(REPETITIONS_ANIMATION);
  if (rep < 0) {
    this->misc |= LOOP_ANIMATION;
  } else {
    this->misc &= ~(LOOP_ANIMATION);
    uint8_t _repInternal = static_cast<uint8_t>(rep);
    this->misc |= _repInternal;
  }
}

void DPOS_Animation_Frame::setCurrentFrame(uint8_t step) {
  if (step < this->totalFrames) {
    this->currentFrame = step;
  } else {
    this->currentFrame = this->totalFrames - 1;
  }
}

void DPOS_Animation_Frame::startAnimation() {
  this->misc |= START_ANIMATION;
  this->misc &= ~(ANIMAITON_DONE);
}

void DPOS_Animation_Frame::stopAnimation() {
  this->misc &= ~(START_ANIMATION);
}

bool DPOS_Animation_Frame::getAniStartBit() {
  return (this->misc & START_ANIMATION) != 0;
}

bool DPOS_Animation_Frame::getAniLoopBit() {
  return (this->misc & LOOP_ANIMATION) != 0;
}

uint8_t DPOS_Animation_Frame::getAniRepetitions() {
  return static_cast<uint8_t>(this->misc & REPETITIONS_ANIMATION);
}

void DPOS_Animation_Frame::clearFrames() {
  this->frames.clear();
  this->currentFrame = 0;
  this->totalFrames = 0;
}

bool DPOS_Animation_Frame::getAnimationDone() {
  return this->misc & ANIMAITON_DONE;
}

void DPOS_Animation_Frame::draw() {
  this->canvas.drawBitmap(0, 0, this->frames.at(this->currentFrame), this->fieldWidth, this->fieldHeight, 1, 0);
  this->lastUpdate = millis();
}

void DPOS_Animation_Frame::update() {
  //check if animation is started
  if (!this->getAniStartBit()) {
    return;
  }

  if((!this->getAniLoopBit())){
    if(this->getAniRepetitions() <= 0){
      misc |= ANIMAITON_DONE;
      return;
    }
  }

  //check time for update
  if (millis() - this->lastUpdate < this->frameDuration) {
    return;
  }

  //move to next frame
  this->currentFrame++;

  // Check if we need to loop back or end the animation
  if (this->currentFrame >= this->totalFrames) {
    if (this->getAniLoopBit()) {
      this->currentFrame = 0;
    } else {
      // Reduce repetitions and loop back if there are repetitions left
      if (this->getAniRepetitions() > 0) {
        this->setRepetitions(this->getAniRepetitions() - 1);
        this->currentFrame = 0;
      } else {
        // No repetitions left, stop the animation
        this->stopAnimation();
      }
    }
  }

  this->draw();
}