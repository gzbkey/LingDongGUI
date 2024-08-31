#include "uiAxis.h"
#include "ldGui.h"

void uiAxisInit(ld_scene_t* ptScene);
void uiAxisLoop(ld_scene_t* ptScene);
void uiAxisQuit(ld_scene_t* ptScene);


const ldPageFuncGroup_t uiAxisFunc={
    .init=uiAxisInit,
    .loop=uiAxisLoop,
    .quit=uiAxisQuit,
#if (USE_LOG_LEVEL>=LOG_LEVEL_INFO)
    .pageName="uiAxis",
#endif
};

static bool slotJumpReady(ld_scene_t *ptScene,ldMsg_t msg)
{
    ldGuiJumpPageFast(uiReadyFunc);
    return false;
}

static uint8_t *numStr[]={"0","1","2","3","4","5","6","7","8","9"};
void uiAxisInit(ld_scene_t* ptScene)
{
    void *obj,*win;

    obj=ldWindowInit(ID_BG, ID_BG, 0, 0, LD_CFG_SCEEN_WIDTH, LD_CFG_SCEEN_HEIGHT);
    ldWindowSetBgColor(obj,GLCD_COLOR_BLACK);

    obj=ldButtonInit(ID_BTN_RET,ID_BG,20,5,80,40);
    ldButtonSetFont(obj,FONT_SIMSUN_18);
    ldButtonSetColor(obj,__RGB(0x64,0x95,0xED),__RGB(0x41,0x69,0xE1));
    ldButtonSetRoundCorner(obj,true);
    ldButtonSetText(obj,"返回");
    connect(ID_BTN_RET,SIGNAL_RELEASE,slotJumpReady);

    obj=ldLabelInit(ID_LABEL_X,ID_BG,70,60,30,70,FONT_SIMHEI_20);
    ldLabelSetText(obj,"X:");
    ldLabelSetTransparent(obj,true);
    ldLabelSetTextColor(obj,GLCD_COLOR_WHITE);
    obj=ldScrollSelecterInit(ID_SCROLL_SELCETER_X_TENS,ID_BG,100,60,30,70,FONT_SIMHEI_20);
    ldScrollSelecterSetItems(obj,numStr,10);
    ldScrollSelecterSetTextColor(obj,GLCD_COLOR_WHITE);
    obj=ldScrollSelecterInit(ID_SCROLL_SELCETER_X_UNITS,ID_BG,130,60,30,70,FONT_SIMHEI_20);
    ldScrollSelecterSetItems(obj,numStr,10);
    ldScrollSelecterSetTextColor(obj,GLCD_COLOR_WHITE);
    obj=ldLabelInit(ID_LABEL_X_POINT,ID_BG,160,60,10,70,FONT_SIMHEI_20);
    ldLabelSetText(obj,".");
    ldLabelSetTransparent(obj,true);
    ldLabelSetTextColor(obj,GLCD_COLOR_WHITE);
    obj=ldScrollSelecterInit(ID_SCROLL_SELCETER_X_DECIMALS,ID_BG,170,60,30,70,FONT_SIMHEI_20);
    ldScrollSelecterSetItems(obj,numStr,10);
    ldScrollSelecterSetTextColor(obj,GLCD_COLOR_WHITE);
    obj=ldLabelInit(ID_LABEL_X_MM,ID_BG,200,60,30,70,FONT_SIMHEI_20);
    ldLabelSetText(obj,"MM");
    ldLabelSetTransparent(obj,true);
    ldLabelSetTextColor(obj,GLCD_COLOR_WHITE);

    obj=ldLabelInit(ID_LABEL_Y,ID_BG,270,60,30,70,FONT_SIMHEI_20);
    ldLabelSetText(obj,"Y:");
    ldLabelSetTransparent(obj,true);
    ldLabelSetTextColor(obj,GLCD_COLOR_WHITE);
    obj=ldScrollSelecterInit(ID_SCROLL_SELCETER_Y_TENS,ID_BG,300,60,30,70,FONT_SIMHEI_20);
    ldScrollSelecterSetItems(obj,numStr,10);
    ldScrollSelecterSetTextColor(obj,GLCD_COLOR_WHITE);
    obj=ldScrollSelecterInit(ID_SCROLL_SELCETER_Y_UNITS,ID_BG,330,60,30,70,FONT_SIMHEI_20);
    ldScrollSelecterSetItems(obj,numStr,10);
    ldScrollSelecterSetTextColor(obj,GLCD_COLOR_WHITE);
    obj=ldLabelInit(ID_LABEL_Y_POINT,ID_BG,360,60,10,70,FONT_SIMHEI_20);
    ldLabelSetText(obj,".");
    ldLabelSetTransparent(obj,true);
    ldLabelSetTextColor(obj,GLCD_COLOR_WHITE);
    obj=ldScrollSelecterInit(ID_SCROLL_SELCETER_Y_DECIMALS,ID_BG,370,60,30,70,FONT_SIMHEI_20);
    ldScrollSelecterSetItems(obj,numStr,10);
    ldScrollSelecterSetTextColor(obj,GLCD_COLOR_WHITE);
    obj=ldLabelInit(ID_LABEL_Y_MM,ID_BG,400,60,30,70,FONT_SIMHEI_20);
    ldLabelSetText(obj,"MM");
    ldLabelSetTransparent(obj,true);
    ldLabelSetTextColor(obj,GLCD_COLOR_WHITE);

    obj=ldLabelInit(ID_LABEL_Z,ID_BG,70,180,30,70,FONT_SIMHEI_20);
    ldLabelSetText(obj,"Z:");
    ldLabelSetTransparent(obj,true);
    ldLabelSetTextColor(obj,GLCD_COLOR_WHITE);
    obj=ldScrollSelecterInit(ID_SCROLL_SELCETER_Z_TENS,ID_BG,100,180,30,70,FONT_SIMHEI_20);
    ldScrollSelecterSetItems(obj,numStr,10);
    ldScrollSelecterSetTextColor(obj,GLCD_COLOR_WHITE);
    obj=ldScrollSelecterInit(ID_SCROLL_SELCETER_Z_UNITS,ID_BG,130,180,30,70,FONT_SIMHEI_20);
    ldScrollSelecterSetItems(obj,numStr,10);
    ldScrollSelecterSetTextColor(obj,GLCD_COLOR_WHITE);
    obj=ldLabelInit(ID_LABEL_Z_POINT,ID_BG,160,180,10,70,FONT_SIMHEI_20);
    ldLabelSetText(obj,".");
    ldLabelSetTransparent(obj,true);
    ldLabelSetTextColor(obj,GLCD_COLOR_WHITE);
    obj=ldScrollSelecterInit(ID_SCROLL_SELCETER_Z_DECIMALS,ID_BG,170,180,30,70,FONT_SIMHEI_20);
    ldScrollSelecterSetItems(obj,numStr,10);
    ldScrollSelecterSetTextColor(obj,GLCD_COLOR_WHITE);
    obj=ldLabelInit(ID_LABEL_Z_MM,ID_BG,200,180,30,70,FONT_SIMHEI_20);
    ldLabelSetText(obj,"MM");
    ldLabelSetTransparent(obj,true);
    ldLabelSetTextColor(obj,GLCD_COLOR_WHITE);
}

void uiAxisLoop(ld_scene_t* ptScene)
{




}

void uiAxisQuit(ld_scene_t* ptScene)
{




}
