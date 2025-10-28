
#ifndef _DPOS_CLASS_PAGE_H
#define _DPOS_CLASS_PAGE_H

#include <Arduino.h>
#include <vector>
#include "DPOS_cfg.h"
#include "DPOS_Class_UIElement.h"

class DPOS_Page : public DPOS_UIElement {
private:
  std::vector<DPOS_UIElement*> pageElements;  //holds the pointers to all elements in the page
  uint16_t pageID;                            //stores the ID of the page, has to be set by user (avoid using same ID's!)
protected:
public:
  DPOS_Page();
  DPOS_Page(uint16_t id);
  ~DPOS_Page();

  void setPageID(uint16_t id);
  uint16_t getPageID();

  void addElement(DPOS_UIElement* element);

  void draw() override;
  void update() override;

  DPOS_UIElement_Type getElementType() override {
    return DPOS_ELEMENT_TYPE_PAGE;
  }
};

#endif