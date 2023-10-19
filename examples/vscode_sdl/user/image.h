#ifndef _IMAGE_H_
#define _IMAGE_H_
#ifdef __cplusplus
extern "C" {
#endif

#include "stdint.h"
#include "ldCommon.h"

#if USE_VIRTUAL_RESOURCE == 0

extern const uint8_t barFrame_png[];
#define BARFRAME_PNG (uint32_t)barFrame_png

extern const uint8_t frame_png[];
#define FRAME_PNG (uint32_t)frame_png

extern const uint8_t press_bmp[];
#define PRESS_BMP (uint32_t)&press_bmp[0]

extern const uint8_t release_bmp[];
#define RELEASE_BMP (uint32_t)release_bmp

extern const uint8_t release_png[];
#define RELEASE_PNG (uint32_t)release_png

#define MICROSOFT_YAHEI_REGULAR_20 (&dict_Microsoft_YaHei_Regular_20)
extern ldFontDict_t dict_Microsoft_YaHei_Regular_20;

#else

//barFrame.png w:100 h:18 < 565+Mask >
#define BARFRAME_PNG 0

//frame.png w:72 h:72 < A8 >
#define FRAME_PNG 5400

//press.bmp w:72 h:72 < 565 >
#define PRESS_BMP 10584

//release.bmp w:72 h:72 < 565 >
#define RELEASE_BMP 20952

//release.png w:72 h:72 < 565+Mask >
#define RELEASE_PNG 31320

#define MICROSOFT_YAHEI_REGULAR_20 (&dict_Microsoft_YaHei_Regular_20)
extern ldFontDict_t dict_Microsoft_YaHei_Regular_20;

#endif

#ifdef __cplusplus
}
#endif
#endif //_IMAGE_H_
