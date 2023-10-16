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

ldWindow_t *win0;
ldWindow_t *win1;

ldScrollSelecter_t *scroll;
ldRadialMenu_t* menu;
void userInit(void)
{
    win0=ldWindowInit(0, 0, 0,0,LD_CFG_SCEEN_WIDTH,LD_CFG_SCEEN_HEIGHT);
    ldWindowSetBgColor(win0,__RGB(220,220,220));

    win1=ldWindowInit(1, 0, 30,20,400,200);
    ldWindowSetBgColor(win1,__RGB(180,180,180));

    scroll=ldScrollSelecterInit(2,1,10,10,72,72,SIMSUN_REGULAR_12,3);
    ldScrollSelecterAddItem(scroll,"123");
    ldScrollSelecterAddItem(scroll,"212");
//ldScrollSelecterSetBgColor(scroll,__RGB(180,0,0));
    ldScrollSelecterSetBgImage(scroll,PRESS_BMP);
//    //radial menu
//    menu=ldRadialMenuInit(10,1,0,10,480,200,300,100,9);

////    ldRadialMenuAddItem(menu,ICON1_JPEG,100,100,1,false);
//    ldRadialMenuAddItem(menu,PRESS_BMP,72,72,1,false);
////    ldRadialMenuAddItem(menu,ICON3_JPEG,100,100,1,false);
//    ldRadialMenuAddItem(menu,PRESS_BMP,72,72,1,false);
//    ldRadialMenuAddItem(menu,PRESS_BMP,72,72,1,false);
//    ldRadialMenuAddItem(menu,PRESS_BMP,72,72,1,false);
//    ldRadialMenuAddItem(menu,PRESS_BMP,72,72,1,false);
//    ldRadialMenuAddItem(menu,PRESS_BMP,72,72,1,false);
//    ldRadialMenuAddItem(menu,PRESS_BMP,72,72,1,false);

}


void userInit1(void)
{
}

void userLoop(void)
{
}

void userLoop1(void)
{
}

void userQuit(void)
{
}

void userQuit1(void)
{
}

void (*ldUserPageInitFunc[LD_PAGE_MAX])(void)={userInit,userInit1};
void (*ldUserPageLoopFunc[LD_PAGE_MAX])(void)={userLoop,userLoop1};
void (*ldUserPageQuitFunc[LD_PAGE_MAX])(void)={userQuit,userQuit1};


