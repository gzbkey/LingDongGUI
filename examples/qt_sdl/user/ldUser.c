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

#define ID_BG    0
#define ID_BUTTON   1
#define ID_IMAGE   2
#define ID_TEXT   3
#define ID_PROGRESS_BAR   4
#define ID_RADIAL_MENU   5
#define ID_CHECK_BOX   6
#define ID_LABLE   7
#define ID_SCROLL_SELECTER   8
#define ID_DATE_TIME   9
#define ID_ICON_SLIDER   10
#define ID_GAUGE   11
#define ID_QR_CODE   12
#define ID_TABLE   13
#define ID_KB   14
#define ID_LINE_EDIT   15
#define ID_GRAPH   16
#define ID_COMBO_BOX   17
#define ID_WIN   18

extern size_t xFreeBytesRemaining;

//#define LD_PFB(object) ldBaseAddDirtyRegion(object,&pSence->ptDirtyRegion)

#define LE_H          200

void userInit(void)
{
    obj=ldMalloc(1);
    ldFree(obj);
//    LOG_DEBUG("start :%llu\n",xFreeBytesRemaining);
    obj=ldWindowInit(ID_BG, ID_BG, 0,0,LD_CFG_SCEEN_WIDTH,LD_CFG_SCEEN_HEIGHT);

    ldWindowSetBgColor(obj,__RGB(220,220,220));

//    LOG_DEBUG("win :%llu\n",xFreeBytesRemaining);

    obj=ldButtonInit(ID_BUTTON,ID_BG,10,10,70,30);
    ldButtonSetFont(obj,SIMSUN_REGULAR_12);
    ldButtonSetText(obj,"123");
    ldButtonSetImage(obj,RELEASE_BMP,false,PRESS_BMP,false);
    ldButtonSetRoundCorner(obj,true);
    ldButtonSetSelectImage(obj,FRAME_PNG,LD_COLOR_BLUE);
    ldButtonSetSelect(obj,true);

    obj=ldImageInit(ID_IMAGE,ID_BG,10,50,52,52,BBG_BMP,false);

    obj=ldTextInit(ID_TEXT,ID_BG,10,110,121,121,SIMSUN_REGULAR_12);
    ldTextSetBgImage(obj,TREE_BMP);
    ldTextSetText(obj,"12345679\n123");
    ldTextSetTextColor(obj,LD_COLOR_WHITE);
    ldTextSetAlign(obj,LD_ALIGN_LEFT|LD_ALIGN_TOP);

    obj=ldGaugeInit(ID_GAUGE,ID_BG,150,10,121,121,TREE_BMP,false);
    ldGaugeSetPointerImage(obj,POINTER_PNG,9,55,4,37);
    ldGaugeSetPointerImageType(obj,withMask,0);

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
    if( ldTimeOut(2000, &timer,true))
    {

        isTop=!isTop;
        if(isTop)
        {
//        ldWindowSetBgColor(ldBaseGetWidgetById(0),0xFF);
//        ldLineEditMove(le,10,LE_H);
//        ldGraphSetHidden(ldBaseGetWidgetById(1),1);

            ldGaugeSetAngle(ldBaseGetWidgetById(ID_GAUGE),30);
        }
        else
        {
//        ldWindowSetBgColor(ldBaseGetWidgetById(0),0xFFFF);
//        ldLineEditMove(le,50,LE_H);
//        ldGraphSetHidden(ldBaseGetWidgetById(1),0);
            ldGaugeSetAngle(ldBaseGetWidgetById(ID_GAUGE),180);
        }

    }
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


