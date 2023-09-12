#ifndef _IMAGE_H_
#define _IMAGE_H_
#ifdef __cplusplus
extern "C" {
#endif

#include "stdint.h"
#include "ldCommon.h"

#if USE_VIRTUAL_RESOURCE == 0

extern const uint8_t icon1_jpeg[];
#define ICON1_JPEG (uint32_t)&icon1_jpeg[0]

extern const uint8_t icon2_jpeg[];
#define ICON2_JPEG (uint32_t)&icon2_jpeg[0]

extern const uint8_t icon3_jpeg[];
#define ICON3_JPEG (uint32_t)&icon3_jpeg[0]

extern const uint8_t icon4_jpeg[];
#define ICON4_JPEG (uint32_t)&icon4_jpeg[0]

extern const uint8_t icon5_jpeg[];
#define ICON5_JPEG (uint32_t)&icon5_jpeg[0]

extern const uint8_t icon6_jpeg[];
#define ICON6_JPEG (uint32_t)&icon6_jpeg[0]

extern const uint8_t icon7_jpeg[];
#define ICON7_JPEG (uint32_t)&icon7_jpeg[0]

extern const uint8_t icon8_jpeg[];
#define ICON8_JPEG (uint32_t)&icon8_jpeg[0]

extern const uint8_t icon9_jpeg[];
#define ICON9_JPEG (uint32_t)&icon9_jpeg[0]

#define JETBRAINS_MONO_NL_REGULAR_9 (&dict_JetBrains_Mono_NL_Regular_9)
extern ldFontDict_t dict_JetBrains_Mono_NL_Regular_9;

#else

//icon1.jpeg w:100 h:100 < 565 >
#define ICON1_JPEG 0

//icon2.jpeg w:100 h:100 < 565 >
#define ICON2_JPEG 20000

//icon3.jpeg w:100 h:100 < 565 >
#define ICON3_JPEG 40000

//icon4.jpeg w:100 h:100 < 565 >
#define ICON4_JPEG 60000

//icon5.jpeg w:100 h:100 < 565 >
#define ICON5_JPEG 80000

//icon6.jpeg w:100 h:100 < 565 >
#define ICON6_JPEG 100000

//icon7.jpeg w:100 h:100 < 565 >
#define ICON7_JPEG 120000

//icon8.jpeg w:100 h:100 < 565 >
#define ICON8_JPEG 140000

//icon9.jpeg w:100 h:100 < 565 >
#define ICON9_JPEG 160000

#define JETBRAINS_MONO_NL_REGULAR_9 (&dict_JetBrains_Mono_NL_Regular_9)
extern ldFontDict_t dict_JetBrains_Mono_NL_Regular_9;

#endif

#ifdef __cplusplus
}
#endif
#endif //_IMAGE_H_
