#ifndef _UI_DEMO_H_
#define _UI_DEMO_H_

#ifdef __cplusplus
extern "C" {
#endif

#define DEMO_WATCH               0
#define DEMO_WIDGET              1

// select target demo
#define TARGET_DEMO              DEMO_WIDGET


// do not eidt below
#if TARGET_DEMO == DEMO_WATCH
#define DEMO_SCEEN_WIDTH         240
#define DEMO_SCEEN_HEIGHT        240
#define LD_ADD_PAGE_DEMO         LD_ADD_PAGE(uiWatch)
#endif

#if TARGET_DEMO == DEMO_WIDGET
#define DEMO_SCEEN_WIDTH         800
#define DEMO_SCEEN_HEIGHT        480
#define LD_ADD_PAGE_DEMO         LD_ADD_PAGE(uiWidget)
#endif

#ifdef __cplusplus
}
#endif

#endif //_UI_DEMO_H_
