#ifndef _LD_PROGRESSBAR_H_
#define _LD_PROGRESSBAR_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "ldCommon.h"

#define PROGRESS_BAR_SPEED 10

typedef struct {
    LD_COMMON_ATTRIBUTES;
    bool isBgMove:1;
    bool isFgMove:1;
    uint16_t permille;

    ldColor bgColor;
    ldColor fgColor;
    ldColor frameColor;

    uint32_t bgAddr;
    uint32_t fgAddr;
    uint32_t frameAddr;//must png
    uint16_t bgWidth;
    uint16_t fgWidth;
    uint16_t frameWidth;
    uint8_t bgOffset;
    uint8_t fgOffset;
    uint64_t timer;
}ldProgressBar_t;

ldProgressBar_t* ldProgressBarInit(uint16_t nameId, uint16_t parentNameId, int16_t x,int16_t y,int16_t width,int16_t height);
void ldProgressBarLoop(ldProgressBar_t *pWidget,const arm_2d_tile_t *pParentTile,bool bIsNewFrame);
void ldProgressBarDel(ldProgressBar_t *pWidget);

void ldProgressBarSetPercent(ldProgressBar_t *pWidget,float percent);
void ldProgressBarSetBgImage(ldProgressBar_t *pWidget,uint32_t bgAddr,uint16_t bgWidth,bool isMove);
void ldProgressBarSetFgImage(ldProgressBar_t *pWidget,uint32_t fgAddr,uint16_t fgWidth,bool isMove);
void ldProgressBarSetFrameImage(ldProgressBar_t *pWidget, uint32_t frameAddr, uint16_t frameWidth);

void ldProgressBarSetColor(ldProgressBar_t *pWidget,ldColor bgColor,ldColor fgColor,ldColor frameColor);

#define ldProgressBarSetHidden        ldBaseSetHidden
#define ldProgressBarMove             ldBaseMove
#ifdef __cplusplus
}
#endif

#endif //_LD_PROGRESSBAR_H_
