#ifndef __UI_FONTS_H__
#define __UI_FONTS_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "arm_2d_helper.h"

extern const
struct {
    implement(arm_2d_user_font_t);
    arm_2d_char_idx_t tUTF8Table;
} ARM_2D_FONT_simsun_48_A8;

#define FONT_SIMSUN_48          (arm_2d_font_t*)&ARM_2D_FONT_simsun_48_A8

extern const
struct {
    implement(arm_2d_user_font_t);
    arm_2d_char_idx_t tUTF8Table;
} ARM_2D_FONT_simsun_24_A8;

#define FONT_SIMSUN_24          (arm_2d_font_t*)&ARM_2D_FONT_simsun_24_A8

extern const
struct {
    implement(arm_2d_user_font_t);
    arm_2d_char_idx_t tUTF8Table;
} ARM_2D_FONT_simsun_18_A8;

#define FONT_SIMSUN_18          (arm_2d_font_t*)&ARM_2D_FONT_simsun_18_A8

extern const
struct {
    implement(arm_2d_user_font_t);
    arm_2d_char_idx_t tUTF8Table;
} ARM_2D_FONT_simsun_16_A8;

#define FONT_SIMSUN_16          (arm_2d_font_t*)&ARM_2D_FONT_simsun_16_A8

extern const
struct {
    implement(arm_2d_user_font_t);
    arm_2d_char_idx_t tUTF8Table;
} ARM_2D_FONT_simhei_20_A8;

#define FONT_SIMHEI_20          (arm_2d_font_t*)&ARM_2D_FONT_simhei_20_A8

#ifdef __cplusplus
}
#endif

#endif
