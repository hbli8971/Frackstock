#include <memory>
#include <vector>
#ifndef _DPOS_CLASS_MENU_H
#define _DPOS_CLASS_MENU_H

#include <Arduino.h>
#include "DPOS_cfg.h"
#include "DPOS_Class_UIElement.h"
#include "DPOS_Class_TextBody.h"
#include "DPOS_Class_Scroll_Indicator.h"



class DPOS_Menu : public DPOS_UIElement {
private:
  std::vector<std::shared_ptr<DPOS_TextBody>> tBodyEntries;
  DPOS_TextBody tBodyTitle;
  DPOS_ScrollIndicator scroll;
  int indexFirstVisible;
  int indexSelected;
  uint8_t numberOfEntries;
  uint8_t style;
  uint8_t visibleEntries;
  bool loop;

  uint8_t calculateSpacing();

public:

  DPOS_Menu();
  ~DPOS_Menu(); 

  void setTitle(const char* title);
  void setEntries(const char* entries[], uint8_t numOfEntries);
  // void setStyle(uint8_t style);
  void setTypeMenu();
  void setTypeInfo();
  void setTypeInfoBasic();


  //menu navigation
  void next();
  void prev();
  uint8_t select();

  void resetCursor();

  void draw() override;
  void update() override;

  DPOS_UIElement_Type getElementType() override {
    return DPOS_ELEMENT_TYPE_MENU;
  }
  uint32_t getElementBit() override {
    return DPOS_ELEMENT_BIT_MENU;
  }
};

#endif