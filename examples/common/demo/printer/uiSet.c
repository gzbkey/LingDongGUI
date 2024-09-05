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

uint8_t itemNum=0;
static bool slotTabChange(ld_scene_t *ptScene,ldMsg_t msg)
{
    ldCheckBox_t *ptWidget=msg.ptSender;

    ldCheckBox_t *pCB0,*pCB1,*pCB2;
    if(msg.signal==SIGNAL_CLICKED_ITEM)
    {
        pCB0=ldBaseGetWidgetById(ID_CHECK_BOX_SET);
        pCB1=ldBaseGetWidgetById(ID_CHECK_BOX_SYS);
        pCB2=ldBaseGetWidgetById(ID_CHECK_BOX_ABOUT);

        ldCheckBoxSetTextColor(pCB0,GLCD_COLOR_WHITE);
        ldCheckBoxSetTextColor(pCB1,GLCD_COLOR_WHITE);
        ldCheckBoxSetTextColor(pCB2,GLCD_COLOR_WHITE);

        ldCheckBoxSetTextColor(ptWidget,__RGB(244,139,0));

        if(pCB0==ptWidget)
        {
            itemNum=0;
            ldListSetHidden(ldBaseGetWidgetById(ID_LIST),true);
        }
        else if(pCB1==ptWidget)
        {
            itemNum=1;
            ldListSetHidden(ldBaseGetWidgetById(ID_LIST),true);
        }
        else if(pCB2==ptWidget)
        {
            itemNum=2;
            ldListSetHidden(ldBaseGetWidgetById(ID_LIST),false);
        }

    }
    return false;
}

const uint8_t *listStr[]={"设备名称     3D_Printer","型号          V0.8","power by ldgui","","","","",""};
#define DEF_X   480
void uiSetInit(ld_scene_t* ptScene)
{
    void *obj;

    obj=ldWindowInit(ID_BG, ID_BG, 0, 0, LD_CFG_SCEEN_WIDTH, LD_CFG_SCEEN_HEIGHT);
    ldWindowSetBgColor(obj,GLCD_COLOR_BLACK);

    obj=ldButtonInit(ID_BTN_RET,ID_BG,20,5,80,40);
    ldButtonSetFont(obj,FONT_ALIBABAPUHUITI_3_55_REGULAR_18);
    ldButtonSetColor(obj,__RGB(0x64,0x95,0xED),__RGB(0x41,0x69,0xE1));
    ldButtonSetRoundCorner(obj,true);
    ldButtonSetText(obj,(uint8_t*)"返回");
    connect(ID_BTN_RET,SIGNAL_RELEASE,slotJumpMain);

    obj=ldCheckBoxInit(ID_CHECK_BOX_SET,ID_BG,0,50,100,74);
    ldCheckBoxSetImage(obj,IMAGE_TABBTNRELEASE_BMP,NULL,IMAGE_TABBTNPRESS_BMP,NULL);
    ldCheckBoxSetRadioButtonGroup(obj,0);
    ldCheckBoxSetChecked(obj,true);
    ldCheckBoxSetStringLeftSpace(obj,25);
    ldCheckBoxSetText(obj,FONT_ALIBABAPUHUITI_3_55_REGULAR_24,(uint8_t*)"配置");
    ldCheckBoxSetTextColor(obj,__RGB(244,139,0));
    connect(ID_CHECK_BOX_SET,SIGNAL_CLICKED_ITEM,slotTabChange);

    obj=ldCheckBoxInit(ID_CHECK_BOX_SYS,ID_BG,0,124,100,74);
    ldCheckBoxSetImage(obj,IMAGE_TABBTNRELEASE_BMP,NULL,IMAGE_TABBTNPRESS_BMP,NULL);
    ldCheckBoxSetRadioButtonGroup(obj,0);
    ldCheckBoxSetStringLeftSpace(obj,25);
    ldCheckBoxSetText(obj,FONT_ALIBABAPUHUITI_3_55_REGULAR_24,(uint8_t*)"系统\n设置");
    ldCheckBoxSetTextColor(obj,GLCD_COLOR_WHITE);
    connect(ID_CHECK_BOX_SYS,SIGNAL_CLICKED_ITEM,slotTabChange);

    obj=ldCheckBoxInit(ID_CHECK_BOX_ABOUT,ID_BG,0,198,100,74);
    ldCheckBoxSetImage(obj,IMAGE_TABBTNRELEASE_BMP,NULL,IMAGE_TABBTNPRESS_BMP,NULL);
    ldCheckBoxSetRadioButtonGroup(obj,0);
    ldCheckBoxSetStringLeftSpace(obj,25);
    ldCheckBoxSetText(obj,FONT_ALIBABAPUHUITI_3_55_REGULAR_24,(uint8_t*)"关于");
    ldCheckBoxSetTextColor(obj,GLCD_COLOR_WHITE);
    connect(ID_CHECK_BOX_ABOUT,SIGNAL_CLICKED_ITEM,slotTabChange);

    obj=ldListInit(ID_LIST,ID_BG,100,50,380,222);
    ldListSetItemHeight(obj,60);
    ldListSetText(obj,listStr,8, FONT_ALIBABAPUHUITI_3_55_REGULAR_24);
    ldListSetAlign(obj,ARM_2D_ALIGN_LEFT);
    ldListSetMargin(obj,5,5,10,10);
    ldListSetSelectColor(obj,__RGB(244,139,0));
    ldListSetTextColor(obj,GLCD_COLOR_WHITE);
    ldListSetBgColor(obj,__RGB(50,50,50));
    ldListSetHidden(obj,true);

    tabSelect=0;
}

void uiSetLoop(ld_scene_t* ptScene)
{




}

void uiSetDraw(ld_scene_t *ptScene,arm_2d_tile_t *ptTile,bool bIsNewFrame)
{
    if(itemNum<2)
    {
        arm_2d_region_t boxRegion={
            .tLocation={
                .iX=120,
                .iY=60,
            },
            .tSize={
                .iWidth=160,
                .iHeight=60,
            }
        };
        arm_2d_tile_t boxTile;

        arm_2d_region_t iconRegion={
            .tLocation={
                .iX=10,
                .iY=15,
            },
            .tSize={
                .iWidth=30,
                .iHeight=30,
            }
        };
        arm_2d_region_t textRegion={
            .tLocation={
                .iX=45,
                .iY=0,
            },
            .tSize={
                .iWidth=115,
                .iHeight=60,
            }
        };

        arm_2d_tile_generate_child(ptTile,&boxRegion,&boxTile,true);

        draw_round_corner_box(&boxTile,NULL,__RGB(25,25,25),255,bIsNewFrame);
        if(itemNum==0)
        {
            ldBaseImage(&boxTile,&iconRegion,IMAGE_PLA_BMP,NULL,0,255);
            textRegion.tSize.iHeight=30;
            ldBaseLabel(&boxTile,&textRegion,(uint8_t*)"PLA温度设置",FONT_ALIBABAPUHUITI_3_55_REGULAR_18,ARM_2D_ALIGN_LEFT,GLCD_COLOR_GREEN,255);
            textRegion.tLocation.iY=30;
            ldBaseLabel(&boxTile,&textRegion,(uint8_t*)"180℃",FONT_ALIBABAPUHUITI_3_55_REGULAR_18,ARM_2D_ALIGN_LEFT,GLCD_COLOR_WHITE,255);
        }
        else
        {
            ldBaseImage(&boxTile,&iconRegion,IMAGE_WIFI_BMP,NULL,0,255);
            ldBaseLabel(&boxTile,&textRegion,(uint8_t*)"WIFI  >",FONT_ALIBABAPUHUITI_3_55_REGULAR_24,ARM_2D_ALIGN_LEFT,GLCD_COLOR_WHITE,255);
        }

        boxTile.tRegion.tLocation.iX+=180;
        draw_round_corner_box(&boxTile,NULL,__RGB(25,25,25),255,bIsNewFrame);
        if(itemNum==0)
        {
            ldBaseImage(&boxTile,&iconRegion,IMAGE_ABS_BMP,NULL,0,255);
            textRegion.tLocation.iY=0;
            ldBaseLabel(&boxTile,&textRegion,(uint8_t*)"ABS温度设置",FONT_ALIBABAPUHUITI_3_55_REGULAR_18,ARM_2D_ALIGN_LEFT,__RGB(244,139,0),255);
            textRegion.tLocation.iY=30;
            ldBaseLabel(&boxTile,&textRegion,(uint8_t*)"180℃",FONT_ALIBABAPUHUITI_3_55_REGULAR_18,ARM_2D_ALIGN_LEFT,GLCD_COLOR_WHITE,255);
        }
        else
        {
            ldBaseImage(&boxTile,&iconRegion,IMAGE_SPEED_BMP,NULL,0,255);
            ldBaseLabel(&boxTile,&textRegion,(uint8_t*)"速度  >",FONT_ALIBABAPUHUITI_3_55_REGULAR_24,ARM_2D_ALIGN_LEFT,GLCD_COLOR_WHITE,255);
        }

        boxTile.tRegion.tLocation.iX-=180;
        boxTile.tRegion.tLocation.iY+=72;
        draw_round_corner_box(&boxTile,NULL,__RGB(25,25,25),255,bIsNewFrame);
        if(itemNum==0)
        {
            ldBaseImage(&boxTile,&iconRegion,IMAGE_BALANCE_BMP,NULL,0,255);
            textRegion.tLocation.iY=0;
            textRegion.tSize.iHeight=60;
            ldBaseLabel(&boxTile,&textRegion,(uint8_t*)"调平   >",FONT_ALIBABAPUHUITI_3_55_REGULAR_18,ARM_2D_ALIGN_LEFT,GLCD_COLOR_WHITE,255);
        }
        else
        {
            ldBaseImage(&boxTile,&iconRegion,IMAGE_ZERO_BMP,NULL,0,255);
            ldBaseLabel(&boxTile,&textRegion,(uint8_t*)"回零  >",FONT_ALIBABAPUHUITI_3_55_REGULAR_24,ARM_2D_ALIGN_LEFT,GLCD_COLOR_WHITE,255);
        }

        boxTile.tRegion.tLocation.iX+=180;
        draw_round_corner_box(&boxTile,NULL,__RGB(25,25,25),255,bIsNewFrame);
        if(itemNum==0)
        {
            ldBaseImage(&boxTile,&iconRegion,IMAGE_LIGHT_BMP,NULL,0,255);
            ldBaseLabel(&boxTile,&textRegion,(uint8_t*)"灯光",FONT_ALIBABAPUHUITI_3_55_REGULAR_18,ARM_2D_ALIGN_LEFT,GLCD_COLOR_WHITE,255);
        }
        else
        {
            ldBaseImage(&boxTile,&iconRegion,IMAGE_PID_BMP,NULL,0,255);
            ldBaseLabel(&boxTile,&textRegion,(uint8_t*)"一键调温>",FONT_ALIBABAPUHUITI_3_55_REGULAR_24,ARM_2D_ALIGN_LEFT,GLCD_COLOR_WHITE,255);
        }

        if(itemNum==0)
        {
            boxTile.tRegion.tLocation.iX-=180;
            boxTile.tRegion.tLocation.iY+=72;
            draw_round_corner_box(&boxTile,NULL,__RGB(25,25,25),255,bIsNewFrame);
            ldBaseImage(&boxTile,&iconRegion,IMAGE_VOICE_BMP,NULL,0,255);
            ldBaseLabel(&boxTile,&textRegion,(uint8_t*)"声音",FONT_ALIBABAPUHUITI_3_55_REGULAR_18,ARM_2D_ALIGN_LEFT,GLCD_COLOR_WHITE,255);

            boxTile.tRegion.tLocation.iX+=180;
            draw_round_corner_box(&boxTile,NULL,__RGB(25,25,25),255,bIsNewFrame);
            ldBaseImage(&boxTile,&iconRegion,IMAGE_BRIGHTNESS_BMP,NULL,0,255);
            ldBaseLabel(&boxTile,&textRegion,(uint8_t*)"亮度   >",FONT_ALIBABAPUHUITI_3_55_REGULAR_18,ARM_2D_ALIGN_LEFT,GLCD_COLOR_WHITE,255);
        }
    }
}

void uiSetQuit(ld_scene_t* ptScene)
{




}
