#include "hbli_EMM_Menu_Entry.h"
#include <hbli_Logging.h>

static const char* tag = "Menu-Entry";

MenuEntry::MenuEntry(String name, uint8_t id)
  : name(name), id(id), parent(nullptr) {
}

MenuEntry::~MenuEntry() {
}

void MenuEntry::addSubMenu(MenuEntry* child) {
  child->parent = this;
  children.push_back(child);
}

const String& MenuEntry::getName() const {
  return this->name;
}

uint8_t MenuEntry::getID() const {
  return this->id;
}

MenuEntry* MenuEntry::getParent() const {
  return this->parent;
}

const std::vector<MenuEntry*>& MenuEntry::getSubMenus() const {
  return this->children;
}

uint8_t MenuEntry::getSubMenuAmount() const {
  return this->children.size();
}