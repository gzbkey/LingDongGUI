#include "ldUser.h"
#include "ldButton.h"
#include "ldWindow.h"
#include "ldGui.h"
#include "ldConfig.h"
#include "xBtnAction.h"
#include "image.h"
#include "ldText.h"
#include "ldProgressBar.h"
#include "ldRadialMenu.h"
#include "ldCheckBox.h"

ldImage_t *img0;
ldWindow_t *win0;
ldImage_t *img1;
ldWindow_t *win1;

ldButton_t *btn0,*btn1,*targetBtn;
int64_t timer=0;
ldText_t *txt0;
ldProgressBar_t *bar,*bar1,*bar2,*bar3;

ldRadialMenu_t* menu;

ldCheckBox_t *cBox;


void userInit(void)
{
    win0=ldWindowInit(0, 0, 0,0,LD_CFG_SCEEN_WIDTH,LD_CFG_SCEEN_HEIGHT);
    ldWindowSetBgColor(win0,__RGB(220,220,220));

    win1=ldWindowInit(1, 0, 30,20,400,200);
    ldWindowSetBgColor(win1,__RGB(180,180,180));

    img0=ldImageInit(2, 1, 10,40,72,72,RELEASE_PNG,true);
    ldImageSetOpacity(img0,128);


    btn0=ldButtonInit(3, 1, 120,20,72,72);

    ldButtonSetImage(btn0,RELEASE_PNG,true,PRESS_BMP,false);

    ldButtonSetSelectImage(btn0,FRAME_PNG,__RGB(255,0,0));
    ldButtonSetRoundCorner(btn0,true);
//    ldButtonSetSelect(btn0,true);

    btn1=ldButtonInit(4, 1, 120,100,72,72);
    ldButtonSetFont(btn1,MICROSOFT_YAHEI_REGULAR_20);
    ldButtonSetTextColor(btn1,GLCD_COLOR_RED);

    ldButtonSetText(btn1,(uint8_t*)"栟𨭉");
//    ldButtonSetAlign(btn1,LD_ALIGN_TOP|LD_ALIGN_LEFT);

    ldButtonSetRoundCorner(btn1,true);



    ldButtonSetSelect(btn1,true);

    txt0=ldTextInit(5, 1, 310, 10, 50, 100,MICROSOFT_YAHEI_REGULAR_20);
//    ldTextSetTransparent(txt0,true);
    ldTextSetText(ldGetWidgetById(5),"栟𨭉栟𨭉栟𨭉栟𨭉栟𨭉栟𨭉栟𨭉栟𨭉栟𨭉栟栟");
    ldTextSetAlign(ldGetWidgetById(5),LD_ALIGN_TOP|LD_ALIGN_LEFT);
    ldTextSetScroll(txt0,true);

    bar=ldProgressBarInit(6,1,200,15,100,18);
    ldProgressBarSetPercent(bar,50);
    ldProgressBarSetBgImage(bar,PRESS_BMP,72,false);
    ldProgressBarSetFgImage(bar,RELEASE_BMP,72,true);
    ldProgressBarSetFrameImage(bar,BARFRAME_PNG,100);

    bar1=ldProgressBarInit(7,1,200,50,100,18);
    ldProgressBarSetPercent(bar1,50);
    ldProgressBarSetBgImage(bar1,PRESS_BMP,72,true);
    ldProgressBarSetFgImage(bar1,RELEASE_BMP,72,false);
//    ldProgressBarSetFrameImage(bar1,BARFRAME_PNG);

    bar2=ldProgressBarInit(8,1,200,80,100,18);
    ldProgressBarSetPercent(bar2,50);
    ldProgressBarSetBgImage(bar2,PRESS_BMP,72,false);
    ldProgressBarSetFgImage(bar2,RELEASE_BMP,72,true);
//    ldProgressBarSetFrameImage(bar2,BARFRAME_PNG);

    bar3=ldProgressBarInit(9,1,200,110,90,18);
    ldProgressBarSetPercent(bar3,50);

    //radial menu
//    menu=ldRadialMenuInit(10,0,0,10,480,200,300,100,9);

////    ldRadialMenuAddItem(menu,ICON1_JPEG,100,100,1,false);
//    ldRadialMenuAddItem(menu,ICON2_JPEG,100,100,1,false);
////    ldRadialMenuAddItem(menu,ICON3_JPEG,100,100,1,false);
//    ldRadialMenuAddItem(menu,ICON4_JPEG,100,100,1,false);
//    ldRadialMenuAddItem(menu,ICON5_JPEG,100,100,1,false);
//    ldRadialMenuAddItem(menu,ICON6_JPEG,100,100,1,false);
//    ldRadialMenuAddItem(menu,ICON7_JPEG,100,100,1,false);
//    ldRadialMenuAddItem(menu,ICON8_JPEG,100,100,1,false);
//    ldRadialMenuAddItem(menu,ICON9_PNG,100,100,1,true);

//        menu=ldRadialMenuInit(10,1,0,10,480,200,300,100,9);

//        ldRadialMenuAddItem(menu,PRESS_BMP,72,72,1,false);
//        ldRadialMenuAddItem(menu,PRESS_BMP,72,72,1,false);
//        ldRadialMenuAddItem(menu,RELEASE_BMP,72,72,1,false);
//        ldRadialMenuAddItem(menu,RELEASE_BMP,72,72,1,false);

    cBox=ldCheckBoxInit(11,1,10,120,144,72);

    ldCheckBoxSetFont(cBox,MICROSOFT_YAHEI_REGULAR_20);
    ldCheckBoxSetText(cBox,"栟𨭉");
//    ldCheckBoxSetColor(cBox,GLCD_COLOR_BLUE,GLCD_COLOR_WHITE);
    ldCheckBoxSetImage(cBox,72,PRESS_BMP,false,RELEASE_PNG,true);
}


void userInit1(void)
{
//    win1=ldWindowInit(0, 0, 0,0,LD_CFG_SCEEN_WIDTH,LD_CFG_SCEEN_HEIGHT);
//    img1=ldImageInit(1, 0, 100,120,163,65,(uint32_t)&press_bmp[0],false);
}

void userLoop(void)
{
    if( ldTimeOut(200, &timer,true))
    {
//        btn0->isPressed=!btn0->isPressed;
//        ldGuiJumpPage(1);
    }

//    if(xBtnGetState(KEY_NUM_LEFT,BTN_PRESS))
//    {
//        if(targetBtn==btn1)
//        {
//            btn0->isSelected=true;
//            btn1->isSelected=false;
//            targetBtn=btn0;
//        }
//        else
//        {
//            btn0->isSelected=false;
//            btn1->isSelected=true;
//            targetBtn=btn1;
//        }
//    }
//    if(xBtnGetState(KEY_NUM_RIGHT,BTN_PRESS))
//    {
//        if(targetBtn==btn1)
//        {
//            btn0->isSelected=true;
//            btn1->isSelected=false;
//            targetBtn=btn0;
//        }
//        else
//        {
//            btn0->isSelected=false;
//            btn1->isSelected=true;
//            targetBtn=btn1;
//        }
//    }
}

void userLoop1(void)
{
    if( ldTimeOut(3000, &timer,true))
    {
        ldGuiJumpPage(0);
    }
}

void userQuit(void)
{
//        pImageDel(img);
    ldWindowDel(win0);
}

void userQuit1(void)
{
//        pImageDel(img);
    ldWindowDel(win1);
}

void (*ldUserPageInitFunc[LD_PAGE_MAX])(void)={userInit,userInit1};
void (*ldUserPageLoopFunc[LD_PAGE_MAX])(void)={userLoop,userLoop1};
void (*ldUserPageQuitFunc[LD_PAGE_MAX])(void)={userQuit,userQuit1};


