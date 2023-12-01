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
#include "ldQRCode.h"
#include "ldTable.h"
#include "ldKeyboard.h"
#include "ldLineEdit.h"
#include "ldGraph.h"

ldCommon_t* obj,*le,*kb;

#define BG_WIN   0

void userInit(void)
{
    obj=ldWindowInit(BG_WIN, BG_WIN, 0,0,LD_CFG_SCEEN_WIDTH,LD_CFG_SCEEN_HEIGHT);
    ldWindowSetBgColor(obj,__RGB(220,220,220));

    le=ldLineEditInit(1,BG_WIN,10,10,100,40,WENQUANYI_ZEN_HEI_REGULAR_16,25);

    ldLineEditSetType(le,typeString);



    obj=ldGraphInit(3,BG_WIN,10, 150, 220, 120,2);

//    ldGraphSetPointImageMask(obj,LD_ADDR_NONE,0);

//    ldGraphSetAxisOffset(obj,20);
    ldGraphSetAxis(obj,200,100,10);
    ldGraphSetGrid(obj,10);
    ldGraphAddSeries(obj,LD_COLOR_SKY_BLUE,1,10);

    ldGraphAddSeries(obj,LD_COLOR_LIGHT_PINK,2,20);

    ldGraphSetValue(obj,0,0,0);
    ldGraphSetValue(obj,0,1,10);
    ldGraphSetValue(obj,0,2,15);
    ldGraphSetValue(obj,0,3,5);
    ldGraphSetValue(obj,0,4,40);
    ldGraphSetValue(obj,0,5,80);
    ldGraphSetValue(obj,0,6,60);
    ldGraphSetValue(obj,0,7,70);
    ldGraphSetValue(obj,0,8,80);
    ldGraphSetValue(obj,0,9,90);

    ldGraphSetValue(obj,1,0,0);
    ldGraphSetValue(obj,1,1,5);
    ldGraphSetValue(obj,1,2,10);
    ldGraphSetValue(obj,1,3,15);
    ldGraphSetValue(obj,1,4,20);
    ldGraphSetValue(obj,1,5,25);
    ldGraphSetValue(obj,1,6,30);
    ldGraphSetValue(obj,1,7,35);
    ldGraphSetValue(obj,1,8,40);
    ldGraphSetValue(obj,1,9,45);
    ldGraphSetValue(obj,1,10,35);
    ldGraphSetValue(obj,1,11,33);
    ldGraphSetValue(obj,1,12,30);
    ldGraphSetValue(obj,1,13,28);
    ldGraphSetValue(obj,1,14,27);
    ldGraphSetValue(obj,1,15,26);
    ldGraphSetValue(obj,1,16,25);
    ldGraphSetValue(obj,1,17,20);
    ldGraphSetValue(obj,1,18,15);
    ldGraphSetValue(obj,1,19,12);


    kb=ldKeyboardInit(2,BG_WIN,WENQUANYI_ZEN_HEI_REGULAR_16);
    ldLineEditSetKeyboard(le,kb->nameId);
}


void userInit1(void)
{
//    win1=ldWindowInit(0, 0, 0,0,LD_CFG_SCEEN_WIDTH,LD_CFG_SCEEN_HEIGHT);
//    img1=ldImageInit(1, 0, 100,120,163,65,(uint32_t)&press_bmp[0],false);
}

bool isTop=false;
int64_t timer=0;
uint16_t tempValue=0;
void userLoop(void)
{
    if( ldTimeOut(1000, &timer,true))
    {
        tempValue+=5;
        if(tempValue>90)
        {
            tempValue=0;
        }
//        ldGraphMoveAdd(ldGetWidgetById(3),0,tempValue);
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


