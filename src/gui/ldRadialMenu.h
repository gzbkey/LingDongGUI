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
    ldDirtyRegionStateType_t dirtyRegionState:2;
    arm_2d_region_list_item_t dirtyRegionListItem;
    arm_2d_region_t dirtyRegionTemp;
    arm_2d_region_t itemRegion;
    uint8_t count;
#if USE_RADIA_MENU_SCALE == 1
    uint8_t scalePercent;
#endif
    uint16_t angle;
    arm_2d_op_trans_msk_opa_t op;
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
void ldRadialMenuFrameUpdate(ldRadialMenu_t* pWidget);
void ldRadialMenuLoop(ldRadialMenu_t *pWidget,const arm_2d_tile_t *pParentTile,bool bIsNewFrame);
void ldRadialMenuDel(ldRadialMenu_t *pWidget);

void ldRadialMenuAddItem(ldRadialMenu_t *pWidget,uint32_t imageAddr,uint16_t width,uint16_t height,uint8_t itemSubCount,bool isWithMask);
void ldRadialMenuSelectItem(ldRadialMenu_t *pWidget,uint8_t num);

#define ldRadialMenuSetHidden        ldBaseSetHidden
#define ldRadialMenuMove             ldBaseMove

#ifdef __cplusplus
}
#endif

#endif //_LD_RADIALMENU_H_
