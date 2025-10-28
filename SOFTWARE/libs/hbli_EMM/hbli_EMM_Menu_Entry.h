
#ifndef _HBLI_EMM_MENU_ENTRY_H
#define _HBLI_EMM_MENU_ENTRY_H
#include <Arduino.h>
#include <vector>
#include <functional>
#include <sys/_stdint.h>
#include "WString.h"

class MenuEntry {
private:
  String name;
  uint8_t id;
  MenuEntry* parent;
  std::vector<MenuEntry*> children;


public:
  std::function<void()> onEnter;
  std::function<void()> onLoop;
  std::function<void()> onExit;

  MenuEntry(String name, uint8_t id);
  ~MenuEntry();

  void addSubMenu(MenuEntry* child);
  // void execute();


  const String& getName() const;
  uint8_t getID() const;
  MenuEntry* getParent() const;
  const std::vector<MenuEntry*>& getSubMenus() const;
  uint8_t getSubMenuAmount() const;
};

#endif