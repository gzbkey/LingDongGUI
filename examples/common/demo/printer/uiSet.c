#include "uiSet.h"
#include "ldGui.h"

void uiSetInit(ld_scene_t* ptScene);
void uiSetLoop(ld_scene_t* ptScene);
void uiSetQuit(ld_scene_t* ptScene);


const ldPageFuncGroup_t uiSetFunc={
    .init=uiSetInit,
    .loop=uiSetLoop,
    .quit=uiSetQuit,
#if (USE_LOG_LEVEL>=LOG_LEVEL_INFO)
    .pageName="uiSet",
#endif
};

void uiSetInit(ld_scene_t* ptScene)
{
    void *obj,*win;

    obj=ldWindowInit(ID_BG, ID_BG, 0, 0, LD_CFG_SCEEN_WIDTH, LD_CFG_SCEEN_HEIGHT);



}

void uiSetLoop(ld_scene_t* ptScene)
{




}

void uiSetQuit(ld_scene_t* ptScene)
{




}
