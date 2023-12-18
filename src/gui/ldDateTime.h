#ifndef _LD_DATETIME_H_
#define _LD_DATETIME_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "ldCommon.h"

#define DATE_TIME_BUFFER_SIZE   32

typedef struct {
    LD_COMMON_ATTRIBUTES;
    bool isTransparent:1;
    uint8_t align:4;
    uint8_t month;
    uint8_t day;
    uint8_t hour;
    uint8_t minute;
    uint8_t second;
    uint16_t year;
    uint8_t formatStr[DATE_TIME_BUFFER_SIZE];
    uint8_t formatStrTemp[DATE_TIME_BUFFER_SIZE];
    ldColor bgColor;
    ldColor charColor;
    ldFontDict_t* pFontDict;
#if USE_OPACITY == 1
    uint8_t opacity;
#endif
}ldDateTime_t;

ldDateTime_t* ldDateTimeInit(uint16_t nameId, uint16_t parentNameId, int16_t x, int16_t y, int16_t width, int16_t height, ldFontDict_t *pFontDict);
void ldDateTimeLoop(ldDateTime_t *pWidget,const arm_2d_tile_t *pParentTile,bool bIsNewFrame);
void ldDateTimeDel(ldDateTime_t *pWidget);

void ldDateTimeSetTransparent(ldDateTime_t* pWidget,bool isTransparent);
void ldDateTimeSetFormat(ldDateTime_t* pWidget,uint8_t *pStr);
void ldDateTimeSetTextColor(ldDateTime_t* pWidget,ldColor charColor);
void ldDateTimeSetAlign(ldDateTime_t *pWidget,uint8_t align);
void ldDateTimeSetBgColor(ldDateTime_t *pWidget, ldColor bgColor);
void ldDateTimeSetOpacity(ldDateTime_t *pWidget, uint8_t opacity);
void ldDateTimeSetDate(ldDateTime_t *pWidget, uint16_t year, uint8_t month, uint8_t day);
void ldDateTimeSetTime(ldDateTime_t *pWidget, uint8_t hour, uint8_t minute, uint8_t second);

#define ldDateTimeSetHidden          ldBaseSetHidden
#define ldDateTimeMove               ldBaseMove

#ifdef __cplusplus
}
#endif

#endif //_LD_DATETIME_H_
