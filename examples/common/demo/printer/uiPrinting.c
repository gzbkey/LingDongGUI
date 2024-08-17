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

static arm_2d_tile_t *imgList[8]={IMAGE_ALARMCLOCK_PNG,IMAGE_BLOCKRABBIT_PNG,IMAGE_CAKE_PNG,IMAGE_DOLL_PNG,IMAGE_MILK_PNG,IMAGE_MUG_PNG,IMAGE_SMALLROOM_PNG};

extern uint8_t gPrintImgNum;

static bool slotJumpPrint(ld_scene_t *ptScene,ldMsg_t msg)
{
    ldGuiJumpPage(uiPrintFunc,ARM_2D_SCENE_SWITCH_MODE_NONE,0);
    return false;
}

void uiPrintingInit(ld_scene_t* ptScene)
{
    void *obj,*win;

    obj=ldWindowInit(ID_BG, ID_BG, 0, 0, LD_CFG_SCEEN_WIDTH, LD_CFG_SCEEN_HEIGHT);
    ldWindowSetBgColor(obj,GLCD_COLOR_BLACK);

    obj=ldButtonInit(ID_BTN_RET,ID_BG,20,5,80,40);
    ldButtonSetFont(obj,FONT_SIMSUN_18);
    ldButtonSetColor(obj,__RGB(0x64,0x95,0xED),__RGB(0x41,0x69,0xE1));
    ldButtonSetRoundCorner(obj,true);
    ldButtonSetText(obj,"返回");

    connect(ID_BTN_RET,SIGNAL_RELEASE,slotJumpPrint);

    obj=ldDateTimeInit(ID_DATE_TIME,ID_BG,300,5,150,40,FONT_SIMSUN_16);
    ldDateTimeSetTextColor(obj,GLCD_COLOR_WHITE);
    ldDateTimeSetDate(obj,2024,8,17);
    ldDateTimeSetTime(obj,20,10,0);

    obj=ldImageInit(ID_IMG_VIEW,ID_BG,10,50,200,180,imgList[gPrintImgNum],NULL,false);

    obj=ldButtonInit(ID_BTN_STOP,ID_BG,220,180,120,50);
    ldButtonSetFont(obj,FONT_SIMSUN_24);
    ldButtonSetColor(obj,__RGB(0xF4,0xA4,0x60),__RGB(0xD2,0x69,0x1E));
    ldButtonSetRoundCorner(obj,true);
    ldButtonSetText(obj,"停止");

    obj=ldButtonInit(ID_BTN_PAUSE,ID_BG,350,180,120,50);
    ldButtonSetFont(obj,FONT_SIMSUN_24);
    ldButtonSetColor(obj,__RGB(0xF4,0xA4,0x60),__RGB(0xD2,0x69,0x1E));
    ldButtonSetRoundCorner(obj,true);
    ldButtonSetCheckable(obj,true);
    ldButtonSetText(obj,"暂停");

    obj=ldProgressBarInit(ID_PROG_BAR,ID_BG,10,255,460,13);
    ldProgressBarSetPercent(obj,45);
    uiPrintingLogicInit(ptScene);
}

void uiPrintingLoop(ld_scene_t* ptScene)
{
    static float percent=0;
    if(ldTimeOut(100,true))
    {
        ldProgressBar_t* ptWidget = ldBaseGetWidgetById(ID_PROG_BAR);

        percent++;
        if(percent>100)
        {
            percent=0;
        }
        ldProgressBarSetPercent(ptWidget,percent);

    }



    uiPrintingLogicLoop(ptScene);
}

void uiPrintingQuit(ld_scene_t* ptScene)
{




    uiPrintingLogicQuit(ptScene);
}
