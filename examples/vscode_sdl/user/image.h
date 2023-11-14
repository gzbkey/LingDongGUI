#ifndef _IMAGE_H_
#define _IMAGE_H_
#ifdef __cplusplus
extern "C" {
#endif

#include "stdint.h"
#include "ldCommon.h"

//icon.bmp w:52 h:52 < 565 >
#if USE_VIRTUAL_RESOURCE == 0
extern const uint8_t icon_bmp[];
#define ICON_BMP (uint32_t)&icon_bmp[0]
#else
#define ICON_BMP 0
#endif

#define WENQUANYI_ZEN_HEI_REGULAR_36 (&dict_WenQuanYi_Zen_Hei_Regular_36)
extern ldFontDict_t dict_WenQuanYi_Zen_Hei_Regular_36;

#ifdef __cplusplus
}
#endif
#endif //_IMAGE_H_
