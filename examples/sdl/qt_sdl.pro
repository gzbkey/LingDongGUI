TEMPLATE = app
#CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
    ../../src/gui/ldBase.c \
    ../../src/gui/ldButton.c \
    ../../src/gui/ldGui.c \
    ../../src/gui/ldImage.c \
    ../../src/gui/ldScene0.c \
    virtualNor/virtualNor.c \
    user/ldConfig.c \
    user/main.c \
    user/Virtual_TFT_Port.c \
    user/arm_2d_disp_adapter_0.c

HEADERS += \
    ../../src/gui/ldBase.h \
    ../../src/gui/ldButton.h \
    ../../src/gui/ldGui.h \
    ../../src/gui/ldImage.h \
    ../../src/gui/ldScene0.h \
    ../../src/gui/ldWindow.h \
    virtualNor/virtualNor.h \
    user/Virtual_TFT_Port.h \
    user/arm_2d_cfg.h \
    user/arm_2d_disp_adapter_0.h \
    user/arm_2d_user_arch_port.h \
    user/ldConfig.h

HEADERS += \ #$$files(../common/demo/*.h, true) \
           $$files(../common/Arm-2D/examples/common/controls/*.h) \
           $$files(../common/Arm-2D/Library/Source/*.inc) \
           $$files(../common/Arm-2D/Library/Include/*.h) \
           $$files(../common/Arm-2D/Helper/Include/*.h)

SOURCES += \ #$$files(../common/demo/*.c, true) \
           $$files(../common/Arm-2D/examples/common/controls/*.c) \
           $$files(../common/Arm-2D/examples/common/asset/*.c) \
           $$files(../common/Arm-2D/Library/Source/*.c) \
           $$files(../common/Arm-2D/Helper/Source/*.c) \
           $$files(../common/math/*.c, true)

INCLUDEPATH += $$PWD/../common/Arm-2D
INCLUDEPATH += $$PWD/../common/Arm-2D/Helper/Include
INCLUDEPATH += $$PWD/../common/Arm-2D/Library/Include
INCLUDEPATH += $$PWD/../common/Arm-2D/examples/common/controls

INCLUDEPATH += $$PWD/user
INCLUDEPATH += $$PWD/virtualNor
INCLUDEPATH += $$PWD/../common/math
#INCLUDEPATH += $$PWD/../common/demo
#INCLUDEPATH += $$files(../common/demo/*, true)

INCLUDEPATH += $$PWD/../../src/gui
INCLUDEPATH += $$PWD/../../src/misc
INCLUDEPATH += $$PWD/../../src/template

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

LD_CFG = "ldConfig.h"

DEFINES += __ARM_2D_USER_APP_CFG_H__=\"\\\"$${LD_CFG}\\\"\" \
           ARM_SECTION(x)=  \
           __va_list=va_list \
           RTE_Acceleration_Arm_2D_Helper_Disp_Adapter0

LIBS += -lpthread

QMAKE_CFLAGS  +=  -std=gnu11 -MMD -g
QMAKE_CFLAGS  +=  -ffunction-sections -fdata-sections
QMAKE_CFLAGS  +=  -fno-ms-extensions
QMAKE_CFLAGS  +=  -Wno-macro-redefined
QMAKE_CFLAGS  +=  -Ofast
QMAKE_CFLAGS  +=  -flto


