#include "uiWatch.h"

#if (USE_VIRTUAL_RESOURCE == 1) && ( __x86_64__ || __i386__ || __APPLE__ )
#include "virtualNor.h"
static bool isWaitNorInit = true;
#endif

#define ID_BG   0
#define ID_IMAGE 1
#define ID_DATE_TIME 2
#define ID_IMAGE_POS0   3
#define ID_IMAGE_POS1   4
#define ID_IMAGE_POS2   5
#define ID_TABLE        6
#define ID_ICON_SLIDER   7
#define ID_WIN_TOP       8
#define ID_BTN_WIFI   9

#define SCEEN_WIDTH          240
#define SCEEN_HEIGHT         240

uint8_t gPage=0;
uint8_t moveDir=0;
bool isMoveUp=0;
bool isWinTopFullShow=false;

static bool slotBgReset(xConnectInfo_t info)
{
    int16_t x,y;



    if((moveDir=='-')&&(!isWinTopFullShow))
    {
    x=(int16_t)GET_SIGNAL_SPEED_X(info.value);
    y=(int16_t)GET_SIGNAL_SPEED_Y(info.value);

    if(x>0)
    {
        if(gPage>0)
        {
            gPage--;
        }

    }
    else
    {
        if(gPage<2)
        {
            gPage++;
        }
    }

    switch (gPage)
    {
    case 0:
    {
        ldBaseBgMove(SCEEN_WIDTH*3,SCEEN_HEIGHT,0,0);
        break;
    }
    case 1:
    {
        ldBaseBgMove(SCEEN_WIDTH*3,SCEEN_HEIGHT,-SCEEN_WIDTH,0);
        break;
    }
    case 2:
    {
        ldBaseBgMove(SCEEN_WIDTH*3,SCEEN_HEIGHT,-(SCEEN_WIDTH<<1),0);
        break;
    }

    default:
        break;
    }
    }
    else
    {
        if(gPage==1)
        {
            if(isMoveUp)
            {
                ldWindowMove(ldBaseGetWidgetById(ID_WIN_TOP),SCEEN_WIDTH,-SCEEN_HEIGHT);
                isWinTopFullShow=false;
            }
            else
            {
                ldWindowMove(ldBaseGetWidgetById(ID_WIN_TOP),SCEEN_WIDTH,0);
                isWinTopFullShow=true;
            }

        }
    }
    moveDir=0;
    return true;
}

static bool slotBgMove(xConnectInfo_t info)
{
    int16_t offsetX,offsetY;

    offsetX=(int16_t)GET_SIGNAL_OFFSET_X(info.value);
    offsetY=(int16_t)GET_SIGNAL_OFFSET_Y(info.value);

    // cal dir
    switch (moveDir)
    {
    case 0:
    {
        if((abs(offsetX)>10)&&(abs(offsetX)>abs(offsetY)))
        {
            moveDir='-';
        }

        if((abs(offsetY)>10)&&(abs(offsetX)<abs(offsetY)))
        {
            moveDir='|';
        }
        break;
    }
    case '-':
    {
        if((abs(offsetX)>10)&&(abs(offsetX)>abs(offsetY))&&(!isWinTopFullShow))
        {
            switch (gPage)
            {
            case 0:
            {
                if(offsetX<0)
                {
                    ldBaseBgMove(SCEEN_WIDTH*3,SCEEN_HEIGHT,offsetX,0);
                }
                break;
            }
            case 1:
            {
                ldBaseBgMove(SCEEN_WIDTH*3,SCEEN_HEIGHT,-SCEEN_WIDTH+offsetX,0);
                break;
            }
            case 2:
            {
                if(offsetX>0)
                {
                    ldBaseBgMove(SCEEN_WIDTH*3,SCEEN_HEIGHT,-(SCEEN_WIDTH<<1)+offsetX,0);
                }
                break;
            }
            default:
                break;
            }
        }
        break;
    }
    case '|':
    {
        if((abs(offsetY)>10)&&(abs(offsetX)<abs(offsetY))&&(gPage==1))
        {
            if(isWinTopFullShow)
            {
                ldWindowMove(ldBaseGetWidgetById(ID_WIN_TOP),SCEEN_WIDTH,offsetY);
            }
            else
            {
                ldWindowMove(ldBaseGetWidgetById(ID_WIN_TOP),SCEEN_WIDTH,offsetY-SCEEN_HEIGHT);
            }

            if(offsetY>0)//下拉
            {

                isMoveUp=false;
            }
            else
            {
                isMoveUp=true;
            }
        }
        break;
    }
    default:
        break;
    }
    return true;
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

void uiWatchInit(arm_2d_scene_t *pScene,uint8_t page)
{
#if (USE_VIRTUAL_RESOURCE == 1) && ( __x86_64__ || __i386__ || __APPLE__ )
    if(isWaitNorInit)
    {
        norSetBin("../common/demo/watch/srcWatch.bin");
        isWaitNorInit=false;
    }
#endif
    void *obj;

    switch (page) {
    case 0:

        // common
        obj=ldWindowInit(pScene,ID_BG,ID_BG,0,0,SCEEN_WIDTH*3,SCEEN_HEIGHT);
        xConnect(ID_BG,SIGNAL_HOLD_DOWN,ID_BG,slotBgMove);
        xConnect(ID_BG,SIGNAL_RELEASE,ID_BG,slotBgReset);


        // phonebook page
        obj=ldImageInit(pScene,ID_IMAGE_POS0,ID_BG,70,10,100,30,POS1_PNG,true);
        ldImageSetGrayscale(obj,8,LD_COLOR_LIGHT_PINK);

        obj=ldTableInit(pScene,ID_TABLE,ID_BG,20,50,200,180,10,1,2,ARIAL_REGULAR_28);

        for(int i=0;i<10;i++)
        {
            ldTableSetItemHeight(obj,i,40);
            ldTableSetItemImage(obj,i,0,5,5,30,30,CONTACTS_PNG,false,LD_COLOR_GREEN,true);
            ldTableSetItemStaticText(obj,i,0,contactsName[i],ARIAL_REGULAR_28);
//            ldTableSetItemText(obj,i,0,contactsName[i],ARIAL_REGULAR_28);
            ldTableSetItemAlign(obj,i,0,LD_ALIGN_LEFT);
        }

        xConnect(ID_TABLE,SIGNAL_HOLD_DOWN,ID_BG,slotBgMove);
        xConnect(ID_TABLE,SIGNAL_RELEASE,ID_BG,slotBgReset);




        // main page

        ldImageInit(pScene,ID_IMAGE,ID_BG,SCEEN_WIDTH,0,SCEEN_WIDTH,SCEEN_HEIGHT,WALLPAPER_JPG,false);

        obj=ldImageInit(pScene,ID_IMAGE_POS1,ID_BG,SCEEN_WIDTH+70,10,100,30,POS2_PNG,true);
        ldImageSetGrayscale(obj,8,LD_COLOR_LIGHT_PINK);

        obj=ldDateTimeInit(pScene,ID_DATE_TIME,ID_BG,SCEEN_WIDTH+20,180,120,60,ARIAL_REGULAR_28);
        ldDateTimeSetFormat(obj,"hh:nn:ss");
        ldDateTimeSetTextColor(obj,LD_COLOR_PURPLE);

        xConnect(ID_IMAGE,SIGNAL_HOLD_DOWN,ID_BG,slotBgMove);
        xConnect(ID_IMAGE,SIGNAL_RELEASE,ID_BG,slotBgReset);

        xConnect(ID_DATE_TIME,SIGNAL_HOLD_DOWN,ID_BG,slotBgMove);
        xConnect(ID_DATE_TIME,SIGNAL_RELEASE,ID_BG,slotBgReset);

        obj=ldWindowInit(pScene,ID_WIN_TOP,ID_BG,SCEEN_WIDTH,-SCEEN_HEIGHT,SCEEN_WIDTH,SCEEN_HEIGHT);
        ldWindowSetBgColor(obj,LD_COLOR_IVORY);

        xConnect(ID_WIN_TOP,SIGNAL_HOLD_DOWN,ID_BG,slotBgMove);
        xConnect(ID_WIN_TOP,SIGNAL_RELEASE,ID_BG,slotBgReset);

        obj=ldButtonInit(pScene,ID_BTN_WIFI,ID_WIN_TOP,20,10,48,48);
        ldButtonSetColor(obj,LD_COLOR_LIGHT_GREY,LD_COLOR_BLUE);
        ldButtonSetImage(obj,WIFI_ICON_PNG,onlyMask,WIFI_ICON_PNG,onlyMask);
        ldButtonSetCheckable(obj,true);

        // app page
        obj=ldImageInit(pScene,ID_IMAGE_POS2,ID_BG,(SCEEN_WIDTH*2)+70,10,100,30,POS3_PNG,true);
        ldImageSetGrayscale(obj,8,LD_COLOR_LIGHT_PINK);

        obj=ldIconSliderInit(pScene,ID_ICON_SLIDER,ID_BG,(SCEEN_WIDTH*2)+20,50,200,180,72,10,2,3,1,ARIAL_REGULAR_12);
        ldIconSliderAddIcon(obj,ICON_BMP,true,"app1");
        ldIconSliderAddIcon(obj,ICON_BMP,true,"app2");
        ldIconSliderAddIcon(obj,ICON_BMP,true,"app3");
        ldIconSliderAddIcon(obj,ICON_BMP,true,"app4");
        ldIconSliderAddIcon(obj,ICON_BMP,true,"app5");
        ldIconSliderSetHorizontalScroll(obj,false);


        xConnect(ID_ICON_SLIDER,SIGNAL_HOLD_DOWN,ID_BG,slotBgMove);
        xConnect(ID_ICON_SLIDER,SIGNAL_RELEASE,ID_BG,slotBgReset);

        //move to main page
        ldBaseBgMove(SCEEN_WIDTH*3,SCEEN_HEIGHT,-SCEEN_WIDTH,0);
        gPage=1;

        break;
    default:
        break;
    }
}

int64_t autoMoveTimer=0;
int64_t timer1s=0;
uint8_t hour=12,min=0,second=0;
void uiWatchLoop(arm_2d_scene_t *pScene,uint8_t page)
{

    ldDateTime_t *pClock;


    if(ldTimeOut(1000,&timer1s,true))
    {


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
        pClock=ldBaseGetWidgetById(ID_DATE_TIME);
        ldDateTimeSetTime(pClock,hour,min,second);
        }
    }
}

void uiWatchQuit(arm_2d_scene_t *pScene,uint8_t page)
{
}
