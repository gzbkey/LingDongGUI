#ifndef __UI_IMAGES_H__
#define __UI_IMAGES_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "arm_2d.h"
#include "ldConfig.h"

// abs.bmp < 30x30 >
#if LD_CFG_COLOR_DEPTH == 8
extern const arm_2d_tile_t c_tile_abs_bmp_GRAY8;
#define IMAGE_ABS_BMP          (arm_2d_tile_t*)&c_tile_abs_bmp_GRAY8
#elif LD_CFG_COLOR_DEPTH == 16
extern const arm_2d_tile_t c_tile_abs_bmp_RGB565;
#define IMAGE_ABS_BMP          (arm_2d_tile_t*)&c_tile_abs_bmp_RGB565
#else
extern const arm_2d_tile_t c_tile_abs_bmp_CCCN888;
#define IMAGE_ABS_BMP          (arm_2d_tile_t*)&c_tile_abs_bmp_CCCN888
#endif

// alarmClock.png < 200x180 >
#if LD_CFG_COLOR_DEPTH == 8
extern const arm_2d_tile_t c_tile_alarmClock_png_GRAY8;
#define IMAGE_ALARMCLOCK_PNG          (arm_2d_tile_t*)&c_tile_alarmClock_png_GRAY8
#elif LD_CFG_COLOR_DEPTH == 16
extern const arm_2d_tile_t c_tile_alarmClock_png_RGB565;
#define IMAGE_ALARMCLOCK_PNG          (arm_2d_tile_t*)&c_tile_alarmClock_png_RGB565
#else
extern const arm_2d_tile_t c_tile_alarmClock_png_CCCA8888;
#define IMAGE_ALARMCLOCK_PNG          (arm_2d_tile_t*)&c_tile_alarmClock_png_CCCA8888
#endif
extern const arm_2d_tile_t c_tile_alarmClock_png_A1Mask;
#define IMAGE_ALARMCLOCK_PNG_A1Mask   (arm_2d_tile_t*)&c_tile_alarmClock_png_A1Mask
extern const arm_2d_tile_t c_tile_alarmClock_png_A2Mask;
#define IMAGE_ALARMCLOCK_PNG_A2Mask   (arm_2d_tile_t*)&c_tile_alarmClock_png_A2Mask
extern const arm_2d_tile_t c_tile_alarmClock_png_A4Mask;
#define IMAGE_ALARMCLOCK_PNG_A4Mask   (arm_2d_tile_t*)&c_tile_alarmClock_png_A4Mask
extern const arm_2d_tile_t c_tile_alarmClock_png_Mask;
#define IMAGE_ALARMCLOCK_PNG_Mask     (arm_2d_tile_t*)&c_tile_alarmClock_png_Mask

// alarmClock_s.png < 176x160 >
#if LD_CFG_COLOR_DEPTH == 8
extern const arm_2d_tile_t c_tile_alarmClock_s_png_GRAY8;
#define IMAGE_ALARMCLOCK_S_PNG          (arm_2d_tile_t*)&c_tile_alarmClock_s_png_GRAY8
#elif LD_CFG_COLOR_DEPTH == 16
extern const arm_2d_tile_t c_tile_alarmClock_s_png_RGB565;
#define IMAGE_ALARMCLOCK_S_PNG          (arm_2d_tile_t*)&c_tile_alarmClock_s_png_RGB565
#else
extern const arm_2d_tile_t c_tile_alarmClock_s_png_CCCA8888;
#define IMAGE_ALARMCLOCK_S_PNG          (arm_2d_tile_t*)&c_tile_alarmClock_s_png_CCCA8888
#endif
extern const arm_2d_tile_t c_tile_alarmClock_s_png_A1Mask;
#define IMAGE_ALARMCLOCK_S_PNG_A1Mask   (arm_2d_tile_t*)&c_tile_alarmClock_s_png_A1Mask
extern const arm_2d_tile_t c_tile_alarmClock_s_png_A2Mask;
#define IMAGE_ALARMCLOCK_S_PNG_A2Mask   (arm_2d_tile_t*)&c_tile_alarmClock_s_png_A2Mask
extern const arm_2d_tile_t c_tile_alarmClock_s_png_A4Mask;
#define IMAGE_ALARMCLOCK_S_PNG_A4Mask   (arm_2d_tile_t*)&c_tile_alarmClock_s_png_A4Mask
extern const arm_2d_tile_t c_tile_alarmClock_s_png_Mask;
#define IMAGE_ALARMCLOCK_S_PNG_Mask     (arm_2d_tile_t*)&c_tile_alarmClock_s_png_Mask

// balance.bmp < 30x30 >
#if LD_CFG_COLOR_DEPTH == 8
extern const arm_2d_tile_t c_tile_balance_bmp_GRAY8;
#define IMAGE_BALANCE_BMP          (arm_2d_tile_t*)&c_tile_balance_bmp_GRAY8
#elif LD_CFG_COLOR_DEPTH == 16
extern const arm_2d_tile_t c_tile_balance_bmp_RGB565;
#define IMAGE_BALANCE_BMP          (arm_2d_tile_t*)&c_tile_balance_bmp_RGB565
#else
extern const arm_2d_tile_t c_tile_balance_bmp_CCCN888;
#define IMAGE_BALANCE_BMP          (arm_2d_tile_t*)&c_tile_balance_bmp_CCCN888
#endif

// blockRabbit.png < 200x180 >
#if LD_CFG_COLOR_DEPTH == 8
extern const arm_2d_tile_t c_tile_blockRabbit_png_GRAY8;
#define IMAGE_BLOCKRABBIT_PNG          (arm_2d_tile_t*)&c_tile_blockRabbit_png_GRAY8
#elif LD_CFG_COLOR_DEPTH == 16
extern const arm_2d_tile_t c_tile_blockRabbit_png_RGB565;
#define IMAGE_BLOCKRABBIT_PNG          (arm_2d_tile_t*)&c_tile_blockRabbit_png_RGB565
#else
extern const arm_2d_tile_t c_tile_blockRabbit_png_CCCA8888;
#define IMAGE_BLOCKRABBIT_PNG          (arm_2d_tile_t*)&c_tile_blockRabbit_png_CCCA8888
#endif
extern const arm_2d_tile_t c_tile_blockRabbit_png_A1Mask;
#define IMAGE_BLOCKRABBIT_PNG_A1Mask   (arm_2d_tile_t*)&c_tile_blockRabbit_png_A1Mask
extern const arm_2d_tile_t c_tile_blockRabbit_png_A2Mask;
#define IMAGE_BLOCKRABBIT_PNG_A2Mask   (arm_2d_tile_t*)&c_tile_blockRabbit_png_A2Mask
extern const arm_2d_tile_t c_tile_blockRabbit_png_A4Mask;
#define IMAGE_BLOCKRABBIT_PNG_A4Mask   (arm_2d_tile_t*)&c_tile_blockRabbit_png_A4Mask
extern const arm_2d_tile_t c_tile_blockRabbit_png_Mask;
#define IMAGE_BLOCKRABBIT_PNG_Mask     (arm_2d_tile_t*)&c_tile_blockRabbit_png_Mask

// blockRabbit_s.png < 176x160 >
#if LD_CFG_COLOR_DEPTH == 8
extern const arm_2d_tile_t c_tile_blockRabbit_s_png_GRAY8;
#define IMAGE_BLOCKRABBIT_S_PNG          (arm_2d_tile_t*)&c_tile_blockRabbit_s_png_GRAY8
#elif LD_CFG_COLOR_DEPTH == 16
extern const arm_2d_tile_t c_tile_blockRabbit_s_png_RGB565;
#define IMAGE_BLOCKRABBIT_S_PNG          (arm_2d_tile_t*)&c_tile_blockRabbit_s_png_RGB565
#else
extern const arm_2d_tile_t c_tile_blockRabbit_s_png_CCCA8888;
#define IMAGE_BLOCKRABBIT_S_PNG          (arm_2d_tile_t*)&c_tile_blockRabbit_s_png_CCCA8888
#endif
extern const arm_2d_tile_t c_tile_blockRabbit_s_png_A1Mask;
#define IMAGE_BLOCKRABBIT_S_PNG_A1Mask   (arm_2d_tile_t*)&c_tile_blockRabbit_s_png_A1Mask
extern const arm_2d_tile_t c_tile_blockRabbit_s_png_A2Mask;
#define IMAGE_BLOCKRABBIT_S_PNG_A2Mask   (arm_2d_tile_t*)&c_tile_blockRabbit_s_png_A2Mask
extern const arm_2d_tile_t c_tile_blockRabbit_s_png_A4Mask;
#define IMAGE_BLOCKRABBIT_S_PNG_A4Mask   (arm_2d_tile_t*)&c_tile_blockRabbit_s_png_A4Mask
extern const arm_2d_tile_t c_tile_blockRabbit_s_png_Mask;
#define IMAGE_BLOCKRABBIT_S_PNG_Mask     (arm_2d_tile_t*)&c_tile_blockRabbit_s_png_Mask

// brightness.bmp < 30x30 >
#if LD_CFG_COLOR_DEPTH == 8
extern const arm_2d_tile_t c_tile_brightness_bmp_GRAY8;
#define IMAGE_BRIGHTNESS_BMP          (arm_2d_tile_t*)&c_tile_brightness_bmp_GRAY8
#elif LD_CFG_COLOR_DEPTH == 16
extern const arm_2d_tile_t c_tile_brightness_bmp_RGB565;
#define IMAGE_BRIGHTNESS_BMP          (arm_2d_tile_t*)&c_tile_brightness_bmp_RGB565
#else
extern const arm_2d_tile_t c_tile_brightness_bmp_CCCN888;
#define IMAGE_BRIGHTNESS_BMP          (arm_2d_tile_t*)&c_tile_brightness_bmp_CCCN888
#endif

// cake.png < 200x180 >
#if LD_CFG_COLOR_DEPTH == 8
extern const arm_2d_tile_t c_tile_cake_png_GRAY8;
#define IMAGE_CAKE_PNG          (arm_2d_tile_t*)&c_tile_cake_png_GRAY8
#elif LD_CFG_COLOR_DEPTH == 16
extern const arm_2d_tile_t c_tile_cake_png_RGB565;
#define IMAGE_CAKE_PNG          (arm_2d_tile_t*)&c_tile_cake_png_RGB565
#else
extern const arm_2d_tile_t c_tile_cake_png_CCCA8888;
#define IMAGE_CAKE_PNG          (arm_2d_tile_t*)&c_tile_cake_png_CCCA8888
#endif
extern const arm_2d_tile_t c_tile_cake_png_A1Mask;
#define IMAGE_CAKE_PNG_A1Mask   (arm_2d_tile_t*)&c_tile_cake_png_A1Mask
extern const arm_2d_tile_t c_tile_cake_png_A2Mask;
#define IMAGE_CAKE_PNG_A2Mask   (arm_2d_tile_t*)&c_tile_cake_png_A2Mask
extern const arm_2d_tile_t c_tile_cake_png_A4Mask;
#define IMAGE_CAKE_PNG_A4Mask   (arm_2d_tile_t*)&c_tile_cake_png_A4Mask
extern const arm_2d_tile_t c_tile_cake_png_Mask;
#define IMAGE_CAKE_PNG_Mask     (arm_2d_tile_t*)&c_tile_cake_png_Mask

// cake_s.png < 176x160 >
#if LD_CFG_COLOR_DEPTH == 8
extern const arm_2d_tile_t c_tile_cake_s_png_GRAY8;
#define IMAGE_CAKE_S_PNG          (arm_2d_tile_t*)&c_tile_cake_s_png_GRAY8
#elif LD_CFG_COLOR_DEPTH == 16
extern const arm_2d_tile_t c_tile_cake_s_png_RGB565;
#define IMAGE_CAKE_S_PNG          (arm_2d_tile_t*)&c_tile_cake_s_png_RGB565
#else
extern const arm_2d_tile_t c_tile_cake_s_png_CCCA8888;
#define IMAGE_CAKE_S_PNG          (arm_2d_tile_t*)&c_tile_cake_s_png_CCCA8888
#endif
extern const arm_2d_tile_t c_tile_cake_s_png_A1Mask;
#define IMAGE_CAKE_S_PNG_A1Mask   (arm_2d_tile_t*)&c_tile_cake_s_png_A1Mask
extern const arm_2d_tile_t c_tile_cake_s_png_A2Mask;
#define IMAGE_CAKE_S_PNG_A2Mask   (arm_2d_tile_t*)&c_tile_cake_s_png_A2Mask
extern const arm_2d_tile_t c_tile_cake_s_png_A4Mask;
#define IMAGE_CAKE_S_PNG_A4Mask   (arm_2d_tile_t*)&c_tile_cake_s_png_A4Mask
extern const arm_2d_tile_t c_tile_cake_s_png_Mask;
#define IMAGE_CAKE_S_PNG_Mask     (arm_2d_tile_t*)&c_tile_cake_s_png_Mask

// doll.png < 200x180 >
#if LD_CFG_COLOR_DEPTH == 8
extern const arm_2d_tile_t c_tile_doll_png_GRAY8;
#define IMAGE_DOLL_PNG          (arm_2d_tile_t*)&c_tile_doll_png_GRAY8
#elif LD_CFG_COLOR_DEPTH == 16
extern const arm_2d_tile_t c_tile_doll_png_RGB565;
#define IMAGE_DOLL_PNG          (arm_2d_tile_t*)&c_tile_doll_png_RGB565
#else
extern const arm_2d_tile_t c_tile_doll_png_CCCA8888;
#define IMAGE_DOLL_PNG          (arm_2d_tile_t*)&c_tile_doll_png_CCCA8888
#endif
extern const arm_2d_tile_t c_tile_doll_png_A1Mask;
#define IMAGE_DOLL_PNG_A1Mask   (arm_2d_tile_t*)&c_tile_doll_png_A1Mask
extern const arm_2d_tile_t c_tile_doll_png_A2Mask;
#define IMAGE_DOLL_PNG_A2Mask   (arm_2d_tile_t*)&c_tile_doll_png_A2Mask
extern const arm_2d_tile_t c_tile_doll_png_A4Mask;
#define IMAGE_DOLL_PNG_A4Mask   (arm_2d_tile_t*)&c_tile_doll_png_A4Mask
extern const arm_2d_tile_t c_tile_doll_png_Mask;
#define IMAGE_DOLL_PNG_Mask     (arm_2d_tile_t*)&c_tile_doll_png_Mask

// doll_s.png < 176x160 >
#if LD_CFG_COLOR_DEPTH == 8
extern const arm_2d_tile_t c_tile_doll_s_png_GRAY8;
#define IMAGE_DOLL_S_PNG          (arm_2d_tile_t*)&c_tile_doll_s_png_GRAY8
#elif LD_CFG_COLOR_DEPTH == 16
extern const arm_2d_tile_t c_tile_doll_s_png_RGB565;
#define IMAGE_DOLL_S_PNG          (arm_2d_tile_t*)&c_tile_doll_s_png_RGB565
#else
extern const arm_2d_tile_t c_tile_doll_s_png_CCCA8888;
#define IMAGE_DOLL_S_PNG          (arm_2d_tile_t*)&c_tile_doll_s_png_CCCA8888
#endif
extern const arm_2d_tile_t c_tile_doll_s_png_A1Mask;
#define IMAGE_DOLL_S_PNG_A1Mask   (arm_2d_tile_t*)&c_tile_doll_s_png_A1Mask
extern const arm_2d_tile_t c_tile_doll_s_png_A2Mask;
#define IMAGE_DOLL_S_PNG_A2Mask   (arm_2d_tile_t*)&c_tile_doll_s_png_A2Mask
extern const arm_2d_tile_t c_tile_doll_s_png_A4Mask;
#define IMAGE_DOLL_S_PNG_A4Mask   (arm_2d_tile_t*)&c_tile_doll_s_png_A4Mask
extern const arm_2d_tile_t c_tile_doll_s_png_Mask;
#define IMAGE_DOLL_S_PNG_Mask     (arm_2d_tile_t*)&c_tile_doll_s_png_Mask

// height.bmp < 40x33 >
#if LD_CFG_COLOR_DEPTH == 8
extern const arm_2d_tile_t c_tile_height_bmp_GRAY8;
#define IMAGE_HEIGHT_BMP          (arm_2d_tile_t*)&c_tile_height_bmp_GRAY8
#elif LD_CFG_COLOR_DEPTH == 16
extern const arm_2d_tile_t c_tile_height_bmp_RGB565;
#define IMAGE_HEIGHT_BMP          (arm_2d_tile_t*)&c_tile_height_bmp_RGB565
#else
extern const arm_2d_tile_t c_tile_height_bmp_CCCN888;
#define IMAGE_HEIGHT_BMP          (arm_2d_tile_t*)&c_tile_height_bmp_CCCN888
#endif

// light.bmp < 30x30 >
#if LD_CFG_COLOR_DEPTH == 8
extern const arm_2d_tile_t c_tile_light_bmp_GRAY8;
#define IMAGE_LIGHT_BMP          (arm_2d_tile_t*)&c_tile_light_bmp_GRAY8
#elif LD_CFG_COLOR_DEPTH == 16
extern const arm_2d_tile_t c_tile_light_bmp_RGB565;
#define IMAGE_LIGHT_BMP          (arm_2d_tile_t*)&c_tile_light_bmp_RGB565
#else
extern const arm_2d_tile_t c_tile_light_bmp_CCCN888;
#define IMAGE_LIGHT_BMP          (arm_2d_tile_t*)&c_tile_light_bmp_CCCN888
#endif

// mainBtnPrint.bmp < 124x158 >
#if LD_CFG_COLOR_DEPTH == 8
extern const arm_2d_tile_t c_tile_mainBtnPrint_bmp_GRAY8;
#define IMAGE_MAINBTNPRINT_BMP          (arm_2d_tile_t*)&c_tile_mainBtnPrint_bmp_GRAY8
#elif LD_CFG_COLOR_DEPTH == 16
extern const arm_2d_tile_t c_tile_mainBtnPrint_bmp_RGB565;
#define IMAGE_MAINBTNPRINT_BMP          (arm_2d_tile_t*)&c_tile_mainBtnPrint_bmp_RGB565
#else
extern const arm_2d_tile_t c_tile_mainBtnPrint_bmp_CCCN888;
#define IMAGE_MAINBTNPRINT_BMP          (arm_2d_tile_t*)&c_tile_mainBtnPrint_bmp_CCCN888
#endif

// mainBtnReady.bmp < 124x158 >
#if LD_CFG_COLOR_DEPTH == 8
extern const arm_2d_tile_t c_tile_mainBtnReady_bmp_GRAY8;
#define IMAGE_MAINBTNREADY_BMP          (arm_2d_tile_t*)&c_tile_mainBtnReady_bmp_GRAY8
#elif LD_CFG_COLOR_DEPTH == 16
extern const arm_2d_tile_t c_tile_mainBtnReady_bmp_RGB565;
#define IMAGE_MAINBTNREADY_BMP          (arm_2d_tile_t*)&c_tile_mainBtnReady_bmp_RGB565
#else
extern const arm_2d_tile_t c_tile_mainBtnReady_bmp_CCCN888;
#define IMAGE_MAINBTNREADY_BMP          (arm_2d_tile_t*)&c_tile_mainBtnReady_bmp_CCCN888
#endif

// mainBtnSet.bmp < 124x158 >
#if LD_CFG_COLOR_DEPTH == 8
extern const arm_2d_tile_t c_tile_mainBtnSet_bmp_GRAY8;
#define IMAGE_MAINBTNSET_BMP          (arm_2d_tile_t*)&c_tile_mainBtnSet_bmp_GRAY8
#elif LD_CFG_COLOR_DEPTH == 16
extern const arm_2d_tile_t c_tile_mainBtnSet_bmp_RGB565;
#define IMAGE_MAINBTNSET_BMP          (arm_2d_tile_t*)&c_tile_mainBtnSet_bmp_RGB565
#else
extern const arm_2d_tile_t c_tile_mainBtnSet_bmp_CCCN888;
#define IMAGE_MAINBTNSET_BMP          (arm_2d_tile_t*)&c_tile_mainBtnSet_bmp_CCCN888
#endif

// milk.png < 200x180 >
#if LD_CFG_COLOR_DEPTH == 8
extern const arm_2d_tile_t c_tile_milk_png_GRAY8;
#define IMAGE_MILK_PNG          (arm_2d_tile_t*)&c_tile_milk_png_GRAY8
#elif LD_CFG_COLOR_DEPTH == 16
extern const arm_2d_tile_t c_tile_milk_png_RGB565;
#define IMAGE_MILK_PNG          (arm_2d_tile_t*)&c_tile_milk_png_RGB565
#else
extern const arm_2d_tile_t c_tile_milk_png_CCCA8888;
#define IMAGE_MILK_PNG          (arm_2d_tile_t*)&c_tile_milk_png_CCCA8888
#endif
extern const arm_2d_tile_t c_tile_milk_png_A1Mask;
#define IMAGE_MILK_PNG_A1Mask   (arm_2d_tile_t*)&c_tile_milk_png_A1Mask
extern const arm_2d_tile_t c_tile_milk_png_A2Mask;
#define IMAGE_MILK_PNG_A2Mask   (arm_2d_tile_t*)&c_tile_milk_png_A2Mask
extern const arm_2d_tile_t c_tile_milk_png_A4Mask;
#define IMAGE_MILK_PNG_A4Mask   (arm_2d_tile_t*)&c_tile_milk_png_A4Mask
extern const arm_2d_tile_t c_tile_milk_png_Mask;
#define IMAGE_MILK_PNG_Mask     (arm_2d_tile_t*)&c_tile_milk_png_Mask

// milk_s.png < 176x160 >
#if LD_CFG_COLOR_DEPTH == 8
extern const arm_2d_tile_t c_tile_milk_s_png_GRAY8;
#define IMAGE_MILK_S_PNG          (arm_2d_tile_t*)&c_tile_milk_s_png_GRAY8
#elif LD_CFG_COLOR_DEPTH == 16
extern const arm_2d_tile_t c_tile_milk_s_png_RGB565;
#define IMAGE_MILK_S_PNG          (arm_2d_tile_t*)&c_tile_milk_s_png_RGB565
#else
extern const arm_2d_tile_t c_tile_milk_s_png_CCCA8888;
#define IMAGE_MILK_S_PNG          (arm_2d_tile_t*)&c_tile_milk_s_png_CCCA8888
#endif
extern const arm_2d_tile_t c_tile_milk_s_png_A1Mask;
#define IMAGE_MILK_S_PNG_A1Mask   (arm_2d_tile_t*)&c_tile_milk_s_png_A1Mask
extern const arm_2d_tile_t c_tile_milk_s_png_A2Mask;
#define IMAGE_MILK_S_PNG_A2Mask   (arm_2d_tile_t*)&c_tile_milk_s_png_A2Mask
extern const arm_2d_tile_t c_tile_milk_s_png_A4Mask;
#define IMAGE_MILK_S_PNG_A4Mask   (arm_2d_tile_t*)&c_tile_milk_s_png_A4Mask
extern const arm_2d_tile_t c_tile_milk_s_png_Mask;
#define IMAGE_MILK_S_PNG_Mask     (arm_2d_tile_t*)&c_tile_milk_s_png_Mask

// mug.png < 200x180 >
#if LD_CFG_COLOR_DEPTH == 8
extern const arm_2d_tile_t c_tile_mug_png_GRAY8;
#define IMAGE_MUG_PNG          (arm_2d_tile_t*)&c_tile_mug_png_GRAY8
#elif LD_CFG_COLOR_DEPTH == 16
extern const arm_2d_tile_t c_tile_mug_png_RGB565;
#define IMAGE_MUG_PNG          (arm_2d_tile_t*)&c_tile_mug_png_RGB565
#else
extern const arm_2d_tile_t c_tile_mug_png_CCCA8888;
#define IMAGE_MUG_PNG          (arm_2d_tile_t*)&c_tile_mug_png_CCCA8888
#endif
extern const arm_2d_tile_t c_tile_mug_png_A1Mask;
#define IMAGE_MUG_PNG_A1Mask   (arm_2d_tile_t*)&c_tile_mug_png_A1Mask
extern const arm_2d_tile_t c_tile_mug_png_A2Mask;
#define IMAGE_MUG_PNG_A2Mask   (arm_2d_tile_t*)&c_tile_mug_png_A2Mask
extern const arm_2d_tile_t c_tile_mug_png_A4Mask;
#define IMAGE_MUG_PNG_A4Mask   (arm_2d_tile_t*)&c_tile_mug_png_A4Mask
extern const arm_2d_tile_t c_tile_mug_png_Mask;
#define IMAGE_MUG_PNG_Mask     (arm_2d_tile_t*)&c_tile_mug_png_Mask

// mug_s.png < 176x160 >
#if LD_CFG_COLOR_DEPTH == 8
extern const arm_2d_tile_t c_tile_mug_s_png_GRAY8;
#define IMAGE_MUG_S_PNG          (arm_2d_tile_t*)&c_tile_mug_s_png_GRAY8
#elif LD_CFG_COLOR_DEPTH == 16
extern const arm_2d_tile_t c_tile_mug_s_png_RGB565;
#define IMAGE_MUG_S_PNG          (arm_2d_tile_t*)&c_tile_mug_s_png_RGB565
#else
extern const arm_2d_tile_t c_tile_mug_s_png_CCCA8888;
#define IMAGE_MUG_S_PNG          (arm_2d_tile_t*)&c_tile_mug_s_png_CCCA8888
#endif
extern const arm_2d_tile_t c_tile_mug_s_png_A1Mask;
#define IMAGE_MUG_S_PNG_A1Mask   (arm_2d_tile_t*)&c_tile_mug_s_png_A1Mask
extern const arm_2d_tile_t c_tile_mug_s_png_A2Mask;
#define IMAGE_MUG_S_PNG_A2Mask   (arm_2d_tile_t*)&c_tile_mug_s_png_A2Mask
extern const arm_2d_tile_t c_tile_mug_s_png_A4Mask;
#define IMAGE_MUG_S_PNG_A4Mask   (arm_2d_tile_t*)&c_tile_mug_s_png_A4Mask
extern const arm_2d_tile_t c_tile_mug_s_png_Mask;
#define IMAGE_MUG_S_PNG_Mask     (arm_2d_tile_t*)&c_tile_mug_s_png_Mask

// nozzleIcon1.bmp < 42x30 >
#if LD_CFG_COLOR_DEPTH == 8
extern const arm_2d_tile_t c_tile_nozzleIcon1_bmp_GRAY8;
#define IMAGE_NOZZLEICON1_BMP          (arm_2d_tile_t*)&c_tile_nozzleIcon1_bmp_GRAY8
#elif LD_CFG_COLOR_DEPTH == 16
extern const arm_2d_tile_t c_tile_nozzleIcon1_bmp_RGB565;
#define IMAGE_NOZZLEICON1_BMP          (arm_2d_tile_t*)&c_tile_nozzleIcon1_bmp_RGB565
#else
extern const arm_2d_tile_t c_tile_nozzleIcon1_bmp_CCCN888;
#define IMAGE_NOZZLEICON1_BMP          (arm_2d_tile_t*)&c_tile_nozzleIcon1_bmp_CCCN888
#endif

// nozzleIcon2.bmp < 42x30 >
#if LD_CFG_COLOR_DEPTH == 8
extern const arm_2d_tile_t c_tile_nozzleIcon2_bmp_GRAY8;
#define IMAGE_NOZZLEICON2_BMP          (arm_2d_tile_t*)&c_tile_nozzleIcon2_bmp_GRAY8
#elif LD_CFG_COLOR_DEPTH == 16
extern const arm_2d_tile_t c_tile_nozzleIcon2_bmp_RGB565;
#define IMAGE_NOZZLEICON2_BMP          (arm_2d_tile_t*)&c_tile_nozzleIcon2_bmp_RGB565
#else
extern const arm_2d_tile_t c_tile_nozzleIcon2_bmp_CCCN888;
#define IMAGE_NOZZLEICON2_BMP          (arm_2d_tile_t*)&c_tile_nozzleIcon2_bmp_CCCN888
#endif

// nozzleIcon3.bmp < 42x30 >
#if LD_CFG_COLOR_DEPTH == 8
extern const arm_2d_tile_t c_tile_nozzleIcon3_bmp_GRAY8;
#define IMAGE_NOZZLEICON3_BMP          (arm_2d_tile_t*)&c_tile_nozzleIcon3_bmp_GRAY8
#elif LD_CFG_COLOR_DEPTH == 16
extern const arm_2d_tile_t c_tile_nozzleIcon3_bmp_RGB565;
#define IMAGE_NOZZLEICON3_BMP          (arm_2d_tile_t*)&c_tile_nozzleIcon3_bmp_RGB565
#else
extern const arm_2d_tile_t c_tile_nozzleIcon3_bmp_CCCN888;
#define IMAGE_NOZZLEICON3_BMP          (arm_2d_tile_t*)&c_tile_nozzleIcon3_bmp_CCCN888
#endif

// percent.bmp < 40x33 >
#if LD_CFG_COLOR_DEPTH == 8
extern const arm_2d_tile_t c_tile_percent_bmp_GRAY8;
#define IMAGE_PERCENT_BMP          (arm_2d_tile_t*)&c_tile_percent_bmp_GRAY8
#elif LD_CFG_COLOR_DEPTH == 16
extern const arm_2d_tile_t c_tile_percent_bmp_RGB565;
#define IMAGE_PERCENT_BMP          (arm_2d_tile_t*)&c_tile_percent_bmp_RGB565
#else
extern const arm_2d_tile_t c_tile_percent_bmp_CCCN888;
#define IMAGE_PERCENT_BMP          (arm_2d_tile_t*)&c_tile_percent_bmp_CCCN888
#endif

// pid.bmp < 30x30 >
#if LD_CFG_COLOR_DEPTH == 8
extern const arm_2d_tile_t c_tile_pid_bmp_GRAY8;
#define IMAGE_PID_BMP          (arm_2d_tile_t*)&c_tile_pid_bmp_GRAY8
#elif LD_CFG_COLOR_DEPTH == 16
extern const arm_2d_tile_t c_tile_pid_bmp_RGB565;
#define IMAGE_PID_BMP          (arm_2d_tile_t*)&c_tile_pid_bmp_RGB565
#else
extern const arm_2d_tile_t c_tile_pid_bmp_CCCN888;
#define IMAGE_PID_BMP          (arm_2d_tile_t*)&c_tile_pid_bmp_CCCN888
#endif

// pla.bmp < 30x30 >
#if LD_CFG_COLOR_DEPTH == 8
extern const arm_2d_tile_t c_tile_pla_bmp_GRAY8;
#define IMAGE_PLA_BMP          (arm_2d_tile_t*)&c_tile_pla_bmp_GRAY8
#elif LD_CFG_COLOR_DEPTH == 16
extern const arm_2d_tile_t c_tile_pla_bmp_RGB565;
#define IMAGE_PLA_BMP          (arm_2d_tile_t*)&c_tile_pla_bmp_RGB565
#else
extern const arm_2d_tile_t c_tile_pla_bmp_CCCN888;
#define IMAGE_PLA_BMP          (arm_2d_tile_t*)&c_tile_pla_bmp_CCCN888
#endif

// readyBtn1.bmp < 220x60 >
#if LD_CFG_COLOR_DEPTH == 8
extern const arm_2d_tile_t c_tile_readyBtn1_bmp_GRAY8;
#define IMAGE_READYBTN1_BMP          (arm_2d_tile_t*)&c_tile_readyBtn1_bmp_GRAY8
#elif LD_CFG_COLOR_DEPTH == 16
extern const arm_2d_tile_t c_tile_readyBtn1_bmp_RGB565;
#define IMAGE_READYBTN1_BMP          (arm_2d_tile_t*)&c_tile_readyBtn1_bmp_RGB565
#else
extern const arm_2d_tile_t c_tile_readyBtn1_bmp_CCCN888;
#define IMAGE_READYBTN1_BMP          (arm_2d_tile_t*)&c_tile_readyBtn1_bmp_CCCN888
#endif

// readyBtn2.bmp < 220x60 >
#if LD_CFG_COLOR_DEPTH == 8
extern const arm_2d_tile_t c_tile_readyBtn2_bmp_GRAY8;
#define IMAGE_READYBTN2_BMP          (arm_2d_tile_t*)&c_tile_readyBtn2_bmp_GRAY8
#elif LD_CFG_COLOR_DEPTH == 16
extern const arm_2d_tile_t c_tile_readyBtn2_bmp_RGB565;
#define IMAGE_READYBTN2_BMP          (arm_2d_tile_t*)&c_tile_readyBtn2_bmp_RGB565
#else
extern const arm_2d_tile_t c_tile_readyBtn2_bmp_CCCN888;
#define IMAGE_READYBTN2_BMP          (arm_2d_tile_t*)&c_tile_readyBtn2_bmp_CCCN888
#endif

// readyBtn3.bmp < 220x60 >
#if LD_CFG_COLOR_DEPTH == 8
extern const arm_2d_tile_t c_tile_readyBtn3_bmp_GRAY8;
#define IMAGE_READYBTN3_BMP          (arm_2d_tile_t*)&c_tile_readyBtn3_bmp_GRAY8
#elif LD_CFG_COLOR_DEPTH == 16
extern const arm_2d_tile_t c_tile_readyBtn3_bmp_RGB565;
#define IMAGE_READYBTN3_BMP          (arm_2d_tile_t*)&c_tile_readyBtn3_bmp_RGB565
#else
extern const arm_2d_tile_t c_tile_readyBtn3_bmp_CCCN888;
#define IMAGE_READYBTN3_BMP          (arm_2d_tile_t*)&c_tile_readyBtn3_bmp_CCCN888
#endif

// readyBtn3_off.bmp < 220x60 >
#if LD_CFG_COLOR_DEPTH == 8
extern const arm_2d_tile_t c_tile_readyBtn3_off_bmp_GRAY8;
#define IMAGE_READYBTN3_OFF_BMP          (arm_2d_tile_t*)&c_tile_readyBtn3_off_bmp_GRAY8
#elif LD_CFG_COLOR_DEPTH == 16
extern const arm_2d_tile_t c_tile_readyBtn3_off_bmp_RGB565;
#define IMAGE_READYBTN3_OFF_BMP          (arm_2d_tile_t*)&c_tile_readyBtn3_off_bmp_RGB565
#else
extern const arm_2d_tile_t c_tile_readyBtn3_off_bmp_CCCN888;
#define IMAGE_READYBTN3_OFF_BMP          (arm_2d_tile_t*)&c_tile_readyBtn3_off_bmp_CCCN888
#endif

// readyBtn4.bmp < 220x60 >
#if LD_CFG_COLOR_DEPTH == 8
extern const arm_2d_tile_t c_tile_readyBtn4_bmp_GRAY8;
#define IMAGE_READYBTN4_BMP          (arm_2d_tile_t*)&c_tile_readyBtn4_bmp_GRAY8
#elif LD_CFG_COLOR_DEPTH == 16
extern const arm_2d_tile_t c_tile_readyBtn4_bmp_RGB565;
#define IMAGE_READYBTN4_BMP          (arm_2d_tile_t*)&c_tile_readyBtn4_bmp_RGB565
#else
extern const arm_2d_tile_t c_tile_readyBtn4_bmp_CCCN888;
#define IMAGE_READYBTN4_BMP          (arm_2d_tile_t*)&c_tile_readyBtn4_bmp_CCCN888
#endif

// readyBtn4_off.bmp < 220x60 >
#if LD_CFG_COLOR_DEPTH == 8
extern const arm_2d_tile_t c_tile_readyBtn4_off_bmp_GRAY8;
#define IMAGE_READYBTN4_OFF_BMP          (arm_2d_tile_t*)&c_tile_readyBtn4_off_bmp_GRAY8
#elif LD_CFG_COLOR_DEPTH == 16
extern const arm_2d_tile_t c_tile_readyBtn4_off_bmp_RGB565;
#define IMAGE_READYBTN4_OFF_BMP          (arm_2d_tile_t*)&c_tile_readyBtn4_off_bmp_RGB565
#else
extern const arm_2d_tile_t c_tile_readyBtn4_off_bmp_CCCN888;
#define IMAGE_READYBTN4_OFF_BMP          (arm_2d_tile_t*)&c_tile_readyBtn4_off_bmp_CCCN888
#endif

// readyBtn5.bmp < 220x60 >
#if LD_CFG_COLOR_DEPTH == 8
extern const arm_2d_tile_t c_tile_readyBtn5_bmp_GRAY8;
#define IMAGE_READYBTN5_BMP          (arm_2d_tile_t*)&c_tile_readyBtn5_bmp_GRAY8
#elif LD_CFG_COLOR_DEPTH == 16
extern const arm_2d_tile_t c_tile_readyBtn5_bmp_RGB565;
#define IMAGE_READYBTN5_BMP          (arm_2d_tile_t*)&c_tile_readyBtn5_bmp_RGB565
#else
extern const arm_2d_tile_t c_tile_readyBtn5_bmp_CCCN888;
#define IMAGE_READYBTN5_BMP          (arm_2d_tile_t*)&c_tile_readyBtn5_bmp_CCCN888
#endif

// smallRoom.png < 200x180 >
#if LD_CFG_COLOR_DEPTH == 8
extern const arm_2d_tile_t c_tile_smallRoom_png_GRAY8;
#define IMAGE_SMALLROOM_PNG          (arm_2d_tile_t*)&c_tile_smallRoom_png_GRAY8
#elif LD_CFG_COLOR_DEPTH == 16
extern const arm_2d_tile_t c_tile_smallRoom_png_RGB565;
#define IMAGE_SMALLROOM_PNG          (arm_2d_tile_t*)&c_tile_smallRoom_png_RGB565
#else
extern const arm_2d_tile_t c_tile_smallRoom_png_CCCA8888;
#define IMAGE_SMALLROOM_PNG          (arm_2d_tile_t*)&c_tile_smallRoom_png_CCCA8888
#endif
extern const arm_2d_tile_t c_tile_smallRoom_png_A1Mask;
#define IMAGE_SMALLROOM_PNG_A1Mask   (arm_2d_tile_t*)&c_tile_smallRoom_png_A1Mask
extern const arm_2d_tile_t c_tile_smallRoom_png_A2Mask;
#define IMAGE_SMALLROOM_PNG_A2Mask   (arm_2d_tile_t*)&c_tile_smallRoom_png_A2Mask
extern const arm_2d_tile_t c_tile_smallRoom_png_A4Mask;
#define IMAGE_SMALLROOM_PNG_A4Mask   (arm_2d_tile_t*)&c_tile_smallRoom_png_A4Mask
extern const arm_2d_tile_t c_tile_smallRoom_png_Mask;
#define IMAGE_SMALLROOM_PNG_Mask     (arm_2d_tile_t*)&c_tile_smallRoom_png_Mask

// smallRoom_s.png < 176x160 >
#if LD_CFG_COLOR_DEPTH == 8
extern const arm_2d_tile_t c_tile_smallRoom_s_png_GRAY8;
#define IMAGE_SMALLROOM_S_PNG          (arm_2d_tile_t*)&c_tile_smallRoom_s_png_GRAY8
#elif LD_CFG_COLOR_DEPTH == 16
extern const arm_2d_tile_t c_tile_smallRoom_s_png_RGB565;
#define IMAGE_SMALLROOM_S_PNG          (arm_2d_tile_t*)&c_tile_smallRoom_s_png_RGB565
#else
extern const arm_2d_tile_t c_tile_smallRoom_s_png_CCCA8888;
#define IMAGE_SMALLROOM_S_PNG          (arm_2d_tile_t*)&c_tile_smallRoom_s_png_CCCA8888
#endif
extern const arm_2d_tile_t c_tile_smallRoom_s_png_A1Mask;
#define IMAGE_SMALLROOM_S_PNG_A1Mask   (arm_2d_tile_t*)&c_tile_smallRoom_s_png_A1Mask
extern const arm_2d_tile_t c_tile_smallRoom_s_png_A2Mask;
#define IMAGE_SMALLROOM_S_PNG_A2Mask   (arm_2d_tile_t*)&c_tile_smallRoom_s_png_A2Mask
extern const arm_2d_tile_t c_tile_smallRoom_s_png_A4Mask;
#define IMAGE_SMALLROOM_S_PNG_A4Mask   (arm_2d_tile_t*)&c_tile_smallRoom_s_png_A4Mask
extern const arm_2d_tile_t c_tile_smallRoom_s_png_Mask;
#define IMAGE_SMALLROOM_S_PNG_Mask     (arm_2d_tile_t*)&c_tile_smallRoom_s_png_Mask

// speed.bmp < 30x30 >
#if LD_CFG_COLOR_DEPTH == 8
extern const arm_2d_tile_t c_tile_speed_bmp_GRAY8;
#define IMAGE_SPEED_BMP          (arm_2d_tile_t*)&c_tile_speed_bmp_GRAY8
#elif LD_CFG_COLOR_DEPTH == 16
extern const arm_2d_tile_t c_tile_speed_bmp_RGB565;
#define IMAGE_SPEED_BMP          (arm_2d_tile_t*)&c_tile_speed_bmp_RGB565
#else
extern const arm_2d_tile_t c_tile_speed_bmp_CCCN888;
#define IMAGE_SPEED_BMP          (arm_2d_tile_t*)&c_tile_speed_bmp_CCCN888
#endif

// tabBtnPress.bmp < 100x74 >
#if LD_CFG_COLOR_DEPTH == 8
extern const arm_2d_tile_t c_tile_tabBtnPress_bmp_GRAY8;
#define IMAGE_TABBTNPRESS_BMP          (arm_2d_tile_t*)&c_tile_tabBtnPress_bmp_GRAY8
#elif LD_CFG_COLOR_DEPTH == 16
extern const arm_2d_tile_t c_tile_tabBtnPress_bmp_RGB565;
#define IMAGE_TABBTNPRESS_BMP          (arm_2d_tile_t*)&c_tile_tabBtnPress_bmp_RGB565
#else
extern const arm_2d_tile_t c_tile_tabBtnPress_bmp_CCCN888;
#define IMAGE_TABBTNPRESS_BMP          (arm_2d_tile_t*)&c_tile_tabBtnPress_bmp_CCCN888
#endif

// tabBtnRelease.bmp < 100x74 >
#if LD_CFG_COLOR_DEPTH == 8
extern const arm_2d_tile_t c_tile_tabBtnRelease_bmp_GRAY8;
#define IMAGE_TABBTNRELEASE_BMP          (arm_2d_tile_t*)&c_tile_tabBtnRelease_bmp_GRAY8
#elif LD_CFG_COLOR_DEPTH == 16
extern const arm_2d_tile_t c_tile_tabBtnRelease_bmp_RGB565;
#define IMAGE_TABBTNRELEASE_BMP          (arm_2d_tile_t*)&c_tile_tabBtnRelease_bmp_RGB565
#else
extern const arm_2d_tile_t c_tile_tabBtnRelease_bmp_CCCN888;
#define IMAGE_TABBTNRELEASE_BMP          (arm_2d_tile_t*)&c_tile_tabBtnRelease_bmp_CCCN888
#endif

// temp1.bmp < 40x33 >
#if LD_CFG_COLOR_DEPTH == 8
extern const arm_2d_tile_t c_tile_temp1_bmp_GRAY8;
#define IMAGE_TEMP1_BMP          (arm_2d_tile_t*)&c_tile_temp1_bmp_GRAY8
#elif LD_CFG_COLOR_DEPTH == 16
extern const arm_2d_tile_t c_tile_temp1_bmp_RGB565;
#define IMAGE_TEMP1_BMP          (arm_2d_tile_t*)&c_tile_temp1_bmp_RGB565
#else
extern const arm_2d_tile_t c_tile_temp1_bmp_CCCN888;
#define IMAGE_TEMP1_BMP          (arm_2d_tile_t*)&c_tile_temp1_bmp_CCCN888
#endif

// temp2.bmp < 40x33 >
#if LD_CFG_COLOR_DEPTH == 8
extern const arm_2d_tile_t c_tile_temp2_bmp_GRAY8;
#define IMAGE_TEMP2_BMP          (arm_2d_tile_t*)&c_tile_temp2_bmp_GRAY8
#elif LD_CFG_COLOR_DEPTH == 16
extern const arm_2d_tile_t c_tile_temp2_bmp_RGB565;
#define IMAGE_TEMP2_BMP          (arm_2d_tile_t*)&c_tile_temp2_bmp_RGB565
#else
extern const arm_2d_tile_t c_tile_temp2_bmp_CCCN888;
#define IMAGE_TEMP2_BMP          (arm_2d_tile_t*)&c_tile_temp2_bmp_CCCN888
#endif

// voice.bmp < 30x30 >
#if LD_CFG_COLOR_DEPTH == 8
extern const arm_2d_tile_t c_tile_voice_bmp_GRAY8;
#define IMAGE_VOICE_BMP          (arm_2d_tile_t*)&c_tile_voice_bmp_GRAY8
#elif LD_CFG_COLOR_DEPTH == 16
extern const arm_2d_tile_t c_tile_voice_bmp_RGB565;
#define IMAGE_VOICE_BMP          (arm_2d_tile_t*)&c_tile_voice_bmp_RGB565
#else
extern const arm_2d_tile_t c_tile_voice_bmp_CCCN888;
#define IMAGE_VOICE_BMP          (arm_2d_tile_t*)&c_tile_voice_bmp_CCCN888
#endif

// wifi.bmp < 30x30 >
#if LD_CFG_COLOR_DEPTH == 8
extern const arm_2d_tile_t c_tile_wifi_bmp_GRAY8;
#define IMAGE_WIFI_BMP          (arm_2d_tile_t*)&c_tile_wifi_bmp_GRAY8
#elif LD_CFG_COLOR_DEPTH == 16
extern const arm_2d_tile_t c_tile_wifi_bmp_RGB565;
#define IMAGE_WIFI_BMP          (arm_2d_tile_t*)&c_tile_wifi_bmp_RGB565
#else
extern const arm_2d_tile_t c_tile_wifi_bmp_CCCN888;
#define IMAGE_WIFI_BMP          (arm_2d_tile_t*)&c_tile_wifi_bmp_CCCN888
#endif

// zero.bmp < 30x30 >
#if LD_CFG_COLOR_DEPTH == 8
extern const arm_2d_tile_t c_tile_zero_bmp_GRAY8;
#define IMAGE_ZERO_BMP          (arm_2d_tile_t*)&c_tile_zero_bmp_GRAY8
#elif LD_CFG_COLOR_DEPTH == 16
extern const arm_2d_tile_t c_tile_zero_bmp_RGB565;
#define IMAGE_ZERO_BMP          (arm_2d_tile_t*)&c_tile_zero_bmp_RGB565
#else
extern const arm_2d_tile_t c_tile_zero_bmp_CCCN888;
#define IMAGE_ZERO_BMP          (arm_2d_tile_t*)&c_tile_zero_bmp_CCCN888
#endif

#ifdef __cplusplus
}
#endif

#endif
