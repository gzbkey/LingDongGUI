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
} ARM_2D_FONT_arial_12_A8;

#define FONT_ARIAL_12          (arm_2d_font_t*)&ARM_2D_FONT_arial_12_A8

extern const
struct {
    implement(arm_2d_user_font_t);
    arm_2d_char_idx_t tUTF8Table;
} ARM_2D_FONT_arial_48_A1;

#define FONT_ARIAL_48_A1          (arm_2d_font_t*)&ARM_2D_FONT_arial_48_A1

extern const
struct {
    implement(arm_2d_user_font_t);
    arm_2d_char_idx_t tUTF8Table;
} ARM_2D_FONT_arial_48_A2;

#define FONT_ARIAL_48_A2          (arm_2d_font_t*)&ARM_2D_FONT_arial_48_A2

extern const
struct {
    implement(arm_2d_user_font_t);
    arm_2d_char_idx_t tUTF8Table;
} ARM_2D_FONT_arial_48_A4;

#define FONT_ARIAL_48_A4          (arm_2d_font_t*)&ARM_2D_FONT_arial_48_A4

extern const
struct {
    implement(arm_2d_user_font_t);
    arm_2d_char_idx_t tUTF8Table;
} ARM_2D_FONT_arial_48_A8;

#define FONT_ARIAL_48_A8          (arm_2d_font_t*)&ARM_2D_FONT_arial_48_A8

#ifdef __cplusplus
}
#endif

#endif
