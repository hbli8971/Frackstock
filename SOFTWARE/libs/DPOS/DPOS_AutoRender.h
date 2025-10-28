#ifndef _DPOS_AUTORENDER_H
#define _DPOS_AUTORENDER_H
#include <Arduino.h>

#define DPOS_AUTORENDER_QUEUES_AMOUNT 5

typedef bool (*RenderCallback)();

typedef struct {
  const uint32_t* elements;
  uint32_t size;
  RenderCallback callback;
} RenderBlock_t;

extern RenderBlock_t DPOS_AUTORENDER_LIST[DPOS_AUTORENDER_QUEUES_AMOUNT];

void DPOS_setupAutoRender();
#endif