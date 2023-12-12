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
extern size_t xFreeBytesRemaining;

//#define LD_PFB(object) ldBaseAddDirtyRegion(object,&pSence->ptDirtyRegion)

void userInit(void)
{
    obj=ldMalloc(1);
    ldFree(obj);
//    LOG_DEBUG("start :%llu\n",xFreeBytesRemaining);
    obj=ldWindowInit(BG_WIN, BG_WIN, 0,0,LD_CFG_SCEEN_WIDTH,LD_CFG_SCEEN_HEIGHT);

    ldWindowSetBgColor(obj,__RGB(220,220,220));

//    LOG_DEBUG("win :%llu\n",xFreeBytesRemaining);

    le=ldLineEditInit(1,BG_WIN,10,10,100,50,WENQUANYI_ZEN_HEI_REGULAR_16,10);

    ldLineEditSetType(le,typeInt);
    kb=ldKeyboardInit(2,BG_WIN,WENQUANYI_ZEN_HEI_REGULAR_16);

    ldLineEditSetKeyboard(le,((ldCommon_t*)kb)->nameId);

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
//    if( ldTimeOut(2000, &timer,true))
//    {

//        isTop=!isTop;
//        if(isTop)
//        {
//        ldWindowSetBgColor(ldBaseGetWidgetById(0),0xFF);
////        ldGraphMove(ldBaseGetWidgetById(1),20,20);
////        ldGraphSetHidden(ldBaseGetWidgetById(1),1);
//        }
//        else
//        {
//        ldWindowSetBgColor(ldBaseGetWidgetById(0),0xFFFF);
////        ldGraphMove(ldBaseGetWidgetById(1),10,10);
////        ldGraphSetHidden(ldBaseGetWidgetById(1),0);
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


