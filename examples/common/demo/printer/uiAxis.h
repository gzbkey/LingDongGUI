#ifndef __UI_AXIS_H__
#define __UI_AXIS_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "ldBase.h"
#include "fonts/uiFonts.h"
#include "images/uiImages.h"

extern const ldPageFuncGroup_t uiAxisFunc;

#define ID_BG                           0
#define ID_BTN_RET                      1
#define ID_SCROLL_SELCETER_X_TENS       2
#define ID_SCROLL_SELCETER_X_UNITS      3
#define ID_SCROLL_SELCETER_X_DECIMALS   4
#define ID_LABEL_X                      5
#define ID_LABEL_X_POINT                6

#define ID_SCROLL_SELCETER_Y_TENS       7
#define ID_SCROLL_SELCETER_Y_UNITS      8
#define ID_SCROLL_SELCETER_Y_DECIMALS   9
#define ID_LABEL_Y                      10
#define ID_LABEL_Y_POINT                11

#define ID_SCROLL_SELCETER_Z_TENS       12
#define ID_SCROLL_SELCETER_Z_UNITS      13
#define ID_SCROLL_SELCETER_Z_DECIMALS   14
#define ID_LABEL_Z                      15
#define ID_LABEL_Z_POINT                16

#define ID_LABEL_X_MM                   17
#define ID_LABEL_Y_MM                   18
#define ID_LABEL_Z_MM                   19
#ifdef __cplusplus
}
#endif

#endif
