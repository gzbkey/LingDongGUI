#ifndef _VIRTUAL_TFT_PORT_H_
#define _VIRTUAL_TFT_PORT_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>
#include "ldConfig.h"

//
// 虚拟屏幕设定参数,即需要一个什么样的屏幕
//
#define VT_WIDTH           LD_CFG_SCEEN_WIDTH
#define VT_HEIGHT          LD_CFG_SCEEN_HEIGHT
#define VT_COLOR_DEPTH     LD_CFG_COLOR_DEPTH
#define VT_VIRTUAL_MACHINE 0                   /*Different rendering should be used if running in a Virtual machine*/

#if VT_COLOR_DEPTH == 1 || VT_COLOR_DEPTH == 8 || VT_COLOR_DEPTH == 16 || VT_COLOR_DEPTH == 24 || VT_COLOR_DEPTH == 32
#if VT_COLOR_DEPTH == 1 || VT_COLOR_DEPTH == 8
typedef uint8_t color_typedef;
#elif VT_COLOR_DEPTH == 16
typedef uint16_t color_typedef;
#elif VT_COLOR_DEPTH == 24 || VT_COLOR_DEPTH == 32
typedef uint32_t color_typedef;
#endif
#else
#error "Invalid VT_COLOR_DEPTH in Virtual_TFT_Port.h"
#endif

extern bool keyUp;
extern bool keyDown;
extern bool keyLeft;
extern bool keyRight;
extern bool keyEnter;
extern bool keyEsc;


void vtInit(void);
bool vtIsRequestQuit(void);
void vtDeinit(void);
void vtSdlFlush(int32_t nMS);
void vtSdlRefreshTask(void);

void vtFillSingleColor(int32_t x1, int32_t y1, int32_t x2, int32_t y2, color_typedef color);
void vtFillMultipleColors(int32_t x1, int32_t y1, int32_t x2, int32_t y2, color_typedef * color_p);
void vtSetPoint(int32_t x, int32_t y, color_typedef color);
color_typedef vtGetPoint(int32_t x, int32_t y);
bool vtMouseGetPoint(int16_t *x,int16_t *y);
bool vtGetKeyState(size_t value);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif //_VIRTUAL_TFT_PORT_H_
