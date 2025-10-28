#ifndef _FS_ADC_H
#define _FS_ADC_H
#include <stdint.h>

//refVoltage [10/uV]
void initAdc(int16_t refVoltage);
uint8_t getBatteryVoltage();
float getBatteryVoltageRaw();

#endif