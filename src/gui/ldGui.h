#ifndef _LD_GUI_H_
#define _LD_GUI_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "arm_2d.h"
#include "ldCommon.h"


//btn占用0-9

#define SIGNAL_TOUCH_HOLD_MOVE      10 // press and move

//#define SIGNAL_TOUCH_CHECK_PRESSED      4 // 单状态按下
//#define SIGNAL_TOUCH_CHECK_RELEASED      5 // 单状态释放
//
//#define SIGNAL_VALUE_CHANGED           7 //数据变化
//#define SIGNAL_WIDGET_ACTIVE           8 //激活控件
//#define SIGNAL_EIDTING_FINISHED           9 //编辑结束
//#define SIGNAL_SYS_TIME_OUT             10//gui软件定时器





extern uint8_t pageNumNow;
extern uint8_t pageTarget;

void ldGuiInit(void);
void ldGuiLogicLoop(void);
void ldGuiLoop(const arm_2d_tile_t *ptParent,bool bIsNewFrame);
void ldGuiQuit(void);
void ldGuiJumpPage(uint8_t pageNum);
void ldGuiDelWidget(ldCommon_t *widget);

#ifdef __cplusplus
}
#endif

#endif //_LD_GUI_H_
