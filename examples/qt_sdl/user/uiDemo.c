#include "uiDemo.h"
#include "image.h"

#define ID_BG    0
#define ID_BUTTON   1
#define ID_IMAGE   2
#define ID_LINE_EDIT  3
#define ID_KB  4


void uiDemoInit(void)
{
    void* obj;

    obj=ldWindowInit(ID_BG, ID_BG, 0,0,LD_CFG_SCEEN_WIDTH,LD_CFG_SCEEN_HEIGHT);

    ldButtonInit(ID_BUTTON,ID_BG,10,10,30,30);

    ldLineEditInit(ID_LINE_EDIT,ID_BG,10,50,50,30,SIMSUN_REGULAR_12,10);

    ldKeyboardInit(ID_KB,SIMSUN_REGULAR_12);

    ldLineEditSetKeyboard(ldBaseGetWidgetById(ID_LINE_EDIT),ID_KB);
}

void uiDemoLoop(void)
{
}

void uiDemoQuit(void)
{
}
