#include "uiPrinting.h"
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

static const uint8_t titleStr[]="注意";
static const uint8_t msgStr[]="是否停止打印";
static const uint8_t *pBtnStr[]={"是","否"};

static bool isPause=false;

static bool slotJumpPrint(ld_scene_t *ptScene,ldMsg_t msg)
{
    ldGuiJumpPage(uiPrintFunc,ARM_2D_SCENE_SWITCH_MODE_NONE,0);
    return false;
}

static void _msgBoxCallback(ldMessageBox_t *ptWidget)
{
    if(ptWidget->clickNum==0)
    {
        ldGuiJumpPage(uiPrintFunc,ARM_2D_SCENE_SWITCH_MODE_NONE,0);
    }
    else
    {
        isPause=false;
    }
}

static bool slotStopMsg(ld_scene_t *ptScene,ldMsg_t msg)
{
    void *obj=ldMessageBoxInit(ID_MSG_BOX,ID_BG,300,150,FONT_SIMSUN_18);
    ldMessageBoxSetTitle(obj,titleStr);
    ldMessageBoxSetMsg(obj,msgStr);
    ldMessageBoxSetBtn(obj,pBtnStr,2);

    ldMessageBoxSetCallback(obj,_msgBoxCallback);

    isPause=true;

    return false;
}

static bool slotPause(ld_scene_t *ptScene,ldMsg_t msg)
{
    isPause=!isPause;
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

    connect(ID_BTN_STOP,SIGNAL_RELEASE,slotStopMsg);

    obj=ldButtonInit(ID_BTN_PAUSE,ID_BG,350,180,120,50);
    ldButtonSetFont(obj,FONT_SIMSUN_24);
    ldButtonSetColor(obj,__RGB(0xF4,0xA4,0x60),__RGB(0xD2,0x69,0x1E));
    ldButtonSetRoundCorner(obj,true);
    ldButtonSetCheckable(obj,true);
    ldButtonSetText(obj,"暂停");

    connect(ID_BTN_PAUSE,SIGNAL_RELEASE,slotPause);

    isPause=false;

    obj=ldProgressBarInit(ID_PROG_BAR,ID_BG,10,255,460,13);
    ldProgressBarSetPercent(obj,45);

    obj=ldImageInit(ID_IMG_TEMP1,ID_BG,230,55,40,33,IMAGE_TEMP1_BMP,NULL,false);

    obj=ldLabelInit(ID_LABEL_TEMP1,ID_BG,280,50,60,20,FONT_SIMHEI_20);
    ldLabelSetText(obj,"170");
    ldLabelSetTransparent(obj,true);
    ldLabelSetTextColor(obj,GLCD_COLOR_WHITE);
    ldLabelSetAlign(obj,ARM_2D_ALIGN_LEFT);

    obj=ldLabelInit(ID_LABEL_TEMP2,ID_BG,280,70,60,20,FONT_SIMHEI_20);
    ldLabelSetText(obj,"200℃");
    ldLabelSetTransparent(obj,true);
    ldLabelSetTextColor(obj,GLCD_COLOR_WHITE);
    ldLabelSetAlign(obj,ARM_2D_ALIGN_LEFT);

    obj=ldImageInit(ID_IMG_TEMP2,ID_BG,230,120,40,33,IMAGE_TEMP2_BMP,NULL,false);

    obj=ldLabelInit(ID_LABEL_TEMP3,ID_BG,280,110,60,20,FONT_SIMHEI_20);
    ldLabelSetText(obj,"60");
    ldLabelSetTransparent(obj,true);
    ldLabelSetTextColor(obj,GLCD_COLOR_WHITE);
    ldLabelSetAlign(obj,ARM_2D_ALIGN_LEFT);

    obj=ldLabelInit(ID_LABEL_TEMP4,ID_BG,280,130,60,20,FONT_SIMHEI_20);
    ldLabelSetText(obj,"60℃");
    ldLabelSetTransparent(obj,true);
    ldLabelSetTextColor(obj,GLCD_COLOR_WHITE);
    ldLabelSetAlign(obj,ARM_2D_ALIGN_LEFT);


    obj=ldImageInit(ID_IMG_PERCENT,ID_BG,360,55,40,33,IMAGE_PERCENT_BMP,NULL,false);

    obj=ldLabelInit(ID_LABEL_PERCENT,ID_BG,410,60,80,20,FONT_SIMHEI_20);
    ldLabelSetText(obj,"60%%");
    ldLabelSetTransparent(obj,true);
    ldLabelSetTextColor(obj,GLCD_COLOR_WHITE);
    ldLabelSetAlign(obj,ARM_2D_ALIGN_LEFT);


    obj=ldImageInit(ID_IMG_HEIGHT,ID_BG,360,120,40,33,IMAGE_HEIGHT_BMP,NULL,false);

    obj=ldLabelInit(ID_LABEL_HEIGHT,ID_BG,410,130,80,20,FONT_SIMHEI_20);
    ldLabelSetText(obj,"0.1MM");
    ldLabelSetTransparent(obj,true);
    ldLabelSetTextColor(obj,GLCD_COLOR_WHITE);
    ldLabelSetAlign(obj,ARM_2D_ALIGN_LEFT);


    obj=ldLabelInit(ID_LABEL1,ID_BG,10,232,80,20,FONT_SIMSUN_18);
    ldLabelSetText(obj,"打印进度");
    ldLabelSetTransparent(obj,true);
    ldLabelSetTextColor(obj,GLCD_COLOR_WHITE);
    ldLabelSetAlign(obj,ARM_2D_ALIGN_LEFT);

    obj=ldLabelInit(ID_LABEL_PRINT_PERCENT,ID_BG,400,232,70,20,FONT_SIMSUN_18);
    ldLabelSetText(obj,"0%%");
    ldLabelSetTransparent(obj,true);
    ldLabelSetTextColor(obj,GLCD_COLOR_WHITE);
    ldLabelSetAlign(obj,ARM_2D_ALIGN_RIGHT);








}

void uiPrintingLoop(ld_scene_t* ptScene)
{
    uint8_t buf[10];
    static uint8_t percent=0;
    if(ldTimeOut(100,true)&&!isPause)
    {
        ldProgressBar_t* ptWidget = ldBaseGetWidgetById(ID_PROG_BAR);
        ldLabel_t* label=ldBaseGetWidgetById(ID_LABEL_PRINT_PERCENT);
        percent++;
        if(percent>100)
        {
            percent=0;
        }
        ldProgressBarSetPercent(ptWidget,percent);

        sprintf(buf,"%d%%%%",percent);

        ldLabelSetText(label,buf);
    }



}

void uiPrintingQuit(ld_scene_t* ptScene)
{




}
