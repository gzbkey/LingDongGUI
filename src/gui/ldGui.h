#ifndef _LD_GUI_H_
#define _LD_GUI_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "arm_2d.h"
#include "ldCommon.h"
#include "ldButton.h"
#include "ldWindow.h"
#include "ldText.h"
#include "ldProgressBar.h"
#include "ldRadialMenu.h"
#include "ldCheckBox.h"
#include "ldLabel.h"
#include "ldScrollSelecter.h"
#include "ldDateTime.h"
#include "ldIconSlider.h"
#include "ldGauge.h"
#include "ldQRCode.h"
#include "ldTable.h"
#include "ldKeyboard.h"
#include "ldLineEdit.h"
#include "ldGraph.h"
#include "ldComboBox.h"
#include "ldArc.h"

#define SYS_TICK_CYCLE_MS          10

//btn占用0-9
#define SIGNAL_NO_OPERATION         BTN_NO_OPERATION
#define SIGNAL_PRESS                BTN_PRESS           // value = x (2Byte) + y (2Byte)
#define SIGNAL_HOLD_DOWN            BTN_HOLD_DOWN       // value = x offset(2Byte) + y offset(2Byte) + x (2Byte) + y (2Byte)
#define SIGNAL_RELEASE              BTN_RELEASE         // value = x speed(2Byte) + y speed(2Byte) + x (2Byte) + y (2Byte)

#define SIGNAL_CLICKED_ITEM         12                  // value = item num

#define SIGNAL_EDITING_FINISHED     13

//#define SIGNAL_SYS_SLIDER_LEFT      14
//#define SIGNAL_SYS_SLIDER_RIGHT     15
//#define SIGNAL_SYS_SLIDER_UP        16
//#define SIGNAL_SYS_SLIDER_DOWN      17

//#define SIGNAL_VALUE_CHANGED           7 //数据变化
//#define SIGNAL_WIDGET_ACTIVE           8 //激活控件
//#define SIGNAL_SYS_TIME_OUT             10//gui软件定时器

#define GET_SIGNAL_OFFSET_X(dat)     ((dat>>48)&0xFFFF)
#define GET_SIGNAL_OFFSET_Y(dat)     ((dat>>32)&0xFFFF)

#define GET_SIGNAL_SPEED_X(dat)     ((dat>>48)&0xFFFF)
#define GET_SIGNAL_SPEED_Y(dat)     ((dat>>32)&0xFFFF)

#define GET_SIGNAL_VALUE_X(dat)     ((dat>>16)&0xFFFF)
#define GET_SIGNAL_VALUE_Y(dat)     (dat&0xFFFF)

typedef void (*pFuncTypedef)(uint8_t);

extern void (*ldPageInitFunc[LD_PAGE_MAX])(uint8_t pageNum);
extern void (*ldPageLoopFunc[LD_PAGE_MAX])(uint8_t pageNum);
extern void (*ldPageQuitFunc[LD_PAGE_MAX])(uint8_t pageNum);
extern uint8_t pageNumNow;
extern uint8_t pageTarget;
extern uint8_t cursorBlinkCount;
extern bool cursorBlinkFlag;

#define CURSOR_BLINK_TIMEOUT        50 // 500/SYS_TICK_CYCLE_MS

#define LD_ADD_PAGE(pageName)       ldGuiAddPage(pageName##Init,pageName##Loop,pageName##Quit)

void ldGuiAddPage(pFuncTypedef init,pFuncTypedef loop,pFuncTypedef quit);
void ldGuiInit(arm_2d_scene_t *pSence);
void ldGuiLogicLoop(void);
void ldGuiLoop(arm_2d_scene_t *pSence,arm_2d_tile_t *ptParent,bool bIsNewFrame);
void ldGuiQuit(arm_2d_scene_t *pSence);
void ldGuiJumpPage(uint8_t pageNum);
arm_2d_region_list_item_t * ldGuiGetDirtyRegion(void);
void ldGuiFrameStart(arm_2d_scene_t *pSence);
void ldGuiFrameComplete(void);
void ldGuiUpdateScene(void);

#ifdef __cplusplus
}
#endif

#endif //_LD_GUI_H_
