#include "uiNozzle.h"
#include "ldGui.h"

void uiNozzleInit(ld_scene_t* ptScene);
void uiNozzleLoop(ld_scene_t* ptScene);
void uiNozzleQuit(ld_scene_t* ptScene);
void uiNozzleDraw(ld_scene_t* ptScene, arm_2d_tile_t *ptTile, bool bIsNewFrame);

const ldPageFuncGroup_t uiNozzleFunc={
    .init=uiNozzleInit,
    .loop=uiNozzleLoop,
    .quit=uiNozzleQuit,
    .draw=uiNozzleDraw,
#if (USE_LOG_LEVEL>=LOG_LEVEL_INFO)
    .pageName="uiNozzle",
#endif
};

static bool slotJumpReady(ld_scene_t *ptScene,ldMsg_t msg)
{
    ldGuiJumpPage(uiReadyFunc,ARM_2D_SCENE_SWITCH_MODE_NONE,0);
    return false;
}

static bool slotChangeValue(ld_scene_t *ptScene,ldMsg_t msg)
{
    uint8_t buf[10]={0};
    ldLabel_t* ptWidget= ldBaseGetWidgetById(ID_LABEL_TEMP);

    sprintf(buf,"%d℃",msg.value*18/100);
    ldLabelSetText(ptWidget,buf);
    return false;
}

void uiNozzleInit(ld_scene_t* ptScene)
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

    obj=ldImageInit(ID_IMG_1,ID_BG,20,60,42,30,IMAGE_NOZZLEICON1_BMP,NULL,false);
    obj=ldLabelInit(ID_LABEL_1,ID_BG,65,60,80,30,FONT_SIMHEI_20);
    ldLabelSetText(obj,"喷头温度");
    ldLabelSetTransparent(obj,true);
    ldLabelSetTextColor(obj,GLCD_COLOR_WHITE);
    ldLabelSetAlign(obj,ARM_2D_ALIGN_LEFT);


    obj=ldImageInit(ID_IMG_2,ID_BG,20,168,42,30,IMAGE_NOZZLEICON2_BMP,NULL,false);
    obj=ldLabelInit(ID_LABEL_1,ID_BG,65,168,40,30,FONT_SIMHEI_20);
    ldLabelSetText(obj,"进料");
    ldLabelSetTransparent(obj,true);
    ldLabelSetTextColor(obj,GLCD_COLOR_WHITE);
    ldLabelSetAlign(obj,ARM_2D_ALIGN_LEFT);
    obj=ldLineEditInit(ID_LINE_EDIT_2,ID_BG,300,168,50,30,FONT_SIMHEI_20,4);
    ldLineEditSetKeyboard(obj,ID_KB);
    ldLineEditSetType(obj,typeInt);
    ldLineEditSetText(obj,"0");
    ldLineEditSetAlign(obj,ARM_2D_ALIGN_RIGHT);
    obj=ldLabelInit(ID_LABEL_4,ID_BG,400,168,40,30,FONT_SIMHEI_20);
    ldLabelSetText(obj,"MM");
    ldLabelSetTransparent(obj,true);
    ldLabelSetTextColor(obj,GLCD_COLOR_WHITE);
    ldLabelSetAlign(obj,ARM_2D_ALIGN_LEFT);

    obj=ldImageInit(ID_IMG_3,ID_BG,20,223,42,30,IMAGE_NOZZLEICON3_BMP,NULL,false);
    obj=ldLabelInit(ID_LABEL_1,ID_BG,65,223,40,30,FONT_SIMHEI_20);
    ldLabelSetText(obj,"出料");
    ldLabelSetTransparent(obj,true);
    ldLabelSetTextColor(obj,GLCD_COLOR_WHITE);
    ldLabelSetAlign(obj,ARM_2D_ALIGN_LEFT);
    obj=ldLineEditInit(ID_LINE_EDIT_3,ID_BG,300,223,50,30,FONT_SIMHEI_20,4);
    ldLineEditSetKeyboard(obj,ID_KB);
    ldLineEditSetType(obj,typeInt);
    ldLineEditSetText(obj,"0");
    ldLineEditSetAlign(obj,ARM_2D_ALIGN_RIGHT);
    obj=ldLabelInit(ID_LABEL_5,ID_BG,400,223,40,30,FONT_SIMHEI_20);
    ldLabelSetText(obj,"MM");
    ldLabelSetTransparent(obj,true);
    ldLabelSetTextColor(obj,GLCD_COLOR_WHITE);
    ldLabelSetAlign(obj,ARM_2D_ALIGN_LEFT);



    obj=ldSliderInit(ID_SLIDER,ID_BG,40,91,400,30);
    connect(ID_SLIDER,SIGNAL_VALUE_CHANGED,slotChangeValue);

    obj=ldLabelInit(ID_LABEL_TEMP,ID_BG,220,120,60,30,FONT_SIMHEI_20);
    ldLabelSetText(obj,"0℃");
    ldLabelSetAlign(obj,ARM_2D_ALIGN_LEFT);
    ldLabelSetTextColor(obj,GLCD_COLOR_WHITE);
    ldLabelSetTransparent(obj,true);


    ldKeyboardInit(ID_KB,ID_BG,FONT_SIMSUN_16);
}

void uiNozzleLoop(ld_scene_t* ptScene)
{




}

void uiNozzleDraw(ld_scene_t *ptScene,arm_2d_tile_t *ptTile,bool bIsNewFrame)
{
    arm_2d_region_t childRegion={
        .tLocation={
            .iX=10,
            .iY=50,
        },
        .tSize={
            .iWidth=460,
            .iHeight=100,
        }
    };
    arm_2d_tile_t childTile;
    arm_2d_tile_generate_child(ptTile,&childRegion,&childTile,true);
    draw_round_corner_box(&childTile,NULL,__RGB(25,25,25),255,bIsNewFrame);

    childTile.tRegion.tLocation.iY+=110;
    childTile.tRegion.tSize.iHeight=45;
    draw_round_corner_box(&childTile,NULL,__RGB(25,25,25),255,bIsNewFrame);

    childTile.tRegion.tLocation.iY+=55;
    draw_round_corner_box(&childTile,NULL,__RGB(25,25,25),255,bIsNewFrame);
}

void uiNozzleQuit(ld_scene_t* ptScene)
{




}
