#ifndef __UI_IMAGES_H__
#define __UI_IMAGES_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "arm_2d.h"
#include "ldConfig.h"

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

#ifdef __cplusplus
}
#endif

#endif