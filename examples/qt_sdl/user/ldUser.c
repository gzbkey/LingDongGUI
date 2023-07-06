#include "ldUser.h"
#include "ldButton.h"
#include "ldWindow.h"
#include "ldGui.h"


ldImage_t *img0;
ldWindow_t *win0;
ldImage_t *img1;
ldWindow_t *win1;

ldButton_t *btn0;
int64_t timer=0;



extern const uint16_t c_bmpCMSISLogoRGB565[163*65];

void userInit(void)
{
    win0=ldWindowInit(0, 0, 0,0,320,240,0xFF00,0,0,0,0,0);
    img0=ldImageInit(1, 0, 20,20,163,65,0,0,(uint32_t)&c_bmpCMSISLogoRGB565[0],0,0);
    
    
    
    
    btn0=ldButtonInit(2, 0, 20,100,100,40,0xFF,0xFF00,0x01F0,0,0,0,0,false,false,false);

}

void userInit1(void)
{
    win1=ldWindowInit(0, 0, 0,0,320,240,0xFF00,0,0,0,0,0);
    img1=ldImageInit(1, 0, 100,120,163,65,0,0,(uint32_t)&c_bmpCMSISLogoRGB565[0],0,0);
}

void userLoop(void)
{
    if( ldTimeOut(3000, &timer,true))
    {
        btn0->isPressed=!btn0->isPressed;
//        ldGuiJumpPage(1);
    }
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
    pWindowDel(win0);
}

void userQuit1(void)
{
//        pImageDel(img);
    pWindowDel(win1);
}

void (*ldUserPageInitFunc[LD_PAGE_MAX])(void)={userInit,userInit1};
void (*ldUserPageLoopFunc[LD_PAGE_MAX])(void)={userLoop,userLoop1};
void (*ldUserPageQuitFunc[LD_PAGE_MAX])(void)={userQuit,userQuit1};


