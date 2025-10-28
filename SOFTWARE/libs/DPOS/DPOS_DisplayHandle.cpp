#include "DPOS_DisplayHandle.h"
#include "DPOS_cfg.h"

Adafruit_SSD1306 display(DPOS_SCREEN_WIDTH, DPOS_SCREEN_HEIGHT, &Wire, -1);