#include "uiWatch.h"
#include "image.h"

#define ID_BG   0
#define ID_IMAGE 1
#define ID_DATE_TIME 2
#define ID_IMAGE_POS0   3
#define ID_IMAGE_POS1   4
#define ID_IMAGE_POS2   5
#define ID_TABLE        6

#define SCEEN_WIDTH          240
#define SCEEN_HEIGHT         240

uint8_t gPage=1;

static bool slotBgMove(xConnectInfo_t info)
{
    switch (gPage) {
    case 0:
    {
        if(info.signalType==SIGNAL_SYS_SLIDER_LEFT)
        {
            ldBaseBgMove(-SCEEN_WIDTH,0);
            gPage=1;
        }
        break;
    }
    case (1):
    {
        if(info.signalType==SIGNAL_SYS_SLIDER_RIGHT)
        {
            ldBaseBgMove(0,0);
            gPage=0;
        }
        if(info.signalType==SIGNAL_SYS_SLIDER_LEFT)
        {
            ldBaseBgMove(-(SCEEN_WIDTH<<1),0);
            gPage=2;
        }
        break;
    }
    case (2):
    {
        if(info.signalType==SIGNAL_SYS_SLIDER_RIGHT)
        {
            ldBaseBgMove(-SCEEN_WIDTH,0);
            gPage=1;
        }
        break;
    }
    default:
        break;
    }

    return false;
}

uint8_t contactsName[10][8]={
    {"     23\0"},
    {"     67\0"},
    {"     00\0"},
    {"     11\0"},
    {"     22\0"},
    {"     33\0"},
    {"       \0"},
    {"       \0"},
    {"       \0"},
    {"       \0"},
};

void uiWatchInit(uint8_t page)
{
    void *obj;

    switch (page) {
    case 0:

        // common
        obj=ldWindowInit(ID_BG,ID_BG,-SCEEN_WIDTH,0,SCEEN_WIDTH*3,SCEEN_HEIGHT);

        xConnect(0,SIGNAL_SYS_SLIDER_LEFT,0,slotBgMove);
        xConnect(0,SIGNAL_SYS_SLIDER_RIGHT,0,slotBgMove);

        // phonebook page
        obj=ldImageInit(ID_IMAGE_POS0,ID_BG,70,10,100,30,POS1_PNG,true);
        ldImageSetGrayscale(obj,8,LD_COLOR_LIGHT_PINK);

        obj=ldTableInit(ID_TABLE,ID_BG,20,50,200,180,10,1,2,HARMONYOS_SANS_SC_MEDIUM_28);

        for(int i=0;i<10;i++)
        {
            ldTableSetItemHeight(obj,i,40);
            ldTableSetItemImage(obj,i,0,5,5,30,30,CONTACTS_PNG,false,LD_COLOR_GREEN,true);
//            ldTableSetItemStaticText(obj,i,0,contactsName[i],HARMONYOS_SANS_SC_MEDIUM_28);
            ldTableSetItemText(obj,i,0,contactsName[i],HARMONYOS_SANS_SC_MEDIUM_28);
            ldTableSetItemAlign(obj,i,0,LD_ALIGN_LEFT);
        }



        // main page

        ldImageInit(ID_IMAGE,ID_BG,SCEEN_WIDTH,0,SCEEN_WIDTH,SCEEN_HEIGHT,WALLPAPER_JPG,false);

        obj=ldImageInit(ID_IMAGE_POS1,ID_BG,SCEEN_WIDTH+70,10,100,30,POS2_PNG,true);
        ldImageSetGrayscale(obj,8,LD_COLOR_LIGHT_PINK);

        obj=ldDateTimeInit(ID_DATE_TIME,ID_BG,SCEEN_WIDTH+20,180,120,60,HARMONYOS_SANS_SC_MEDIUM_28);
        ldDateTimeSetFormat(obj,"hh:nn:ss");
        ldDateTimeSetTextColor(obj,LD_COLOR_PURPLE);

        // app page
        obj=ldImageInit(ID_IMAGE_POS2,ID_BG,(SCEEN_WIDTH*2)+70,10,100,30,POS3_PNG,true);
        ldImageSetGrayscale(obj,8,LD_COLOR_LIGHT_PINK);

        break;
    default:
        break;
    }
}
int64_t timer1s=0;
uint8_t hour=12,min=0,second=0;
void uiWatchLoop(uint8_t page)
{

    ldDateTime_t *pClock;


    if(ldTimeOut(1000,&timer1s,true))
    {

        pClock=ldBaseGetWidgetById(ID_DATE_TIME);
        second++;
        if(second>=60)
        {
            second=0;
            min++;
            if(min>=60)
            {
                min=0;
                hour++;
                if(hour>=13)
                {
                    hour=1;
                }
            }
        }
//        if(gPage==1)
        {
        ldDateTimeSetTime(pClock,hour,min,second);
        }
    }
}

void uiWatchQuit(uint8_t page)
{
}
