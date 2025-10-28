#include "DPOS_Class_Scroll_Indicator.h"
#include "DPOS_Class_Menu.h"
#include "hbli_Logging.h"
#include <memory>

#define MENU_TYPE_INFO 0
#define MENU_TYPE_MENU 1
#define MENU_TYPE_INFOBASIC 2

static const char* tag = "Menu";



DPOS_Menu::DPOS_Menu()
  : DPOS_UIElement(DPOS_SCREEN_WIDTH, DPOS_SCREEN_HEIGHT),
    scroll(DPOS_SCROLL_INDICATOR_WIDTH, DPOS_SCROLL_INDICATOR_HEIGHT),
    tBodyTitle(DPOS_MENU_TITLE_POS_X, DPOS_MENU_TITLE_POS_Y, DPOS_MENU_TITLE_WIDTH, DPOS_MENU_TITLE_HEIGHT),
    numberOfEntries(0),
    style(MENU_TYPE_INFO),
    visibleEntries(0),
    indexFirstVisible(0),
    indexSelected(0),
    loop(true) {
  tBodyTitle.setTextSize(MEDIUM);
  tBodyTitle.setAlignment(CENTER);
  tBodyTitle.setStyle(RECT_ROUND_BORDER_BG);
  tBodyTitle.setStyleSetting(4);
  tBodyTitle.setTextColor(1);
  tBodyTitle.setTextOffsetY(2);
  tBodyTitle.setScrollDelay(DPOS_SCROLL_DELAY_TITLE_START);
}

DPOS_Menu::~DPOS_Menu() {
  tBodyEntries.clear();
}

void DPOS_Menu::setTitle(const char* title) {
  tBodyTitle.setText(title);
  if (tBodyTitle.getStringOversizeBit()) {
    tBodyTitle.setTextOffsetX(DPOS_MENU_TITLE_OFFSET_IF_OVERSIZE);
  }
}

void DPOS_Menu::setEntries(const char** entries, uint8_t numOfEntries) {

  this->numberOfEntries = numOfEntries;
  //size scroll indicator

  this->tBodyEntries.clear();

  //set numer of visible entries
  if (numOfEntries > DPOS_MENU_ENTRY_MAX_NUM) {
    this->visibleEntries = DPOS_MENU_ENTRY_MAX_NUM;
  } else {
    this->visibleEntries = numOfEntries;
  }

  LOGV(tag, "number of entries", this->numberOfEntries);
  LOGV(tag, "number of visible entires", this->visibleEntries);

  for (int i = 0; i < numOfEntries; i++) {
    std::shared_ptr<DPOS_TextBody> newEntry = std::make_shared<DPOS_TextBody>(DPOS_MENU_ENTRY_WIDTH, DPOS_MENU_ENTRY_HEIGHT);

    newEntry->setTextColor(1);
    newEntry->setTextSize(1);
    newEntry->setStyle(RECT_ROUND_BORDER_BG);
    newEntry->setStyleSetting(3);
    newEntry->setTextOffsetX(3);
    newEntry->setText(entries[i]);

    this->tBodyEntries.push_back(newEntry);
  }
  scroll.setNumberOfSteps(numOfEntries);
  scroll.setCurrentStep(0);
}

void DPOS_Menu::setTypeInfo() {
  this->style = MENU_TYPE_INFO;
}

void DPOS_Menu::setTypeMenu() {
  this->style = MENU_TYPE_MENU;
}

void DPOS_Menu::setTypeInfoBasic() {
  this->style = MENU_TYPE_INFOBASIC;
}

void DPOS_Menu::next() {
  if (this->loop) {
    this->indexSelected = (this->indexSelected + 1) % this->numberOfEntries;
    if (this->indexSelected == 0) {
      this->indexFirstVisible = 0;
    } else if (this->indexSelected >= this->indexFirstVisible + this->visibleEntries) {
      this->indexFirstVisible++;
    }
  } else {
    if (this->indexSelected < this->numberOfEntries - 1) {
      this->indexSelected++;
      if (this->indexSelected >= this->indexFirstVisible + this->visibleEntries) {
        this->indexFirstVisible++;
      }
    }
  }
}

void DPOS_Menu::prev() {
  if (this->loop) {
    this->indexSelected = (this->indexSelected - 1 + this->numberOfEntries) % this->numberOfEntries;
    if (this->indexSelected == this->numberOfEntries - 1) {
      this->indexFirstVisible = this->numberOfEntries - this->visibleEntries;
    } else if (this->indexSelected < this->indexFirstVisible) {
      this->indexFirstVisible = (this->indexFirstVisible - 1 + this->numberOfEntries) % this->numberOfEntries;
    }
  } else {
    if (this->indexSelected > 0) {
      this->indexSelected--;
      if (this->indexSelected < this->indexFirstVisible) {
        this->indexFirstVisible--;
      }
    }
  }
}

uint8_t DPOS_Menu::select() {
  return static_cast<uint8_t>(this->indexSelected);
}

void DPOS_Menu::resetCursor() {
  this->indexFirstVisible = 0;
  this->indexSelected = 0;
}

void DPOS_Menu::draw() {
  uint8_t spacing = calculateSpacing();
  int _cursorY;
  uint8_t indexJ = 0;

  this->canvas.fillScreen(0);

  this->tBodyTitle.draw();
  this->canvas.drawBitmap(tBodyTitle.getPosX(), tBodyTitle.getPosY(), tBodyTitle.getCanvasPtr(), tBodyTitle.getWidth(), tBodyTitle.getHeight(), 1);

  for (int i = 0; i < this->tBodyEntries.size(); i++, indexJ++) {

    std::shared_ptr<DPOS_TextBody> entry = this->tBodyEntries[i];

    //only process if in visible range
    if ((i >= this->indexFirstVisible) && (i < this->indexFirstVisible + this->visibleEntries)) {
      entry->setHasBeenSeen();

      switch (this->style) {
        case MENU_TYPE_INFO:
          entry->setStyle(RECT_ROUND_BORDER_BG);
          entry->setTextColor(1);
          break;

        case MENU_TYPE_MENU:
          if (i == this->indexSelected) {
            entry->setStyle(RECT_ROUND_BG);
            entry->setTextColor(0);
          } else {
            entry->setStyle(RECT_ROUND_BORDER_BG);
            entry->setTextColor(1);
          }
          break;

          case MENU_TYPE_INFOBASIC:
            entry->setStyle(DEFAULT_NO_BG);
          break;

        default:
          LOGE(tag, F("int-Err draw"), F("ERROR: 897"));
          break;
      }

      entry->draw();
      _cursorY = DPOS_MENU_ENTRIES_START_POS_Y + (spacing / 2) + (indexJ * (DPOS_MENU_ENTRY_HEIGHT + (spacing / 2)));
      this->canvas.drawBitmap(0, _cursorY, entry->getCanvasPtr(), entry->getWidth(), entry->getHeight(), 1);

    } else {
      entry->resetHasBeenSeen();
    }

    if (entry->getHasBeenSeen()) {
      entry->setHasBeenSeenOld();
    } else {
      entry->setScrollPosition(0);
      entry->setScrollDirection(0);
      entry->setScrollDelay(DPOS_SCROLL_DELAY_DEFAULT);
    }
  }
  scroll.setCurrentStep(this->indexSelected);
  scroll.draw();
  this->canvas.drawBitmap(DPOS_SCROLL_INDICATOR_POS_X, DPOS_SCROLL_INDICATOR_POS_Y, scroll.getCanvasPtr(), scroll.getWidth(), scroll.getHeight(), 1);
}

void DPOS_Menu::update() {
  uint8_t spacing = calculateSpacing();
  int _cursorY;
  uint8_t indexJ = 0;

  this->tBodyTitle.update();

  for (int i = this->indexFirstVisible; i < this->indexFirstVisible + this->visibleEntries; i++, indexJ++) {
    std::shared_ptr<DPOS_TextBody> entry = this->tBodyEntries[i];

    entry->update();

    _cursorY = DPOS_MENU_ENTRIES_START_POS_Y + (spacing / 2) + (indexJ * (DPOS_MENU_ENTRY_HEIGHT + (spacing / 2)));
    this->canvas.drawBitmap(0, _cursorY, entry->getCanvasPtr(), entry->getWidth(), entry->getHeight(), 1, 0);
  }

  this->canvas.drawBitmap(tBodyTitle.getPosX(), tBodyTitle.getPosY(), tBodyTitle.getCanvasPtr(), tBodyTitle.getWidth(), tBodyTitle.getHeight(), 1, 0);
}

uint8_t DPOS_Menu::calculateSpacing() {
  if (this->numberOfEntries < DPOS_MENU_ENTRY_MAX_NUM) {
    return (((DPOS_SCREEN_HEIGHT - DPOS_MENU_ENTRIES_START_POS_Y) - (this->numberOfEntries * DPOS_MENU_ENTRY_HEIGHT)) / this->numberOfEntries);
  } else {
    return 2;
  }
}