// Compile code with -std=c++0x and manually defnie WIN32

#ifndef OFXPUI_H
#define OFXPUI_H

#include "glew.h"

#define PU_USE_GLUT 1
#include "pu.h"
#include "puAux.h"

#include "ofMain.h"


//#define DEBUG
#ifndef DEBUG
#define DPRINT(...)
#else
#define DPRINT(...) printf (__VA_ARGS__)
#endif

#define EXACT_PIXEL_BEGIN { \
  glMatrixMode (GL_MODELVIEW); \
  ofPushMatrix(); \
  glTranslatef (0.325, 0.325, 0.0); \
}

#define EXACT_PIXEL_END { \
  ofPopMatrix(); \
}

// Code so that _ofRect is called so that rectangles with line width > 1 work.
// _ofRect is defined in ofxMXUtil.
// Comment out define to get original ofRect.
// #warning redefining ofRect to _ofRect to fix line width problem
// void _ofRect(float x1, float y1, float w, float h);
// #define ofRect(...) _ofRect(__VA_ARGS__)

#define ofxpuSetDefaultStyle(...) puSetDefaultStyle(__VA_ARGS__)
#define ofxpuGetDefaultStyle(...) puGetDefaultStyle(__VA_ARGS__)
#define ofxpuSetDefaultFonts(...) puSetDefaultFonts(__VA_ARGS__)
#define ofxpuGetDefaultFonts(...) puGetDefaultFons(__VA_ARGS__)
#define ofxpuGetDefaultLabelFont(...) puGetDefaultLabelFont(__VA_ARGS__)
#define ofxpuSetDefaultLabelFont(...) puSetDefaultLabelFont(__VA_ARGS__)

#define ofxpuInit(...) puInit(__VA_ARGS__)
#define ofxpuDisplay(...) puDisplay(__VA_ARGS__)
#define ofxpuDeleteObject(...) puDeleteObject(__VA_ARGS__)
#define ofxpuKeyboard(...) puKeyboard(__VA_ARGS__)
#define ofxpuMouse(...) puMouse(__VA_ARGS__)

//puType Constants

#define OFXPUCLASS_DIALOGBOX PUCLASS_DIALOGBOX
#define OFXPUCLASS_SLIDER PUCLASS_SLIDER
#define OFXPUCLASS_BUTTONBOX PUCLASS_BUTTONBOX
#define OFXPUCLASS_INPUT PUCLASS_INPUT
#define OFXPUCLASS_MENUBAR PUCLASS_MENUBAR
#define OFXPUCLASS_POPUPMENU PUCLASS_POPUPMENU
#define OFXPUCLASS_POPUP PUCLASS_POPUP
#define OFXPUCLASS_ONESHOT PUCLASS_ONESHOT
#define OFXPUCLASS_BUTTON PUCLASS_BUTTON
#define OFXPUCLASS_TEXT PUCLASS_TEXT
#define OFXPUCLASS_FRAME PUCLASS_FRAME
#define OFXPUCLASS_INTERFACE PUCLASS_INTERFACE
#define OFXPUCLASS_GROUP PUCLASS_GROUP
#define OFXPUCLASS_OBJECT PUCLASS_OBJECT
#define OFXPUCLASS_VALUE PUCLASS_VALUE

//puStyle Constants

#define OFXPUSTYLE_NONE PUSTYLE_NONE
#define OFXPUSTYLE_PLAIN PUSTYLE_PLAIN
#define OFXPUSTYLE_SHADED PUSTYLE_SHADED
#define OFXPUSTYLE_SMALL_SHADED PUSTYLE_SMALL_SHADED
#define OFXPUSTYLE_BEVELLED PUSTYLE_BEVELLED
#define OFXPUSTYLE_SMALL_BEVELLED PUSTYLE_SMALL_BEVELLED
#define OFXPUSTYLE_BOXED PUSTYLE_BOXED
#define OFXPUSTYLE_SPECIAL_UNDERLINED PUSTYLE_SPECIAL_UNDERLINED
#define OFXPUSTYLE_DROPSHADOW PUSTYLE_DROPSHADOW
#define OFXPUSTYLE_RADIO PUSTYLE_RADIO

//puPlace Constants

#define OFXPUPLACE_ABOVE_LEFT PUPLACE_ABOVE_LEFT
#define OFXPUPLACE_ABOVE_RIGHT PUPLACE_ABOVE_RIGHT
#define OFXPUPLACE_TOP_LEFT PUPLACE_TOP_LEFT
#define OFXPUPLACE_TOP_CENTERED PUPLACE_TOP_CENTERED
#define OFXPUPLACE_TOP_RIGHT PUPLACE_TOP_RIGHT
#define OFXPUPLACE_UPPER_LEFT PUPLACE_UPPER_LEFT
#define OFXPUPLACE_UPPER_RIGHT PUPLACE_UPPER_RIGHT
#define OFXPUPLACE_CENTERED_LEFT PUPLACE_CENTERED_LEFT
#define OFXPUPLACE_CENTERED_RIGHT PUPLACE_CENTERED_RIGHT
#define OFXPUPLACE_LOWER_LEFT PUPLACE_LOWER_LEFT
#define OFXPUPLACE_LOWER_RIGHT PUPLACE_LOWER_RIGHT
#define OFXPUPLACE_BOTTOM_LEFT PUPLACE_BOTTOM_LEFT
#define OFXPUPLACE_BOTTOM_CENTERED PUPLACE_BOTTOM_CENTERED
#define OFXPUPLACE_BOTTOM_RIGHT PUPLACE_BOTTOM_RIGHT
#define OFXPUPLACE_BELOW_LEFT PUPLACE_BELOW_LEFT
#define OFXPUPLACE_BELOW_RIGHT PUPLACE_BELOW_RIGHT

//puColor Constants

#define OFXPUCOL_FOREGROUND PUCOL_FOREGROUND
#define OFXPUCOL_BACKGROUND PUCOL_BACKGROUND
#define OFXPUCOL_HIGHLIGHT PUCOL_HIGHLIGHT
#define OFXPUCOL_LABEL PUCOL_LABEL
#define OFXPUCOL_LEGEND PUCOL_LEGEND
#define OFXPUCOL_MISC PUCOL_MISC

//puButton Constants

#define OFXPUBUTTON_NORMAL PUBUTTON_NORMAL
#define OFXPUBUTTON_RADIO PUBUTTON_RADIO
#define OFXPUBUTTON_CIRCLE PUBUTTON_CIRCLE
#define OFXPUBUTTON_VCHECK PUBUTTON_VCHECK
#define OFXPUBUTTON_XCHECK PUBUTTON_XCHECK

//puArrow Constants

#define OFXPUARROW_UP PUARROW_UP
#define OFXPUARROW_DOWN PUARROW_DOWN
#define OFXPUARROW_FASTUP PUARROW_FASTUP
#define OFXPUARROW_FASTDOWN PUARROW_FASTDOWN
#define OFXPUARROW_LEFT PUARROW_LEFT
#define OFXPUARROW_RIGHT PUARROW_RIGHT
#define OFXPUARROW_FASTLEFT PUARROW_FASTLEFT
#define OFXPUARROW_FASTRIGHT PUARROW_FASTRIGHT

//Color Methods

#define REPLACEBASEMETHODS() \
    virtual void setColourScheme(int _r, int _g, int _b, int _a=255) { \
      puObject::setColourScheme(_r/255.0, _g/255.0, _b/255.0, _a/255.0); \
    } \
    virtual void setColorScheme(int _r, int _g, int _b, int _a=255) { \
      puObject::setColourScheme(_r/255.0, _g/255.0, _b/255.0, _a/255.0); \
    } \
    virtual void setColour(int which, int _r, int _g, int _b, int _a=255) { \
      puObject::setColour(which, _r/255.0, _g/255.0, _b/255.0, _a/255.0); \
    } \
    virtual void setColor(int which, int _r, int _g, int _b, int _a=255) { \
      puObject::setColour(which, _r/255.0, _g/255.0, _b/255.0, _a/255.0); \
    } \
    virtual void getColour(int which, int *_r, int *_g, int *_b, int *_a=NULL) { \
      float tr, tg, tb, ta; \
      if(_a) { \
        puObject::getColour(which, &tr, &tg, &tb, &ta); \
        *_a = int(ta * 255.0); \
      } else { \
        puObject::getColour(which, &tr, &tg, &tb); \
      } \
      *_r = int(tr * 255.0); \
      *_g = int(tg * 255.0); \
      *_b = int(tb * 255.0); \
      return; \
    } \
    virtual void ofxsetPosition(float _x, float _y) { \
      int _w, _h; \
      this->getSize(&_w, &_h); \
      ofxsetPosAndSize(this, _x, _y, _w, _h); \
    } \
    virtual void ofxsetSize(float _w, float _h) { \
      int _x, _y, _w1, _h1; \
      this->getPosition(&_x, &_y); \
      this->getSize(&_w1, &_h1); \
      this->setSize(_w, _h); \
      this->setPosition(_x, _y + (_h1 - _h)); \
    } \
    virtual void ofxgetPosition(float *_x, float *_y) { \
      int _x1, _y1, _w, _h; \
      this->getPosition(&_x1, &_y1); \
      *_x = _x1; \
      this->getSize(&_w, &_h); \
      if (this->getParent() != 0) { \
        if (this->getParent()->getParent() == 0) { \
          *_y = puGetWindowHeight() - _y1 - _h; \
        } else { \
          printf("Warning attempt to set group position and size.\n"); \
          *_y = _w - _y1 - _h; \
        } \
      } \
    } \
    virtual void ofxgetSize(float *_w, float *_h) { \
      int _w1, _h1; \
      this->getSize(&_w1, &_h1); \
      *_w = _w1 - 1; \
      *_h = _h1 - 1; \
    } \

// To adjust PUI to openFrameworks coordinates the following adjustments are made
//
// For 4 coordinates: minx = ofx, miny = puGetWindowHeight - ofy - height - 1,
//                    maxx = ofx + width + 1, maxy = puGetWindowHeight() - ofy
//
// For 2 coordinates: minx = ofx, miny = puGetWindowHeight - ofy

void ofxsetPosAndSize(puObject * tptr, int _x, int _y, int _w, int _h) {
  if (tptr->getParent() != 0) {
    if (tptr->getParent()->getParent() == 0) {
      tptr->setPosition(_x, puGetWindowHeight() - _y - _h - 1);
      tptr->setSize(_w+1, _h+1);
    } else {
      printf("Warning attempt to set group position and size.\n");
      tptr->setPosition(_x, _y);
      tptr->setSize(_w+1, _h+1);
    }
  }
}


class ofxpuGroup : public puGroup {
  public:
    ofxpuGroup(float _x, float _y) : puGroup(_x, puGetWindowHeight() - _y) { }

    virtual void setChildColour (int childs, int which,
                                 int r, int g, int b, int a = 255,
                                 int recursive = FALSE) {
      puGroup::setChildColour(childs, which, r/255.0, g/255.0, b/255.0, a/255.0, recursive);
    }
    virtual void setChildColor (int childs, int which,
                                 int r, int g, int b, int a = 255,
                                 int recursive = FALSE) {
      puGroup::setChildColour(childs, which, r/255.0, g/255.0, b/255.0, a/255.0, recursive);
    }
    virtual void setChildColourScheme (int childs,
                                 int r, int g, int b, int a = 255,
                                 int recursive = FALSE) {
      puGroup::setChildColourScheme(childs, r/255.0, g/255.0, b/255.0, a/255.0, recursive);
    }
    virtual void setChildColorScheme (int childs,
                                 int r, int g, int b, int a = 255,
                                 int recursive = FALSE) {
      puGroup::setChildColourScheme(childs, r/255.0, g/255.0, b/255.0, a/255.0, recursive);
    }
};

// _x, puGetWindowHeight() - _y - _h - 1, _x + _w + 1, puGetWindowHeight() - _y

class ofxpuFrame : public puFrame {
  public:
    ofxpuFrame(float _x, float _y, float _w, float _h) :
      puFrame (0, 0, 0, 0) {
      ofxsetPosAndSize(this, _x, _y, _w, _h);
    }
    REPLACEBASEMETHODS();

};

class ofxpuText : public puText {
  public:
    ofxpuText(float _x, float _y) :
      puText (_x, puGetWindowHeight() - _y) {
    }
    REPLACEBASEMETHODS();
};

//_x, puGetWindowHeight()- _y - _h - 1, _x + _w + 1, puGetWindowHeight() - _y,

class ofxpuButton : public puButton {
  public:
    ofxpuButton(float _x, float _y, const char *l) :
      puButton (_x, puGetWindowHeight() - _y -
                    (puGetDefaultLegendFont().getStringHeight ( l ) +
                     puGetDefaultLegendFont().getStringDescender () +
                     PUSTR_TGAP + PUSTR_BGAP),
                l) {
    }
    ofxpuButton(float _x, float _y, float _w, float _h, int btype = OFXPUBUTTON_NORMAL) :
      puButton (0, 0, 0, 0, btype) {
        ofxsetPosAndSize(this, _x, _y, _w, _h);
    }

    REPLACEBASEMETHODS();
};

// _x, puGetWindowHeight() - _y + 1,
//_x, puGetWindowHeight() - _y - _h - 1, _x + _w + 1, puGetWindowHeight() - _y

class ofxpuOneShot : public puOneShot {
  public:
    ofxpuOneShot(float _x, float _y, const char *_l) :
      puOneShot (0, 0, _l) {
        ofxsetPosAndSize(this, _x, _y, 0, 0);
    }
    ofxpuOneShot(float _x, float _y, float _w, float _h) :
      puOneShot (0, 0, 0, 0) {
        ofxsetPosAndSize(this, _x, _y, _w, _h);
    }
    REPLACEBASEMETHODS();
};

class ofxpuArrowButton : public puArrowButton {
  public:
    ofxpuArrowButton(float _x, float _y, float _w, float _h, int ptype) :
      puArrowButton (_x, puGetWindowHeight() - _y - _h - 1, _x + _w + 1, puGetWindowHeight() - _y, ptype) {
    }
    REPLACEBASEMETHODS();
};

class ofxpuSlider : public puSlider {
  public:
    ofxpuSlider(float _x, float _y, float _sz, int _vertical = false) :
      puSlider (_x,
                _vertical ? (puGetWindowHeight()-_y-_sz) - 1 :
                puGetWindowHeight()-_y - (puGetDefaultLegendFont().getStringHeight()+puGetDefaultLegendFont().getStringDescender()+PUSTR_TGAP+PUSTR_BGAP) + 1,
                _sz+1,
                _vertical,
                _vertical ? (puGetDefaultLegendFont().getStringWidth("W")+PUSTR_LGAP+PUSTR_RGAP) :
                (puGetDefaultLegendFont().getStringHeight()+puGetDefaultLegendFont().getStringDescender()+PUSTR_TGAP+PUSTR_BGAP) - 1
                ) {
      }
    ofxpuSlider(float _x, float _y, int _sz, int _vertical, int _width) :
      puSlider (_x,
                _vertical ? (puGetWindowHeight()-_y-_sz) - 1 : puGetWindowHeight()-_y - _width - 1,
                _sz+1,
                _vertical,
                _vertical ? (_width) + 1 : (_width) + 1
                ) {
      }
    REPLACEBASEMETHODS();
};

//_x, puGetWindowHeight() - _y - _h - 1, _x + _w + 1, puGetWindowHeight() - _y,
class ofxpuListBox : public puListBox {
  public:
    ofxpuListBox (float _x, float _y, float _w, float _h, char** list=NULL) :
      puListBox (0, 0, 0, 0, list) {
      ofxsetPosAndSize(this, _x, _y, _w, _h);
    }
    REPLACEBASEMETHODS();
};

// x, puGetWindowHeight() - _y - _sz - 1, _sz + 1
//_x, puGetWindowHeight() - _y - _sz - 1, _sz + 1
class ofxpuDial : public puDial {
  public:
    ofxpuDial(float _x, float _y, float _sz) :
      puDial (0, 0, 0) {
      ofxsetPosAndSize(this, _x, _y, _sz, _sz);
    }
    ofxpuDial(float _x, float _y, float _sz, float _minval, float _maxval, float _step = 0.0f) :
      puDial (0, 0, 0, _minval, _maxval, _step) {
      ofxsetPosAndSize(this, _x, _y, _sz, _sz);
    }
    REPLACEBASEMETHODS();
};

// Nothing prints.

class ofxpuPopup : public puPopup {
  public:
    ofxpuPopup(float _x, float _y) :
      puPopup (_x, puGetWindowHeight() - _y) {
    }
    REPLACEBASEMETHODS();
};

//_x, puGetWindowHeight() - _y + 1

class ofxpuPopupMenu : public puPopupMenu {
  public:
    ofxpuPopupMenu (float _x, float _y) :
      puPopupMenu (0, 0) {
      ofxsetPosAndSize(this, _x, _y, 0, 0);
    }
    REPLACEBASEMETHODS();
};

class ofxpuMenuBar : public puMenuBar {
  public:
    ofxpuMenuBar (float _h = -1) :
      puMenuBar (_h) {
    }
    REPLACEBASEMETHODS();
};

class ofxpuInput : public puInput {
  public:
    ofxpuInput(float _x, float _y, float _w, float _h) :
      puInput (0, 0, 0, 0) {
      ofxsetPosAndSize(this, _x, _y, _w, _h);
    }
    REPLACEBASEMETHODS();
};

//_x, puGetWindowHeight() - _y - _h - 1, _x + _w + 1, puGetWindowHeight() - _y,

class ofxpuButtonBox : public puButtonBox {
  public:
    ofxpuButtonBox (float _x, float _y, float _w, float _h, char **_labels, int _one_button) :
      puButtonBox (0, 0, 0, 0, _labels, _one_button) {
      ofxsetPosAndSize(this, _x, _y, _w, _h);
    }
    REPLACEBASEMETHODS();
};

// Not totally converted internal coordinates still have to be specified in the
// pui coordinate system

class ofxpuDialogBox : public puDialogBox {
  public:
    ofxpuDialogBox (float _x, float _y, float _w, float _h) :
      puDialogBox (0, 0) {
        ofxsetPosAndSize(this, _x, _y, _w, _h);
    }
    REPLACEBASEMETHODS();
};


class ofxpuaFileSelector : public puaFileSelector {
  public:
    ofxpuaFileSelector (float _x, float _y, float _w,  float _h, int _arrows,
                         const char *_dir, const char *_title = "Pick a file" ) :
        puaFileSelector (0, 0, 0, 0, _arrows, _dir, _title) {
          ofxsetPosAndSize(this, _x, _y, _w, _h);
      }

    ofxpuaFileSelector (float _x,  float _y,  float _w, float _h,
                        const char *_dir, const char *_title = "Pick a file" ) :
        puaFileSelector (0, 0, _w, _h, _dir, _title) {
          ofxsetPosAndSize(this, _x, _y, _w, _h);
      }

/*  // Does not work as no width or height is initially defined
    ofxpuaFileSelector (float _x,  float _y, int _arrows,
                        const char *_dir, const char *_title = "Pick a file" ) :
        puaFileSelector (0, 0, _arrows, _dir, _title) {
          ofxsetPosAndSize(this, _x, _y, 0, 0);
      }
    ofxpuaFileSelector (float _x,  float _y,
                        const char *_dir, const char *_title = "Pick a file" ) :
        puaFileSelector (0, 0, _dir, _title) {
          ofxsetPosAndSize(this, _x, _y, 0, 0);
      }
*/
    REPLACEBASEMETHODS();
};

class ofxpuaLargeInput : public puaLargeInput {
  public:
    ofxpuaLargeInput (float _x, float _y, float _w,  float _h, int _arrows,
                      int _sl_width, int _wrap_text = FALSE) :
        puaLargeInput (0, 0, 0, 0, _arrows, _sl_width, _wrap_text) {
          ofxsetPosAndSize(this, _x, _y, _w, _h);
      }
   REPLACEBASEMETHODS();
};

// Not totally converted runs into problem with puGroup and width and height

class ofxpuaComboBox : public puaComboBox {
  public:
    ofxpuaComboBox (float _x, float _y, float _w, float _h, char **_list, int _editable = TRUE) :
      puaComboBox (0, 0, 0, 0, _list, _editable) {
        ofxsetPosAndSize(this, _x, _y, _w, _h);
      }
    REPLACEBASEMETHODS();
};

// Grows Up and also default which translates to 0,0 only gets the last menu item on the screen.

class ofxpuaVerticalMenu : public puaVerticalMenu {
  public:
    ofxpuaVerticalMenu (float _x=-1, float _y=-1) :
      puaVerticalMenu (0, 0) {
        _x = (_x < 0 ? 0 : _x);
        _y = (_y < 0 ? (puGetDefaultLegendFont().getStringHeight()+PUSTR_TGAP+PUSTR_BGAP) : _y);
        ofxsetPosAndSize(this, _x, _y, 0, 0);
      }
    REPLACEBASEMETHODS();
};


class ofxpuaSelectBox : public puaSelectBox {
  public:
    ofxpuaSelectBox (float _x, float _y, float _w, float _h, char **_list) :
      puaSelectBox (0, 0, 0, 0, _list) {
        ofxsetPosAndSize(this, _x, _y, _w, _h);
    }
    REPLACEBASEMETHODS();
};


// arrows - 1 for single arrows and 2 for single and double arrows.

class ofxpuaScrollBar : public puaScrollBar {
  public:
    ofxpuaScrollBar(float _x, float _y, float _sz, int _arrows, int _vertical = false) :
      puaScrollBar (_x,
                _vertical ? (puGetWindowHeight()-_y-_sz) - 1 :
                puGetWindowHeight()-_y - (puGetDefaultLegendFont().getStringHeight()+puGetDefaultLegendFont().getStringDescender()+PUSTR_TGAP+PUSTR_BGAP) + 1,
                _sz+1,
                _arrows,
                _vertical,
                _vertical ? (puGetDefaultLegendFont().getStringWidth("W")+PUSTR_LGAP+PUSTR_RGAP) :
                (puGetDefaultLegendFont().getStringHeight()+puGetDefaultLegendFont().getStringDescender()+PUSTR_TGAP+PUSTR_BGAP) - 1
                ) {
      }
    ofxpuaScrollBar(float _x, float _y, int _sz, int _arrows, int _vertical, int _width) :
      puaScrollBar (_x,
                _vertical ? (puGetWindowHeight()-_y-_sz) - 1 : puGetWindowHeight()-_y - _width - 1,
                _sz+1,
                _arrows,
                _vertical,
                _vertical ? (_width) + 1 : (_width) + 1
                ) {
      }
    REPLACEBASEMETHODS();
};


class ofxpuaBiSlider : public puaBiSlider {
  public:
    ofxpuaBiSlider(float _x, float _y, float _sz, int _vertical = false) :
      puaBiSlider (_x,
                _vertical ? (puGetWindowHeight()-_y-_sz) - 1 :
                puGetWindowHeight()-_y - (puGetDefaultLegendFont().getStringHeight()+puGetDefaultLegendFont().getStringDescender()+PUSTR_TGAP+PUSTR_BGAP) + 1,
                _sz+1,
                _vertical,
                _vertical ? (puGetDefaultLegendFont().getStringWidth("W")+PUSTR_LGAP+PUSTR_RGAP) :
                (puGetDefaultLegendFont().getStringHeight()+puGetDefaultLegendFont().getStringDescender()+PUSTR_TGAP+PUSTR_BGAP) - 1
                ) {
      }
    ofxpuaBiSlider(float _x, float _y, int _sz, int _vertical, int _width) :
      puaBiSlider (_x,
                _vertical ? (puGetWindowHeight()-_y-_sz) - 1 : puGetWindowHeight()-_y - _width - 1,
                _sz+1,
                _vertical,
                _vertical ? (_width) + 1 : (_width) + 1
                ) {
      }
    REPLACEBASEMETHODS();
};

class ofxpuaTriSlider : public puaTriSlider {
  public:
    ofxpuaTriSlider(float _x, float _y, float _sz, int _vertical = false) :
      puaTriSlider (_x,
                _vertical ? (puGetWindowHeight()-_y-_sz) - 1 :
                puGetWindowHeight()-_y - (puGetDefaultLegendFont().getStringHeight()+puGetDefaultLegendFont().getStringDescender()+PUSTR_TGAP+PUSTR_BGAP) + 1,
                _sz+1,
                _vertical,
                _vertical ? (puGetDefaultLegendFont().getStringWidth("W")+PUSTR_LGAP+PUSTR_RGAP) :
                (puGetDefaultLegendFont().getStringHeight()+puGetDefaultLegendFont().getStringDescender()+PUSTR_TGAP+PUSTR_BGAP) - 1
                ) {
      }
    ofxpuaTriSlider(float _x, float _y, int _sz, int _vertical, int _width) :
      puaTriSlider (_x,
                _vertical ? (puGetWindowHeight()-_y-_sz) - 1 : puGetWindowHeight()-_y - _width - 1,
                _sz+1,
                _vertical,
                _vertical ? (_width) + 1 : (_width) + 1
                ) {
      }
    REPLACEBASEMETHODS();
};


// A vertical puBiSlider with a puInput box above it showing the current maximum
// value and a puInput box below it showing the current minimum value

class ofxpuaBiSliderWithEnds : public puaBiSliderWithEnds {
  public:
    ofxpuaBiSliderWithEnds (float _x, float _y, float _w, float _h) :
      puaBiSliderWithEnds (0, 0, 0, 0) {
        ofxsetPosAndSize(this, _x, _y, _w, _h);
    }
    REPLACEBASEMETHODS();
};

// A vertical puSlider with a puInput box above or below it showing its value
// Note to self: Some ability to do a "semi logarithmic" scale if you don't do a setStepSize

class ofxpuaSliderWithInput : public puaSliderWithInput {
  public:
    ofxpuaSliderWithInput (float _x, float _y, float _w, float _h, int _above = FALSE) :
      puaSliderWithInput (0, 0, 0, 0, _above) {
        ofxsetPosAndSize(this, _x, _y, _w, _h);
    }
    REPLACEBASEMETHODS();
};

// Doesn't work.  Probably a puGroup problem

class ofxpuaSpinBox : public puaSpinBox {
  public:
    ofxpuaSpinBox (float _x, float _y, float _w, float _h, int _arrow_pos = 1) :
      puaSpinBox (_x, puGetWindowHeight() - _y - _h - 1, _x + _w + 1, puGetWindowHeight() - _y, _arrow_pos) {
        // ofxsetPosAndSize(this, _x, _y, _w, _h);
    }
    REPLACEBASEMETHODS();
};


class ofxpuaList : public puaList {
  public:
    ofxpuaList (float _x, float _y, float _w,  float _h, int _sl_width = 20) :
      puaList (0, 0, 0, 0, _sl_width) {
        ofxsetPosAndSize(this, _x, _y, _w, _h);
      }
    ofxpuaList (float _x, float _y, float _w,  float _h, char ** _contents, int _sl_width = 20) :
      puaList (0, 0, 0, 0, _contents, _sl_width) {
        ofxsetPosAndSize(this, _x, _y, _w, _h);
      }
    ofxpuaList (float _x, float _y, float _w,  float _h, short _transparent, int _sl_width = 20) :
      puaList (0, 0, 0, 0, _transparent, _sl_width) {
        ofxsetPosAndSize(this, _x, _y, _w, _h);
      }
    ofxpuaList (float _x, float _y, float _w,  float _h, short _transparent, char ** _contents, int _sl_width = 20) :
      puaList (0, 0, 0, 0, _transparent, _contents, _sl_width) {
        ofxsetPosAndSize(this, _x, _y, _w, _h);
      }
    REPLACEBASEMETHODS();
};


#endif

