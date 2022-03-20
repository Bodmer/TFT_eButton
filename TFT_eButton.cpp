#include "TFT_eButton.h"

/***************************************************************************************
** Code for the GFX button UI element
** Grabbed from Adafruit_GFX library and enhanced to handle any label font
***************************************************************************************/
TFT_eButton::TFT_eButton(TFT_eSPI *tft) {
  _gfx       = tft;
  _xd        = 0;
  _yd        = 0;
  _textdatum = MC_DATUM;
  _label[9]  = '\0';
  currstate  = false;
  laststate  = false;
  _inverted  = false;
}

void TFT_eButton::setPressAction(pressCallback action)
{
  pressAction = action;
}

void TFT_eButton::setReleaseAction(releaseCallback action)
{
  releaseAction = action;
}

// Classic initButton() function: pass center & size
void TFT_eButton::initButton(int16_t x, int16_t y, uint16_t w, uint16_t h, uint16_t outline, uint16_t fill, uint16_t textcolor, char *label, uint8_t textsize)
{
  // Tweak arguments and pass to the newer initButtonUL() function...
  initButtonUL(x - (w / 2), y - (h / 2), w, h, outline, fill, textcolor, label, textsize);
}

// Newer function instead accepts upper-left corner & size
void TFT_eButton::initButtonUL(int16_t x1, int16_t y1, uint16_t w, uint16_t h, uint16_t outline, uint16_t fill, uint16_t textcolor, char *label, uint8_t textsize)
{
  _x1           = x1;
  _y1           = y1;
  _w            = w;
  _h            = h;
  _outlinecolor = outline;
  _outlinewidth = 2;
  _fillcolor    = fill;
  _textcolor    = textcolor;
  _textsize     = textsize;
  strncpy(_label, label, 9);
  _pressTime    = 0xFFFFFFFF;
  _releaseTime  = 0xFFFFFFFF;
}

// Adjust text datum and x, y deltas
void TFT_eButton::setLabelDatum(int16_t x_delta, int16_t y_delta, uint8_t datum)
{
  _xd        = x_delta;
  _yd        = y_delta;
  _textdatum = datum;
}

void TFT_eButton::drawButton(bool inverted, String long_name) {
  uint16_t fill, outline, text;

  _inverted  = inverted;

  if(!inverted) {
    fill    = _fillcolor;
    outline = _outlinecolor;
    text    = _textcolor;
  } else {
    fill    = _textcolor;
    outline = _outlinecolor;
    text    = _fillcolor;
  }

  uint8_t r = min(_w, _h) / 4; // Corner radius
  _gfx->fillRoundRect(_x1, _y1, _w, _h, r, fill);
  _gfx->drawRoundRect(_x1, _y1, _w, _h, r, outline);

  if (_gfx->textfont == 255) {
    _gfx->setCursor(_x1 + (_w / 8),
                    _y1 + (_h / 4));
    _gfx->setTextColor(text);
    _gfx->setTextSize(_textsize);
    _gfx->print(_label);
  }
  else {
    _gfx->setTextColor(text, fill);
    _gfx->setTextSize(_textsize);

    uint8_t tempdatum = _gfx->getTextDatum();
    _gfx->setTextDatum(_textdatum);
    uint16_t tempPadding = _gfx->getTextPadding();
    _gfx->setTextPadding(0);

    if (long_name == "")
      _gfx->drawString(_label, _x1 + (_w/2) + _xd, _y1 + (_h/2) - 4 + _yd);
    else
      _gfx->drawString(long_name, _x1 + (_w/2) + _xd, _y1 + (_h/2) - 4 + _yd);

    _gfx->setTextDatum(tempdatum);
    _gfx->setTextPadding(tempPadding);
  }
}

void TFT_eButton::drawSmoothButton(bool inverted, int16_t outlinewidth, uint32_t bgcolor, String long_name) {
  uint16_t fill, outline, text;
  if (bgcolor != 0x00FFFFFF) _bgcolor = bgcolor;
  if (outlinewidth >=0) _outlinewidth = outlinewidth;
  _inverted  = inverted;

  if(!inverted) {
    fill    = _fillcolor;
    outline = _outlinecolor;
    text    = _textcolor;
  } else {
    fill    = _textcolor;
    outline = _outlinecolor;
    text    = _fillcolor;
  }

  uint8_t r = min(_w, _h) / 4; // Corner radius
  if (outlinewidth > 0) _gfx->fillSmoothRoundRect(_x1, _y1, _w, _h, r, outline, _bgcolor);
  _gfx->fillSmoothRoundRect(_x1+_outlinewidth, _y1+_outlinewidth, _w-(2*_outlinewidth), _h-(2*_outlinewidth), r-_outlinewidth, fill, outline);

  if (_gfx->textfont == 255) {
    _gfx->setCursor(_x1 + (_w / 8),
                    _y1 + (_h / 4));
    _gfx->setTextColor(text);
    _gfx->setTextSize(_textsize);
    _gfx->print(_label);
  }
  else {
    _gfx->setTextColor(text, fill);
    _gfx->setTextSize(_textsize);

    uint8_t tempdatum = _gfx->getTextDatum();
    _gfx->setTextDatum(_textdatum);
    uint16_t tempPadding = _gfx->getTextPadding();
    _gfx->setTextPadding(0);

    if (long_name == "")
      _gfx->drawString(_label, _x1 + (_w/2) + _xd, _y1 + (_h/2) - 4 + _yd);
    else
      _gfx->drawString(long_name, _x1 + (_w/2) + _xd, _y1 + (_h/2) - 4 + _yd);

    _gfx->setTextDatum(tempdatum);
    _gfx->setTextPadding(tempPadding);
  }
}

bool TFT_eButton::contains(int16_t x, int16_t y) {
  return ((x >= _x1) && (x < (_x1 + _w)) &&
          (y >= _y1) && (y < (_y1 + _h)));
}

void TFT_eButton::press(bool p) {
  laststate = currstate;
  currstate = p;
}

bool TFT_eButton::isPressed()    { return currstate; }
bool TFT_eButton::justPressed()  { return (currstate && !laststate); }
bool TFT_eButton::justReleased() { return (!currstate && laststate); }
