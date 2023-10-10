#ifndef _LD_SCROLLSELECTER_H_
#define _LD_SCROLLSELECTER_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "ldCommon.h"

typedef struct {
    LD_COMMON_ATTRIBUTES;
    uint8_t itemMax;
    uint8_t itemCount;
    void **ppItemStrGroup;
    ldChar_t *pTextInfo;
    ldColor charColor;
    int16_t scrollOffset;
#if USE_OPACITY == 1
    uint8_t opacity;
#endif
}ldScrollSelecter_t;

ldScrollSelecter_t* ldScrollSelecterInit(uint16_t nameId, uint16_t parentNameId, int16_t x, int16_t y, int16_t width, int16_t height, ldFontDict_t *pFontDict, uint8_t itemMax);
void ldScrollSelecterLoop(ldScrollSelecter_t *pWidget,const arm_2d_tile_t *pParentTile,bool bIsNewFrame);
void ldScrollSelecterDel(ldScrollSelecter_t *pWidget);
//void ldScrollSelecterSetText(ldScrollSelecter_t* pWidget,uint8_t *pStr);
void ldScrollSelecterSetFont(ldScrollSelecter_t *pWidget,ldFontDict_t *pFontDict);
void ldScrollSelecterAddItem(ldScrollSelecter_t* pWidget,uint8_t *pStr);

#define ldScrollSelecterSetHidden          ldBaseSetHidden
#define ldScrollSelecterMove               ldBaseMove

#ifdef __cplusplus
}
#endif

#endif //_LD_SCROLLSELECTER_H_
