#ifndef _LD_GUI_H_
#define _LD_GUI_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "arm_2d.h"
#include "ldCommon.h"


//btn占用0-9
#define SIGNAL_NO_OPERATION         BTN_NO_OPERATION
#define SIGNAL_PRESS                BTN_PRESS           // value = x (2Byte) + y (2Byte)
#define SIGNAL_HOLD_DOWN            BTN_HOLD_DOWN
#define SIGNAL_RELEASE              BTN_RELEASE         // value = x (2Byte) + y (2Byte)

#define SIGNAL_TOUCH_HOLD_MOVE      10                  // value = x offset(2Byte) + y offset(2Byte)
#define SIGNAL_MOVE_SPEED           11                  // value = x speed(2Byte) + y speed(2Byte)
#define SIGNAL_CLICKED_ITEM         12                  // value = item num

#define SIGNAL_INPUT_ASCII          13
#define SIGNAL_EDITING_FINISHED     14

//#define SIGNAL_VALUE_CHANGED           7 //数据变化
//#define SIGNAL_WIDGET_ACTIVE           8 //激活控件
//#define SIGNAL_EIDTING_FINISHED           9 //编辑结束
//#define SIGNAL_SYS_TIME_OUT             10//gui软件定时器


#define GET_SIGNAL_VALUE_X(dat)     ((dat>>16)&0xFFFF)
#define GET_SIGNAL_VALUE_Y(dat)     (dat&0xFFFF)



extern uint8_t pageNumNow;
extern uint8_t pageTarget;
//extern bool isUpdateBackground;

void ldGuiInit(arm_2d_scene_t *pSence);
void ldGuiLogicLoop(void);
void ldGuiLoop(arm_2d_scene_t *pSence,arm_2d_tile_t *ptParent,bool bIsNewFrame);
void ldGuiQuit(void);
void ldGuiJumpPage(uint8_t pageNum);
void ldGuiDelWidget(ldCommon_t *widget);
arm_2d_region_list_item_t * ldGuiGetDirtyRegion(void);
void ldGuiFrameStart(void);

#ifdef __cplusplus
}
#endif

#endif //_LD_GUI_H_
