#ifndef _LD_RADIALMENU_H_
#define _LD_RADIALMENU_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "ldCommon.h"

#ifndef USE_RADIA_MENU_SCALE
#define USE_RADIA_MENU_SCALE     1
#endif

typedef struct {
    uint32_t addr;
    bool isWithMask:1;
    uint8_t count;
#if USE_RADIA_MENU_SCALE == 1
    uint8_t scalePercent;
#endif
    ldSize_t size;
    ldPoint_t pos;
    uint16_t angle;
}ldRadialMenuItem_t;

typedef struct {
    LD_COMMON_ATTRIBUTES;
    bool isMove:1;
    bool isWaitInit:1;
    ldRadialMenuItem_t *pItemList;
    uint8_t itemCount;
    uint8_t itemMax;
    uint16_t xAxis;
    uint16_t yAxis;
    ldPoint_t originPos;
    int16_t offsetAngle;
    int16_t nowAngle;
    uint8_t targetItem;
    uint8_t selectItem;
    uint32_t timer;
    uint8_t *showList;
}ldRadialMenu_t;

ldRadialMenu_t* ldRadialMenuInit(uint16_t nameId, uint16_t parentNameId, int16_t x,int16_t y,int16_t width,int16_t height,
                               uint16_t xAxis, uint16_t yAxis, uint8_t itemMax);
void ldRadialMenuLoop(ldRadialMenu_t *pWidget,const arm_2d_tile_t *ptParent,bool bIsNewFrame);
void ldRadialMenuDel(ldRadialMenu_t *pWidget);

void ldRadialMenuAddItem(ldRadialMenu_t *pWidget,uint32_t imageAddr,uint16_t width,uint16_t height,uint8_t itemSubCount,bool isWithMask);
void ldRadialMenuSelectItem(ldRadialMenu_t *pWidget,uint8_t num);

#ifdef __cplusplus
}
#endif

#endif //_LD_RADIALMENU_H_
