#include "uiTemplate.h"
#include "uiTemplateLogic.h"
#include "ldGui.h"

void uiTemplateInit(ld_scene_t* ptScene);
void uiTemplateLoop(ld_scene_t* ptScene);
void uiTemplateQuit(ld_scene_t* ptScene);


const ldPageFuncGroup_t uiTemplateFunc={
    .init=uiTemplateInit,
    .loop=uiTemplateLoop,
    .quit=uiTemplateQuit,
#if (USE_LOG_LEVEL>=LOG_LEVEL_INFO)
    .pageName="uiTemplate",
#endif
};

void uiTemplateInit(ld_scene_t* ptScene)
{
    void *obj,*win;

    ldWindowInit(0, 0, 0, 0, LD_CFG_SCEEN_WIDTH, LD_CFG_SCEEN_HEIGHT);



    uiTemplateLogicInit(ptScene);
}

void uiTemplateLoop(ld_scene_t* ptScene)
{




    uiTemplateLogicLoop(ptScene);
}

void uiTemplateQuit(ld_scene_t* ptScene)
{




    uiTemplateLogicQuit(ptScene);
}
