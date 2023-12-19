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

#define ID_Radio1   19
#define ID_Radio2   20

extern size_t xFreeBytesRemaining;

void userInit(void)
{
//    obj=ldMalloc(1);
//    ldFree(obj);
//    LOG_DEBUG("start :%llu\n",xFreeBytesRemaining);
    obj=ldWindowInit(ID_BG, ID_BG, 0,0,LD_CFG_SCEEN_WIDTH,LD_CFG_SCEEN_HEIGHT);

    ldWindowSetBgColor(obj,__RGB(220,220,220));

//    LOG_DEBUG("win :%llu\n",xFreeBytesRemaining);

    obj=ldWindowInit(ID_WIN,ID_BG,10,10,LD_CFG_SCEEN_WIDTH,LD_CFG_SCEEN_WIDTH);

    obj=ldButtonInit(ID_BUTTON,ID_WIN,10,10,70,30);
    ldButtonSetFont(obj,SIMSUN_REGULAR_12);
    ldButtonSetText(obj,(uint8_t*)"123");
    ldButtonSetImage(obj,RELEASE_BMP,false,PRESS_BMP,false);
    ldButtonSetRoundCorner(obj,true);
    ldButtonSetSelectImage(obj,FRAME_PNG,LD_COLOR_BLUE);
    ldButtonSetSelect(obj,true);

    obj=ldImageInit(ID_IMAGE,ID_WIN,10,50,52,52,BBG_BMP,false);

    obj=ldTextInit(ID_TEXT,ID_WIN,10,110,121,121,SIMSUN_REGULAR_12);
    ldTextSetBgImage(obj,TREE_BMP);
    ldTextSetText(obj,(uint8_t*)"12345679\n123");
    ldTextSetTextColor(obj,LD_COLOR_WHITE);
    ldTextSetAlign(obj,LD_ALIGN_LEFT|LD_ALIGN_TOP);

    obj=ldProgressBarInit(ID_PROGRESS_BAR,ID_WIN,10,250,100,30);
    ldProgressBarSetPercent(obj,65);
    ldProgressBarSetBgImage(obj,BBG_BMP,52,false);
    ldProgressBarSetFgImage(obj,ICON_BMP,52,true);

    obj=ldRadialMenuInit(ID_RADIAL_MENU,ID_WIN,10,320,200,150,170,100,5);
    ldRadialMenuAddItem(obj,BBG_BMP,52,52,0,false);
    ldRadialMenuAddItem(obj,ICON_BMP,52,52,0,false);
    ldRadialMenuAddItem(obj,PRESS_BMP,70,30,0,false);
    ldRadialMenuAddItem(obj,RELEASE_BMP,70,30,0,false);



    obj=ldDateTimeInit(ID_DATE_TIME,ID_WIN,150,10,150,30,SIMSUN_REGULAR_12);
    ldDateTimeSetBgColor(obj,LD_COLOR_LIGHT_CYAN);

    obj=ldIconSliderInit(ID_ICON_SLIDER,ID_WIN,150,50,100,65,52,2,5,1,1,SIMSUN_REGULAR_12);
    ldIconSliderAddIcon(obj,BBG_BMP,(uint8_t*)"111");
    ldIconSliderAddIcon(obj,ICON_BMP,(uint8_t*)"22");
    ldIconSliderAddIcon(obj,BBG_BMP,(uint8_t*)"33");
    ldIconSliderAddIcon(obj,ICON_BMP,(uint8_t*)"44");
    ldIconSliderAddIcon(obj,BBG_BMP,(uint8_t*)"55");

    obj=ldGaugeInit(ID_GAUGE,ID_WIN,150,120,121,121,TREE_BMP,false);
    ldGaugeSetPointerImage(obj,POINTER_PNG,9,55,4,37);
    ldGaugeSetPointerImageType(obj,withMask,0);

    obj=ldQRCodeInit(ID_QR_CODE,ID_WIN,150,250,60,60,(uint8_t*)"ldgui",LD_COLOR_LIGHT_BLUE,LD_COLOR_WHITE,eccHigh,2,2);

    obj=ldTableInit(ID_TABLE,ID_WIN,320,10,100,100,3,3,1);
    ldTableSetItemWidth(obj,1,60);
    ldTableSetBgColor(obj,__RGB(230,230,230));
    ldTableSetItemColor(obj,0,0,LD_COLOR_BLACK,__RGB(238,238,238));
    ldTableSetItemColor(obj,0,1,LD_COLOR_BLACK,__RGB(238,238,238));
    ldTableSetItemColor(obj,0,2,LD_COLOR_BLACK,__RGB(238,238,238));

    ldTableSetItemText(obj,0,0,(uint8_t*)"id",SIMSUN_REGULAR_12);
    ldTableSetItemText(obj,0,1,(uint8_t*)"name",SIMSUN_REGULAR_12);
    ldTableSetItemText(obj,0,2,(uint8_t*)"size",SIMSUN_REGULAR_12);

    ldTableSetItemText(obj,1,0,(uint8_t*)"1",SIMSUN_REGULAR_12);
    ldTableSetItemText(obj,1,1,(uint8_t*)"button",SIMSUN_REGULAR_12);
    ldTableSetItemText(obj,1,2,(uint8_t*)"30*20",SIMSUN_REGULAR_12);

    ldTableSetItemText(obj,2,0,(uint8_t*)"2",SIMSUN_REGULAR_12);
    ldTableSetItemText(obj,2,1,(uint8_t*)"image",SIMSUN_REGULAR_12);
    ldTableSetItemText(obj,2,2,(uint8_t*)"100*100",SIMSUN_REGULAR_12);

    obj=ldLabelInit(ID_LABLE,ID_WIN,320,165,100,30,SIMSUN_REGULAR_12);
    ldLabelSetText(obj,(uint8_t*)"lable demo");

    obj=ldCheckBoxInit(ID_CHECK_BOX,ID_WIN,320,200,60,30);
    ldCheckBoxSetText(obj,SIMSUN_REGULAR_12,(uint8_t*)"123");
    ldCheckBoxSetCorner(obj,true);

    obj=ldCheckBoxInit(ID_Radio1,ID_WIN,320,240,60,30);
    ldCheckBoxSetText(obj,SIMSUN_REGULAR_12,(uint8_t*)"abc");
    ldCheckBoxSetRadioButtonGroup(obj,0);

    obj=ldCheckBoxInit(ID_Radio2,ID_WIN,380,240,60,30);
    ldCheckBoxSetText(obj,SIMSUN_REGULAR_12,(uint8_t*)"xyz");
    ldCheckBoxSetRadioButtonGroup(obj,0);

    obj=ldGraphInit(ID_GRAPH,ID_WIN,320,350,100,100,2);
    ldGraphSetAxis(obj,80,80,5);
    ldGraphSetGridOffset(obj,4);
    ldGraphAddSeries(obj,LD_COLOR_LIGHT_PINK,2,16);
    srand(10);
    for (int i = 0; i < 16; i++)
    {
        ldGraphSetValue(obj,0,i,rand() % 81);
    }
    ldGraphAddSeries(obj,LD_COLOR_LIGHT_BLUE,2,16);
    for (int i = 0; i < 16; i++)
    {
        ldGraphSetValue(obj,1,i,rand() % 81);
    }

    obj=ldComboBoxInit(ID_COMBO_BOX,ID_WIN,220,300,80,30,SIMSUN_REGULAR_12,3);
    ldComboBoxAddItem(obj,(uint8_t*)"tool");
    ldComboBoxAddItem(obj,(uint8_t*)"setting");
    ldComboBoxAddItem(obj,(uint8_t*)"about");

    // 120 280
    obj=ldLineEditInit(ID_LINE_EDIT,ID_WIN,320,280,100,40,SIMSUN_REGULAR_12,10);
    ldKeyboardInit(ID_KB,SIMSUN_REGULAR_12);
    ldLineEditSetKeyboard(obj,ID_KB);
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
//            ldRadialMenuSelectItem(ldBaseGetWidgetById(ID_RADIAL_MENU),1);
        }
        else
        {
//        ldWindowSetBgColor(ldBaseGetWidgetById(0),0xFFFF);
//        ldLineEditMove(le,50,LE_H);
//        ldGraphSetHidden(ldBaseGetWidgetById(1),0);
            ldGaugeSetAngle(ldBaseGetWidgetById(ID_GAUGE),180);
//            ldRadialMenuSelectItem(ldBaseGetWidgetById(ID_RADIAL_MENU),2);
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


