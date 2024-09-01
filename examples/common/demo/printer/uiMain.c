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
    ldGuiJumpPageFast(uiPrintFunc);
    return false;
}

static bool slotJumpReady(ld_scene_t *ptScene,ldMsg_t msg)
{
    ldGuiJumpPageFast(uiReadyFunc);
    return false;
}

static bool slotJumpSet(ld_scene_t *ptScene,ldMsg_t msg)
{
    ldGuiJumpPageFast(uiSetFunc);
    return false;
}

void uiMainInit(ld_scene_t* ptScene)
{
    void *obj,*win;

    obj=ldWindowInit(0, 0, 0, 0, LD_CFG_SCEEN_WIDTH, LD_CFG_SCEEN_HEIGHT);
    ldWindowSetBgColor(obj,GLCD_COLOR_BLACK);

    obj=ldButtonInit(ID_BTN_PRINT,ID_BG,27,67,124,158);
    ldButtonSetRoundCorner(obj,true);
    ldButtonSetImage(obj,IMAGE_MAINBTNPRINT_BMP,NULL,IMAGE_MAINBTNPRINT_BMP,NULL);

    obj=ldButtonInit(ID_BTN_WAIT,ID_BG,178,67,124,158);
    ldButtonSetRoundCorner(obj,true);
    ldButtonSetImage(obj,IMAGE_MAINBTNREADY_BMP,NULL,IMAGE_MAINBTNREADY_BMP,NULL);

    obj=ldButtonInit(ID_BTN_SET,ID_BG,329,67,124,158);
    ldButtonSetRoundCorner(obj,true);
    ldButtonSetImage(obj,IMAGE_MAINBTNSET_BMP,NULL,IMAGE_MAINBTNSET_BMP,NULL);

    connect(ID_BTN_PRINT,SIGNAL_RELEASE,slotJumpPrint);
    connect(ID_BTN_WAIT,SIGNAL_RELEASE,slotJumpReady);
    connect(ID_BTN_SET,SIGNAL_RELEASE,slotJumpSet);



}

void uiMainLoop(ld_scene_t* ptScene)
{




}

void uiMainQuit(ld_scene_t* ptScene)
{




}
