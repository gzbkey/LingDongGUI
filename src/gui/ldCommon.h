#ifndef _LDCOMMON_H_
#define _LDCOMMON_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "stdbool.h"
#include "stdint.h"
#include "arm_extra_controls.h"
#include "xList.h"
#include "xLog.h"
#include "ldGui.h"


#ifndef SET_BITS
#define SET_BITS(data,value)                    ((data)|=(value))
#endif
#ifndef CLR_BITS
#define CLR_BITS(data,value)                    ((data)&=~(value))
#endif
#ifndef SETBIT
#define SETBIT(data,move)                       ((data)|=(1<<(move)))
#endif
#ifndef CLRBIT
#define CLRBIT(data,move)                       ((data)&=~(1<<(move)))
#endif
#ifndef GETBIT
#define GETBIT(data,move)                       (((data)>>(move))&0x01)
#endif
#ifndef PUTBIT
#define PUTBIT(data,value,move)                 ((value)?SETBIT(data,move):CLRBIT(data,move))
#endif
#ifndef GET16H
#define GET16H(data)                            (((data)>>8)&0xFF)
#endif
#ifndef GET16L
#define GET16L(data)                            ((data)&0xFF)
#endif
#ifndef CONNECT16
#define CONNECT16(H,L)                          (((H)<<8)+(L))
#endif
#ifndef GET32H
#define   GET32H(data)                          (((data)>>24)&0xFF)
#endif
#ifndef GET32MH
#define GET32MH(data)                           (((data)>>16)&0xFF)
#endif
#ifndef GET32ML
#define GET32ML(data)                           (((data)>>8)&0xFF)
#endif
#ifndef GET32L
#define GET32L(data)                            ((data)&0xFF)
#endif
#ifndef CONNECT32
#define CONNECT32(H,MH,ML,L)                    (((H)<<24)+((MH)<<16)+((ML)<<8)+(L))
#endif
#ifndef SWAP
#define SWAP(x,y)                               ((x) = (x)+(y),(y)=(x)-(y),(x)=(x)-(y))
#endif

#define ldColor                                 COLOUR_INT

#define XMALLOC                                 ldMalloc
#define XFREE                                   ldFree

#define LD_MALLOC_WIDGET_INFO(widgetTypedef)    (widgetTypedef*)ldMalloc(sizeof(widgetTypedef))
#define LD_MALLOC_STRING(str)                   (uint8_t *)ldMalloc((strlen((const char *)str)+1)*sizeof(uint8_t))

#define LD_ALIGN_CENTER          0
#define LD_ALIGN_TOP             _BV(0)
#define LD_ALIGN_BOTTOM          _BV(1)
#define LD_ALIGN_LEFT            _BV(2)
#define LD_ALIGN_RIGHT           _BV(3)

typedef enum{
    widgetTypeNone,
    widgetTypeWindow,
    widgetTypeButton,
    widgetTypeImage,
    widgetTypeText,
    widgetTypeLineEdit,
    widgetTypeDashboardSimple,
    widgetTypeGraph,
    widgetTypeCheckBox,
    widgetTypeSlider,
    widgetTypeProgressBar,
    widgetTypeGauge,
    widgetTypeQRCode,
    widgetTypeDateTime,
    widgetTypeIconSlider,
    widgetTypeComboBox,
    widgetTypeIconRotateAround,
    widgetTypeNumber,
    widgetTypeStaticText,
    widgetTypeStartupImage,
    widgetTypeArc,
}ldWidgetType;
    

#if USE_VIRTUAL_RESOURCE == 0
#define LD_COMMON_ATTRIBUTES  arm_2d_tile_t resource; \
                              ldWidgetType widgetType; \
                              ldWidgetType parentType; \
                              void * parentWidget; \
                              xListNode *childList; \
                              uint16_t nameId; \
                              bool isHidden:1; \
                              bool isParentHidden:1

typedef struct{
    uint8_t utf8[4];
    uint8_t advW[2];
    uint8_t offsetX[2];
    uint8_t offsetY[2];
    uint8_t width[2];
    uint8_t height[2];
    uint8_t addr[4];
}ldFontInfo_t;

typedef struct{
    uint8_t maskType;
    uint32_t count;
    uint16_t lineOffset;
    int16_t descender;
    const ldFontInfo_t *pInfoList;
    const uint8_t *pFontSrc;
}ldFontDict_t;

typedef struct{
    arm_2d_tile_t tFontTile;//字库
    ldFontDict_t* ptFontDict;//字典(目录)
    uint16_t strLen;
    ldColor charColor;
    uint8_t* pStr;
    uint8_t align:4;
}ldChar_t;
#else
#define LD_COMMON_ATTRIBUTES  arm_2d_vres_t resource; \
                              ldWidgetType widgetType; \
                              ldWidgetType parentType; \
                              void * parentWidget; \
                              xListNode *childList; \
                              uint16_t nameId; \
                              bool isHidden:1; \
                              bool isParentHidden:1

typedef struct{
    uint8_t utf8[4];
    uint8_t advW[2];
    uint8_t offsetX[2];
    uint8_t offsetY[2];
    uint8_t width[2];
    uint8_t height[2];
    uint8_t addr[4];
}ldFontInfo_t;

typedef struct{
    uint8_t maskType;
    uint32_t count;
    uint16_t lineOffset;
    int16_t descender;
    const ldFontInfo_t *pInfoList;
    uint32_t pFontSrc;
}ldFontDict_t;

typedef struct{
    arm_2d_vres_t tFontTile;//字库
    ldFontDict_t* ptFontDict;//字典(目录)
    uint16_t strLen;
    ldColor charColor;
    uint8_t* pStr;
    uint8_t align:4;
}ldChar_t;
#endif

typedef struct{
    LD_COMMON_ATTRIBUTES;
}ldCommon_t;

typedef struct{
    int16_t x;
    int16_t y;
}ldPoint_t;





void ldCommonAutoLoop(ldCommon_t* pWidget,arm_2d_tile_t *ptTile,bool bIsNewFrame);
void ldCommonWidgetHidRefresh(uint8_t *hidBuf);
void ldCommonCmdRefreshNormal(uint8_t *dat);
void ldCommonCmdRefreshFixed(uint8_t *dat);

extern xListNode ldWidgetLink;


void *ldMalloc(uint32_t size);

void ldFree(void *p);

void *ldRealloc(void *ptr,uint32_t newSize);



ldPoint_t ldGetGlobalPos(ldCommon_t *widget);

xListNode* ldGetWidgetInfoById(uint16_t nameId);
xListNode* ldGetWidgetInfoByPos(int16_t x,int16_t y);
void* ldGetWidgetById(uint16_t nameId);

bool ldTimeOut(uint16_t ms, int64_t *plTimer,bool isReset);
void ldDelWidget(ldCommon_t *widget);

void ldBaseColor(arm_2d_tile_t* ptTile,ldColor color,uint8_t opacity);
void ldBaseImage(arm_2d_tile_t* ptTile,arm_2d_tile_t *ptResource,bool isWithMask,uint8_t opacity);
void ldBaseMaskImage(arm_2d_tile_t* ptTile, arm_2d_tile_t *ptResource, ldColor textColor, uint8_t opacity);

void ldBaseSetTextInfo(arm_2d_tile_t* ptTile,ldChar_t *ptCharInfo,uint8_t opacity);
void ldBaseSetFont(ldChar_t **pptCharInfo, ldFontDict_t *fontDictAddr);

void ldBaseTextDel(ldChar_t *charInfo);
ldChar_t * ldBaseCheckText(ldChar_t **charInfo);

uint8_t ldBaseGetCharInfo(ldFontDict_t *ptFontDict,uint8_t *charUtf8,int16_t *advWidth,int16_t *offsetX,int16_t *offsetY,int16_t *width,int16_t *height,uint32_t *imgAddr);
void ldBaseShowText(arm_2d_tile_t tTile,ldChar_t *ptTextInfo,int16_t scrollOffset);

void ldBaseSetHidden(ldCommon_t* widget,bool isHidden);
void ldBaseSetText(ldChar_t **pptTextInfo, uint8_t *pStr);
void ldBaseSetTextColor(ldChar_t **pptTextInfo, ldColor charColor);
void ldBaseSetAlign(ldChar_t **pptTextInfo, uint8_t align);

#ifdef __cplusplus
}
#endif

#endif //_LDCOMMON_H_
