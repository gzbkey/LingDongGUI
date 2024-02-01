#ifndef _UI_WATCH_H_
#define _UI_WATCH_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "ldGui.h"
#include "image.h"

void uiWatchInit(uint8_t page);
void uiWatchLoop(uint8_t page);
void uiWatchQuit(uint8_t page);

#ifdef __cplusplus
}
#endif

#endif //_UI_WATCH_H_
