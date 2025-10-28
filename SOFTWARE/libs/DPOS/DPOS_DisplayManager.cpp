#include "freertos/projdefs.h"
#include "DPOS_DisplayManager.h"
#include "hbli_Logging.h"
#include "DPOS_DisplayHandle.h"
#include "DPOS_AutoRender.h"


//bits for EventGroup:
#define DPOS_DISPLAY_AVAILABLE (1 << 0)
#define DPOS_TRANSITION_DONE (1 << 1)

//bits for misc:
#define DPOS_DPMANAGER_BIT_TRANS_START (1 << 0)

#define DPOS_RENDERMODE_AUTO 1
#define DPOS_RENDERMODE_QUEUE 2
#define DPOS_RENDERMODE_PAGE 3


static const char* tag = "DisplayManager";

EventGroupHandle_t xDisplayEventGroup;
SemaphoreHandle_t xDisplayMutex;
DPOS_DisplayManager DPOS;


void DPOS_StartDPManager() {
  xEventGroupSetBits(xDisplayEventGroup, DPOS_DISPLAY_AVAILABLE);
}

void DPOS_StopDPManager() {
  xEventGroupClearBits(xDisplayEventGroup, DPOS_DISPLAY_AVAILABLE);
}

void DPOS_LockDPManager() {
  xSemaphoreTake(xDisplayMutex, portMAX_DELAY);
}

void DPOS_ReleaseDPManager() {
  xSemaphoreGive(xDisplayMutex);
}

void DPOS_task(void* pvParameters) {
  while (1) {
    //read "DPOS_DISPLAY_AVAILABLE"-bit, don't change value of "xDisplayEventGroup", wait until bit is set
    EventBits_t uxBits = xEventGroupWaitBits(xDisplayEventGroup, DPOS_DISPLAY_AVAILABLE, pdFALSE, pdFALSE, portMAX_DELAY);
    if (uxBits & DPOS_DISPLAY_AVAILABLE) {
      //perform update cycle when Display is available
      DPOS.update();
    }
    vTaskDelay(DPOS_TASK_TIME_MS / portTICK_PERIOD_MS);
  }
}

void DPOS_init_DisplayManager() {
  xDisplayMutex = xSemaphoreCreateMutex();
  xDisplayEventGroup = xEventGroupCreate();
  xSemaphoreGive(xDisplayMutex);

  DPOS_setupAutoRender();

  xTaskCreate(DPOS_task, "Display-task (DPOS)", 4096 * 2, NULL, 3, NULL);
}


DPOS_DisplayManager::DPOS_DisplayManager()
  : activatedElements(0),
    activePage(0),
    activePageTrans(0),
    renderMode(DPOS_RENDERMODE_PAGE),
    misc(0) {
}

DPOS_DisplayManager::~DPOS_DisplayManager() {
  this->pages.clear();
  this->textBodies.clear();
  this->elements.clear();
}

void DPOS_DisplayManager::setRenderModeAuto() {
  this->renderMode = DPOS_RENDERMODE_AUTO;
}

void DPOS_DisplayManager::setRenderModeQueue() {
  this->renderMode = DPOS_RENDERMODE_QUEUE;
}

void DPOS_DisplayManager::setRenderModePage() {
  this->renderMode = DPOS_RENDERMODE_PAGE;
}

void DPOS_DisplayManager::clearElements() {
  this->elements.clear();
  this->textBodies.clear();
}

void DPOS_DisplayManager::addElement(DPOS_UIElement* element) {
  if (element->getElementType() == DPOS_ELEMENT_TYPE_NONE) {
    LOGE(tag, F("invalid Element for this operation"));
  }
  switch (element->getElementType()) {
    case DPOS_ELEMENT_TYPE_TEXTBODY:
      this->textBodies.push_back(element);
      break;

    case DPOS_ELEMENT_TYPE_PAGE:
      LOGE(tag, F("to add a page call"), F("addPage(&pageName)"));
      break;

    default:
      this->elements.push_back(element);
      break;
  }
}

void DPOS_DisplayManager::activateElement(uint32_t element_bit) {
  this->activatedElements |= element_bit;
}

void DPOS_DisplayManager::deactivateElement(uint32_t element_bit) {
  this->activatedElements &= ~(element_bit);
}

void DPOS_DisplayManager::deactivateAllElements() {
  this->activatedElements = 0;
}

uint32_t DPOS_DisplayManager::getActivatedElements() {
  return this->activatedElements;
}

bool DPOS_DisplayManager::addPage(DPOS_Page* page) {
  this->pages.push_back(page);
  return true;
}

bool DPOS_DisplayManager::removePage(uint16_t page_id) {
  for (auto i = this->pages.begin(); i != this->pages.end(); i++) {
    if ((*i)->getPageID() == page_id) {
      this->pages.erase(i);
      return true;
    }
  }
  return false;
}

void DPOS_DisplayManager::setActivePage(uint16_t page_id) {
  this->activePage = page_id;
}

void DPOS_DisplayManager::setTransitionMode(uint8_t mode) {
  this->trans.setTransitionMode(mode);
}

void DPOS_DisplayManager::setEndPage(uint16_t pageID) {
  for (auto* page : this->pages) {
    if (page->getPageID() == pageID) {
      this->trans.setEndPage(page);
      this->activePageTrans = pageID;
      break;
    }
  }
}

bool DPOS_DisplayManager::startTransition() {
  if (this->activePage == 0) {
    LOGE(tag, F("set active Page first"));
    return false;
  }

  if (this->activePageTrans == 0) {
    LOGE(tag, F("set end Page first"));
    return false;
  }

  this->setStartPage(this->activePage);
  this->trans.startTransition();
  this->trans.draw();
  this->misc |= DPOS_DPMANAGER_BIT_TRANS_START;  //start transition
  return true;
}

void DPOS_DisplayManager::draw() {
  uint8_t _err = 1;
  switch (this->renderMode) {
    case DPOS_RENDERMODE_AUTO:
      //itterate over all autoRender-blocks
      for (int i = 0; i < DPOS_AUTORENDER_QUEUES_AMOUNT; i++) {

        //extract current autoRender-block
        RenderBlock_t _block = DPOS_AUTORENDER_LIST[i];

        //check callback return val and if it exists
        if (_block.callback != nullptr && _block.callback()) {

          //itterate over entire autoRender-block
          for (int j = 0; j < _block.size; j++) {

            //only draw if element is even activated
            if (_block.elements[j] & this->activatedElements) {

              if (_block.elements[j] == DPOS_ELEMENT_BIT_TEXTBODY) {
                for (auto* textBody : this->textBodies) {
                  textBody->draw();
                  display.drawBitmap(textBody->getPosX(), textBody->getPosY(), textBody->getCanvasPtr(), textBody->getWidth(), textBody->getHeight(), 1, 0);
                }
              } else {
                for (auto* element : this->elements) {
                  //search corresponding element in vector and draw
                  if (element->getElementBit() == _block.elements[j]) {
                    element->draw();
                    display.drawBitmap(element->getPosX(), element->getPosY(), element->getCanvasPtr(), element->getWidth(), element->getHeight(), 1, 0);
                  }
                }
              }
            }
          }
        }
      }
      break;

    case DPOS_RENDERMODE_QUEUE:
      for (auto* element : this->elements) {
        element->draw();
        display.drawBitmap(element->getPosX(), element->getPosY(), element->getCanvasPtr(), element->getWidth(), element->getHeight(), 1, 0);
      }

      for (auto* textBody : this->textBodies) {
        textBody->draw();
        display.drawBitmap(textBody->getPosX(), textBody->getPosY(), textBody->getCanvasPtr(), textBody->getWidth(), textBody->getHeight(), 1, 0);
      }
      break;
    case DPOS_RENDERMODE_PAGE:
      for (auto* page : this->pages) {
        if (page->getPageID() == this->activePage) {
          _err = 0;
          page->draw();
          display.drawBitmap(0, 0, page->getCanvasPtr(), DPOS_SCREEN_WIDTH, DPOS_SCREEN_HEIGHT, 1, 0);
          break;
        }
      }
      //catch error and return / don't execute display.display();
      if (_err != 0) {
        LOGE(tag, F("draw-page failed"), F("no page found with matching ID"));
        return;
      }
      break;

    default:
      LOGE(tag, F("int-DPm"), F("ERROR: 897"));
      break;
  }
  display.display();
}

void DPOS_DisplayManager::update() {
  uint8_t _err = 1;
  switch (this->renderMode) {
    case DPOS_RENDERMODE_AUTO:
      //itterate over all autoRender-blocks
      for (int i = 0; i < DPOS_AUTORENDER_QUEUES_AMOUNT; i++) {

        //extract current autoRender-block
        RenderBlock_t _block = DPOS_AUTORENDER_LIST[i];

        //check callback return val and if it exists
        if (_block.callback != nullptr && _block.callback()) {

          //itterate over entire autoRender-block
          for (int j = 0; j < _block.size; j++) {

            //only draw if element is even activated
            if (_block.elements[j] & this->activatedElements) {

              if (_block.elements[j] == DPOS_ELEMENT_BIT_TEXTBODY) {
                for (auto* textBody : this->textBodies) {
                  textBody->update();
                  display.drawBitmap(textBody->getPosX(), textBody->getPosY(), textBody->getCanvasPtr(), textBody->getWidth(), textBody->getHeight(), 1, 0);
                }
              } else {
                for (auto* element : this->elements) {
                  //search corresponding element in vector and draw
                  if (element->getElementBit() == _block.elements[j]) {
                    element->update();
                    display.drawBitmap(element->getPosX(), element->getPosY(), element->getCanvasPtr(), element->getWidth(), element->getHeight(), 1, 0);
                  }
                }
              }
            }
          }
        }
      }
      break;

    case DPOS_RENDERMODE_QUEUE:
      for (auto* element : this->elements) {
        element->update();
        display.drawBitmap(element->getPosX(), element->getPosY(), element->getCanvasPtr(), element->getWidth(), element->getHeight(), 1, 0);
      }

      for (auto* textBody : this->textBodies) {
        textBody->update();
        display.drawBitmap(textBody->getPosX(), textBody->getPosY(), textBody->getCanvasPtr(), textBody->getWidth(), textBody->getHeight(), 1, 0);
      }
      break;

    case DPOS_RENDERMODE_PAGE:



      if (this->misc & DPOS_DPMANAGER_BIT_TRANS_START) {
        //keep updating start page
        for (auto* page : this->pages) {
          if (page->getPageID() == this->activePage) {
            _err = 0;
            page->update();
            break;
          }
        }
        //already update endPage
        for (auto* page : this->pages) {
          if (page->getPageID() == this->activePageTrans) {
            _err = 0;
            page->update();
            break;
          }
        }
        this->trans.update();
        display.drawBitmap(0, 0, this->trans.getCanvasPtr(), DPOS_SCREEN_WIDTH, DPOS_SCREEN_HEIGHT, 1, 0);


        if (this->trans.getTransitionDone(true)) {
          this->activePage = this->activePageTrans;                      //transfer active page to endpage
          this->activePageTrans = 0;                                     //force user to redefine endPage
          this->misc &= ~(DPOS_DPMANAGER_BIT_TRANS_START);               //stop transition mode
          xEventGroupSetBits(xDisplayEventGroup, DPOS_TRANSITION_DONE);  //signal transition done for "waitForTransition()"
        }


      } else {
        for (auto* page : this->pages) {
          if (page->getPageID() == this->activePage) {
            _err = 0;
            page->update();
            display.drawBitmap(0, 0, page->getCanvasPtr(), DPOS_SCREEN_WIDTH, DPOS_SCREEN_HEIGHT, 1, 0);
            break;
          }
        }
      }


      //catch error and return / don't execute display.display();
      if (_err != 0) {
        LOGE(tag, F("draw-page failed"), F("no page found with matching ID"));
        return;
      }
      break;

    default:
      LOGE(tag, F("int-Update"), F("ERROR: 897"));
      break;
  }
  display.display();
}


void DPOS_DisplayManager::waitForTransition() {
  //waits for DPOS_TRANSITION_DONE-bit to be set and clears it on exit
  xEventGroupWaitBits(xDisplayEventGroup, DPOS_TRANSITION_DONE, pdTRUE, pdFALSE, portMAX_DELAY);
}


// private funciton
void DPOS_DisplayManager::setStartPage(uint16_t pageID) {
  for (auto* page : this->pages) {
    if (page->getPageID() == pageID) {
      this->trans.setStartPage(page);
      break;
    }
  }
}