#include <sys/_stdint.h>
#include <vector>
#ifndef _DPOS_CLASS_ANIMATION_FRAME_H
#define _DPOS_CLASS_ANIMATION_FRAME_H

#include <Arduino.h>
#include "DPOS_Class_UIElement.h"
#include "DPOS_cfg.h"



class DPOS_Animation_Frame : public DPOS_UIElement {
private:
  std::vector<const uint8_t*> frames;
  uint16_t totalFrames;
  uint16_t currentFrame;
  unsigned long lastUpdate;
  unsigned long frameDuration;
  uint16_t misc;

  bool getAniStartBit();
  bool getAniLoopBit();
  uint8_t getAniRepetitions();


public:
  DPOS_Animation_Frame(uint16_t width, uint16_t height);
  ~DPOS_Animation_Frame();

  void addFrame(const uint8_t* frame);
  void setRepetitions(int8_t rep);
  void setCurrentFrame(uint8_t step);
  void startAnimation();
  void stopAnimation();

  bool getAnimationDone();

  void clearFrames();



  void draw() override;
  void update() override;

  DPOS_UIElement_Type getElementType() override {
    return DPOS_ELEMENT_TYPE_ANI_FRAME;
  }
  uint32_t getElementBit() override {
    return DPOS_ELEMENT_BIT_ANI_FRAME;
  }
};
#endif