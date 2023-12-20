#ifndef _IMAGE_H_
#define _IMAGE_H_
#ifdef __cplusplus
extern "C" {
#endif

#include "stdint.h"
#include "ldCommon.h"

//frame.png w:72 h:72 < A8 >
#if USE_VIRTUAL_RESOURCE == 0
extern const uint8_t frame_png[];
#define FRAME_PNG (uint32_t)&frame_png[0]
#else
#define FRAME_PNG 0
#endif

//press.bmp w:72 h:72 < 565 >
#if USE_VIRTUAL_RESOURCE == 0
extern const uint8_t press_bmp[];
#define PRESS_BMP (uint32_t)&press_bmp[0]
#else
#define PRESS_BMP 5184
#endif

//release.bmp w:72 h:72 < 565 >
#if USE_VIRTUAL_RESOURCE == 0
extern const uint8_t release_bmp[];
#define RELEASE_BMP (uint32_t)&release_bmp[0]
#else
#define RELEASE_BMP 15552
#endif

#define HARMONYOS_SANS_SC_MEDIUM_16 (&dict_HarmonyOS_Sans_SC_Medium_16)
extern ldFontDict_t dict_HarmonyOS_Sans_SC_Medium_16;

#ifdef __cplusplus
}
#endif
#endif //_IMAGE_H_
