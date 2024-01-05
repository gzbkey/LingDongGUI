#include "uiDemo.h"

#define ID_BG    0
#define ID_BUTTON   1

void uiDemoInit(void)
{
    void* obj;

    obj=ldWindowInit(ID_BG, ID_BG, 0,0,LD_CFG_SCEEN_WIDTH,LD_CFG_SCEEN_HEIGHT);

    ldButtonInit(ID_BUTTON,ID_BG,10,10,30,30);
}

void uiDemoLoop(void)
{
}

void uiDemoQuit(void)
{
}