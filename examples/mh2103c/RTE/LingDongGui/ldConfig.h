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

// <o>LingDongGui memory size
// <i> The size of memory allocated to the ldgui
#define LD_MEM_SIZE                               (16*1024) //BYTE

// <o>LingDongGui signal emit buffer size
// <i> The size of signal emit buffer to the ldgui
#define LD_EMIT_SIZE                              (8)

// <o>Number of project pages
// <i> Maximum number of pages
#define LD_PAGE_MAX                               (1)

// <q>Dirty Region Function
// <i> It is recommended to enable this option for applications that do not refresh full screen
#define USE_DIRTY_REGION                          (1)

// <q>External NOR support
// <i> Read external nor, please enable this option
#define USE_VIRTUAL_RESOURCE                      (0)

// widget config

// <q>Opacity support
// <i> There is no need to adjust the opacity of the widget. Please turn off this option
#define USE_OPACITY                               (0)

// <q>tlsf support
// <i> Two Level Segregated Fit memory allocator
#define USE_TLSF                                  (0)

// <q>Radia menu's scale function support
// <i> Radia menu's scale function support
#define USE_RADIA_MENU_SCALE                      (0)

// debug config

// <o> Log output level
//     <0=>    LOG_LEVEL_NONE
//     <1=>    LOG_LEVEL_ERROR
//     <2=>    LOG_LEVEL_WARNING
//     <3=>    LOG_LEVEL_INFO
//     <4=>    LOG_LEVEL_DEBUG
// <i> Do not use log. Please select LOG_LEVEL_NONE
#define USE_LOG_LEVEL                             (0)

#define LD_DEBUG                                  (0)
#define __DISP0_CFG_DEBUG_DIRTY_REGIONS__         (0)

// default = 1
#define LD_PAGE_STATIC                            (1)

// <<< end of configuration section >>>

// do not eidt below

#define __DISP0_CFG_NAVIGATION_LAYER_MODE__       (0)
#define __DISP0_CFG_DISABLE_DEFAULT_SCENE__       (1)
#define __DISP0_CFG_PFB_BLOCK_WIDTH__             LD_CFG_PFB_WIDTH
#define __DISP0_CFG_PFB_BLOCK_HEIGHT__            LD_CFG_PFB_HEIGHT
#define __DISP0_CFG_COLOUR_DEPTH__                LD_CFG_COLOR_DEPTH
#define __DISP0_CFG_SCEEN_WIDTH__                 LD_CFG_SCEEN_WIDTH
#define __DISP0_CFG_SCEEN_HEIGHT__                LD_CFG_SCEEN_HEIGHT
#if USE_VIRTUAL_RESOURCE == 0
#define __DISP0_CFG_VIRTUAL_RESOURCE_HELPER__     (0)
#else
#define __DISP0_CFG_VIRTUAL_RESOURCE_HELPER__     (2)
#endif
#define __GLCD_CFG_COLOUR_DEPTH__                 LD_CFG_COLOR_DEPTH

bool ldCfgTouchGetPoint(int16_t *x,int16_t *y);

#ifdef __cplusplus
}
#endif

#endif //_LD_CONFIG_H_
