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
    
// ldWidgetType     4
// arm_2d_tile_t  24

#if USE_VIRTUAL_RESOURCE == 0
#define LD_COMMON_ATTRIBUTES  arm_2d_tile_t resource; \
                              ldWidgetType widgetType; \
                              ldWidgetType parentType; \
                              void * parentWidget; \
                              xListNode *childList; \
                              uint16_t nameId; \
                              bool isHidden:1; \
                              bool isParentHidden:1
#else
#define LD_COMMON_ATTRIBUTES  arm_2d_vres_t resource; \
                              ldWidgetType widgetType; \
                              ldWidgetType parentType; \
                              void * parentWidget; \
                              xListNode *childList; \
                              uint16_t nameId; \
                              bool isHidden:1; \
                              bool isParentHidden:1
#endif
                             
                             

typedef struct{
    LD_COMMON_ATTRIBUTES;
}ldCommon_t;

typedef struct{
    int16_t x;
    int16_t y;
}ldPoint_t;

typedef struct{
    arm_2d_tile_t *ptRes;
    arm_2d_font_t *ptFont;
    ldColor charColor;
    uint16_t len;
    uint8_t* pStr;
}ldChar_t;

//typedef struct{
//    int16_t width;
//    int16_t height;
//    uint32_t addr;
//    uint8_t ascii;
//}ldNumImgInfo;

//typedef struct{
//    int16_t width;
//    int16_t height;
//    uint32_t addr;
//    uint8_t ascii;
//}ldDateTimeImgInfo;

//typedef enum
//{
//    hLeft,
//    hCenter,
//    hRight
//}ldHorizontalAlign;

//typedef enum
//{
//    vTop,
//    vCenter,
//    vBottom
//}ldVerticalAlign;



//extern uint16_t cfgMonitorWidth;
//extern uint16_t cfgMonitorHeight;


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
void ldBaseImage(arm_2d_tile_t* ptTile,arm_2d_tile_t *resource,bool isWithMask,uint8_t opacity);
void ldBaseMaskImage(arm_2d_tile_t* ptTile,arm_2d_tile_t *resource,ldColor textColor,uint8_t opacity);

void ldBaseSetTextInfo(arm_2d_tile_t* ptTile,arm_2d_font_t *ptFont,ldColor textColor,uint8_t opacity);
int ldBaseSetText(const char *format, ...);

void ldBaseTextDel(ldChar_t *charInfo);

#ifdef __cplusplus
}
#endif

#endif //_LDCOMMON_H_
