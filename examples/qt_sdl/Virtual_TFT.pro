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
    ../common/virtual_flash/virtualNor.c \
    user/ldConfig.c \
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
    ../common/virtual_flash/virtualNor.h \
    user/Virtual_TFT_Port.h \
    user/arm_2d_cfg.h \
    user/arm_2d_disp_adapter_0.h \
    user/arm_2d_user_arch_port.h \
    user/ldConfig.h

HEADERS += $$files(../common/demo/*.h, true) \
           $$files(../common/Arm-2D/examples/common/controls/*.h, false) \
           $$files(../common/Arm-2D/Library/Source/*.inc, false) \
           $$files(../common/Arm-2D/Library/Include/*.h, false) \
           $$files(../common/Arm-2D/Helper/Include/*.h, false)

SOURCES += $$files(../common/demo/*.c, true) \
           $$files(../common/Arm-2D/examples/common/controls/*.c, false) \
           $$files(../common/Arm-2D/examples/common/asset/*.c, false) \
           $$files(../common/Arm-2D/Library/Source/*.c, false) \
           $$files(../common/Arm-2D/Helper/Source/*.c, false) \
           $$files(../common/math/*.c, true)

INCLUDEPATH += $$PWD/../common/Arm-2D
INCLUDEPATH += $$PWD/../common/Arm-2D/Helper/Include
INCLUDEPATH += $$PWD/../common/Arm-2D/Library/Include
INCLUDEPATH += $$PWD/../common/Arm-2D/examples/common/controls

INCLUDEPATH += $$PWD/user
INCLUDEPATH += $$PWD/../common/math
INCLUDEPATH += $$PWD/../common/virtual_flash
#INCLUDEPATH += $$PWD/../common/demo/watch
INCLUDEPATH += $$files(../common/demo/*, true)

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


