#include "uiAbout.h"
#include "ldGui.h"

void uiAboutInit(ld_scene_t* ptScene);
void uiAboutLoop(ld_scene_t* ptScene);
void uiAboutQuit(ld_scene_t* ptScene);
void uiAboutDraw(ld_scene_t *ptScene,arm_2d_tile_t *ptTile,bool bIsNewFrame);

const ldPageFuncGroup_t uiAboutFunc={
    .init=uiAboutInit,
    .loop=uiAboutLoop,
    .quit=uiAboutQuit,
    .draw=uiAboutDraw,
#if (USE_LOG_LEVEL>=LOG_LEVEL_INFO)
    .pageName="uiAbout",
#endif
};

static bool slotJumpReady(ld_scene_t *ptScene,ldMsg_t msg)
{
    ldGuiJumpPageFast(uiReadyFunc);
    return false;
}

const uint8_t aboutText[]="灵动GUI DEMO\n屏幕尺寸 480x272";
void uiAboutInit(ld_scene_t* ptScene)
{
    void *obj,*win;

    obj=ldWindowInit(ID_BG, ID_BG, 0, 0, LD_CFG_SCEEN_WIDTH, LD_CFG_SCEEN_HEIGHT);
    ldWindowSetBgColor(obj,GLCD_COLOR_BLACK);

    obj=ldButtonInit(ID_BTN_RET,ID_BG,20,5,80,40);
    ldButtonSetFont(obj,FONT_ALIBABAPUHUITI_3_55_REGULAR_18);
    ldButtonSetColor(obj,__RGB(0x64,0x95,0xED),__RGB(0x41,0x69,0xE1));
    ldButtonSetRoundCorner(obj,true);
    ldButtonSetText(obj,"返回");
    connect(ID_BTN_RET,SIGNAL_RELEASE,slotJumpReady);

    obj=ldTextInit(ID_TXT,ID_BG,20,50,440,200,FONT_ALIBABAPUHUITI_3_55_REGULAR_18,false);
    ldTextSetText(obj,aboutText);
}

void uiAboutDraw(ld_scene_t *ptScene,arm_2d_tile_t *ptTile,bool bIsNewFrame)
{

}

void uiAboutLoop(ld_scene_t* ptScene)
{




}

void uiAboutQuit(ld_scene_t* ptScene)
{




}
