#ifndef _BIN_WATCH_H_
#define _BIN_WATCH_H_
#ifdef __cplusplus
extern "C" {
#endif

#include "stdint.h"
#include "ldCommon.h"

//contacts.png w:30 h:30 < A8 >
#if USE_VIRTUAL_RESOURCE == 0
extern const uint8_t contacts_png[];
#define CONTACTS_PNG (uint32_t)&contacts_png[0]
#else
#define CONTACTS_PNG 0
#endif

//icon.bmp w:72 h:72 < 565 >
#if USE_VIRTUAL_RESOURCE == 0
extern const uint8_t icon_bmp[];
#define ICON_BMP (uint32_t)&icon_bmp[0]
#else
#define ICON_BMP 900
#endif

//pos1.png w:100 h:30 < A8 >
#if USE_VIRTUAL_RESOURCE == 0
extern const uint8_t pos1_png[];
#define POS1_PNG (uint32_t)&pos1_png[0]
#else
#define POS1_PNG 11268
#endif

//pos2.png w:100 h:30 < A8 >
#if USE_VIRTUAL_RESOURCE == 0
extern const uint8_t pos2_png[];
#define POS2_PNG (uint32_t)&pos2_png[0]
#else
#define POS2_PNG 14268
#endif

//pos3.png w:100 h:30 < A8 >
#if USE_VIRTUAL_RESOURCE == 0
extern const uint8_t pos3_png[];
#define POS3_PNG (uint32_t)&pos3_png[0]
#else
#define POS3_PNG 17268
#endif

//wallpaper.jpg w:240 h:240 < 565 >
#if USE_VIRTUAL_RESOURCE == 0
extern const uint8_t wallpaper_jpg[];
#define WALLPAPER_JPG (uint32_t)&wallpaper_jpg[0]
#else
#define WALLPAPER_JPG 20268
#endif

#define ARIAL_REGULAR_28 (&dict_Arial_Regular_28)
extern ldFontDict_t dict_Arial_Regular_28;

#define ARIAL_REGULAR_12 (&dict_Arial_Regular_12)
extern ldFontDict_t dict_Arial_Regular_12;

#ifdef __cplusplus
}
#endif
#endif //_BIN_WATCH_H_
