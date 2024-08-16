#include "uiLogo.h"
#include "uiLogoLogic.h"
#include "ldGui.h"

void uiLogoInit(ld_scene_t* ptScene);
void uiLogoLoop(ld_scene_t* ptScene);
void uiLogoQuit(ld_scene_t* ptScene);


const ldPageFuncGroup_t uiLogoFunc={
    .init=uiLogoInit,
    .loop=uiLogoLoop,
    .quit=uiLogoQuit,
#if (USE_LOG_LEVEL>=LOG_LEVEL_INFO)
    .pageName="uiLogo",
#endif
};

void uiLogoInit(ld_scene_t* ptScene)
{
    void *obj,*win;

    ldWindowInit(0, 0, 0, 0, LD_CFG_SCEEN_WIDTH, LD_CFG_SCEEN_HEIGHT);

    obj=ldLabelInit(ID_LOGO,ID_BG,0,0,200,200,FONT_ARIAL_48_A8);
    ldLabelSetText(obj,"灵动GUI");

    uiLogoLogicInit(ptScene);
}

void uiLogoLoop(ld_scene_t* ptScene)
{




    uiLogoLogicLoop(ptScene);
}

void uiLogoQuit(ld_scene_t* ptScene)
{




    uiLogoLogicQuit(ptScene);
}
