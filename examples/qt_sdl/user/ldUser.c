#include "ldUser.h"
#include "ldButton.h"
#include "ldWindow.h"
#include "ldGui.h"
#include "ldConfig.h"
#include "xBtnAction.h"
#include "image.h"

ldImage_t *img0;
ldWindow_t *win0;
ldImage_t *img1;
ldWindow_t *win1;

ldButton_t *btn0,*btn1,*targetBtn;
int64_t timer=0;

#define ID_BTN0      2
#define ID_BTN1      3
void userInit(void)
{
    win0=ldWindowInit(0, 0, 0,0,LD_CFG_SCEEN_WIDTH,LD_CFG_SCEEN_HEIGHT);
    ldWindowSetBgColor(win0,__RGB(255,255,255));
    img0=ldImageInit(1, 0, 20,20,72,72,RELEASE_PNG,true);
    ldImageSetOpacity(img0,128);
    
    

    btn0=ldButtonInit(ID_BTN0, 0, 20,100,72,72);

    ldButtonSetImage(btn0,RELEASE_PNG,true,PRESS_BMP,false);

    ldButtonSetSelectImage(btn0,FRAME_PNG,__RGB(255,0,0));



    btn1=ldButtonInit(ID_BTN1, 0, 120,100,72,72);
//    ldButtonSetTextColor(btn1,0x0);
//    ldBaseSetTextInfo(btn1,,GLCD_COLOR_RED,64);

    ldButtonSetFont(btn1,MICROSOFT_YAHEI_UI_BOLD_20);
//    ldButtonSetTextColor(btn1,GLCD_COLOR_RED);
    ldButtonSetText(btn1,(uint8_t*)"栟𨭉");

//    ldButtonSetAlign(btn1,LD_ALIGN_TOP|LD_ALIGN_LEFT);



//    targetBtn=btn1;
//    btn1->isSelected=true;
}

void userInit1(void)
{
//    win1=ldWindowInit(0, 0, 0,0,LD_CFG_SCEEN_WIDTH,LD_CFG_SCEEN_HEIGHT);
//    img1=ldImageInit(1, 0, 100,120,163,65,(uint32_t)&press_bmp[0],false);
}

void userLoop(void)
{
//    if( ldTimeOut(3000, &timer,true))
//    {
//        btn0->isPressed=!btn0->isPressed;
////        ldGuiJumpPage(1);
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


