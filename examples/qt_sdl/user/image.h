#ifndef _IMAGE_H_
#define _IMAGE_H_
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

//pos1.png w:100 h:30 < A8 >
#if USE_VIRTUAL_RESOURCE == 0
extern const uint8_t pos1_png[];
#define POS1_PNG (uint32_t)&pos1_png[0]
#else
#define POS1_PNG 900
#endif

//pos2.png w:100 h:30 < A8 >
#if USE_VIRTUAL_RESOURCE == 0
extern const uint8_t pos2_png[];
#define POS2_PNG (uint32_t)&pos2_png[0]
#else
#define POS2_PNG 3900
#endif

//pos3.png w:100 h:30 < A8 >
#if USE_VIRTUAL_RESOURCE == 0
extern const uint8_t pos3_png[];
#define POS3_PNG (uint32_t)&pos3_png[0]
#else
#define POS3_PNG 6900
#endif

//wallpaper.jpg w:240 h:240 < 565 >
#if USE_VIRTUAL_RESOURCE == 0
extern const uint8_t wallpaper_jpg[];
#define WALLPAPER_JPG (uint32_t)&wallpaper_jpg[0]
#else
#define WALLPAPER_JPG 9900
#endif

#define HARMONYOS_SANS_SC_MEDIUM_28 (&dict_HarmonyOS_Sans_SC_Medium_28)
extern ldFontDict_t dict_HarmonyOS_Sans_SC_Medium_28;

#ifdef __cplusplus
}
#endif
#endif //_IMAGE_H_
