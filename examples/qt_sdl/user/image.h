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

//tree.bmp w:121 h:121 < 565 >
#if USE_VIRTUAL_RESOURCE == 0
extern const uint8_t tree_bmp[];
#define TREE_BMP (uint32_t)&tree_bmp[0]
#else
#define TREE_BMP 13786
#endif

//frame.png w:70 h:30 < A8 >
#if USE_VIRTUAL_RESOURCE == 0
extern const uint8_t frame_png[];
#define FRAME_PNG (uint32_t)&frame_png[0]
#else
#define FRAME_PNG 43068
#endif

//press.bmp w:70 h:30 < 565 >
#if USE_VIRTUAL_RESOURCE == 0
extern const uint8_t press_bmp[];
#define PRESS_BMP (uint32_t)&press_bmp[0]
#else
#define PRESS_BMP 45168
#endif

//release.bmp w:70 h:30 < 565 >
#if USE_VIRTUAL_RESOURCE == 0
extern const uint8_t release_bmp[];
#define RELEASE_BMP (uint32_t)&release_bmp[0]
#else
#define RELEASE_BMP 49368
#endif

#define SIMSUN_REGULAR_12 (&dict_SimSun_Regular_12)
extern ldFontDict_t dict_SimSun_Regular_12;

//arc51.png w:51 h:51 < A8 >
#if USE_VIRTUAL_RESOURCE == 0
extern const uint8_t arc51_png[];
#define ARC51_PNG (uint32_t)&arc51_png[0]
#endif

#ifdef __cplusplus
}
#endif
#endif //_IMAGE_H_
