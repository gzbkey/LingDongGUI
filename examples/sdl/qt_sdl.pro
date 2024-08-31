TEMPLATE = app
CONFIG -= app_bundle
CONFIG -= qt

#//     <0=> None
#//     <1=> Show all widget
#//     <2=> Printer
USE_DEMO=2

SOURCES += \
    virtualNor/virtualNor.c \
    user/ldConfig.c \
    user/main.c \
    user/Virtual_TFT_Port.c \
    user/arm_2d_disp_adapter_0.c

HEADERS += \
    virtualNor/virtualNor.h \
    user/Virtual_TFT_Port.h \
    user/arm_2d_cfg.h \
    user/arm_2d_disp_adapter_0.h \
    user/arm_2d_user_arch_port.h \
    user/ldConfig.h

HEADERS += $$files(../common/Arm-2D/examples/common/controls/*.h) \
           $$files(../common/Arm-2D/Library/Source/*.inc) \
           $$files(../common/Arm-2D/Library/Include/*.h) \
           $$files(../common/Arm-2D/Helper/Include/*.h) \
           $$files(../common/math/*.h) \
           $$files(../../src/gui/*.h) \
           $$files(../../src/misc/*.h)

SOURCES += $$files(../common/Arm-2D/examples/common/controls/*.c) \
           $$files(../common/Arm-2D/examples/common/asset/*.c) \
           $$files(../common/Arm-2D/Library/Source/*.c) \
           $$files(../common/Arm-2D/Helper/Source/*.c) \
           $$files(../common/math/*.c, true) \
           $$files(../../src/gui/*.c) \
           $$files(../../src/misc/*.c)

contains(USE_DEMO, 1){
message(demo: Show all widget)
DEFINES += USE_DEMO=1
HEADERS += $$files(../common/demo/widget/*.h, true)
SOURCES += $$files(../common/demo/widget/*.c, true)
INCLUDEPATH += $$PWD/../common/demo/widget
}

contains(USE_DEMO, 2){
message(demo: Printer)
DEFINES += USE_DEMO=2
HEADERS += $$files(../common/demo/printer/*.h, true)
SOURCES += $$files(../common/demo/printer/*.c, true)
INCLUDEPATH += $$PWD/../common/demo/printer
}


INCLUDEPATH += $$PWD/../common/Arm-2D
INCLUDEPATH += $$PWD/../common/Arm-2D/Helper/Include
INCLUDEPATH += $$PWD/../common/Arm-2D/Library/Include
INCLUDEPATH += $$PWD/../common/Arm-2D/examples/common/controls

INCLUDEPATH += $$PWD/user
INCLUDEPATH += $$PWD/virtualNor
INCLUDEPATH += $$PWD/../common/math

INCLUDEPATH += $$PWD/../../src/gui
INCLUDEPATH += $$PWD/../../src/misc

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


