#ifndef __UI_IMAGES_H__
#define __UI_IMAGES_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "arm_2d.h"
#include "ldConfig.h"

// arc_quarter.png < 51x51 >
#if LD_CFG_COLOR_DEPTH == 8
extern const arm_2d_tile_t c_tile_arc_quarter_png_GRAY8;
#define IMAGE_ARC_QUARTER_PNG          (arm_2d_tile_t*)&c_tile_arc_quarter_png_GRAY8
#elif LD_CFG_COLOR_DEPTH == 16
extern const arm_2d_tile_t c_tile_arc_quarter_png_RGB565;
#define IMAGE_ARC_QUARTER_PNG          (arm_2d_tile_t*)&c_tile_arc_quarter_png_RGB565
#else
extern const arm_2d_tile_t c_tile_arc_quarter_png_CCCA8888;
#define IMAGE_ARC_QUARTER_PNG          (arm_2d_tile_t*)&c_tile_arc_quarter_png_CCCA8888
#endif
extern const arm_2d_tile_t c_tile_arc_quarter_png_A1Mask;
#define IMAGE_ARC_QUARTER_PNG_A1Mask   (arm_2d_tile_t*)&c_tile_arc_quarter_png_A1Mask
extern const arm_2d_tile_t c_tile_arc_quarter_png_A2Mask;
#define IMAGE_ARC_QUARTER_PNG_A2Mask   (arm_2d_tile_t*)&c_tile_arc_quarter_png_A2Mask
extern const arm_2d_tile_t c_tile_arc_quarter_png_A4Mask;
#define IMAGE_ARC_QUARTER_PNG_A4Mask   (arm_2d_tile_t*)&c_tile_arc_quarter_png_A4Mask
extern const arm_2d_tile_t c_tile_arc_quarter_png_Mask;
#define IMAGE_ARC_QUARTER_PNG_Mask     (arm_2d_tile_t*)&c_tile_arc_quarter_png_Mask

// arc_quarter_mask.png < 51x51 >
#if LD_CFG_COLOR_DEPTH == 8
extern const arm_2d_tile_t c_tile_arc_quarter_mask_png_GRAY8;
#define IMAGE_ARC_QUARTER_MASK_PNG          (arm_2d_tile_t*)&c_tile_arc_quarter_mask_png_GRAY8
#elif LD_CFG_COLOR_DEPTH == 16
extern const arm_2d_tile_t c_tile_arc_quarter_mask_png_RGB565;
#define IMAGE_ARC_QUARTER_MASK_PNG          (arm_2d_tile_t*)&c_tile_arc_quarter_mask_png_RGB565
#else
extern const arm_2d_tile_t c_tile_arc_quarter_mask_png_CCCA8888;
#define IMAGE_ARC_QUARTER_MASK_PNG          (arm_2d_tile_t*)&c_tile_arc_quarter_mask_png_CCCA8888
#endif
extern const arm_2d_tile_t c_tile_arc_quarter_mask_png_A1Mask;
#define IMAGE_ARC_QUARTER_MASK_PNG_A1Mask   (arm_2d_tile_t*)&c_tile_arc_quarter_mask_png_A1Mask
extern const arm_2d_tile_t c_tile_arc_quarter_mask_png_A2Mask;
#define IMAGE_ARC_QUARTER_MASK_PNG_A2Mask   (arm_2d_tile_t*)&c_tile_arc_quarter_mask_png_A2Mask
extern const arm_2d_tile_t c_tile_arc_quarter_mask_png_A4Mask;
#define IMAGE_ARC_QUARTER_MASK_PNG_A4Mask   (arm_2d_tile_t*)&c_tile_arc_quarter_mask_png_A4Mask
extern const arm_2d_tile_t c_tile_arc_quarter_mask_png_Mask;
#define IMAGE_ARC_QUARTER_MASK_PNG_Mask     (arm_2d_tile_t*)&c_tile_arc_quarter_mask_png_Mask

// book.png < 48x48 >
#if LD_CFG_COLOR_DEPTH == 8
extern const arm_2d_tile_t c_tile_book_png_GRAY8;
#define IMAGE_BOOK_PNG          (arm_2d_tile_t*)&c_tile_book_png_GRAY8
#elif LD_CFG_COLOR_DEPTH == 16
extern const arm_2d_tile_t c_tile_book_png_RGB565;
#define IMAGE_BOOK_PNG          (arm_2d_tile_t*)&c_tile_book_png_RGB565
#else
extern const arm_2d_tile_t c_tile_book_png_CCCA8888;
#define IMAGE_BOOK_PNG          (arm_2d_tile_t*)&c_tile_book_png_CCCA8888
#endif
extern const arm_2d_tile_t c_tile_book_png_A1Mask;
#define IMAGE_BOOK_PNG_A1Mask   (arm_2d_tile_t*)&c_tile_book_png_A1Mask
extern const arm_2d_tile_t c_tile_book_png_A2Mask;
#define IMAGE_BOOK_PNG_A2Mask   (arm_2d_tile_t*)&c_tile_book_png_A2Mask
extern const arm_2d_tile_t c_tile_book_png_A4Mask;
#define IMAGE_BOOK_PNG_A4Mask   (arm_2d_tile_t*)&c_tile_book_png_A4Mask
extern const arm_2d_tile_t c_tile_book_png_Mask;
#define IMAGE_BOOK_PNG_Mask     (arm_2d_tile_t*)&c_tile_book_png_Mask

// chart.png < 48x48 >
#if LD_CFG_COLOR_DEPTH == 8
extern const arm_2d_tile_t c_tile_chart_png_GRAY8;
#define IMAGE_CHART_PNG          (arm_2d_tile_t*)&c_tile_chart_png_GRAY8
#elif LD_CFG_COLOR_DEPTH == 16
extern const arm_2d_tile_t c_tile_chart_png_RGB565;
#define IMAGE_CHART_PNG          (arm_2d_tile_t*)&c_tile_chart_png_RGB565
#else
extern const arm_2d_tile_t c_tile_chart_png_CCCA8888;
#define IMAGE_CHART_PNG          (arm_2d_tile_t*)&c_tile_chart_png_CCCA8888
#endif
extern const arm_2d_tile_t c_tile_chart_png_A1Mask;
#define IMAGE_CHART_PNG_A1Mask   (arm_2d_tile_t*)&c_tile_chart_png_A1Mask
extern const arm_2d_tile_t c_tile_chart_png_A2Mask;
#define IMAGE_CHART_PNG_A2Mask   (arm_2d_tile_t*)&c_tile_chart_png_A2Mask
extern const arm_2d_tile_t c_tile_chart_png_A4Mask;
#define IMAGE_CHART_PNG_A4Mask   (arm_2d_tile_t*)&c_tile_chart_png_A4Mask
extern const arm_2d_tile_t c_tile_chart_png_Mask;
#define IMAGE_CHART_PNG_Mask     (arm_2d_tile_t*)&c_tile_chart_png_Mask

// Gauge.png < 120x98 >
#if LD_CFG_COLOR_DEPTH == 8
extern const arm_2d_tile_t c_tile_Gauge_png_GRAY8;
#define IMAGE_GAUGE_PNG          (arm_2d_tile_t*)&c_tile_Gauge_png_GRAY8
#elif LD_CFG_COLOR_DEPTH == 16
extern const arm_2d_tile_t c_tile_Gauge_png_RGB565;
#define IMAGE_GAUGE_PNG          (arm_2d_tile_t*)&c_tile_Gauge_png_RGB565
#else
extern const arm_2d_tile_t c_tile_Gauge_png_CCCA8888;
#define IMAGE_GAUGE_PNG          (arm_2d_tile_t*)&c_tile_Gauge_png_CCCA8888
#endif
extern const arm_2d_tile_t c_tile_Gauge_png_A1Mask;
#define IMAGE_GAUGE_PNG_A1Mask   (arm_2d_tile_t*)&c_tile_Gauge_png_A1Mask
extern const arm_2d_tile_t c_tile_Gauge_png_A2Mask;
#define IMAGE_GAUGE_PNG_A2Mask   (arm_2d_tile_t*)&c_tile_Gauge_png_A2Mask
extern const arm_2d_tile_t c_tile_Gauge_png_A4Mask;
#define IMAGE_GAUGE_PNG_A4Mask   (arm_2d_tile_t*)&c_tile_Gauge_png_A4Mask
extern const arm_2d_tile_t c_tile_Gauge_png_Mask;
#define IMAGE_GAUGE_PNG_Mask     (arm_2d_tile_t*)&c_tile_Gauge_png_Mask

// gaugePointer.png < 11x60 >
#if LD_CFG_COLOR_DEPTH == 8
extern const arm_2d_tile_t c_tile_gaugePointer_png_GRAY8;
#define IMAGE_GAUGEPOINTER_PNG          (arm_2d_tile_t*)&c_tile_gaugePointer_png_GRAY8
#elif LD_CFG_COLOR_DEPTH == 16
extern const arm_2d_tile_t c_tile_gaugePointer_png_RGB565;
#define IMAGE_GAUGEPOINTER_PNG          (arm_2d_tile_t*)&c_tile_gaugePointer_png_RGB565
#else
extern const arm_2d_tile_t c_tile_gaugePointer_png_CCCA8888;
#define IMAGE_GAUGEPOINTER_PNG          (arm_2d_tile_t*)&c_tile_gaugePointer_png_CCCA8888
#endif
extern const arm_2d_tile_t c_tile_gaugePointer_png_A1Mask;
#define IMAGE_GAUGEPOINTER_PNG_A1Mask   (arm_2d_tile_t*)&c_tile_gaugePointer_png_A1Mask
extern const arm_2d_tile_t c_tile_gaugePointer_png_A2Mask;
#define IMAGE_GAUGEPOINTER_PNG_A2Mask   (arm_2d_tile_t*)&c_tile_gaugePointer_png_A2Mask
extern const arm_2d_tile_t c_tile_gaugePointer_png_A4Mask;
#define IMAGE_GAUGEPOINTER_PNG_A4Mask   (arm_2d_tile_t*)&c_tile_gaugePointer_png_A4Mask
extern const arm_2d_tile_t c_tile_gaugePointer_png_Mask;
#define IMAGE_GAUGEPOINTER_PNG_Mask     (arm_2d_tile_t*)&c_tile_gaugePointer_png_Mask

// indicator.png < 24x34 >
#if LD_CFG_COLOR_DEPTH == 8
extern const arm_2d_tile_t c_tile_indicator_png_GRAY8;
#define IMAGE_INDICATOR_PNG          (arm_2d_tile_t*)&c_tile_indicator_png_GRAY8
#elif LD_CFG_COLOR_DEPTH == 16
extern const arm_2d_tile_t c_tile_indicator_png_RGB565;
#define IMAGE_INDICATOR_PNG          (arm_2d_tile_t*)&c_tile_indicator_png_RGB565
#else
extern const arm_2d_tile_t c_tile_indicator_png_CCCA8888;
#define IMAGE_INDICATOR_PNG          (arm_2d_tile_t*)&c_tile_indicator_png_CCCA8888
#endif
extern const arm_2d_tile_t c_tile_indicator_png_A1Mask;
#define IMAGE_INDICATOR_PNG_A1Mask   (arm_2d_tile_t*)&c_tile_indicator_png_A1Mask
extern const arm_2d_tile_t c_tile_indicator_png_A2Mask;
#define IMAGE_INDICATOR_PNG_A2Mask   (arm_2d_tile_t*)&c_tile_indicator_png_A2Mask
extern const arm_2d_tile_t c_tile_indicator_png_A4Mask;
#define IMAGE_INDICATOR_PNG_A4Mask   (arm_2d_tile_t*)&c_tile_indicator_png_A4Mask
extern const arm_2d_tile_t c_tile_indicator_png_Mask;
#define IMAGE_INDICATOR_PNG_Mask     (arm_2d_tile_t*)&c_tile_indicator_png_Mask

// keyPress.png < 79x53 >
#if LD_CFG_COLOR_DEPTH == 8
extern const arm_2d_tile_t c_tile_keyPress_png_GRAY8;
#define IMAGE_KEYPRESS_PNG          (arm_2d_tile_t*)&c_tile_keyPress_png_GRAY8
#elif LD_CFG_COLOR_DEPTH == 16
extern const arm_2d_tile_t c_tile_keyPress_png_RGB565;
#define IMAGE_KEYPRESS_PNG          (arm_2d_tile_t*)&c_tile_keyPress_png_RGB565
#else
extern const arm_2d_tile_t c_tile_keyPress_png_CCCA8888;
#define IMAGE_KEYPRESS_PNG          (arm_2d_tile_t*)&c_tile_keyPress_png_CCCA8888
#endif
extern const arm_2d_tile_t c_tile_keyPress_png_A1Mask;
#define IMAGE_KEYPRESS_PNG_A1Mask   (arm_2d_tile_t*)&c_tile_keyPress_png_A1Mask
extern const arm_2d_tile_t c_tile_keyPress_png_A2Mask;
#define IMAGE_KEYPRESS_PNG_A2Mask   (arm_2d_tile_t*)&c_tile_keyPress_png_A2Mask
extern const arm_2d_tile_t c_tile_keyPress_png_A4Mask;
#define IMAGE_KEYPRESS_PNG_A4Mask   (arm_2d_tile_t*)&c_tile_keyPress_png_A4Mask
extern const arm_2d_tile_t c_tile_keyPress_png_Mask;
#define IMAGE_KEYPRESS_PNG_Mask     (arm_2d_tile_t*)&c_tile_keyPress_png_Mask

// keyRelease.png < 79x53 >
#if LD_CFG_COLOR_DEPTH == 8
extern const arm_2d_tile_t c_tile_keyRelease_png_GRAY8;
#define IMAGE_KEYRELEASE_PNG          (arm_2d_tile_t*)&c_tile_keyRelease_png_GRAY8
#elif LD_CFG_COLOR_DEPTH == 16
extern const arm_2d_tile_t c_tile_keyRelease_png_RGB565;
#define IMAGE_KEYRELEASE_PNG          (arm_2d_tile_t*)&c_tile_keyRelease_png_RGB565
#else
extern const arm_2d_tile_t c_tile_keyRelease_png_CCCA8888;
#define IMAGE_KEYRELEASE_PNG          (arm_2d_tile_t*)&c_tile_keyRelease_png_CCCA8888
#endif
extern const arm_2d_tile_t c_tile_keyRelease_png_A1Mask;
#define IMAGE_KEYRELEASE_PNG_A1Mask   (arm_2d_tile_t*)&c_tile_keyRelease_png_A1Mask
extern const arm_2d_tile_t c_tile_keyRelease_png_A2Mask;
#define IMAGE_KEYRELEASE_PNG_A2Mask   (arm_2d_tile_t*)&c_tile_keyRelease_png_A2Mask
extern const arm_2d_tile_t c_tile_keyRelease_png_A4Mask;
#define IMAGE_KEYRELEASE_PNG_A4Mask   (arm_2d_tile_t*)&c_tile_keyRelease_png_A4Mask
extern const arm_2d_tile_t c_tile_keyRelease_png_Mask;
#define IMAGE_KEYRELEASE_PNG_Mask     (arm_2d_tile_t*)&c_tile_keyRelease_png_Mask

// letter_paper.bmp < 150x200 >
#if LD_CFG_COLOR_DEPTH == 8
extern const arm_2d_tile_t c_tile_letter_paper_bmp_GRAY8;
#define IMAGE_LETTER_PAPER_BMP          (arm_2d_tile_t*)&c_tile_letter_paper_bmp_GRAY8
#elif LD_CFG_COLOR_DEPTH == 16
extern const arm_2d_tile_t c_tile_letter_paper_bmp_RGB565;
#define IMAGE_LETTER_PAPER_BMP          (arm_2d_tile_t*)&c_tile_letter_paper_bmp_RGB565
#else
extern const arm_2d_tile_t c_tile_letter_paper_bmp_CCCN888;
#define IMAGE_LETTER_PAPER_BMP          (arm_2d_tile_t*)&c_tile_letter_paper_bmp_CCCN888
#endif

// nature.bmp < 100x100 >
#if LD_CFG_COLOR_DEPTH == 8
extern const arm_2d_tile_t c_tile_nature_bmp_GRAY8;
#define IMAGE_NATURE_BMP          (arm_2d_tile_t*)&c_tile_nature_bmp_GRAY8
#elif LD_CFG_COLOR_DEPTH == 16
extern const arm_2d_tile_t c_tile_nature_bmp_RGB565;
#define IMAGE_NATURE_BMP          (arm_2d_tile_t*)&c_tile_nature_bmp_RGB565
#else
extern const arm_2d_tile_t c_tile_nature_bmp_CCCN888;
#define IMAGE_NATURE_BMP          (arm_2d_tile_t*)&c_tile_nature_bmp_CCCN888
#endif

// note.png < 48x48 >
#if LD_CFG_COLOR_DEPTH == 8
extern const arm_2d_tile_t c_tile_note_png_GRAY8;
#define IMAGE_NOTE_PNG          (arm_2d_tile_t*)&c_tile_note_png_GRAY8
#elif LD_CFG_COLOR_DEPTH == 16
extern const arm_2d_tile_t c_tile_note_png_RGB565;
#define IMAGE_NOTE_PNG          (arm_2d_tile_t*)&c_tile_note_png_RGB565
#else
extern const arm_2d_tile_t c_tile_note_png_CCCA8888;
#define IMAGE_NOTE_PNG          (arm_2d_tile_t*)&c_tile_note_png_CCCA8888
#endif
extern const arm_2d_tile_t c_tile_note_png_A1Mask;
#define IMAGE_NOTE_PNG_A1Mask   (arm_2d_tile_t*)&c_tile_note_png_A1Mask
extern const arm_2d_tile_t c_tile_note_png_A2Mask;
#define IMAGE_NOTE_PNG_A2Mask   (arm_2d_tile_t*)&c_tile_note_png_A2Mask
extern const arm_2d_tile_t c_tile_note_png_A4Mask;
#define IMAGE_NOTE_PNG_A4Mask   (arm_2d_tile_t*)&c_tile_note_png_A4Mask
extern const arm_2d_tile_t c_tile_note_png_Mask;
#define IMAGE_NOTE_PNG_Mask     (arm_2d_tile_t*)&c_tile_note_png_Mask

// ProgressBarBg.bmp < 1x30 >
#if LD_CFG_COLOR_DEPTH == 8
extern const arm_2d_tile_t c_tile_ProgressBarBg_bmp_GRAY8;
#define IMAGE_PROGRESSBARBG_BMP          (arm_2d_tile_t*)&c_tile_ProgressBarBg_bmp_GRAY8
#elif LD_CFG_COLOR_DEPTH == 16
extern const arm_2d_tile_t c_tile_ProgressBarBg_bmp_RGB565;
#define IMAGE_PROGRESSBARBG_BMP          (arm_2d_tile_t*)&c_tile_ProgressBarBg_bmp_RGB565
#else
extern const arm_2d_tile_t c_tile_ProgressBarBg_bmp_CCCN888;
#define IMAGE_PROGRESSBARBG_BMP          (arm_2d_tile_t*)&c_tile_ProgressBarBg_bmp_CCCN888
#endif

// ProgressBarFg.bmp < 20x30 >
#if LD_CFG_COLOR_DEPTH == 8
extern const arm_2d_tile_t c_tile_ProgressBarFg_bmp_GRAY8;
#define IMAGE_PROGRESSBARFG_BMP          (arm_2d_tile_t*)&c_tile_ProgressBarFg_bmp_GRAY8
#elif LD_CFG_COLOR_DEPTH == 16
extern const arm_2d_tile_t c_tile_ProgressBarFg_bmp_RGB565;
#define IMAGE_PROGRESSBARFG_BMP          (arm_2d_tile_t*)&c_tile_ProgressBarFg_bmp_RGB565
#else
extern const arm_2d_tile_t c_tile_ProgressBarFg_bmp_CCCN888;
#define IMAGE_PROGRESSBARFG_BMP          (arm_2d_tile_t*)&c_tile_ProgressBarFg_bmp_CCCN888
#endif

// slider.png < 317x23 >
#if LD_CFG_COLOR_DEPTH == 8
extern const arm_2d_tile_t c_tile_slider_png_GRAY8;
#define IMAGE_SLIDER_PNG          (arm_2d_tile_t*)&c_tile_slider_png_GRAY8
#elif LD_CFG_COLOR_DEPTH == 16
extern const arm_2d_tile_t c_tile_slider_png_RGB565;
#define IMAGE_SLIDER_PNG          (arm_2d_tile_t*)&c_tile_slider_png_RGB565
#else
extern const arm_2d_tile_t c_tile_slider_png_CCCA8888;
#define IMAGE_SLIDER_PNG          (arm_2d_tile_t*)&c_tile_slider_png_CCCA8888
#endif
extern const arm_2d_tile_t c_tile_slider_png_A1Mask;
#define IMAGE_SLIDER_PNG_A1Mask   (arm_2d_tile_t*)&c_tile_slider_png_A1Mask
extern const arm_2d_tile_t c_tile_slider_png_A2Mask;
#define IMAGE_SLIDER_PNG_A2Mask   (arm_2d_tile_t*)&c_tile_slider_png_A2Mask
extern const arm_2d_tile_t c_tile_slider_png_A4Mask;
#define IMAGE_SLIDER_PNG_A4Mask   (arm_2d_tile_t*)&c_tile_slider_png_A4Mask
extern const arm_2d_tile_t c_tile_slider_png_Mask;
#define IMAGE_SLIDER_PNG_Mask     (arm_2d_tile_t*)&c_tile_slider_png_Mask

// weather.png < 48x48 >
#if LD_CFG_COLOR_DEPTH == 8
extern const arm_2d_tile_t c_tile_weather_png_GRAY8;
#define IMAGE_WEATHER_PNG          (arm_2d_tile_t*)&c_tile_weather_png_GRAY8
#elif LD_CFG_COLOR_DEPTH == 16
extern const arm_2d_tile_t c_tile_weather_png_RGB565;
#define IMAGE_WEATHER_PNG          (arm_2d_tile_t*)&c_tile_weather_png_RGB565
#else
extern const arm_2d_tile_t c_tile_weather_png_CCCA8888;
#define IMAGE_WEATHER_PNG          (arm_2d_tile_t*)&c_tile_weather_png_CCCA8888
#endif
extern const arm_2d_tile_t c_tile_weather_png_A1Mask;
#define IMAGE_WEATHER_PNG_A1Mask   (arm_2d_tile_t*)&c_tile_weather_png_A1Mask
extern const arm_2d_tile_t c_tile_weather_png_A2Mask;
#define IMAGE_WEATHER_PNG_A2Mask   (arm_2d_tile_t*)&c_tile_weather_png_A2Mask
extern const arm_2d_tile_t c_tile_weather_png_A4Mask;
#define IMAGE_WEATHER_PNG_A4Mask   (arm_2d_tile_t*)&c_tile_weather_png_A4Mask
extern const arm_2d_tile_t c_tile_weather_png_Mask;
#define IMAGE_WEATHER_PNG_Mask     (arm_2d_tile_t*)&c_tile_weather_png_Mask

#ifdef __cplusplus
}
#endif

#endif
