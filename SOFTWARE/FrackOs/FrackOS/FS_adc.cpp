#include "FS_adc.h"
#include "hal/adc_types.h"
#include "driver/adc_types_legacy.h"
#include <driver/adc.h>
#include <hbli_Logging.h>
#include "config.h"

#define BAT_REF_VOLTAGE 4
#define BAR_REF_FACTOR_REFVOLTAGE 10000

static const char* tag = "ADC";
static float adcRefVoltage = 0;

//refVoltage is offset from 4V ; refVoltage/10000 [V], so refVoltage = 10000 -> BAT_REF_VOLTAGE + 1V; refVoltage = 100 -> BAT_REF_VOLTAGE + BAT_REF_VOLTAGE + 10mV
//refVoltage unit is -> refVoltage * 10uV
void initAdc(int16_t refVoltage) {
  adc1_config_width(ADC_WIDTH_BIT_12);
  adc1_config_channel_atten(ADC1_CHANNEL_0, ADC_ATTEN_DB_12);

  LOGI(tag, F("initialized channel 0"));

  adcRefVoltage = float(refVoltage) / float(BAR_REF_FACTOR_REFVOLTAGE);

  LOGV(tag, F("Ref-Voltage is"), BAT_REF_VOLTAGE + adcRefVoltage);
}

float getBatteryVoltageRaw() {
  int adcValue = adc1_get_raw(ADC1_CHANNEL_0);
  return adcValue * ((BAT_REF_VOLTAGE + adcRefVoltage) / 4095.0) * 2;
}

uint8_t getBatteryVoltage() {
  float batVoltage = getBatteryVoltageRaw();

  if (batVoltage > BAT_V_100) {  //100%
    return 100;
  } else if (batVoltage > BAT_V_75) {  // 75%
    return 75;
  } else if (batVoltage > BAT_V_50) {  // 50%
    return 50;
  } else if (batVoltage > BAT_V_25) {  // 25%
    return 25;
  } else {  //  0%
    return 0;
  }
}
