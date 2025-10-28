#include "hbli_EMM_Menu_Manager.h"
#include <hbli_Logging.h>

static const char* tag = "Menu-Manager";

MenuManager::MenuManager(MenuEntry* rootMenu)
  : currentMenu(rootMenu),
    rootMenu(rootMenu) {
}

void MenuManager::goTo(MenuEntry* menu) {
  if (menu && currentMenu && currentMenu->onExit) {
    currentMenu->onExit();  // Call exit action for the current menu
  }
  currentMenu = menu;
  if (currentMenu && currentMenu->onEnter) {
    currentMenu->onEnter();  // Call enter action for the new menu
  }
}

void MenuManager::goToSubmenu(uint8_t index) {
  if (index < currentMenu->getSubMenus().size()) {
    goTo(currentMenu->getSubMenus()[index]);
  } else {
    LOGE(tag, F("Submenu not found"));
  }
}

void MenuManager::goBack() {
  if (currentMenu && currentMenu->getParent()) {
    goTo(currentMenu->getParent());
  } else {
    LOGE(tag, F("Parent not found"));
  }
}

void MenuManager::goBackToRoot() {
  this->goTo(this->rootMenu);
}

void MenuManager::loop() {
  if(this->currentMenu && currentMenu->onLoop){
    this->currentMenu->onLoop();
  }
}

MenuEntry* MenuManager::getCurrentMenu() const {
  return currentMenu;
}