#include "uiMain.h"
#include "uiMainLogic.h"
#include "ldGui.h"

void uiMainInit(ld_scene_t* ptScene);
void uiMainLoop(ld_scene_t* ptScene);
void uiMainQuit(ld_scene_t* ptScene);


const ldPageFuncGroup_t uiMainFunc={
    .init=uiMainInit,
    .loop=uiMainLoop,
    .quit=uiMainQuit,
#if (USE_LOG_LEVEL>=LOG_LEVEL_INFO)
    .pageName="uiMain",
#endif
};

void uiMainInit(ld_scene_t* ptScene)
{




    uiMainLogicInit(ptScene);
}

void uiMainLoop(ld_scene_t* ptScene)
{




    uiMainLogicLoop(ptScene);
}

void uiMainQuit(ld_scene_t* ptScene)
{




    uiMainLogicQuit(ptScene);
}
