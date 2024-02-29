#ifndef _SRC_WIDGET_H_
#define _SRC_WIDGET_H_
#ifdef __cplusplus
extern "C" {
#endif

#include "stdint.h"
#include "ldCommon.h"

//arc_quarter.png w:51 h:51 < A8 >
#if USE_VIRTUAL_RESOURCE == 0
extern const uint8_t arc_quarter_png[];
#define ARC_QUARTER_PNG (uint32_t)&arc_quarter_png[0]
#else
#define ARC_QUARTER_PNG 0
#endif

//arc_quarter_mask.png w:51 h:51 < A8 >
#if USE_VIRTUAL_RESOURCE == 0
extern const uint8_t arc_quarter_mask_png[];
#define ARC_QUARTER_MASK_PNG (uint32_t)&arc_quarter_mask_png[0]
#else
#define ARC_QUARTER_MASK_PNG 2601
#endif

//book.png w:48 h:48 < 565+Mask >
#if USE_VIRTUAL_RESOURCE == 0
extern const uint8_t book_png[];
#define BOOK_PNG (uint32_t)&book_png[0]
#else
#define BOOK_PNG 5202
#endif

//chart.png w:48 h:48 < 565+Mask >
#if USE_VIRTUAL_RESOURCE == 0
extern const uint8_t chart_png[];
#define CHART_PNG (uint32_t)&chart_png[0]
#else
#define CHART_PNG 12114
#endif

//keyPress.png w:79 h:53 < 565+Mask >
#if USE_VIRTUAL_RESOURCE == 0
extern const uint8_t keyPress_png[];
#define KEYPRESS_PNG (uint32_t)&keyPress_png[0]
#else
#define KEYPRESS_PNG 19026
#endif

//keyRelease.png w:79 h:53 < 565+Mask >
#if USE_VIRTUAL_RESOURCE == 0
extern const uint8_t keyRelease_png[];
#define KEYRELEASE_PNG (uint32_t)&keyRelease_png[0]
#else
#define KEYRELEASE_PNG 31587
#endif

//letter_paper.bmp w:150 h:200 < 565 >
#if USE_VIRTUAL_RESOURCE == 0
extern const uint8_t letter_paper_bmp[];
#define LETTER_PAPER_BMP (uint32_t)&letter_paper_bmp[0]
#else
#define LETTER_PAPER_BMP 44148
#endif

//nature.bmp w:100 h:100 < 565 >
#if USE_VIRTUAL_RESOURCE == 0
extern const uint8_t nature_bmp[];
#define NATURE_BMP (uint32_t)&nature_bmp[0]
#else
#define NATURE_BMP 104148
#endif

//note.png w:48 h:48 < 565+Mask >
#if USE_VIRTUAL_RESOURCE == 0
extern const uint8_t note_png[];
#define NOTE_PNG (uint32_t)&note_png[0]
#else
#define NOTE_PNG 124148
#endif

//ProgressBarBg.bmp w:1 h:30 < 565 >
#if USE_VIRTUAL_RESOURCE == 0
extern const uint8_t ProgressBarBg_bmp[];
#define PROGRESSBARBG_BMP (uint32_t)&ProgressBarBg_bmp[0]
#else
#define PROGRESSBARBG_BMP 131060
#endif

//ProgressBarFg.bmp w:20 h:30 < 565 >
#if USE_VIRTUAL_RESOURCE == 0
extern const uint8_t ProgressBarFg_bmp[];
#define PROGRESSBARFG_BMP (uint32_t)&ProgressBarFg_bmp[0]
#else
#define PROGRESSBARFG_BMP 131120
#endif

//weather.png w:48 h:48 < 565+Mask >
#if USE_VIRTUAL_RESOURCE == 0
extern const uint8_t weather_png[];
#define WEATHER_PNG (uint32_t)&weather_png[0]
#else
#define WEATHER_PNG 132320
#endif

#define SIMSUN_REGULAR_12 (&dict_SimSun_Regular_12)
extern ldFontDict_t dict_SimSun_Regular_12;

#ifdef __cplusplus
}
#endif
#endif //_SRC_WIDGET_H_
