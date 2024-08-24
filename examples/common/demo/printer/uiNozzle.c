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
