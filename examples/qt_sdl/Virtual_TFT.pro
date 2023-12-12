TEMPLATE = app
#CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
    ../../src/gui/ldArc.c \
    ../../src/gui/ldButton.c \
    ../../src/gui/ldCheckBox.c \
    ../../src/gui/ldComboBox.c \
    ../../src/gui/ldCommon.c \
    ../../src/gui/ldDateTime.c \
    ../../src/gui/ldGauge.c \
    ../../src/gui/ldGraph.c \
    ../../src/gui/ldGui.c \
    ../../src/gui/ldIconSlider.c \
    ../../src/gui/ldImage.c \
    ../../src/gui/ldKeyboard.c \
    ../../src/gui/ldLabel.c \
    ../../src/gui/ldLineEdit.c \
    ../../src/gui/ldQRCode.c \
    ../../src/gui/ldRadialMenu.c \
    ../../src/gui/ldProgressBar.c \
    ../../src/gui/ldScene0.c \
    ../../src/gui/ldScrollSelecter.c \
    ../../src/gui/ldTable.c \
    ../../src/gui/ldText.c \
    ../../src/gui/ldWindow.c \
    ../../src/misc/freeRtosHeap4.c \
    ../../src/misc/qrcodegen.c \
    ../../src/misc/tlsf.c \
    ../../src/misc/xBtnAction.c \
    ../../src/misc/xConnect.c \
    ../../src/misc/xList.c \
    ../../src/misc/xQueue.c \
    ../../src/misc/xString.c \
    ../common/Arm-2D/Helper/Source/arm_2d_helper.c \
    ../common/Arm-2D/Helper/Source/arm_2d_helper_list.c \
    ../common/Arm-2D/Helper/Source/arm_2d_helper_map.c \
    ../common/Arm-2D/Helper/Source/arm_2d_helper_pfb.c \
    ../common/Arm-2D/Helper/Source/arm_2d_helper_scene.c \
    ../common/Arm-2D/Library/Source/__arm_2d_colour_filling_with_alpha.c \
    ../common/Arm-2D/Library/Source/__arm_2d_conversion_helium.c \
    ../common/Arm-2D/Library/Source/__arm_2d_core.c \
    ../common/Arm-2D/Library/Source/__arm_2d_tile.c \
    ../common/Arm-2D/Library/Source/__arm_2d_transform_helium.c \
    ../common/Arm-2D/Library/Source/arm_2d.c \
    ../common/Arm-2D/Library/Source/arm_2d_alpha_blending.c \
    ../common/Arm-2D/Library/Source/arm_2d_async.c \
    ../common/Arm-2D/Library/Source/arm_2d_conversion.c \
    ../common/Arm-2D/Library/Source/arm_2d_draw.c \
    ../common/Arm-2D/Library/Source/arm_2d_helium.c \
    ../common/Arm-2D/Library/Source/arm_2d_transform.c \
    ../common/Arm-2D/examples/common/asset/DigitsFont.c \
    ../common/Arm-2D/examples/common/asset/GreenCircle.c \
    ../common/Arm-2D/examples/common/asset/Lighting.c \
    ../common/Arm-2D/examples/common/asset/ListCover.c \
    ../common/Arm-2D/examples/common/asset/MeterPanel.c \
    ../common/Arm-2D/examples/common/asset/Pointer.c \
    ../common/Arm-2D/examples/common/asset/QuaterArc.c \
    ../common/Arm-2D/examples/common/asset/SinWave.c \
    ../common/Arm-2D/examples/common/asset/SpinWheel.c \
    ../common/Arm-2D/examples/common/asset/arrow.c \
    ../common/Arm-2D/examples/common/asset/background.c \
    ../common/Arm-2D/examples/common/asset/battery_boarder_1.c \
    ../common/Arm-2D/examples/common/asset/battery_gasgauge_block.c \
    ../common/Arm-2D/examples/common/asset/battery_gasgauge_grade_boarder.c \
    ../common/Arm-2D/examples/common/asset/blue_slashes.c \
    ../common/Arm-2D/examples/common/asset/circle_mask.c \
    ../common/Arm-2D/examples/common/asset/circle_small.c \
    ../common/Arm-2D/examples/common/asset/clockface.c \
    ../common/Arm-2D/examples/common/asset/cmsis_logo.c \
    ../common/Arm-2D/examples/common/asset/gear_01.c \
    ../common/Arm-2D/examples/common/asset/gear_02.c \
    ../common/Arm-2D/examples/common/asset/glass_reflection_narrow.c \
    ../common/Arm-2D/examples/common/asset/glass_reflection_wide.c \
    ../common/Arm-2D/examples/common/asset/green28_28.c \
    ../common/Arm-2D/examples/common/asset/helium.c \
    ../common/Arm-2D/examples/common/asset/pointer_sec.c \
    ../common/Arm-2D/examples/common/asset/small_icon_sun.c \
    ../common/Arm-2D/examples/common/asset/software.c \
    ../common/Arm-2D/examples/common/asset/star.c \
    ../common/Arm-2D/examples/common/asset/watch_panel.c \
    ../common/Arm-2D/examples/common/asset/wave.c \
    ../common/Arm-2D/examples/common/asset/white_dot.c \
    ../common/Arm-2D/examples/common/asset/wifi_signal.c \
    ../common/Arm-2D/examples/common/controls/GLCD_Fonts.c \
    ../common/Arm-2D/examples/common/controls/battery_gasgauge.c \
    ../common/Arm-2D/examples/common/controls/busy_wheel.c \
    ../common/Arm-2D/examples/common/controls/controls.c \
    ../common/Arm-2D/examples/common/controls/dynamic_nebula.c \
    ../common/Arm-2D/examples/common/controls/lcd_printf.c \
    ../common/Arm-2D/examples/common/controls/list_view.c \
    ../common/Arm-2D/examples/common/controls/number_list.c \
    ../common/Arm-2D/examples/common/controls/progress_bar_drill.c \
    ../common/Arm-2D/examples/common/controls/progress_bar_flowing.c \
    ../common/Arm-2D/examples/common/controls/progress_bar_simple.c \
    ../common/Arm-2D/examples/common/controls/progress_wheel.c \
    ../common/Arm-2D/examples/common/controls/shape_round_corner_box.c \
    ../common/Arm-2D/examples/common/controls/spinning_wheel.c \
    ../common/virtual_flash/virtualNor.c \
    ../common/math/arm_common_tables.c \
    ../common/math/arm_cos_f32.c \
    ../common/math/arm_cos_q31.c \
    ../common/math/arm_sin_f32.c \
    ../common/math/arm_sin_q31.c \
    user/image.c \
    user/ldConfig.c \
    user/ldUser.c \
    user/main.c \
    user/Virtual_TFT_Port.c \
    user/arm_2d_disp_adapter_0.c

HEADERS += \
    ../../src/gui/ldArc.h \
    ../../src/gui/ldButton.h \
    ../../src/gui/ldCheckBox.h \
    ../../src/gui/ldComboBox.h \
    ../../src/gui/ldCommon.h \
    ../../src/gui/ldDateTime.h \
    ../../src/gui/ldGauge.h \
    ../../src/gui/ldGraph.h \
    ../../src/gui/ldGui.h \
    ../../src/gui/ldIconSlider.h \
    ../../src/gui/ldImage.h \
    ../../src/gui/ldKeyboard.h \
    ../../src/gui/ldLabel.h \
    ../../src/gui/ldLineEdit.h \
    ../../src/gui/ldQRCode.h \
    ../../src/gui/ldRadialMenu.h \
    ../../src/gui/ldProgressBar.h \
    ../../src/gui/ldScene0.h \
    ../../src/gui/ldScrollSelecter.h \
    ../../src/gui/ldTable.h \
    ../../src/gui/ldText.h \
    ../../src/gui/ldWindow.h \
    ../../src/misc/freeRtosHeap4.h \
    ../../src/misc/qrcodegen.h \
    ../../src/misc/tlsf.h \
    ../../src/misc/xBtnAction.h \
    ../../src/misc/xConnect.h \
    ../../src/misc/xList.h \
    ../../src/misc/xLog.h \
    ../../src/misc/xQueue.h \
    ../../src/misc/xString.h \
    ../common/Arm-2D/Helper/Include/__arm_2d_helper_common.h \
    ../common/Arm-2D/Helper/Include/arm_2d_disp_adapters.h \
    ../common/Arm-2D/Helper/Include/arm_2d_helper.h \
    ../common/Arm-2D/Helper/Include/arm_2d_helper_list.h \
    ../common/Arm-2D/Helper/Include/arm_2d_helper_map.h \
    ../common/Arm-2D/Helper/Include/arm_2d_helper_pfb.h \
    ../common/Arm-2D/Helper/Include/arm_2d_helper_scene.h \
    ../common/Arm-2D/Helper/Include/arm_2d_scenes.h \
    ../common/Arm-2D/Library/Include/__arm_2d_direct.h \
    ../common/Arm-2D/Library/Include/__arm_2d_impl.h \
    ../common/Arm-2D/Library/Include/__arm_2d_math.h \
    ../common/Arm-2D/Library/Include/__arm_2d_math_helium.h \
    ../common/Arm-2D/Library/Include/__arm_2d_paving.h \
    ../common/Arm-2D/Library/Include/__arm_2d_paving_helium.h \
    ../common/Arm-2D/Library/Include/__arm_2d_utils_helium.h \
    ../common/Arm-2D/Library/Include/arm_2d.h \
    ../common/Arm-2D/Library/Include/arm_2d_alpha_blending.h \
    ../common/Arm-2D/Library/Include/arm_2d_conversion.h \
    ../common/Arm-2D/Library/Include/arm_2d_draw.h \
    ../common/Arm-2D/Library/Include/arm_2d_features.h \
    ../common/Arm-2D/Library/Include/arm_2d_op.h \
    ../common/Arm-2D/Library/Include/arm_2d_tile.h \
    ../common/Arm-2D/Library/Include/arm_2d_transform.h \
    ../common/Arm-2D/Library/Include/arm_2d_types.h \
    ../common/Arm-2D/Library/Include/arm_2d_utils.h \
    ../common/Arm-2D/Library/Source/__arm_2d_alpha_blending_wrapper.inc \
    ../common/Arm-2D/Library/Source/__arm_2d_copy_mirror_wrapper.inc \
    ../common/Arm-2D/Library/Source/__arm_2d_copy_with_colour_keying_and_mirror_wrapper.inc \
    ../common/Arm-2D/Library/Source/__arm_2d_copy_with_mask_and_mirror_wrapper.inc \
    ../common/Arm-2D/Library/Source/__arm_2d_ll_alpha_blending.inc \
    ../common/Arm-2D/Library/Source/__arm_2d_ll_alpha_mask.inc \
    ../common/Arm-2D/Library/Source/__arm_2d_ll_alpha_mask_helium.inc \
    ../common/Arm-2D/Library/Source/__arm_2d_ll_colour_filling_with_alpha.inc \
    ../common/Arm-2D/Library/Source/__arm_2d_ll_copy.inc \
    ../common/Arm-2D/Library/Source/__arm_2d_ll_copy_helium.inc \
    ../common/Arm-2D/Library/Source/__arm_2d_ll_draw_pattern.inc \
    ../common/Arm-2D/Library/Source/__arm_2d_ll_draw_pattern_helium.inc \
    ../common/Arm-2D/Library/Source/__arm_2d_ll_fill_colour.inc \
    ../common/Arm-2D/Library/Source/__arm_2d_ll_fill_colour_helium.inc \
    ../common/Arm-2D/Library/Source/__arm_2d_ll_meta_copy_and_fill.inc \
    ../common/Arm-2D/Library/Source/__arm_2d_ll_meta_copy_with_masks.inc \
    ../common/Arm-2D/Library/Source/__arm_2d_ll_meta_copy_with_masks_helium.inc \
    ../common/Arm-2D/Library/Source/__arm_2d_ll_meta_trans_with_masks.inc \
    ../common/Arm-2D/Library/Source/__arm_2d_ll_meta_trans_with_masks_helium.inc \
    ../common/Arm-2D/Library/Source/__arm_2d_ll_transform.inc \
    ../common/Arm-2D/Library/Source/__arm_2d_ll_transform_helium.inc \
    ../common/Arm-2D/Library/Source/__arm_2d_meta_copy_mirror.inc \
    ../common/Arm-2D/Library/Source/__arm_2d_meta_copy_with_colour_keying_and_mirror.inc \
    ../common/Arm-2D/Library/Source/__arm_2d_meta_copy_with_des_mask_and_mirror.inc \
    ../common/Arm-2D/Library/Source/__arm_2d_meta_copy_with_masks_and_mirror.inc \
    ../common/Arm-2D/Library/Source/__arm_2d_meta_copy_with_src_mask_and_mirror.inc \
    ../common/Arm-2D/Library/Source/__arm_2d_meta_fill_mirror.inc \
    ../common/Arm-2D/Library/Source/__arm_2d_meta_fill_with_colour_keying_and_mirror.inc \
    ../common/Arm-2D/Library/Source/__arm_2d_meta_fill_with_des_mask_and_mirror.inc \
    ../common/Arm-2D/Library/Source/__arm_2d_meta_fill_with_masks_and_mirror.inc \
    ../common/Arm-2D/Library/Source/__arm_2d_meta_fill_with_src_mask_and_mirror.inc \
    ../common/Arm-2D/examples/common/controls/__common.h \
    ../common/Arm-2D/examples/common/controls/arm_extra_controls.h \
    ../common/Arm-2D/examples/common/controls/arm_extra_lcd_printf.h \
    ../common/Arm-2D/examples/common/controls/battery_gasgauge.h \
    ../common/Arm-2D/examples/common/controls/busy_wheel.h \
    ../common/Arm-2D/examples/common/controls/dynamic_nebula.h \
    ../common/Arm-2D/examples/common/controls/list_view.h \
    ../common/Arm-2D/examples/common/controls/number_list.h \
    ../common/Arm-2D/examples/common/controls/progress_bar_drill.h \
    ../common/Arm-2D/examples/common/controls/progress_bar_flowing.h \
    ../common/Arm-2D/examples/common/controls/progress_bar_simple.h \
    ../common/Arm-2D/examples/common/controls/progress_wheel.h \
    ../common/Arm-2D/examples/common/controls/shape_round_corner_box.h \
    ../common/Arm-2D/examples/common/controls/spinning_wheel.h \
    ../common/virtual_flash/virtualNor.h \
    user/Virtual_TFT_Port.h \
    user/arm_2d_cfg.h \
    user/arm_2d_disp_adapter_0.h \
    user/arm_2d_user_arch_port.h \
    user/image.h \
    user/ldConfig.h \
    user/ldUser.h

INCLUDEPATH += $$PWD/../common/Arm-2D
INCLUDEPATH += $$PWD/../common/Arm-2D/Helper/Include
INCLUDEPATH += $$PWD/../common/Arm-2D/Library/Include
INCLUDEPATH += $$PWD/../common/Arm-2D/examples/common/controls

INCLUDEPATH += $$PWD/user
INCLUDEPATH += $$PWD/../common/math
INCLUDEPATH += $$PWD/../common/virtual_flash

INCLUDEPATH += $$PWD/../../src/gui
INCLUDEPATH += $$PWD/../../src/misc

contains(QT_ARCH, i386){
INCLUDEPATH += $$PWD/../common/sdl2/32/include/SDL2
LIBS += -L$$PWD/../common/sdl2/32/lib -lSDL2
LIBS += -L$$PWD/../common/sdl2/32/lib -lSDL2main
SDL2_PATH = $$PWD/../common/sdl2/32/bin/SDL2.dll
}else{
INCLUDEPATH += $$PWD/../common/sdl2/64/include/SDL2
LIBS += -L$$PWD/../common/sdl2/64/lib -lSDL2
LIBS += -L$$PWD/../common/sdl2/64/lib -lSDL2main
SDL2_PATH = $$PWD/../common/sdl2/64/bin/SDL2.dll
}

SDL2_PATH = $$replace(SDL2_PATH, /, \\)
OutLibFile = $$OUT_PWD/SDL2.dll
OutLibFile = $$replace(OutLibFile, /, \\)
QMAKE_PRE_LINK +=  copy $$SDL2_PATH $$OutLibFile /y

DEFINES += ARM_SECTION(x)=  \
           __va_list=va_list \
           RTE_Acceleration_Arm_2D_Helper_Disp_Adapter0

LIBS += -lpthread

QMAKE_CFLAGS  +=  -std=gnu11 -MMD -g
QMAKE_CFLAGS  +=  -ffunction-sections -fdata-sections
QMAKE_CFLAGS  +=  -fno-ms-extensions
QMAKE_CFLAGS  +=  -Wno-macro-redefined
QMAKE_CFLAGS  +=  -Ofast
QMAKE_CFLAGS  +=  -flto


