#include "ldUser.h"
#include "ldWindow.h"
#include "ldGui.h"


ldImage *img0;
ldWindow *win0;
ldImage *img1;
ldWindow *win1;
extern const uint16_t c_bmpCMSISLogoRGB565[163*65];

void userInit(void)
{
    win0=ldWindowInit(0, 0, 0,0,320,240,0xFF00,0,0,0,0,0);

    img0=ldImageInit(1, 0, 20,20,163,65,0xFF,(uint32_t)&c_bmpCMSISLogoRGB565[0],1,0,0,0);


}

void userInit1(void)
{
    win1=ldWindowInit(0, 0, 0,0,320,240,0xFF00,0,0,0,0,0);

    img1=ldImageInit(1, 0, 100,120,163,65,0xFF,(uint32_t)&c_bmpCMSISLogoRGB565[0],1,0,0,0);


}

int64_t timer=0;

void userLoop(void)
{
    if( ldTimeOut(5000, &timer,true))
    {
        ldGuiJumpPage(1);
    }
}

void userLoop1(void)
{
    if( ldTimeOut(5000, &timer,true))
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


