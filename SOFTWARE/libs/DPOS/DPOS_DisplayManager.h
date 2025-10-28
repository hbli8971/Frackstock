#include <sys/_stdint.h>

#ifndef _DPOS_DISPLAYMANAGER_H
#define _DPOS_DISPLAYMANAGER_H

#include <Arduino.h>
#include <vector>

#include "DPOS_Class_UIElement.h"
#include "DPOS_Class_Animation_Transition.h"
#include "DPOS_classes.h"



class DPOS_DisplayManager;

extern EventGroupHandle_t xDisplayEventGroup;
extern SemaphoreHandle_t xDisplayMutex;
extern DPOS_DisplayManager DPOS;

void DPOS_init_DisplayManager();

void DPOS_StartDPManager();
void DPOS_StopDPManager();

void DPOS_task(void *pvParameters);


class DPOS_DisplayManager {
private:
  std::vector<DPOS_Page *> pages;            //used in page-rendering mode
  std::vector<DPOS_UIElement *> textBodies;  //stores multiple textBodies for auto and queue mode
  std::vector<DPOS_UIElement *> elements;    //used for auto and queue rendering
  DPOS_Animation_Transition trans;           //class for Transition animation between pages
  uint32_t activatedElements;                //stores the bits for the activated elements
  uint16_t activePage;                       //stores the activated page ID
  uint16_t activePageTrans;                  //store the activated page ID (used for transition for the second page)
  uint8_t renderMode;                        //stores the renderMode
  uint8_t misc;

  void setStartPage(uint16_t pageID);

public:

  DPOS_DisplayManager();
  ~DPOS_DisplayManager();

  void setRenderModeAuto();
  void setRenderModeQueue();
  void setRenderModePage();

  void clearElements();
  void addElement(DPOS_UIElement *element);
  void activateElement(uint32_t element_bit);
  void deactivateElement(uint32_t element_bit);
  void deactivateAllElements();
  uint32_t getActivatedElements();

  void setTransitionMode(uint8_t mode);
  void setEndPage(uint16_t pageID);
  bool startTransition();

  void waitForTransition();


  bool addPage(DPOS_Page *page);
  bool removePage(uint16_t page_id);
  void setActivePage(uint16_t page_id);

  void draw();
  void update();
};

#endif