#ifndef _LD_CONFIG_H_
#define _LD_CONFIG_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "stdint.h"
#include "stdbool.h"

//-------- <<< Use Configuration Wizard in Context Menu >>> -----------------

// base config

#ifndef LD_CFG_COLOR_DEPTH
// <o> Select the screen colour depth
//     <8=>     8 Bits
//     <16=>    16Bits
//     <32=>    32Bits
// <i> The colour depth of your screen
#define LD_CFG_COLOR_DEPTH                        (16)
#endif

#ifndef LD_CFG_SCREEN_WIDTH
// <o>Width of the screen <8-32767>
// <i> The width of your screen
#define LD_CFG_SCREEN_WIDTH                       (320)
#endif

#ifndef LD_CFG_SCREEN_HEIGHT
// <o>Height of the screen <8-32767>
// <i> The height of your screen
#define LD_CFG_SCREEN_HEIGHT                      (240)
#endif

#ifndef LD_CFG_PFB_WIDTH
// <o>Width of the PFB block
// <i> The width of your PFB block size
#define LD_CFG_PFB_WIDTH                          (320)
#endif

#ifndef LD_CFG_PFB_HEIGHT
// <o>Height of the PFB block
// <i> The height of your PFB block size
#define LD_CFG_PFB_HEIGHT                         (24)
#endif

#ifndef LD_MEM_MODE
// <o>Select Memory Manager
//     <0=>    freertos heap4
//     <1=>    tlfs
//     <2=>    lwmem
//     <3=>    stdlib
//     <4=>    user
// <i> default = 0
#define LD_MEM_MODE                               (0)
#endif

#ifndef LD_MEM_SIZE
// <o>LingDongGui memory size
// <i> The size of memory allocated to the ldgui
#define LD_MEM_SIZE                               (8*1024) //BYTE
#endif

#ifndef USE_VIRTUAL_RESOURCE
// <q>External NOR support
// <i> Read external nor, please enable this option
#define USE_VIRTUAL_RESOURCE                      (0)
#endif

// widget config

#ifndef USE_RADIA_MENU_SCALE
// <q>Radia menu's scale function support
// <i> Radia menu's scale function support
#define USE_RADIA_MENU_SCALE                      (1)
#endif

#ifndef USE_SCENE_SWITCHING
// <o>scene switcing support
//     <0=> One scene
//     <1=> Less ram
//     <2=> More effects
// <i> 0: only one scene,user manually clean widget
// <i> 1: Less ram,fast switching(default)
// <i> 2: More effects
#define USE_SCENE_SWITCHING                       (1)
#endif

#if USE_SCENE_SWITCHING == 0
#define __DISP0_CFG_NANO_ONLY__                   (1)
#else
#define __DISP0_CFG_NANO_ONLY__                   (0)
#endif

// debug config
#ifndef USE_LCD_TEST
// <o> LCD test function
//     <0=>    OFF
//     <1=>    ON
// <i> DrawBitmap porting sanity check
#define USE_LCD_TEST                              (0)
#endif

#ifndef USE_LOG_LEVEL
// <o> Log output level
//     <0=>    LOG_LEVEL_NONE
//     <1=>    LOG_LEVEL_ERROR
//     <2=>    LOG_LEVEL_WARNING
//     <3=>    LOG_LEVEL_INFO
//     <4=>    LOG_LEVEL_DEBUG
// <i> Do not use log. Please select LOG_LEVEL_NONE
#define USE_LOG_LEVEL                             (0)
#endif

#if USE_LOG_LEVEL > 0

#ifndef USE_LOG_COLOR
// <q>Use Color Log
// <i> Use Color Log
#define USE_LOG_COLOR                             (0)
#endif

#ifndef LOG_NEW_LINE_TYPE
// <o> Log new line type
//     <0=>    LF \n
//     <1=>    CR \r
//     <2=>    CRLF \r\n
// <i> default select \n
#define LOG_NEW_LINE_TYPE                         (0)
#endif

#endif

// <q>view dirty region
// <i> Opening this mode will refresh the full screen
#define __DISP0_CFG_DEBUG_DIRTY_REGIONS__         (0)

// <q>ARM 2D output log
// <i> debug mode
#define __ARM_2D_CFG_ENABLE_LOG__                 (0)

// <q>debug layer
// <i> view fps
#define __DISP0_CFG_NAVIGATION_LAYER_MODE__       (0)

// <q>diable ARM 2D default demo
// <i> default=1 use ldgui
#define __DISP0_CFG_DISABLE_DEFAULT_SCENE__       (1)

// <<< end of configuration section >>>

// do not eidt below

#define __DISP0_CFG_PFB_BLOCK_WIDTH__             LD_CFG_PFB_WIDTH
#define __DISP0_CFG_PFB_BLOCK_HEIGHT__            LD_CFG_PFB_HEIGHT
#define __DISP0_CFG_COLOUR_DEPTH__                LD_CFG_COLOR_DEPTH
#define __DISP0_CFG_SCEEN_WIDTH__                 LD_CFG_SCREEN_WIDTH
#define __DISP0_CFG_SCEEN_HEIGHT__                LD_CFG_SCREEN_HEIGHT
#define __DISP0_CFG_FPS_CACULATION_MODE__         (1)
#define __GLCD_CFG_SCEEN_WIDTH__                  LD_CFG_SCREEN_WIDTH
#define __GLCD_CFG_SCEEN_HEIGHT__                 LD_CFG_SCREEN_HEIGHT
#if USE_VIRTUAL_RESOURCE == 0
#define __DISP0_CFG_VIRTUAL_RESOURCE_HELPER__     (0)
#else
#define __DISP0_CFG_VIRTUAL_RESOURCE_HELPER__     (3)
#endif
#define __GLCD_CFG_COLOUR_DEPTH__                 LD_CFG_COLOR_DEPTH

#define __ARM_2D_CFG_SUPPORT_COLOUR_CHANNEL_ACCESS__                  0
#define __ARM_2D_CFG_SUPPORT_CCCA8888_IMPLICIT_CONVERSION__           0
#define __ARM_2D_CFG_SUPPORT_TRANSFORM_FOR_NON_A8_FONTS__             0
#define __ARM_2D_CFG_OPTIMIZE_FOR_PFB_IN_LAYOUT_ASSISTANT__           0
#define __DISP0_CFG_ENABLE_3FB_HELPER_SERVICE__                       0

#ifdef __cplusplus
}
#endif

#endif //_LD_CONFIG_H_
