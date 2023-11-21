#ifndef _IMAGE_H_
#define _IMAGE_H_
#ifdef __cplusplus
extern "C" {
#endif

#include "stdint.h"
#include "ldCommon.h"

//bbg.bmp w:52 h:52 < 565 >
#if USE_VIRTUAL_RESOURCE == 0
extern const uint8_t bbg_bmp[];
#define BBG_BMP (uint32_t)&bbg_bmp[0]
#else
#define BBG_BMP 0
#endif

//icon.bmp w:52 h:52 < 565 >
#if USE_VIRTUAL_RESOURCE == 0
extern const uint8_t icon_bmp[];
#define ICON_BMP (uint32_t)&icon_bmp[0]
#else
#define ICON_BMP 5408
#endif

//pointer.bmp w:9 h:55 < 565 >
#if USE_VIRTUAL_RESOURCE == 0
extern const uint8_t pointer_bmp[];
#define POINTER_BMP (uint32_t)&pointer_bmp[0]
#else
#define POINTER_BMP 10816
#endif

//pointer.png w:9 h:55 < 565+Mask >
#if USE_VIRTUAL_RESOURCE == 0
extern const uint8_t pointer_png[];
#define POINTER_PNG (uint32_t)&pointer_png[0]
#else
#define POINTER_PNG 11806
#endif

//pointerMask.png w:9 h:55 < A8 >
#if USE_VIRTUAL_RESOURCE == 0
extern const uint8_t pointerMask_png[];
#define POINTERMASK_PNG (uint32_t)&pointerMask_png[0]
#else
#define POINTERMASK_PNG 13291
#endif

#define WENQUANYI_ZEN_HEI_REGULAR_9 (&dict_WenQuanYi_Zen_Hei_Regular_9)
extern ldFontDict_t dict_WenQuanYi_Zen_Hei_Regular_9;

#ifdef __cplusplus
}
#endif
#endif //_IMAGE_H_
