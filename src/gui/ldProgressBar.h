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
    uint8_t bgOffset;
    uint8_t fgOffset;
    uint64_t timer;
}ldProgressBar_t;

ldProgressBar_t* ldProgressBarInit(uint16_t nameId, uint16_t parentNameId, int16_t x,int16_t y,int16_t width,int16_t height);
void ldProgressBarLoop(ldProgressBar_t *widget,const arm_2d_tile_t *ptParent,bool bIsNewFrame);
void ldProgressBarDel(ldProgressBar_t *widget);

void ldProgressBarSetPercent(ldProgressBar_t *widget,float percent);
void ldProgressBarSetBgImage(ldProgressBar_t *widget,uint32_t bgAddr,uint16_t bgWidth,bool isMove);
void ldProgressBarSetFgImage(ldProgressBar_t *widget,uint32_t fgAddr,uint16_t fgWidth,bool isMove);
void ldProgressBarSetFrameImage(ldProgressBar_t *widget,uint32_t frameAddr);

void ldProgressBarSetColor(ldProgressBar_t *widget,ldColor bgColor,ldColor fgColor,ldColor frameColor);

#ifdef __cplusplus
}
#endif

#endif //_LD_PROGRESSBAR_H_
