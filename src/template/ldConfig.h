#ifndef _LD_CONFIG_H_
#define _LD_CONFIG_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "stdint.h"
#include "stdbool.h"

//-------- <<< Use Configuration Wizard in Context Menu >>> -----------------

// base config

// <o> Select the screen colour depth
//     <8=>     8 Bits
//     <16=>    16Bits
//     <32=>    32Bits
// <i> The colour depth of your screen
#define LD_CFG_COLOR_DEPTH                        (16)

// <o>Width of the screen <8-32767>
// <i> The width of your screen
#define LD_CFG_SCEEN_WIDTH                        (320)

// <o>Height of the screen <8-32767>
// <i> The height of your screen
#define LD_CFG_SCEEN_HEIGHT                       (240)

// <o>Width of the PFB block
// <i> The width of your PFB block size
#define LD_CFG_PFB_WIDTH                          (320)

// <o>Height of the PFB block
// <i> The height of your PFB block size
#define LD_CFG_PFB_HEIGHT                         (24)

// <o>Select Memory Manager
//     <0=>    freertos heap4
//     <1=>    tlfs
//     <2=>    stdlib
//     <3=>    user
// <i> default = 0
#define LD_MEM_MODE                               (0)

// <o>LingDongGui memory size
// <i> The size of memory allocated to the ldgui
#define LD_MEM_SIZE                               (16*1024) //BYTE

// <q>External NOR support
// <i> Read external nor, please enable this option
#define USE_VIRTUAL_RESOURCE                      (0)

// widget config

// <q>Radia menu's scale function support
// <i> Radia menu's scale function support
#define USE_RADIA_MENU_SCALE                      (1)

// <q>scene switcing support
// <i> 1:More effects 0£ºLess ram
#define USE_SCENE_SWITCHING                       (1)

// debug config

// <o> Log output level
//     <0=>    LOG_LEVEL_NONE
//     <1=>    LOG_LEVEL_ERROR
//     <2=>    LOG_LEVEL_WARNING
//     <3=>    LOG_LEVEL_INFO
//     <4=>    LOG_LEVEL_DEBUG
// <i> Do not use log. Please select LOG_LEVEL_NONE
#define USE_LOG_LEVEL                             (0)

#if USE_LOG_LEVEL > 0
// <q>Use Color Log
// <i> Use Color Log
#define USE_LOG_COLOR                             (0)

// <o> Log new line type
//     <0=>    LF \n
//     <1=>    CR \r
//     <2=>    CRLF \r\n
// <i> default select \n
#define LOG_NEW_LINE_TYPE                         (0)
#endif

#define LD_DEBUG                                  (0)
#define __DISP0_CFG_DEBUG_DIRTY_REGIONS__         (0)
#define __ARM_2D_CFG_ENABLE_LOG__                 (0)

#ifndef USE_DEMO
// <o> choose demo to test
//     <0=> None
//     <1=> Show all widget
//     <2=> Printer
#define USE_DEMO                                  (0)
#endif

#if USE_DEMO == 1
#undef LD_CFG_COLOR_DEPTH
#define LD_CFG_COLOR_DEPTH                        (16)
#undef LD_CFG_SCEEN_WIDTH
#define LD_CFG_SCEEN_WIDTH                        (1024)
#undef LD_CFG_SCEEN_HEIGHT
#define LD_CFG_SCEEN_HEIGHT                       (600)
#undef LD_CFG_PFB_WIDTH
#define LD_CFG_PFB_WIDTH                          (LD_CFG_SCEEN_WIDTH)
#undef LD_CFG_PFB_HEIGHT
#define LD_CFG_PFB_HEIGHT                         (LD_CFG_SCEEN_HEIGHT/10)
#define LD_DEMO_GUI_INCLUDE                       "uiWidget.h"
#define LD_DEMO_GUI_FUNC                          &uiWidgetFunc
#endif

#if USE_DEMO == 2
#undef LD_CFG_COLOR_DEPTH
#define LD_CFG_COLOR_DEPTH                        (16)
#undef LD_CFG_SCEEN_WIDTH
#define LD_CFG_SCEEN_WIDTH                        (480)
#undef LD_CFG_SCEEN_HEIGHT
#define LD_CFG_SCEEN_HEIGHT                       (272)
#undef LD_CFG_PFB_WIDTH
#define LD_CFG_PFB_WIDTH                          (LD_CFG_SCEEN_WIDTH)
#undef LD_CFG_PFB_HEIGHT
#define LD_CFG_PFB_HEIGHT                         (LD_CFG_SCEEN_HEIGHT/10)
#define LD_DEMO_GUI_INCLUDE                       "uiLogo.h"
#define LD_DEMO_GUI_FUNC                          &uiLogoFunc
#endif

// <<< end of configuration section >>>

// do not eidt below

#define __DISP0_CFG_NAVIGATION_LAYER_MODE__       (0)
#define __DISP0_CFG_DISABLE_DEFAULT_SCENE__       (1)
#define __DISP0_CFG_PFB_BLOCK_WIDTH__             LD_CFG_PFB_WIDTH
#define __DISP0_CFG_PFB_BLOCK_HEIGHT__            LD_CFG_PFB_HEIGHT
#define __DISP0_CFG_COLOUR_DEPTH__                LD_CFG_COLOR_DEPTH
#define __DISP0_CFG_SCEEN_WIDTH__                 LD_CFG_SCEEN_WIDTH
#define __DISP0_CFG_SCEEN_HEIGHT__                LD_CFG_SCEEN_HEIGHT
#define __DISP0_CFG_FPS_CACULATION_MODE__         (1)
#define __GLCD_CFG_SCEEN_WIDTH__                  LD_CFG_SCEEN_WIDTH
#define __GLCD_CFG_SCEEN_HEIGHT__                 LD_CFG_SCEEN_HEIGHT
#if USE_VIRTUAL_RESOURCE == 0
#define __DISP0_CFG_VIRTUAL_RESOURCE_HELPER__     (0)
#else
#define __DISP0_CFG_VIRTUAL_RESOURCE_HELPER__     (3)
#endif
#define __GLCD_CFG_COLOUR_DEPTH__                 LD_CFG_COLOR_DEPTH

bool ldCfgTouchGetPoint(int16_t *x,int16_t *y);

#ifdef __cplusplus
}
#endif

#endif //_LD_CONFIG_H_
