#include "DPOS.h"
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "DPOS_cfg.h"
#include "DPOS_DisplayHandle.h" 
#include "hbli_Logging.h"

static const char* tag = "DPOS-Init";



void DPOS_init() {
  bool displayInit = true;
  if (!display.begin(SSD1306_SWITCHCAPVCC, DPOS_SSD_1306_I2C_ADDRESS)) {
    LOGE(tag, F("Display not found!"));
    displayInit = false;
    while (LOCK_PRG_ON_DISPLAY_FAIL) {};
  }
  if (displayInit) {
    display.setTextWrap(false);
    display.setTextColor(SSD1306_WHITE);
    display.setTextSize(TEXT_SIZE_SMALL);
    display.clearDisplay();
    display.display();
    DPOS_init_DisplayManager();
  } else {
    LOGE(tag, F("display init failed!, returning to code"), F("DPOS will not work!"));
  }
}