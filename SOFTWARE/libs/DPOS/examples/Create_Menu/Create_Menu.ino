#include <DPOS.h>

/*
  I2C Pin definitions for Frackstock-Board
*/

#define FS_PIN_I2C_SDA 6
#define FS_PIN_I2C_SCL 7
#define FS_PIN_BUTTON 9
#define FS_PIN_BUTTON_DEFAULT_VALUE 1

DPOS_Menu menu;

const char* exampleTextList[]{
  "menu entry 1",
  "menu entry 2 is very long to show that scrolling works",
  "menu entry 3",
  "menu entry 4",
  "menu entry 5",
  "menu entry 6 is another very long entry to show again, that scrolling works just fine",
  "menu entry 7",
  "menu entry 8 is the last one"
};

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Wire.begin(FS_PIN_I2C_SDA, FS_PIN_I2C_SCL);
  pinMode(FS_PIN_BUTTON, INPUT);
  DPOS_init();



  menu.setTitle("TITLE");
  menu.setTextOffsetY(1);
  menu.setEntries(exampleTextList, 8);
  menu.setMenuStyle(DPOS_MENU_STYLE_MENU);


  dp.addElement(&menu);
  dp.setRenderMode(DPOS_DPMANAGER_RENDER_MODE_AUTO);
  dp.activateElement(DPOS_ELEMENT_MENU);
  dp.draw();

  DPOS_StartDPManager();
}

void loop() {
  // put your main code here, to run repeatedly:
  if (digitalRead(FS_PIN_BUTTON) != FS_PIN_BUTTON_DEFAULT_VALUE) {
    menu.next();
    dp.draw();
  }

  vTaskDelay(100 / portTICK_PERIOD_MS); // when using DPOS, don't use delay(), use non-blocking waits / delays
}
