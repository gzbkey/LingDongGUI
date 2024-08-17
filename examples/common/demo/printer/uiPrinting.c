#include "uiPrinting.h"
#include "uiPrintingLogic.h"
#include "ldGui.h"

void uiPrintingInit(ld_scene_t* ptScene);
void uiPrintingLoop(ld_scene_t* ptScene);
void uiPrintingQuit(ld_scene_t* ptScene);


const ldPageFuncGroup_t uiPrintingFunc={
    .init=uiPrintingInit,
    .loop=uiPrintingLoop,
    .quit=uiPrintingQuit,
#if (USE_LOG_LEVEL>=LOG_LEVEL_INFO)
    .pageName="uiPrinting",
#endif
};

void uiPrintingInit(ld_scene_t* ptScene)
{
    void *obj,*win;

    obj=ldWindowInit(ID_BG, ID_BG, 0, 0, LD_CFG_SCEEN_WIDTH, LD_CFG_SCEEN_HEIGHT);



    uiPrintingLogicInit(ptScene);
}

void uiPrintingLoop(ld_scene_t* ptScene)
{




    uiPrintingLogicLoop(ptScene);
}

void uiPrintingQuit(ld_scene_t* ptScene)
{




    uiPrintingLogicQuit(ptScene);
}
