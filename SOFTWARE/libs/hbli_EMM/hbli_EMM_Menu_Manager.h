#ifndef _HBLI_EMM_MENU_MANAGER_H
#define _HBLI_EMM_MENU_MANAGER_H
#include <Arduino.h>
#include "hbli_EMM_Menu_Entry.h"

class MenuManager {
private:
  MenuEntry* currentMenu;
  MenuEntry* rootMenu;

public:
  MenuManager(MenuEntry* rootMenu);

  void goTo(MenuEntry* menu);
  void goToSubmenu(uint8_t index);
  void goBack();
  void goBackToRoot();

  void loop();

  MenuEntry* getCurrentMenu() const;
};

#endif