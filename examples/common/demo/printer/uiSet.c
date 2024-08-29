#include "uiSet.h"
#include "ldGui.h"

void uiSetInit(ld_scene_t* ptScene);
void uiSetLoop(ld_scene_t* ptScene);
void uiSetQuit(ld_scene_t* ptScene);
void uiSetDraw(ld_scene_t *ptScene,arm_2d_tile_t *ptTile,bool bIsNewFrame);

const ldPageFuncGroup_t uiSetFunc={
    .init=uiSetInit,
    .loop=uiSetLoop,
    .quit=uiSetQuit,
    .draw=uiSetDraw,
#if (USE_LOG_LEVEL>=LOG_LEVEL_INFO)
    .pageName="uiSet",
#endif
};

uint8_t tabSelect=0;

static bool slotJumpMain(ld_scene_t *ptScene,ldMsg_t msg)
{
    ldGuiJumpPageFast(uiMainFunc);
    return false;
}

static bool slotTabShow(ld_scene_t *ptScene,ldMsg_t msg)
{
//    ldButton_t *ptWidget=msg.ptSender;

//    ldButton_t *pBtn0,*pBtn1,*pBtn2;
//    if(msg.signal==SIGNAL_RELEASE)
//    {
//        pBtn0=ldBaseGetWidgetById(ID_BTN_SET);
//        pBtn1=ldBaseGetWidgetById(ID_BTN_SYS);
//        pBtn2=ldBaseGetWidgetById(ID_BTN_ABOUT);
//        switch(ptWidget->keyValue)
//        {
//            case 0:
//        {

//            break;
//        }
//        case 1:
//    {
//        break;
//    }
//        case 2:
//    {
//        break;
//    }
//            break;
//        }
//    }
}

void uiSetInit(ld_scene_t* ptScene)
{
    void *obj,*win;

    obj=ldWindowInit(ID_BG, ID_BG, 0, 0, LD_CFG_SCEEN_WIDTH, LD_CFG_SCEEN_HEIGHT);
    ldWindowSetBgColor(obj,GLCD_COLOR_BLACK);

    obj=ldButtonInit(ID_BTN_RET,ID_BG,20,5,80,40);
    ldButtonSetFont(obj,FONT_SIMSUN_18);
    ldButtonSetColor(obj,__RGB(0x64,0x95,0xED),__RGB(0x41,0x69,0xE1));
    ldButtonSetRoundCorner(obj,true);
    ldButtonSetText(obj,"返回");
    connect(ID_BTN_RET,SIGNAL_RELEASE,slotJumpMain);

    obj=ldCheckBoxInit(ID_CHECK_BOX_SET,ID_BG,0,50,100,74);
    ldCheckBoxSetImage(obj,IMAGE_TABBTNRELEASE_BMP,NULL,IMAGE_TABBTNPRESS_BMP,NULL);
    ldCheckBoxSetRadioButtonGroup(obj,0);
    ldCheckBoxSetChecked(obj,true);
    ldCheckBoxSetStringLeftSpace(obj,25);
    ldCheckBoxSetText(obj,FONT_SIMSUN_24,"配置");
    ldCheckBoxSetCharColor(obj,GLCD_COLOR_WHITE);

    obj=ldCheckBoxInit(ID_CHECK_BOX_SYS,ID_BG,0,124,100,74);
    ldCheckBoxSetImage(obj,IMAGE_TABBTNRELEASE_BMP,NULL,IMAGE_TABBTNPRESS_BMP,NULL);
    ldCheckBoxSetRadioButtonGroup(obj,0);
    ldCheckBoxSetStringLeftSpace(obj,25);
    ldCheckBoxSetText(obj,FONT_SIMSUN_24,"系统\n设置");
    ldCheckBoxSetCharColor(obj,GLCD_COLOR_WHITE);

    obj=ldCheckBoxInit(ID_CHECK_BOX_ABOUT,ID_BG,0,198,100,74);
    ldCheckBoxSetImage(obj,IMAGE_TABBTNRELEASE_BMP,NULL,IMAGE_TABBTNPRESS_BMP,NULL);
    ldCheckBoxSetRadioButtonGroup(obj,0);
    ldCheckBoxSetStringLeftSpace(obj,25);
    ldCheckBoxSetText(obj,FONT_SIMSUN_24,"关于");
    ldCheckBoxSetCharColor(obj,GLCD_COLOR_WHITE);


//    obj=ldButtonInit(ID_BTN_SET,ID_BG,0,50,100,74);
//    ldButtonSetKeyValue(obj,0);
//    ldButtonSetCheckable(obj,true);
//    ldButtonSetFont(obj,FONT_SIMSUN_24);
//    ldButtonSetText(obj,"准备");
//    connect(ID_BTN_SET,SIGNAL_RELEASE,slotTabShow);

//    obj=ldButtonInit(ID_BTN_SYS,ID_BG,0,124,100,74);
//    ldButtonSetKeyValue(obj,1);
//    ldButtonSetCheckable(obj,true);
//    ldButtonSetFont(obj,FONT_SIMSUN_24);
//    ldButtonSetText(obj,"准备");
//    connect(ID_BTN_SYS,SIGNAL_RELEASE,slotTabShow);

//    obj=ldButtonInit(ID_BTN_ABOUT,ID_BG,0,198,100,74);
//    ldButtonSetKeyValue(obj,2);
//    ldButtonSetCheckable(obj,true);
//    ldButtonSetFont(obj,FONT_SIMSUN_24);
//    ldButtonSetText(obj,"准备");
//    connect(ID_BTN_ABOUT,SIGNAL_RELEASE,slotTabShow);

    tabSelect=0;
}

void uiSetLoop(ld_scene_t* ptScene)
{




}

void uiSetDraw(ld_scene_t *ptScene,arm_2d_tile_t *ptTile,bool bIsNewFrame)
{
    arm_2d_region_t childRegion={
        .tLocation={
            .iX=120,
            .iY=60,
        },
        .tSize={
            .iWidth=160,
            .iHeight=60,
        }
    };
    arm_2d_tile_t childTile;

    arm_2d_tile_generate_child(ptTile,&childRegion,&childTile,true);
    draw_round_corner_box(&childTile,NULL,__RGB(125,125,125),255,bIsNewFrame);

    childTile.tRegion.tLocation.iX+=180;
    draw_round_corner_box(&childTile,NULL,__RGB(125,125,125),255,bIsNewFrame);

    childTile.tRegion.tLocation.iX-=180;
    childTile.tRegion.tLocation.iY+=72;
    draw_round_corner_box(&childTile,NULL,__RGB(125,125,125),255,bIsNewFrame);

    childTile.tRegion.tLocation.iX+=180;
    draw_round_corner_box(&childTile,NULL,__RGB(125,125,125),255,bIsNewFrame);

    childTile.tRegion.tLocation.iX-=180;
    childTile.tRegion.tLocation.iY+=72;
    draw_round_corner_box(&childTile,NULL,__RGB(125,125,125),255,bIsNewFrame);
}

void uiSetQuit(ld_scene_t* ptScene)
{




}
