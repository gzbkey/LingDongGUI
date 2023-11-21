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

void* obj;

#define BG_WIN   0

uint8_t strTest[]="5693";
void userInit(void)
{
    obj=ldWindowInit(BG_WIN, BG_WIN, 0,0,LD_CFG_SCEEN_WIDTH,LD_CFG_SCEEN_HEIGHT);
    ldWindowSetBgColor(obj,__RGB(220,220,220));

    obj=ldQRCodeInit(2,BG_WIN,10, 100, 50, 50, "ldgui",0,__RGB(255,255,255), qrcodegen_Ecc_LOW, qrcodegen_Mask_0, 2, 2);

    obj=ldTableInit(3,BG_WIN,10,10, 200, 80, 3, 3, 2);

    ldTableSetItemWidth(obj,3,12);

    ldTableSetItemText(obj,0,0,"1",WENQUANYI_ZEN_HEI_REGULAR_9);
    ldTableSetItemText(obj,0,1,"2",WENQUANYI_ZEN_HEI_REGULAR_9);
    ldTableSetItemText(obj,0,2,"3",WENQUANYI_ZEN_HEI_REGULAR_9);
    ldTableSetItemText(obj,1,2,"4",WENQUANYI_ZEN_HEI_REGULAR_9);
    ldTableSetItemStaticText(obj,2,2,strTest,WENQUANYI_ZEN_HEI_REGULAR_9);
    ldTableSetItemColor(obj,2,2,GLCD_COLOR_WHITE,GLCD_COLOR_YELLOW);

    ldTableSetItemWidth(obj,0,100);
//    ldTableSetItemHeight(obj,0,100);

    ldTableSetItemButton(obj,2,2, 10,5, 52, 52, ICON_BMP, BBG_BMP, true);


    obj=ldCheckBoxInit(4,BG_WIN,10,200,80,50);
    ldCheckBoxSetText(obj,"123");
    ldCheckBoxSetFont(obj,WENQUANYI_ZEN_HEI_REGULAR_9);
//    ldCheckBoxSetColor(obj,0xFF,0xFF00);
//    ldCheckBoxSetCharColor(obj,0xFF);

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


