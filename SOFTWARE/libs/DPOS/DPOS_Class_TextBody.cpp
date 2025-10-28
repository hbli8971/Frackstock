#include <sys/_stdint.h>
#include "DPOS_Class_TextBody.h"
#include "hbli_Logging.h"
#include "glcdfont.c"

#define SCROLLING_RIGHT 0
#define SCROLLING_LEFT 1

#define BIT_MASK_STYLE_SETTING 0x0F
#define BIT_SCROLL_DIRECTION (1 << 4)
#define BIT_HAS_BEEN_SEEN (1 << 5)
#define BIT_HAS_BEEN_SEEN_OLD (1 << 6)
#define BIT_STRING_OVERSIZE_RESET (1 << 7)

static const char* tag = "TextBody";

DPOS_TextBody::DPOS_TextBody(uint32_t w, uint32_t h)
  : DPOS_UIElement(w, h),    // Call the base class constructor with width and height
    originalText(nullptr),   // No text set initially
    displayText(nullptr),    // No text to display initially
    scrollPosition(0),       // No scrolling initially
    scrollSpeed(1.0),        // Default scroll speed
    textColor(1),            // Default text color (white)
    stringWidth(0),          // Initially, string width is 0
    stringHeight(0),         // Initially, string height is 0
    scrollDelay(DPOS_SCROLL_DELAY_DEFAULT),          // No delay for scrolling
    textOffsetX(0),          // No horizontal offset
    textOffsetY(0),          // No vertical offset
    settings(0),             // Default settings (can be customized)
    textSize(SMALL),         // Default text size
    alignment(CENTER_LEFT),  // Default alignment (e.g., left-aligned)
    style(DEFAULT_NO_BG)     // Default style
{
  this->setScrollDirection(SCROLLING_LEFT);
}

DPOS_TextBody::DPOS_TextBody(int32_t x, int32_t y, uint32_t w, uint32_t h)
  : DPOS_UIElement(w, h),    // Call the base class constructor with width and height
    originalText(nullptr),   // No text set initially
    displayText(nullptr),    // No text to display initially
    scrollPosition(0),       // No scrolling initially
    scrollSpeed(1.0),        // Default scroll speed
    textColor(1),            // Default text color (white)
    stringWidth(0),          // Initially, string width is 0
    stringHeight(0),         // Initially, string height is 0
    scrollDelay(DPOS_SCROLL_DELAY_DEFAULT),          // No delay for scrolling
    textOffsetX(0),          // No horizontal offset
    textOffsetY(0),          // No vertical offset
    settings(0),             // Default settings (can be customized)
    textSize(SMALL),         // Default text size
    alignment(CENTER_LEFT),  // Default alignment (e.g., left-aligned)
    style(DEFAULT_NO_BG)     // Default style
{
  this->setScrollDirection(SCROLLING_LEFT);
  this->setPos(x, y);
}

DPOS_TextBody::~DPOS_TextBody() {
  // If displayText was dynamically allocated, we need to free the memory
  if (this->displayText != nullptr) {
    delete[] this->displayText;   // Use delete[] since displayText is a char array
    this->displayText = nullptr;  // Ensure pointer is set to nullptr after deletion
  }
}

void DPOS_TextBody::setAlignment(uint8_t alignment) {
  switch (alignment) {
    case TOP_LEFT:
    case TOP_CENTER:
    case TOP_RIGHT:
    case CENTER_LEFT:
    case CENTER:
    case CENTER_RIGHT:
    case BOTTOM_LEFT:
    case BOTTOM_CENTER:
    case BOTTOM_RIGHT:
      this->alignment = alignment;
      break;
    default:
      LOGE(tag, "alignment", "selected alignment out of range");
      break;
  }
}

void DPOS_TextBody::setText(const char* text) {

  // Free previously allocated memory
  if (this->displayText) {
    free(this->displayText);
    this->displayText = nullptr;
  }

  this->originalText = text;

  if (this->originalText) {
    if (checkForDynamicEntry() == 0) {
      //if there is no dynamic Entry:
      size_t length = strlen(this->originalText);
      this->displayText = (char*)malloc(length + 1);

      if (this->displayText == nullptr) {
        Serial.println("Memory allocation failed for displayText in TextBody!");
        return;
      }

      strcpy(this->displayText, this->originalText);

      this->stringWidth = length * this->textSize * CHAR_WIDTH_SMALL;
      this->stringHeight = this->textSize * CHAR_HEIGHT_SMALL;

      if (this->stringWidth >= this->fieldWidth) {
        setStringOversizeBit();
      } else {
        resetStringOversizeBit();
      }
    }
  }
}

void DPOS_TextBody::setTextColor(uint8_t color) {
  this->textColor = color;
}

void DPOS_TextBody::setScrollSpeed(float newspeed) {
  this->scrollSpeed = newspeed;
}

void DPOS_TextBody::setScrollPosition(float newPos) {
  this->scrollPosition = newPos;
}

void DPOS_TextBody::setTextSize(uint8_t newsize) {
  if (newsize >= TEXT_SIZE_LARGE) {
    this->textSize = TEXT_SIZE_LARGE;
  } else {
    this->textSize = newsize;
  }
}

void DPOS_TextBody::setStyle(uint8_t newStyle) {
  switch (newStyle) {
    case DEFAULT_NO_BG:
    case DEFAULT_W_BG:
    case RECT_ROUND_BG:
    case RECT_ROUND_BORDER_BG:
      this->style = newStyle;
      break;
    default:
      LOGE(tag, "style", "selected style out of range");
      break;
  }
}

void DPOS_TextBody::setStyleSetting(uint8_t extra) {
  if (extra > 0x0F) {
    this->setStyleSettingsInternal(0x0F);
  } else {
    this->setStyleSettingsInternal(extra);
  }
}

void DPOS_TextBody::setTextOffsetX(int8_t offset) {
  this->textOffsetX = offset;
}

void DPOS_TextBody::setTextOffsetY(int8_t offset) {
  this->textOffsetY = offset;
}

bool DPOS_TextBody::setDynamicText(const char* newDynamicText) {
  if (!this->originalText) {
    LOGE(tag, "Original Text not set", "call setText() first!");
    return false;
  }

  // Convert the original text and markers to String for easier manipulation
  String entry = this->originalText;                  // Original text containing the marker
  String marker = DPOS_TEXT_MARKER;                   // Marker to find in the text
  int pos = entry.indexOf(marker);                    // Find the position of the marker
  if (pos != -1) {                                    // If the marker is found
    String replacementText = String(newDynamicText);  // Convert the C-string to String for easy manipulation
    entry.replace(marker, replacementText);           // Replace the marker with the new text
    if (this->displayText) {                          // Free the previous dynamicText memory if allocated
      free(this->displayText);
      this->displayText = nullptr;
    }

    // Allocate new memory for the updated dynamic text
    this->displayText = (char*)malloc(entry.length() + 1);  // +1 for the null terminator
    if (this->displayText == nullptr) {
      LOGE(tag, "Memory allocation failed for displayText!");
      return false;
    }

    // Copy the updated string into dynamicText
    strcpy(this->displayText, entry.c_str());
    calculateStringDimensions();
    return true;
  } else {
    LOGE(tag, "Marker not found in the original text!");
    LOGE(tag, this->originalText);
    return false;
  }
}

bool DPOS_TextBody::setDynamicValue(float dynamicValue, uint8_t digits, uint8_t dec) {

  if (!this->originalText) {
    LOGE(tag, "Original Text not set", "call setText() first!");
    return false;
  }

  String entry = this->originalText;
  String marker = DPOS_VALUE_MARKER;

  int pos = entry.indexOf(marker);

  if (pos != -1) {
    char buffer[32];
    dtostrf(dynamicValue, digits, dec, buffer);

    String dynamicValueStr = String(buffer);
    LOGV("string was", entry);
    entry.replace(marker, dynamicValueStr);
    LOGV("new string is", entry);
    if (this->displayText) {
      free(this->displayText);
      this->displayText = nullptr;
    }
    this->displayText = (char*)malloc(entry.length() + 1);
    if (this->displayText == nullptr) {
      LOGE(tag, "Memory allocation failed for displayText!");
      return false;
    }

    strcpy(this->displayText, entry.c_str());
    calculateStringDimensions();
    return true;
  } else {
    LOGE(tag, "Marker not found in the original text!");
    LOGE(tag, this->originalText);
    return false;
  }
}

void DPOS_TextBody::setScrollDelay(int8_t cycles) {
  if (cycles >= 0) {
    this->scrollDelay = cycles;
  }
}

void DPOS_TextBody::draw() {
  this->canvas.fillScreen(0);

  if (this->getHasBeenSeen() && (!this->getHasBeenSeenOld())) {
    this->scrollPosition = 0;
    this->setHasBeenSeenOld();
  }

  switch (this->style) {
    case DEFAULT_NO_BG:
      // this->canvas.fillScreen(0);
      break;

    case DEFAULT_W_BG:
      this->canvas.fillScreen(this->textColor ? 0 : 1);  //fill background to inverted value of textColor
      break;

    case RECT_ROUND_BG:
      this->canvas.fillRoundRect(0, 0, this->fieldWidth, this->fieldHeight, this->getStyleSettingsInternal(), this->textColor ? 0 : 1);
      break;

    case RECT_ROUND_BORDER_BG:
      this->canvas.drawRoundRect(0, 0, this->fieldWidth, this->fieldHeight, this->getStyleSettingsInternal(), this->textColor);
      break;

    default:
      LOGE(tag, "draw", "ERROR: 897");
      break;
  }
  this->printText();
}

void DPOS_TextBody::update() {
  if (this->stringWidth + this->textOffsetX > this->fieldWidth) {
    scrollingHandler();
    this->draw();
  }
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////PRIVATE///////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



void DPOS_TextBody::printCharLine(int16_t x, int16_t y, char c, uint8_t line, uint8_t color) {

  //clip left and right
  if (x < 0 || x >= this->fieldWidth) {
    LOGG(tag, "clip L/R");
    return;
  }

  uint8_t lineIntern = line % 5;  //limit to 0...4

  uint8_t _charLine = font[c * 5 + lineIntern];  //get current line of char

  for (int i = 0; i < 7 * this->textSize; i++) {

    if ((y - (this->textSize * 8) + i) < 0) continue;                   // clip top
    if ((y - (this->textSize * 8) + i) >= this->fieldHeight) continue;  //clip bot

    //clip top and bot
    if (((_charLine >> (i / this->textSize)) & 1)) {
      if (color) {
        this->canvas.drawPixel(x, y - (this->textSize * 8) + i, 1);
      } else {
        this->canvas.drawPixel(x, y - (this->textSize * 8) + i, 0);
      }
    }
  }
}

void DPOS_TextBody::printText() {

  int _scrollPosUint = abs((int)this->scrollPosition);   //scrollpos as positive value
  int16_t curX = this->getCursorXcalc();                 //get cursor, calculated from alignment
  int16_t curY = this->getCursorYcalc();                 //get cursor, calculated from alignment
  char c;                                                //holds currently indexed char
  uint8_t line = _scrollPosUint % (6 * this->textSize);  //holds line of currently indexed char

  for (int i = _scrollPosUint; i < this->stringWidth; i++, curX++, line++) {

    //only print, if an "active" line is indexed
    if (curX >= this->fieldWidth - this->textOffsetX) {
      break;
    }

    if (line >= this->textSize * 6) {
      line = 0;
    }

    if (line < this->textSize * 5) {
      int charIndex = i / CHAR_WIDTH(this->textSize);
      c = this->displayText[charIndex];
      if (c == '\0') {
        break;
      }
      this->printCharLine(curX, curY, c, (line / this->textSize), this->textColor);
    }
  }
}

int16_t DPOS_TextBody::getCursorXcalc() {

  if (this->stringWidth >= this->fieldWidth - (this->textOffsetX * 2)) {
    return this->textOffsetX;
  }

  switch (this->alignment) {
    case TOP_LEFT:
    case CENTER_LEFT:
    case BOTTOM_LEFT:
      LOGG(tag, "left");
      return this->textOffsetX;
      break;


    case TOP_CENTER:
    case CENTER:
    case BOTTOM_CENTER:
      LOGG(tag, "center");
      return ((this->fieldWidth - this->stringWidth) / 2) + this->textOffsetX;
      break;

    case TOP_RIGHT:
    case CENTER_RIGHT:
    case BOTTOM_RIGHT:
      LOGG(tag, "right");
      return (this->fieldWidth - this->stringWidth - this->textOffsetX);
      break;

    default:
      LOGE(tag, "internal-xcalc", "ERROR: 101");
      return this->textOffsetX;
      break;
  }
}

int16_t DPOS_TextBody::getCursorYcalc() {
  switch (alignment) {
    case TOP_LEFT:
    case TOP_CENTER:
    case TOP_RIGHT:
      return this->stringHeight + this->textOffsetY;
      break;
    case CENTER_LEFT:
    case CENTER:
    case CENTER_RIGHT:
      return ((this->fieldHeight - this->stringHeight) / 2) + this->textOffsetY + this->stringHeight;
      break;
    case BOTTOM_LEFT:
    case BOTTOM_CENTER:
    case BOTTOM_RIGHT:
      return this->fieldHeight - 1 + this->textOffsetY;
      break;
    default:
      LOGE(tag, "internal-ycalc", "ERROR: 101");
      break;
  }
}

bool DPOS_TextBody::getScrollDirection() {
  return this->settings & BIT_SCROLL_DIRECTION;
}

void DPOS_TextBody::setScrollDirection(bool direction) {
  if (direction) {
    this->settings |= BIT_SCROLL_DIRECTION;
  } else {
    this->settings &= ~(BIT_SCROLL_DIRECTION);
  }
}

void DPOS_TextBody::setHasBeenSeen() {
  this->settings |= BIT_HAS_BEEN_SEEN;
}

void DPOS_TextBody::resetHasBeenSeen() {
  this->settings &= ~(BIT_HAS_BEEN_SEEN);
}

bool DPOS_TextBody::getHasBeenSeen() {
  return this->settings & BIT_HAS_BEEN_SEEN;
}

void DPOS_TextBody::setHasBeenSeenOld() {
  this->settings |= BIT_HAS_BEEN_SEEN_OLD;
}

void DPOS_TextBody::resetHasBeenSeenOld() {
  this->settings &= ~(BIT_HAS_BEEN_SEEN_OLD);
}

bool DPOS_TextBody::getHasBeenSeenOld() {
  return this->settings & BIT_HAS_BEEN_SEEN_OLD;
}

void DPOS_TextBody::setStringOversizeBit() {
  this->settings |= BIT_STRING_OVERSIZE_RESET;
}

bool DPOS_TextBody::getStringOversizeBit() {
  return this->settings & BIT_STRING_OVERSIZE_RESET;
}

void DPOS_TextBody::resetStringOversizeBit() {
  this->settings &= ~(BIT_STRING_OVERSIZE_RESET);
}

void DPOS_TextBody::setStyleSettingsInternal(uint8_t _set) {
  this->settings &= ~(BIT_MASK_STYLE_SETTING);        //clear old settings
  this->settings |= (_set & BIT_MASK_STYLE_SETTING);  //set new settings
}

uint8_t DPOS_TextBody::getStyleSettingsInternal() {
  return this->settings & BIT_MASK_STYLE_SETTING;
}

void DPOS_TextBody::scrollingHandler() {

  if (this->scrollDelay > 0) {
    this->scrollDelay--;
  } else {
    if (this->getScrollDirection() == SCROLLING_LEFT) {
      //scrolling left means, scrollPosition gets smaller
      this->scrollPosition -= this->scrollSpeed;
      //reversing direction check
      if (this->stringWidth + this->scrollPosition + (this->textOffsetX * 2) <= this->fieldWidth) {
        this->setScrollDirection(SCROLLING_RIGHT);
        this->scrollDelay = DPOS_SCROLL_DELAY_REVERSE;
      }
    } else {
      //scrolling right means, scrollPosition gets larger
      this->scrollPosition += this->scrollSpeed;
      //reversing direction check
      if (this->scrollPosition >= 0) {
        this->setScrollDirection(SCROLLING_LEFT);
        this->scrollDelay = DPOS_SCROLL_DELAY_START;
      }
    }
  }
}

void DPOS_TextBody::calculateStringDimensions() {

  if (!displayText) {
    LOGE(tag, "intern-strDimCalc", "ERROR: 102");
    return;
  }

  int length = strlen(this->displayText);

  this->stringWidth = length * CHAR_WIDTH(this->textSize);
  this->stringHeight = this->textSize * CHAR_HEIGHT_SMALL;
  if (this->stringWidth >= this->fieldWidth) {
    setStringOversizeBit();
  } else {
    resetStringOversizeBit();
  }
}

uint8_t DPOS_TextBody::checkForDynamicEntry() {
  uint8_t ret = 0;
  String entry = this->originalText;
  String marker_text = DPOS_TEXT_MARKER;
  String marker_value = DPOS_VALUE_MARKER;

  int pos = entry.indexOf(marker_value);
  if (pos != -1) {
    LOGI(tag, "dynamic Value found!");
    ret |= 0x01;
  }
  pos = entry.indexOf(marker_text);
  if (pos != -1) {
    LOGI(tag, "dynamic Text found!");
    ret |= 0x02;
  }
  return ret;
}