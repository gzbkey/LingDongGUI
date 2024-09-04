#include "uiReady.h"
#include "ldGui.h"

void uiReadyInit(ld_scene_t* ptScene);
void uiReadyLoop(ld_scene_t* ptScene);
void uiReadyQuit(ld_scene_t* ptScene);
void uiReadyDraw(ld_scene_t *ptScene,arm_2d_tile_t *ptTile,bool bIsNewFrame);

const ldPageFuncGroup_t uiReadyFunc={
    .init=uiReadyInit,
    .loop=uiReadyLoop,
    .quit=uiReadyQuit,
    .draw=uiReadyDraw,
#if (USE_LOG_LEVEL>=LOG_LEVEL_INFO)
    .pageName="uiReady",
#endif
};

static bool slotJumpMain(ld_scene_t *ptScene,ldMsg_t msg)
{
    ldGuiJumpPageFast(uiMainFunc);
    return false;
}

static bool slotJumpAxis(ld_scene_t *ptScene,ldMsg_t msg)
{
    ldGuiJumpPageFast(uiAxisFunc);
    return false;
}

static bool slotJumpNozzle(ld_scene_t *ptScene,ldMsg_t msg)
{
    ldGuiJumpPageFast(uiNozzleFunc);
    return false;
}

static bool slotJumpAbout(ld_scene_t *ptScene,ldMsg_t msg)
{
    ldGuiJumpPageFast(uiAboutFunc);
    return false;
}

void uiReadyInit(ld_scene_t* ptScene)
{
    void *obj,*win;

    obj=ldWindowInit(ID_BG, ID_BG, 0, 0, LD_CFG_SCEEN_WIDTH, LD_CFG_SCEEN_HEIGHT);
    ldWindowSetBgColor(obj,GLCD_COLOR_BLACK);

    obj=ldButtonInit(ID_BTN_RET,ID_BG,20,5,80,40);
    ldButtonSetFont(obj,FONT_ALIBABAPUHUITI_3_55_REGULAR_18);
    ldButtonSetColor(obj,__RGB(0x64,0x95,0xED),__RGB(0x41,0x69,0xE1));
    ldButtonSetRoundCorner(obj,true);
    ldButtonSetText(obj,"返回");
    connect(ID_BTN_RET,SIGNAL_RELEASE,slotJumpMain);

    obj=ldButtonInit(ID_BTN_1,ID_BG,10,50,220,60);
    ldButtonSetImage(obj,IMAGE_READYBTN1_BMP,NULL,IMAGE_READYBTN1_BMP,NULL);
    connect(ID_BTN_1,SIGNAL_RELEASE,slotJumpAxis);

    obj=ldButtonInit(ID_BTN_2,ID_BG,250,50,220,60);
    ldButtonSetImage(obj,IMAGE_READYBTN2_BMP,NULL,IMAGE_READYBTN2_BMP,NULL);
    connect(ID_BTN_2,SIGNAL_RELEASE,slotJumpNozzle);

    obj=ldButtonInit(ID_BTN_3,ID_BG,10,124,220,60);
    ldButtonSetImage(obj,IMAGE_READYBTN3_BMP,NULL,IMAGE_READYBTN3_OFF_BMP,NULL);
    ldButtonSetCheckable(obj,true);

    obj=ldButtonInit(ID_BTN_4,ID_BG,250,124,220,60);
    ldButtonSetImage(obj,IMAGE_READYBTN4_BMP,NULL,IMAGE_READYBTN4_OFF_BMP,NULL);
    ldButtonSetCheckable(obj,true);

    obj=ldButtonInit(ID_BTN_5,ID_BG,10,198,220,60);
    ldButtonSetImage(obj,IMAGE_READYBTN5_BMP,NULL,IMAGE_READYBTN5_BMP,NULL);
    connect(ID_BTN_5,SIGNAL_RELEASE,slotJumpAbout);

}

void uiReadyDraw(ld_scene_t *ptScene,arm_2d_tile_t *ptTile,bool bIsNewFrame)
{

}

void uiReadyLoop(ld_scene_t* ptScene)
{




}

void uiReadyQuit(ld_scene_t* ptScene)
{




}
