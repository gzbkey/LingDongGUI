#ifndef __UI_IMAGES_H__
#define __UI_IMAGES_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "arm_2d.h"
#include "ldConfig.h"

#if LD_CFG_COLOR_DEPTH == 8
extern const arm_2d_tile_t c_tile_arc_quarter_png_GRAY8;
#define IMAGE_ARC_QUARTER_PNG          (arm_2d_tile_t*)&c_tile_arc_quarter_png_GRAY8
#elif LD_CFG_COLOR_DEPTH == 16
extern const arm_2d_tile_t c_tile_arc_quarter_png_RGB565;
#define IMAGE_ARC_QUARTER_PNG          (arm_2d_tile_t*)&c_tile_arc_quarter_png_RGB565
#else
extern const arm_2d_tile_t c_tile_arc_quarter_png_CCCA8888;
#define IMAGE_ARC_QUARTER_PNG                 (arm_2d_tile_t*)&c_tile_arc_quarter_png_CCCA8888
extern const arm_2d_tile_t c_tile_arc_quarter_png_A1Mask;
#define IMAGE_ARC_QUARTER_PNG_A1Mask          (arm_2d_tile_t*)&c_tile_arc_quarter_png_A1Mask
extern const arm_2d_tile_t c_tile_arc_quarter_png_A2Mask;
#define IMAGE_ARC_QUARTER_PNG_A2Mask          (arm_2d_tile_t*)&c_tile_arc_quarter_png_A2Mask
extern const arm_2d_tile_t c_tile_arc_quarter_png_A4Mask;
#define IMAGE_ARC_QUARTER_PNG_A4Mask          (arm_2d_tile_t*)&c_tile_arc_quarter_png_A4Mask
extern const arm_2d_tile_t c_tile_arc_quarter_png_Mask;
#define IMAGE_ARC_QUARTER_PNG_Mask            (arm_2d_tile_t*)&c_tile_arc_quarter_png_Mask
#endif

#if LD_CFG_COLOR_DEPTH == 8
extern const arm_2d_tile_t c_tile_arc_quarter_mask_png_GRAY8;
#define IMAGE_ARC_QUARTER_MASK_PNG          (arm_2d_tile_t*)&c_tile_arc_quarter_mask_png_GRAY8
#elif LD_CFG_COLOR_DEPTH == 16
extern const arm_2d_tile_t c_tile_arc_quarter_mask_png_RGB565;
#define IMAGE_ARC_QUARTER_MASK_PNG          (arm_2d_tile_t*)&c_tile_arc_quarter_mask_png_RGB565
#else
extern const arm_2d_tile_t c_tile_arc_quarter_mask_png_CCCA8888;
#define IMAGE_ARC_QUARTER_MASK_PNG                 (arm_2d_tile_t*)&c_tile_arc_quarter_mask_png_CCCA8888
extern const arm_2d_tile_t c_tile_arc_quarter_mask_png_A1Mask;
#define IMAGE_ARC_QUARTER_MASK_PNG_A1Mask          (arm_2d_tile_t*)&c_tile_arc_quarter_mask_png_A1Mask
extern const arm_2d_tile_t c_tile_arc_quarter_mask_png_A2Mask;
#define IMAGE_ARC_QUARTER_MASK_PNG_A2Mask          (arm_2d_tile_t*)&c_tile_arc_quarter_mask_png_A2Mask
extern const arm_2d_tile_t c_tile_arc_quarter_mask_png_A4Mask;
#define IMAGE_ARC_QUARTER_MASK_PNG_A4Mask          (arm_2d_tile_t*)&c_tile_arc_quarter_mask_png_A4Mask
extern const arm_2d_tile_t c_tile_arc_quarter_mask_png_Mask;
#define IMAGE_ARC_QUARTER_MASK_PNG_Mask            (arm_2d_tile_t*)&c_tile_arc_quarter_mask_png_Mask
#endif

#if LD_CFG_COLOR_DEPTH == 8
extern const arm_2d_tile_t c_tile_book_png_GRAY8;
#define IMAGE_BOOK_PNG          (arm_2d_tile_t*)&c_tile_book_png_GRAY8
#elif LD_CFG_COLOR_DEPTH == 16
extern const arm_2d_tile_t c_tile_book_png_RGB565;
#define IMAGE_BOOK_PNG          (arm_2d_tile_t*)&c_tile_book_png_RGB565
#else
extern const arm_2d_tile_t c_tile_book_png_CCCA8888;
#define IMAGE_BOOK_PNG                 (arm_2d_tile_t*)&c_tile_book_png_CCCA8888
extern const arm_2d_tile_t c_tile_book_png_A1Mask;
#define IMAGE_BOOK_PNG_A1Mask          (arm_2d_tile_t*)&c_tile_book_png_A1Mask
extern const arm_2d_tile_t c_tile_book_png_A2Mask;
#define IMAGE_BOOK_PNG_A2Mask          (arm_2d_tile_t*)&c_tile_book_png_A2Mask
extern const arm_2d_tile_t c_tile_book_png_A4Mask;
#define IMAGE_BOOK_PNG_A4Mask          (arm_2d_tile_t*)&c_tile_book_png_A4Mask
extern const arm_2d_tile_t c_tile_book_png_Mask;
#define IMAGE_BOOK_PNG_Mask            (arm_2d_tile_t*)&c_tile_book_png_Mask
#endif

#if LD_CFG_COLOR_DEPTH == 8
extern const arm_2d_tile_t c_tile_chart_png_GRAY8;
#define IMAGE_CHART_PNG          (arm_2d_tile_t*)&c_tile_chart_png_GRAY8
#elif LD_CFG_COLOR_DEPTH == 16
extern const arm_2d_tile_t c_tile_chart_png_RGB565;
#define IMAGE_CHART_PNG          (arm_2d_tile_t*)&c_tile_chart_png_RGB565
#else
extern const arm_2d_tile_t c_tile_chart_png_CCCA8888;
#define IMAGE_CHART_PNG                 (arm_2d_tile_t*)&c_tile_chart_png_CCCA8888
extern const arm_2d_tile_t c_tile_chart_png_A1Mask;
#define IMAGE_CHART_PNG_A1Mask          (arm_2d_tile_t*)&c_tile_chart_png_A1Mask
extern const arm_2d_tile_t c_tile_chart_png_A2Mask;
#define IMAGE_CHART_PNG_A2Mask          (arm_2d_tile_t*)&c_tile_chart_png_A2Mask
extern const arm_2d_tile_t c_tile_chart_png_A4Mask;
#define IMAGE_CHART_PNG_A4Mask          (arm_2d_tile_t*)&c_tile_chart_png_A4Mask
extern const arm_2d_tile_t c_tile_chart_png_Mask;
#define IMAGE_CHART_PNG_Mask            (arm_2d_tile_t*)&c_tile_chart_png_Mask
#endif

#if LD_CFG_COLOR_DEPTH == 8
extern const arm_2d_tile_t c_tile_Gauge_png_GRAY8;
#define IMAGE_GAUGE_PNG          (arm_2d_tile_t*)&c_tile_Gauge_png_GRAY8
#elif LD_CFG_COLOR_DEPTH == 16
extern const arm_2d_tile_t c_tile_Gauge_png_RGB565;
#define IMAGE_GAUGE_PNG          (arm_2d_tile_t*)&c_tile_Gauge_png_RGB565
#else
extern const arm_2d_tile_t c_tile_Gauge_png_CCCA8888;
#define IMAGE_GAUGE_PNG                 (arm_2d_tile_t*)&c_tile_Gauge_png_CCCA8888
extern const arm_2d_tile_t c_tile_Gauge_png_A1Mask;
#define IMAGE_GAUGE_PNG_A1Mask          (arm_2d_tile_t*)&c_tile_Gauge_png_A1Mask
extern const arm_2d_tile_t c_tile_Gauge_png_A2Mask;
#define IMAGE_GAUGE_PNG_A2Mask          (arm_2d_tile_t*)&c_tile_Gauge_png_A2Mask
extern const arm_2d_tile_t c_tile_Gauge_png_A4Mask;
#define IMAGE_GAUGE_PNG_A4Mask          (arm_2d_tile_t*)&c_tile_Gauge_png_A4Mask
extern const arm_2d_tile_t c_tile_Gauge_png_Mask;
#define IMAGE_GAUGE_PNG_Mask            (arm_2d_tile_t*)&c_tile_Gauge_png_Mask
#endif

#if LD_CFG_COLOR_DEPTH == 8
extern const arm_2d_tile_t c_tile_gaugePointer_png_GRAY8;
#define IMAGE_GAUGEPOINTER_PNG          (arm_2d_tile_t*)&c_tile_gaugePointer_png_GRAY8
#elif LD_CFG_COLOR_DEPTH == 16
extern const arm_2d_tile_t c_tile_gaugePointer_png_RGB565;
#define IMAGE_GAUGEPOINTER_PNG          (arm_2d_tile_t*)&c_tile_gaugePointer_png_RGB565
#else
extern const arm_2d_tile_t c_tile_gaugePointer_png_CCCA8888;
#define IMAGE_GAUGEPOINTER_PNG                 (arm_2d_tile_t*)&c_tile_gaugePointer_png_CCCA8888
extern const arm_2d_tile_t c_tile_gaugePointer_png_A1Mask;
#define IMAGE_GAUGEPOINTER_PNG_A1Mask          (arm_2d_tile_t*)&c_tile_gaugePointer_png_A1Mask
extern const arm_2d_tile_t c_tile_gaugePointer_png_A2Mask;
#define IMAGE_GAUGEPOINTER_PNG_A2Mask          (arm_2d_tile_t*)&c_tile_gaugePointer_png_A2Mask
extern const arm_2d_tile_t c_tile_gaugePointer_png_A4Mask;
#define IMAGE_GAUGEPOINTER_PNG_A4Mask          (arm_2d_tile_t*)&c_tile_gaugePointer_png_A4Mask
extern const arm_2d_tile_t c_tile_gaugePointer_png_Mask;
#define IMAGE_GAUGEPOINTER_PNG_Mask            (arm_2d_tile_t*)&c_tile_gaugePointer_png_Mask
#endif

#if LD_CFG_COLOR_DEPTH == 8
extern const arm_2d_tile_t c_tile_keyPress_png_GRAY8;
#define IMAGE_KEYPRESS_PNG          (arm_2d_tile_t*)&c_tile_keyPress_png_GRAY8
#elif LD_CFG_COLOR_DEPTH == 16
extern const arm_2d_tile_t c_tile_keyPress_png_RGB565;
#define IMAGE_KEYPRESS_PNG          (arm_2d_tile_t*)&c_tile_keyPress_png_RGB565
#else
extern const arm_2d_tile_t c_tile_keyPress_png_CCCA8888;
#define IMAGE_KEYPRESS_PNG                 (arm_2d_tile_t*)&c_tile_keyPress_png_CCCA8888
extern const arm_2d_tile_t c_tile_keyPress_png_A1Mask;
#define IMAGE_KEYPRESS_PNG_A1Mask          (arm_2d_tile_t*)&c_tile_keyPress_png_A1Mask
extern const arm_2d_tile_t c_tile_keyPress_png_A2Mask;
#define IMAGE_KEYPRESS_PNG_A2Mask          (arm_2d_tile_t*)&c_tile_keyPress_png_A2Mask
extern const arm_2d_tile_t c_tile_keyPress_png_A4Mask;
#define IMAGE_KEYPRESS_PNG_A4Mask          (arm_2d_tile_t*)&c_tile_keyPress_png_A4Mask
extern const arm_2d_tile_t c_tile_keyPress_png_Mask;
#define IMAGE_KEYPRESS_PNG_Mask            (arm_2d_tile_t*)&c_tile_keyPress_png_Mask
#endif

#if LD_CFG_COLOR_DEPTH == 8
extern const arm_2d_tile_t c_tile_keyRelease_png_GRAY8;
#define IMAGE_KEYRELEASE_PNG          (arm_2d_tile_t*)&c_tile_keyRelease_png_GRAY8
#elif LD_CFG_COLOR_DEPTH == 16
extern const arm_2d_tile_t c_tile_keyRelease_png_RGB565;
#define IMAGE_KEYRELEASE_PNG          (arm_2d_tile_t*)&c_tile_keyRelease_png_RGB565
#else
extern const arm_2d_tile_t c_tile_keyRelease_png_CCCA8888;
#define IMAGE_KEYRELEASE_PNG                 (arm_2d_tile_t*)&c_tile_keyRelease_png_CCCA8888
extern const arm_2d_tile_t c_tile_keyRelease_png_A1Mask;
#define IMAGE_KEYRELEASE_PNG_A1Mask          (arm_2d_tile_t*)&c_tile_keyRelease_png_A1Mask
extern const arm_2d_tile_t c_tile_keyRelease_png_A2Mask;
#define IMAGE_KEYRELEASE_PNG_A2Mask          (arm_2d_tile_t*)&c_tile_keyRelease_png_A2Mask
extern const arm_2d_tile_t c_tile_keyRelease_png_A4Mask;
#define IMAGE_KEYRELEASE_PNG_A4Mask          (arm_2d_tile_t*)&c_tile_keyRelease_png_A4Mask
extern const arm_2d_tile_t c_tile_keyRelease_png_Mask;
#define IMAGE_KEYRELEASE_PNG_Mask            (arm_2d_tile_t*)&c_tile_keyRelease_png_Mask
#endif

#if LD_CFG_COLOR_DEPTH == 8
extern const arm_2d_tile_t c_tile_letter_paper_bmp_GRAY8;
#define IMAGE_LETTER_PAPER_BMP          (arm_2d_tile_t*)&c_tile_letter_paper_bmp_GRAY8
#elif LD_CFG_COLOR_DEPTH == 16
extern const arm_2d_tile_t c_tile_letter_paper_bmp_RGB565;
#define IMAGE_LETTER_PAPER_BMP          (arm_2d_tile_t*)&c_tile_letter_paper_bmp_RGB565
#else
extern const arm_2d_tile_t c_tile_letter_paper_bmp_CCCN888;
#define IMAGE_LETTER_PAPER_BMP          &c_tile_letter_paper_bmp_CCCN888
#endif

#if LD_CFG_COLOR_DEPTH == 8
extern const arm_2d_tile_t c_tile_nature_bmp_GRAY8;
#define IMAGE_NATURE_BMP          (arm_2d_tile_t*)&c_tile_nature_bmp_GRAY8
#elif LD_CFG_COLOR_DEPTH == 16
extern const arm_2d_tile_t c_tile_nature_bmp_RGB565;
#define IMAGE_NATURE_BMP          (arm_2d_tile_t*)&c_tile_nature_bmp_RGB565
#else
extern const arm_2d_tile_t c_tile_nature_bmp_CCCN888;
#define IMAGE_NATURE_BMP          &c_tile_nature_bmp_CCCN888
#endif

#if LD_CFG_COLOR_DEPTH == 8
extern const arm_2d_tile_t c_tile_note_png_GRAY8;
#define IMAGE_NOTE_PNG          (arm_2d_tile_t*)&c_tile_note_png_GRAY8
#elif LD_CFG_COLOR_DEPTH == 16
extern const arm_2d_tile_t c_tile_note_png_RGB565;
#define IMAGE_NOTE_PNG          (arm_2d_tile_t*)&c_tile_note_png_RGB565
#else
extern const arm_2d_tile_t c_tile_note_png_CCCA8888;
#define IMAGE_NOTE_PNG                 (arm_2d_tile_t*)&c_tile_note_png_CCCA8888
extern const arm_2d_tile_t c_tile_note_png_A1Mask;
#define IMAGE_NOTE_PNG_A1Mask          (arm_2d_tile_t*)&c_tile_note_png_A1Mask
extern const arm_2d_tile_t c_tile_note_png_A2Mask;
#define IMAGE_NOTE_PNG_A2Mask          (arm_2d_tile_t*)&c_tile_note_png_A2Mask
extern const arm_2d_tile_t c_tile_note_png_A4Mask;
#define IMAGE_NOTE_PNG_A4Mask          (arm_2d_tile_t*)&c_tile_note_png_A4Mask
extern const arm_2d_tile_t c_tile_note_png_Mask;
#define IMAGE_NOTE_PNG_Mask            (arm_2d_tile_t*)&c_tile_note_png_Mask
#endif

#if LD_CFG_COLOR_DEPTH == 8
extern const arm_2d_tile_t c_tile_ProgressBarBg_bmp_GRAY8;
#define IMAGE_PROGRESSBARBG_BMP          (arm_2d_tile_t*)&c_tile_ProgressBarBg_bmp_GRAY8
#elif LD_CFG_COLOR_DEPTH == 16
extern const arm_2d_tile_t c_tile_ProgressBarBg_bmp_RGB565;
#define IMAGE_PROGRESSBARBG_BMP          (arm_2d_tile_t*)&c_tile_ProgressBarBg_bmp_RGB565
#else
extern const arm_2d_tile_t c_tile_ProgressBarBg_bmp_CCCN888;
#define IMAGE_PROGRESSBARBG_BMP          &c_tile_ProgressBarBg_bmp_CCCN888
#endif

#if LD_CFG_COLOR_DEPTH == 8
extern const arm_2d_tile_t c_tile_ProgressBarFg_bmp_GRAY8;
#define IMAGE_PROGRESSBARFG_BMP          (arm_2d_tile_t*)&c_tile_ProgressBarFg_bmp_GRAY8
#elif LD_CFG_COLOR_DEPTH == 16
extern const arm_2d_tile_t c_tile_ProgressBarFg_bmp_RGB565;
#define IMAGE_PROGRESSBARFG_BMP          (arm_2d_tile_t*)&c_tile_ProgressBarFg_bmp_RGB565
#else
extern const arm_2d_tile_t c_tile_ProgressBarFg_bmp_CCCN888;
#define IMAGE_PROGRESSBARFG_BMP          &c_tile_ProgressBarFg_bmp_CCCN888
#endif

#if LD_CFG_COLOR_DEPTH == 8
extern const arm_2d_tile_t c_tile_weather_png_GRAY8;
#define IMAGE_WEATHER_PNG          (arm_2d_tile_t*)&c_tile_weather_png_GRAY8
#elif LD_CFG_COLOR_DEPTH == 16
extern const arm_2d_tile_t c_tile_weather_png_RGB565;
#define IMAGE_WEATHER_PNG          (arm_2d_tile_t*)&c_tile_weather_png_RGB565
#else
extern const arm_2d_tile_t c_tile_weather_png_CCCA8888;
#define IMAGE_WEATHER_PNG                 (arm_2d_tile_t*)&c_tile_weather_png_CCCA8888
extern const arm_2d_tile_t c_tile_weather_png_A1Mask;
#define IMAGE_WEATHER_PNG_A1Mask          (arm_2d_tile_t*)&c_tile_weather_png_A1Mask
extern const arm_2d_tile_t c_tile_weather_png_A2Mask;
#define IMAGE_WEATHER_PNG_A2Mask          (arm_2d_tile_t*)&c_tile_weather_png_A2Mask
extern const arm_2d_tile_t c_tile_weather_png_A4Mask;
#define IMAGE_WEATHER_PNG_A4Mask          (arm_2d_tile_t*)&c_tile_weather_png_A4Mask
extern const arm_2d_tile_t c_tile_weather_png_Mask;
#define IMAGE_WEATHER_PNG_Mask            (arm_2d_tile_t*)&c_tile_weather_png_Mask
#endif

#ifdef __cplusplus
}
#endif

#endif
