#include "uiMain.h"
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

static bool slotJumpPrint(ld_scene_t *ptScene,ldMsg_t msg)
{
    ldGuiJumpPage(uiPrintFunc,ARM_2D_SCENE_SWITCH_MODE_NONE,0);
    return false;
}

static bool slotJumpWait(ld_scene_t *ptScene,ldMsg_t msg)
{
    ldGuiJumpPage(uiWaitFunc,ARM_2D_SCENE_SWITCH_MODE_NONE,0);
    return false;
}

static bool slotJumpSet(ld_scene_t *ptScene,ldMsg_t msg)
{
    ldGuiJumpPage(uiSetFunc,ARM_2D_SCENE_SWITCH_MODE_NONE,0);
    return false;
}

void uiMainInit(ld_scene_t* ptScene)
{
    void *obj,*win;

    obj=ldWindowInit(0, 0, 0, 0, LD_CFG_SCEEN_WIDTH, LD_CFG_SCEEN_HEIGHT);
    ldWindowSetBgColor(obj,GLCD_COLOR_BLACK);

    obj=ldButtonInit(ID_BTN_PRINT,ID_BG,27,67,125,155);
    ldButtonSetRoundCorner(obj,true);
    ldButtonSetFont(obj,FONT_SIMSUN_24);
    ldButtonSetText(obj,"打印");

    obj=ldButtonInit(ID_BTN_WAIT,ID_BG,178,67,125,155);
    ldButtonSetRoundCorner(obj,true);
    ldButtonSetFont(obj,FONT_SIMSUN_24);
    ldButtonSetText(obj,"准备");

    obj=ldButtonInit(ID_BTN_SET,ID_BG,329,67,125,155);
    ldButtonSetRoundCorner(obj,true);
    ldButtonSetFont(obj,FONT_SIMSUN_24);
    ldButtonSetText(obj,"设置");

    connect(ID_BTN_PRINT,SIGNAL_RELEASE,slotJumpPrint);
    connect(ID_BTN_WAIT,SIGNAL_RELEASE,slotJumpWait);
    connect(ID_BTN_SET,SIGNAL_RELEASE,slotJumpSet);



}

void uiMainLoop(ld_scene_t* ptScene)
{




}

void uiMainQuit(ld_scene_t* ptScene)
{




}
