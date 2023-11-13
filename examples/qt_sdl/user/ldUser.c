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
#include "ldLabel.h"
#include "ldScrollSelecter.h"
#include "ldDateTime.h"
#include "ldIconSlider.h"
#include "ldGauge.h"

void* obj;

#define BG_WIN   0
#define VOL_LABEL_NAME 1
#define VOL_NUM_TEN  2
#define VOL_NUM_IND  3
#define VOL_LABEL_POINT 4
#define VOL_NUM_POINT1  5

void userInit(void)
{
    obj=ldWindowInit(BG_WIN, BG_WIN, 0,0,LD_CFG_SCEEN_WIDTH,LD_CFG_SCEEN_HEIGHT);
    ldWindowSetBgColor(obj,__RGB(220,220,220));

    obj=ldGaugeInit(1,0, 10, 10, 100, 100,BBG_BMP,false);

    ldGaugeSetPointerImage(obj,POINTERMASK_PNG,9,55,5,45);

    ldGaugeSetPointerImageType(obj,onlyMask,__RGB(0,0,255));

//    ldGaugeSetCenterOffset(obj,0,30);

    ldGaugeSetAngle(obj,120);
}


void userInit1(void)
{
//    win1=ldWindowInit(0, 0, 0,0,LD_CFG_SCEEN_WIDTH,LD_CFG_SCEEN_HEIGHT);
//    img1=ldImageInit(1, 0, 100,120,163,65,(uint32_t)&press_bmp[0],false);
}

bool isTop=false;
void userLoop(void)
{
//    if( ldTimeOut(3000, &timer,true))
//    {
////        btn0->isPressed=!btn0->isPressed;
////        ldGuiJumpPage(1);

//        if(isTop)
//        {
//            ldScrollSelecterSetItem(scroll,0);
//        }
//        else
//        {
//            ldScrollSelecterSetItem(scroll,9);
//        }
//isTop=!isTop;
//    }

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
//    if( ldTimeOut(3000, &timer,true))
//    {
//        ldGuiJumpPage(0);
//    }
}

void userQuit(void)
{
//        pImageDel(img);
//    ldWindowDel(win0);
}

void userQuit1(void)
{
//        pImageDel(img);
//    ldWindowDel(win1);
}

void (*ldUserPageInitFunc[LD_PAGE_MAX])(void)={userInit,userInit1};
void (*ldUserPageLoopFunc[LD_PAGE_MAX])(void)={userLoop,userLoop1};
void (*ldUserPageQuitFunc[LD_PAGE_MAX])(void)={userQuit,userQuit1};


