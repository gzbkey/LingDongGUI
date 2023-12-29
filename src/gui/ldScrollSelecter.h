#ifndef _LD_SCROLLSELECTER_H_
#define _LD_SCROLLSELECTER_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "ldCommon.h"

typedef struct {
    LD_COMMON_ATTRIBUTES;
    bool isTransparent:1;
    bool isWaitMove:1;
    uint8_t align:4;
    bool isAutoMove:1;
    uint8_t itemMax;
    uint8_t itemCount;
    ldFontDict_t* pFontDict;
    void **ppItemStrGroup;
    int16_t scrollOffset;
    ldColor charColor;
    ldColor bgColor;
    uint8_t itemSelect;
    uint8_t moveOffset;
#if USE_OPACITY == 1
    uint8_t opacity;
#endif
}ldScrollSelecter_t;

ldScrollSelecter_t* ldScrollSelecterInit(uint16_t nameId, uint16_t parentNameId, int16_t x, int16_t y, int16_t width, int16_t height, ldFontDict_t *pFontDict, uint8_t itemMax);
void ldScrollSelecterFrameUpdate(ldScrollSelecter_t* pWidget);
void ldScrollSelecterLoop(ldScrollSelecter_t *pWidget,const arm_2d_tile_t *pParentTile,bool bIsNewFrame);
void ldScrollSelecterDel(ldScrollSelecter_t *pWidget);
//void ldScrollSelecterSetText(ldScrollSelecter_t* pWidget,uint8_t *pStr);
void ldScrollSelecterSetFont(ldScrollSelecter_t *pWidget,ldFontDict_t *pFontDict);
void ldScrollSelecterAddItem(ldScrollSelecter_t* pWidget,uint8_t *pStr);
void ldScrollSelecterSetBgColor(ldScrollSelecter_t* pWidget,ldColor bgColor);
void ldScrollSelecterSetBgImage(ldScrollSelecter_t* pWidget,uint32_t imgAddr);
void ldScrollSelecterSetTransparent(ldScrollSelecter_t* pWidget,bool isTransparent);
void ldScrollSelecterSetOpacity(ldScrollSelecter_t *pWidget, uint8_t opacity);
void ldScrollSelecterSetSpeed(ldScrollSelecter_t *pWidget, uint8_t speed);
void ldScrollSelecterSetItem(ldScrollSelecter_t *pWidget, uint8_t itemNum);
void ldScrollSelecterSetAlign(ldScrollSelecter_t *pWidget,uint8_t align);

#define ldScrollSelecterSetHidden          ldBaseSetHidden
#define ldScrollSelecterMove               ldBaseMove

#ifdef __cplusplus
}
#endif

#endif //_LD_SCROLLSELECTER_H_
