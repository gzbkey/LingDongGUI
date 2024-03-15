#ifndef _LD_TEXT_H_
#define _LD_TEXT_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "ldCommon.h"

typedef struct {
    LD_COMMON_ATTRIBUTES;
    bool isTransparent:1;
    bool isRelease:1;
    bool isWaitInit:1;
    arm_2d_helper_pi_slider_t tPISlider;
    uintptr_t bgImgAddr;
    ldChar_t *pTextInfo;
    ldColor bgColor;
    int16_t scrollOffset;
    int16_t strHeight;
#if USE_OPACITY == 1
    uint8_t opacity;
#endif
}ldText_t;

ldText_t* ldTextInit(uint16_t nameId, uint16_t parentNameId, int16_t x, int16_t y, int16_t width, int16_t height, ldFontDict_t *pFontDict);
void ldTextFrameUpdate(ldText_t* pWidget);
void ldTextLoop(ldText_t *pWidget,const arm_2d_tile_t *pParentTile,bool bIsNewFrame);
void ldTextDel(ldText_t *pWidget);

void ldTextSetText(ldText_t* pWidget,uint8_t *pStr);
void ldTextSetTextColor(ldText_t* pWidget,ldColor charColor);
void ldTextSetAlign(ldText_t *pWidget,uint8_t align);
void ldTextSetTransparent(ldText_t* pWidget,bool isTransparent);
void ldTextScrollSeek(ldText_t *pWidget,int16_t offset);
void ldTextScrollMove(ldText_t *pWidget, int8_t moveValue);
void ldTextSetScroll(ldText_t *pWidget,bool isEnable);
void ldTextSetOpacity(ldText_t *pWidget, uint8_t opacity);
void ldTextSetBgImage(ldText_t *pWidget, uintptr_t imageAddr);
void ldTextSetBgColor(ldText_t *pWidget, ldColor bgColor);

#define ldTextSetHidden        ldBaseSetHidden
#define ldTextMove             ldBaseMove



#ifdef __cplusplus
}
#endif

#endif //_LD_TEXT_H_
