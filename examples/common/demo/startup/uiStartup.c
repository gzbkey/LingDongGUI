#include "uiStartup.h"
#include "ldGui.h"

void uiStartupInit(ld_scene_t* ptScene);
void uiStartupLoop(ld_scene_t* ptScene);
void uiStartupQuit(ld_scene_t* ptScene);
void uiStartupDraw(ld_scene_t* ptScene, arm_2d_tile_t *ptTile, bool bIsNewFrame);

const ldPageFuncGroup_t uiStartupFunc={
    .init=uiStartupInit,
    .loop=uiStartupLoop,
    .quit=uiStartupQuit,
    .draw=uiStartupDraw,
#if (USE_LOG_LEVEL>=LOG_LEVEL_INFO)
    .pageName="uiStartup",
#endif
};

void uiStartupInit(ld_scene_t* ptScene)
{
    void *obj,*win;

    obj=ldWindowInit(ID_BG, ID_BG, 0, 0, LD_CFG_SCEEN_WIDTH, LD_CFG_SCEEN_HEIGHT);



}

void uiStartupLoop(ld_scene_t* ptScene)
{




}

void uiStartupDraw(ld_scene_t *ptScene,arm_2d_tile_t *ptTile,bool bIsNewFrame)
{

}

void uiStartupQuit(ld_scene_t* ptScene)
{




}
