#include "uiLogo.h"
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

    obj=ldLabelInit(ID_LOGO,ID_BG,0,0,270,270,FONT_ALIBABAPUHUITI_3_55_REGULAR_48);
    ldLabelSetTransparent(obj,true);
    ldLabelSetText(obj,"灵动GUI");
    ldBaseSetCenter(obj);

}

void uiLogoLoop(ld_scene_t* ptScene)
{
    if(ldTimeOut(1000,false))
    {
        ldGuiJumpPage(uiMainFunc,ARM_2D_SCENE_SWITCH_MODE_FADE_WHITE,3000);
    }



}

void uiLogoQuit(ld_scene_t* ptScene)
{




}
