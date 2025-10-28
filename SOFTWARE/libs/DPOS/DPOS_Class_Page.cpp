#include "DPOS_Class_Page.h"
#include "hbli_Logging.h"

static const char* tag = "Page";

DPOS_Page::DPOS_Page()
  : DPOS_UIElement(DPOS_SCREEN_WIDTH, DPOS_SCREEN_HEIGHT),
    pageID(0) {
}

DPOS_Page::DPOS_Page(uint16_t id)
  : DPOS_UIElement(DPOS_SCREEN_WIDTH, DPOS_SCREEN_HEIGHT),
    pageID(id) {
}

DPOS_Page::~DPOS_Page() {
  this->pageElements.clear();
}

void DPOS_Page::setPageID(uint16_t id) {
  this->pageID = id;
}

uint16_t DPOS_Page::getPageID() {
  return this->pageID;
}

void DPOS_Page::addElement(DPOS_UIElement* element) {
  if(element->getElementType() != DPOS_ELEMENT_TYPE_NONE){
    this->pageElements.push_back(element);
  } else {
    LOGE(tag, F("invalid Element-Type for this operation"));
  }
}

void DPOS_Page::draw() {
  if (this->pageID == 0) {
    LOGE(tag, F("PageID not set"), F("set PageID first!"));
    return;
  }

  for (auto* element : this->pageElements) {
    element->draw();
    this->canvas.drawBitmap(element->getPosX(), element->getPosY(), element->getCanvasPtr(), element->getWidth(), element->getHeight(), 1, 0);
  }
}

void DPOS_Page::update() {
  for (auto* element : this->pageElements) {
    element->update();
    this->canvas.drawBitmap(element->getPosX(), element->getPosY(), element->getCanvasPtr(), element->getWidth(), element->getHeight(), 1, 0);
  }
}