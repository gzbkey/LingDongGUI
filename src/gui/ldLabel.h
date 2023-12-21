#ifndef _LD_LABEL_H_
#define _LD_LABEL_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "ldCommon.h"

typedef struct {
    LD_COMMON_ATTRIBUTES;
    bool isTransparent:1;
    uint8_t align:4;
    uint32_t bgImgAddr;
    ldColor bgColor;
    ldColor charColor;
    ldFontDict_t* pFontDict;
    uint8_t* pStr;
#if USE_OPACITY == 1
    uint8_t opacity;
#endif
}ldLabel_t;

ldLabel_t* ldLabelInit(uint16_t nameId, uint16_t parentNameId, int16_t x, int16_t y, int16_t width, int16_t height, ldFontDict_t *pFontDict);
void ldLabelFrameStart(ldLabel_t* pWidget);
void ldLabelLoop(ldLabel_t *pWidget,const arm_2d_tile_t *pParentTile,bool bIsNewFrame);
void ldLabelDel(ldLabel_t *pWidget);

void ldLabelSetTransparent(ldLabel_t* pWidget,bool isTransparent);
void ldLabelSetText(ldLabel_t* pWidget,uint8_t *pStr);
void ldLabelSetTextColor(ldLabel_t* pWidget,ldColor charColor);
void ldLabelSetAlign(ldLabel_t *pWidget,uint8_t align);
void ldLabelSetBgImage(ldLabel_t *pWidget, uint32_t imageAddr);
void ldLabelSetBgColor(ldLabel_t *pWidget, ldColor bgColor);
void ldLabelSetOpacity(ldLabel_t *pWidget, uint8_t opacity);

#define ldLabelSetHidden          ldBaseSetHidden
#define ldLabelMove               ldBaseMove

#ifdef __cplusplus
}
#endif

#endif //_LD_LABEL_H_
