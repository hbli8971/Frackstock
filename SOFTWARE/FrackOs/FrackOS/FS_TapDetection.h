#include <sys/_stdint.h>
#ifndef _FS_TAPDETECTION_H
#define _FS_TAPDETECTION_H

#include "config.h"
#include "FS_Datatypes.h"


extern SystemInfo_t sys;
extern TapDetect_t tapDetect;
extern Lis3dh_t lisCfg;

void lis3dh_loadDefaultConfig(Lis3dh_t* data);

bool TapsDetected(uint8_t tap_amount);
bool TapsDetectedMoreThan(uint8_t tap_amount);

void initTapDetection(Lis3dh_t* init);
uint8_t resetTapInterrupt();
void tapDetectedISR();

void lis3dh_set_range(uint8_t range);
void lis3dh_set_thresh(uint8_t thresh);
void lis3dh_set_time_lim(uint8_t lim);
void lis3dh_set_time_latency(uint8_t latency);
void lis3dh_set_window(uint8_t window);

#endif