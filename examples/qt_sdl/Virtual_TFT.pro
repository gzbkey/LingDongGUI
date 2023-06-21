TEMPLATE = app
#CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
    Arm-2D/Helper/Source/arm_2d_helper.c \
    Arm-2D/Helper/Source/arm_2d_helper_list.c \
    Arm-2D/Helper/Source/arm_2d_helper_pfb.c \
    Arm-2D/Helper/Source/arm_2d_helper_scene.c \
    Arm-2D/Library/Source/__arm_2d_colour_filling_with_alpha.c \
    Arm-2D/Library/Source/__arm_2d_conversion_helium.c \
    Arm-2D/Library/Source/__arm_2d_core.c \
    Arm-2D/Library/Source/__arm_2d_tile.c \
    Arm-2D/Library/Source/__arm_2d_transform_helium.c \
    Arm-2D/Library/Source/arm_2d.c \
    Arm-2D/Library/Source/arm_2d_alpha_blending.c \
    Arm-2D/Library/Source/arm_2d_conversion.c \
    Arm-2D/Library/Source/arm_2d_draw.c \
    Arm-2D/Library/Source/arm_2d_helium.c \
    Arm-2D/Library/Source/arm_2d_transform.c \
    Arm-2D/examples/common/asset/DigitsFont.c \
    Arm-2D/examples/common/asset/GreenCircle.c \
    Arm-2D/examples/common/asset/Lighting.c \
    Arm-2D/examples/common/asset/ListCover.c \
    Arm-2D/examples/common/asset/MeterPanel.c \
    Arm-2D/examples/common/asset/Pointer.c \
    Arm-2D/examples/common/asset/QuaterArc.c \
    Arm-2D/examples/common/asset/SinWave.c \
    Arm-2D/examples/common/asset/SpinWheel.c \
    Arm-2D/examples/common/asset/arrow.c \
    Arm-2D/examples/common/asset/background.c \
    Arm-2D/examples/common/asset/battery_boarder_1.c \
    Arm-2D/examples/common/asset/battery_gasgauge_block.c \
    Arm-2D/examples/common/asset/battery_gasgauge_grade_boarder.c \
    Arm-2D/examples/common/asset/blue_slashes.c \
    Arm-2D/examples/common/asset/circle_mask.c \
    Arm-2D/examples/common/asset/circle_small.c \
    Arm-2D/examples/common/asset/clockface.c \
    Arm-2D/examples/common/asset/cmsis_logo.c \
    Arm-2D/examples/common/asset/gear_01.c \
    Arm-2D/examples/common/asset/gear_02.c \
    Arm-2D/examples/common/asset/glass_reflection_narrow.c \
    Arm-2D/examples/common/asset/glass_reflection_wide.c \
    Arm-2D/examples/common/asset/green28_28.c \
    Arm-2D/examples/common/asset/helium.c \
    Arm-2D/examples/common/asset/pointer_sec.c \
    Arm-2D/examples/common/asset/small_icon_sun.c \
    Arm-2D/examples/common/asset/software.c \
    Arm-2D/examples/common/asset/star.c \
    Arm-2D/examples/common/asset/watch_panel.c \
    Arm-2D/examples/common/asset/wave.c \
    Arm-2D/examples/common/asset/white_dot.c \
    Arm-2D/examples/common/asset/wifi_signal.c \
    Arm-2D/examples/common/benchmark/benchmark_watch_panel.c \
    Arm-2D/examples/common/benchmark/arm_2d_scene_benchmark_watch_panel.c \
    Arm-2D/examples/common/benchmark/arm_2d_scene_benchmark_watch_panel_cover.c \
    Arm-2D/examples/common/controls/GLCD_Fonts.c \
    Arm-2D/examples/common/controls/battery_gasgauge.c \
    Arm-2D/examples/common/controls/busy_wheel.c \
    Arm-2D/examples/common/controls/controls.c \
    Arm-2D/examples/common/controls/lcd_printf.c \
    Arm-2D/examples/common/controls/list_view.c \
    Arm-2D/examples/common/controls/number_list.c \
    Arm-2D/examples/common/controls/progress_bar_drill.c \
    Arm-2D/examples/common/controls/progress_bar_flowing.c \
    Arm-2D/examples/common/controls/progress_bar_simple.c \
    Arm-2D/examples/common/controls/progress_wheel.c \
    Arm-2D/examples/common/controls/shape_round_corner_box.c \
    Arm-2D/examples/common/controls/spinning_wheel.c \
    math/arm_common_tables.c \
    math/arm_cos_f32.c \
    math/arm_cos_q31.c \
    math/arm_sin_f32.c \
    math/arm_sin_q31.c \
    user/main.c \
    user/Virtual_TFT_Port.c \
    user/arm_2d_disp_adapter_0.c

HEADERS += \
    Arm-2D/Helper/Include/__arm_2d_helper_common.h \
    Arm-2D/Helper/Include/arm_2d_disp_adapters.h \
    Arm-2D/Helper/Include/arm_2d_helper.h \
    Arm-2D/Helper/Include/arm_2d_helper_list.h \
    Arm-2D/Helper/Include/arm_2d_helper_pfb.h \
    Arm-2D/Helper/Include/arm_2d_helper_scene.h \
    Arm-2D/Helper/Include/arm_2d_scenes.h \
    Arm-2D/Library/Include/__arm_2d_direct.h \
    Arm-2D/Library/Include/__arm_2d_impl.h \
    Arm-2D/Library/Include/__arm_2d_math.h \
    Arm-2D/Library/Include/__arm_2d_math_helium.h \
    Arm-2D/Library/Include/__arm_2d_paving.h \
    Arm-2D/Library/Include/__arm_2d_paving_helium.h \
    Arm-2D/Library/Include/__arm_2d_utils_helium.h \
    Arm-2D/Library/Include/arm_2d.h \
    Arm-2D/Library/Include/arm_2d_alpha_blending.h \
    Arm-2D/Library/Include/arm_2d_conversion.h \
    Arm-2D/Library/Include/arm_2d_draw.h \
    Arm-2D/Library/Include/arm_2d_features.h \
    Arm-2D/Library/Include/arm_2d_op.h \
    Arm-2D/Library/Include/arm_2d_tile.h \
    Arm-2D/Library/Include/arm_2d_transform.h \
    Arm-2D/Library/Include/arm_2d_types.h \
    Arm-2D/Library/Include/arm_2d_utils.h \
    Arm-2D/Library/Source/__arm_2d_alpha_blending_wrapper.inc \
    Arm-2D/Library/Source/__arm_2d_copy_mirror_wrapper.inc \
    Arm-2D/Library/Source/__arm_2d_copy_with_colour_keying_and_mirror_wrapper.inc \
    Arm-2D/Library/Source/__arm_2d_copy_with_mask_and_mirror_wrapper.inc \
    Arm-2D/Library/Source/__arm_2d_ll_alpha_blending.inc \
    Arm-2D/Library/Source/__arm_2d_ll_alpha_mask.inc \
    Arm-2D/Library/Source/__arm_2d_ll_alpha_mask_helium.inc \
    Arm-2D/Library/Source/__arm_2d_ll_colour_filling_with_alpha.inc \
    Arm-2D/Library/Source/__arm_2d_ll_copy.inc \
    Arm-2D/Library/Source/__arm_2d_ll_copy_helium.inc \
    Arm-2D/Library/Source/__arm_2d_ll_draw_pattern.inc \
    Arm-2D/Library/Source/__arm_2d_ll_draw_pattern_helium.inc \
    Arm-2D/Library/Source/__arm_2d_ll_fill_colour.inc \
    Arm-2D/Library/Source/__arm_2d_ll_fill_colour_helium.inc \
    Arm-2D/Library/Source/__arm_2d_ll_meta_copy_and_fill.inc \
    Arm-2D/Library/Source/__arm_2d_ll_meta_copy_with_masks.inc \
    Arm-2D/Library/Source/__arm_2d_ll_meta_copy_with_masks_helium.inc \
    Arm-2D/Library/Source/__arm_2d_ll_meta_trans_with_masks.inc \
    Arm-2D/Library/Source/__arm_2d_ll_meta_trans_with_masks_helium.inc \
    Arm-2D/Library/Source/__arm_2d_ll_transform.inc \
    Arm-2D/Library/Source/__arm_2d_ll_transform_helium.inc \
    Arm-2D/Library/Source/__arm_2d_meta_copy_mirror.inc \
    Arm-2D/Library/Source/__arm_2d_meta_copy_with_colour_keying_and_mirror.inc \
    Arm-2D/Library/Source/__arm_2d_meta_copy_with_des_mask_and_mirror.inc \
    Arm-2D/Library/Source/__arm_2d_meta_copy_with_masks_and_mirror.inc \
    Arm-2D/Library/Source/__arm_2d_meta_copy_with_src_mask_and_mirror.inc \
    Arm-2D/Library/Source/__arm_2d_meta_fill_mirror.inc \
    Arm-2D/Library/Source/__arm_2d_meta_fill_with_colour_keying_and_mirror.inc \
    Arm-2D/Library/Source/__arm_2d_meta_fill_with_des_mask_and_mirror.inc \
    Arm-2D/Library/Source/__arm_2d_meta_fill_with_masks_and_mirror.inc \
    Arm-2D/Library/Source/__arm_2d_meta_fill_with_src_mask_and_mirror.inc \
    Arm-2D/examples/common/controls/__common.h \
    Arm-2D/examples/common/controls/arm_extra_controls.h \
    Arm-2D/examples/common/controls/arm_extra_lcd_printf.h \
    Arm-2D/examples/common/controls/battery_gasgauge.h \
    Arm-2D/examples/common/controls/busy_wheel.h \
    Arm-2D/examples/common/controls/list_view.h \
    Arm-2D/examples/common/controls/number_list.h \
    Arm-2D/examples/common/controls/progress_bar_drill.h \
    Arm-2D/examples/common/controls/progress_bar_flowing.h \
    Arm-2D/examples/common/controls/progress_bar_simple.h \
    Arm-2D/examples/common/controls/progress_wheel.h \
    Arm-2D/examples/common/controls/shape_round_corner_box.h \
    Arm-2D/examples/common/controls/spinning_wheel.h \
    user/Virtual_TFT_Port.h \
    user/arm_2d_cfg.h \
    user/arm_2d_disp_adapter_0.h

INCLUDEPATH += $$PWD/Arm-2D
INCLUDEPATH += $$PWD/Arm-2D/Helper/Include
INCLUDEPATH += $$PWD/Arm-2D/Library/Include
INCLUDEPATH += $$PWD/Arm-2D/examples/common/controls
INCLUDEPATH += $$PWD/Arm-2D/examples/common/benchmark

INCLUDEPATH += $$PWD/user
INCLUDEPATH += $$PWD/math

contains(QT_ARCH, i386){
INCLUDEPATH += $$PWD/sdl2/32/include/SDL2
LIBS += -L$$PWD/sdl2/32/lib -lSDL2
LIBS += -L$$PWD/sdl2/32/lib -lSDL2main
SDL2_PATH = $$PWD/sdl2/32/bin/SDL2.dll
}else{
INCLUDEPATH += $$PWD/sdl2/64/include/SDL2
LIBS += -L$$PWD/sdl2/64/lib -lSDL2
LIBS += -L$$PWD/sdl2/64/lib -lSDL2main
SDL2_PATH = $$PWD/sdl2/64/bin/SDL2.dll
}

SDL2_PATH = $$replace(SDL2_PATH, /, \\)
OutLibFile = $$OUT_PWD/SDL2.dll
OutLibFile = $$replace(OutLibFile, /, \\)
QMAKE_PRE_LINK +=  copy $$SDL2_PATH $$OutLibFile /y

DEFINES += ARM_SECTION(x)=  __va_list=va_list \
           RTE_Acceleration_Arm_2D_Extra_Benchmark_Watch_Panel \
           RTE_Acceleration_Arm_2D_Helper_Disp_Adapter0
