#ifndef _IMAGE_H_
#define _IMAGE_H_
#ifdef __cplusplus
extern "C" {
#endif

#include "stdint.h"
#include "ldCommon.h"

extern const uint8_t frame_png[];
#define FRAME_PNG (uint32_t)&frame_png[0]

extern const uint8_t press_bmp[];
#define PRESS_BMP (uint32_t)&press_bmp[0]

extern const uint8_t release_bmp[];
#define RELEASE_BMP (uint32_t)&release_bmp[0]

extern const uint8_t release_png[];
#define RELEASE_PNG (uint32_t)&release_png[0]

#define MICROSOFT_YAHEI_REGULAR_20 (&dict_Microsoft_YaHei_Regular_20)
extern ldFontDict_t dict_Microsoft_YaHei_Regular_20;

#ifdef __cplusplus
}
#endif
#endif //_IMAGE_H_
