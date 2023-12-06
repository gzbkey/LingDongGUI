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
#include "ldComboBox.h"

void* obj,*le,*kb;

#define BG_WIN   0

void userInit(void)
{
    obj=ldWindowInit(BG_WIN, BG_WIN, 0,0,LD_CFG_SCEEN_WIDTH,LD_CFG_SCEEN_HEIGHT);
    ldWindowSetBgColor(obj,__RGB(220,220,0));

    ldWindowSetStatic(obj,true);

    obj=ldGraphInit(1,BG_WIN,10,10,220,220,1);
//    ldGraphSetGridOffset(obj,8);
//    ldGraphSetAxis(obj,180,180,5);

//    ldGraphSetFrameSpace(obj,0,false);

    obj=ldDateTimeInit(2,BG_WIN,10,250,200,30,WENQUANYI_ZEN_HEI_REGULAR_16);

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
//        tempValue+=5;
//        if(tempValue>90)
//        {
//            tempValue=0;
//        }
        isTop=!isTop;
        if(isTop)
        {
        ldWindowSetBgColor(ldGetWidgetById(0),0xFF);
        }
        else
        {
        ldWindowSetBgColor(ldGetWidgetById(0),0xFFFF);
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


