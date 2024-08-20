#include "uiWait.h"
#include "ldGui.h"

void uiWaitInit(ld_scene_t* ptScene);
void uiWaitLoop(ld_scene_t* ptScene);
void uiWaitQuit(ld_scene_t* ptScene);


const ldPageFuncGroup_t uiWaitFunc={
    .init=uiWaitInit,
    .loop=uiWaitLoop,
    .quit=uiWaitQuit,
#if (USE_LOG_LEVEL>=LOG_LEVEL_INFO)
    .pageName="uiWait",
#endif
};

void uiWaitInit(ld_scene_t* ptScene)
{
    void *obj,*win;

    obj=ldWindowInit(ID_BG, ID_BG, 0, 0, LD_CFG_SCEEN_WIDTH, LD_CFG_SCEEN_HEIGHT);



}

void uiWaitLoop(ld_scene_t* ptScene)
{




}

void uiWaitQuit(ld_scene_t* ptScene)
{




}
