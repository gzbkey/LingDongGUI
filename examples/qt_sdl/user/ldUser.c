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

//    obj=ldLabelInit(VOL_LABEL_NAME,BG_WIN,20,200,80,40,WENQUANYI_ZEN_HEI_REGULAR_36);
//    ldLabelSetText(obj,"电压:");
//    ldLabelSetAlign(obj,LD_ALIGN_LEFT);
//    ldLabelSetBgColor(obj,__RGB(128,128,128));

    obj=ldScrollSelecterInit(VOL_NUM_TEN,BG_WIN,105,200,22,40,WENQUANYI_ZEN_HEI_REGULAR_36,10);
    ldScrollSelecterSetBgColor(obj,__RGB(255,255,255));
//    ldScrollSelecterSetBgImage(scroll,PRESS_BMP);
    ldScrollSelecterSetSpeed(obj,10);
    ldScrollSelecterAddItem(obj,(uint8_t*)"2");
    ldScrollSelecterAddItem(obj,(uint8_t*)"1");
    ldScrollSelecterAddItem(obj,(uint8_t*)"0");
    ldScrollSelecterSetItem(obj,0);

    obj=ldScrollSelecterInit(VOL_NUM_IND,BG_WIN,127,200,22,40,WENQUANYI_ZEN_HEI_REGULAR_36,10);
    ldScrollSelecterSetBgColor(obj,__RGB(255,255,255));
//    ldScrollSelecterSetBgImage(scroll,PRESS_BMP);
    ldScrollSelecterSetAlign(obj,LD_ALIGN_LEFT);
    ldScrollSelecterSetSpeed(obj,10);
    ldScrollSelecterAddItem(obj,(uint8_t*)"0123456789");
    ldScrollSelecterAddItem(obj,(uint8_t*)"8");
    ldScrollSelecterAddItem(obj,(uint8_t*)"7");
    ldScrollSelecterAddItem(obj,(uint8_t*)"6");
    ldScrollSelecterAddItem(obj,(uint8_t*)"5");
    ldScrollSelecterAddItem(obj,(uint8_t*)"4");
    ldScrollSelecterAddItem(obj,(uint8_t*)"3");
    ldScrollSelecterAddItem(obj,(uint8_t*)"2");
    ldScrollSelecterAddItem(obj,(uint8_t*)"1");
    ldScrollSelecterAddItem(obj,(uint8_t*)"10");

//    ldScrollSelecterSetItem(obj,7);

    obj=ldDateTimeInit(6,BG_WIN,0,10,240, 40,WENQUANYI_ZEN_HEI_REGULAR_36);

    ldDateTimeSetFormat(obj,"yyyy-mm-dd hh:nn:ss");

    obj=ldIconSliderInit(7,BG_WIN,0,60,200,140,52,10,5,1,1,WENQUANYI_ZEN_HEI_REGULAR_36);

    ldIconSliderAddIcon(obj,ICON_BMP,"12");
    ldIconSliderAddIcon(obj,ICON_BMP,"34");
    ldIconSliderAddIcon(obj,ICON_BMP,"56");
    ldIconSliderAddIcon(obj,ICON_BMP,"78");
    ldIconSliderAddIcon(obj,ICON_BMP,"90");

//    ldIconSliderSetHorizontalScroll(obj,false);
    ldIconSliderSetSpeed(obj,3);
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


