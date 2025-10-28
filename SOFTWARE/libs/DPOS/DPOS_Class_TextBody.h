#include <stdint.h>
#ifndef _DPOS_CLASS_TEXTBODY_H
#define _DPOS_CLASS_TEXTBODY_H
#include <Arduino.h>
#include "DPOS_cfg.h"
#include "DPOS_Class_UIElement.h"

enum DPOS_TextBody_Alignment {
  TOP_LEFT,
  TOP_CENTER,
  TOP_RIGHT,
  CENTER_LEFT,
  CENTER,
  CENTER_RIGHT,
  BOTTOM_LEFT,
  BOTTOM_CENTER,
  BOTTOM_RIGHT
};

enum DPOS_TextSize {
  SMALL = 1,
  MEDIUM,
  LARGE
};

enum DPOS_TextBodyStyle {
  DEFAULT_NO_BG,
  DEFAULT_W_BG,
  RECT_ROUND_BG,
  RECT_ROUND_BORDER_BG
};

class DPOS_TextBody : public DPOS_UIElement {
protected:
  const char* originalText;  //original string, set by using setText()
  char* displayText;         //final string to be displayed, maybe altered due to dynamic numbers or text
  float scrollPosition;      //holds the current scroll position of the text. [n * pixles to the Right from starting pos(only negative numbers or 0)]
  float scrollSpeed;         //holds the speed, at which scrolling occurs. (gets added every scrolling interval)
  uint16_t stringWidth;      //holds the width of the entire string in [pixels]. (string width of displayText, NOT originalText!)
  uint16_t stringHeight;     //holds the height of the string in [pixles].
  int8_t scrollDelay;        //delay until scrolling stars [update cycles]
  int8_t textOffsetX;        //text Offset X
  int8_t textOffsetY;        //text Offset Y
  uint8_t settings;          //hold different information
  uint8_t textSize;          //holds the textSize
  uint8_t alignment;         //holds the text alignment
  uint8_t style;             //determins the style of textBody
  uint8_t textColor;         //holds the text color

private:
  void printCharLine(int16_t x, int16_t y, char c, uint8_t line, uint8_t color);

  void printText();

  int16_t getCursorXcalc();
  int16_t getCursorYcalc();

  void setStringOversizeBit();
  void resetStringOversizeBit();
  


  void setStyleSettingsInternal(uint8_t _set);
  uint8_t getStyleSettingsInternal();

  void scrollingHandler();
  void calculateStringDimensions();

  uint8_t checkForDynamicEntry();
public:
  DPOS_TextBody(uint32_t w, uint32_t h);
  DPOS_TextBody(int32_t x, int32_t y, uint32_t w, uint32_t h);
  ~DPOS_TextBody();

  void setHasBeenSeen();
  void resetHasBeenSeen();
  bool getHasBeenSeen();
  void setHasBeenSeenOld();
  void resetHasBeenSeenOld();
  bool getHasBeenSeenOld();
  bool getStringOversizeBit();

  bool getScrollDirection();  //done
  void setScrollDirection(bool direction);

  void setScrollDelay(int8_t cycles);

  void setAlignment(uint8_t alignment);

  void setText(const char* text);
  void setTextColor(uint8_t color);

  void setScrollSpeed(float newspeed);
  void setScrollPosition(float newPos);

  void setTextSize(uint8_t newsize);
  void setStyle(uint8_t newStyle);
  void setStyleSetting(uint8_t extra);

  void setTextOffsetX(int8_t offset);
  void setTextOffsetY(int8_t offset);

  bool setDynamicText(const char* newDynamicText);
  bool setDynamicValue(float dynamicValue, uint8_t digits, uint8_t dec);

  void draw() override;
  void update() override;

  DPOS_UIElement_Type getElementType() override {
    return DPOS_ELEMENT_TYPE_TEXTBODY;
  }
  uint32_t getElementBit() override {
    return DPOS_ELEMENT_BIT_TEXTBODY;
  }
};
#endif