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

    obj=ldLabelInit(ID_LOGO,ID_BG,0,0,200,200,FONT_SIMSUN_48);
    ldLabelSetText(obj,"灵动GUI");
    ldBaseSetCenter(obj);


    uiLogoLogicInit(ptScene);
}

void uiLogoLoop(ld_scene_t* ptScene)
{
    if(ldTimeOut(1000,false))
    {
        ldGuiJumpPage(uiMainFunc,ARM_2D_SCENE_SWITCH_MODE_ERASE_LEFT,1000);
    }



    uiLogoLogicLoop(ptScene);
}

void uiLogoQuit(ld_scene_t* ptScene)
{




    uiLogoLogicQuit(ptScene);
}