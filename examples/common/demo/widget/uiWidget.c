#include "uiWidget.h"
#if (USE_VIRTUAL_RESOURCE == 1) && ( __x86_64__ || __i386__ || __APPLE__ )
#include "virtualNor.h"
static bool isWaitNorInit = true;
#endif

#include "freeRtosHeap4.h"

// 800*480

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

#define ID_ARC   21

extern size_t xFreeBytesRemaining;

void uiWidgetInit(uint8_t page)
{
#if (USE_VIRTUAL_RESOURCE == 1) && ( __x86_64__ || __i386__ || __APPLE__ )
    if(isWaitNorInit)
    {
        norSetBin("../common/demo/widget/srcWidget.bin");
        isWaitNorInit=false;
    }
#endif
    void *obj;

        obj=ldCalloc(1);
        ldFree(obj);
        LOG_DEBUG("start :%zu\n",xFreeBytesRemaining);

    obj=ldWindowInit(ID_BG, ID_BG, 0,0,LD_CFG_SCEEN_WIDTH,LD_CFG_SCEEN_HEIGHT);

//        ldWindowSetBgColor(obj,__RGB(220,220,220));

        LOG_DEBUG("win :%zu\n",xFreeBytesRemaining);

        obj=ldWindowInit(ID_WIN,ID_BG,10,10,LD_CFG_SCEEN_WIDTH,LD_CFG_SCEEN_WIDTH);

        obj=ldButtonInit(ID_BUTTON,ID_WIN,5,5,79,53);
        ldButtonSetFont(obj,SIMSUN_REGULAR_12);
        ldButtonSetText(obj,(uint8_t*)"123");
        ldButtonSetImage(obj,KEYRELEASE_PNG,true,KEYPRESS_PNG,true);
        ldButtonSetTextColor(obj,LD_COLOR_WHITE);
//        ldButtonSetRoundCorner(obj,true);
//        ldButtonSetSelectImage(obj,FRAME_PNG,LD_COLOR_BLUE);
//        ldButtonSetSelect(obj,true);

        obj=ldImageInit(ID_IMAGE,ID_WIN,10,70,100,100,NATURE_BMP,false);

        obj=ldTextInit(ID_TEXT,ID_WIN,10,200,150,200,SIMSUN_REGULAR_12);
        ldTextSetBgImage(obj,LETTER_PAPER_BMP);
        ldTextSetText(obj,(uint8_t*)"12345679\n123");
        ldTextSetTextColor(obj,LD_COLOR_BLACK);
        ldTextSetAlign(obj,LD_ALIGN_LEFT|LD_ALIGN_TOP);

        obj=ldProgressBarInit(ID_PROGRESS_BAR,ID_WIN,10,420,100,30);
        ldProgressBarSetPercent(obj,65);
        ldProgressBarSetBgImage(obj,PROGRESSBARBG_BMP,1,false);
        ldProgressBarSetFgImage(obj,PROGRESSBARFG_BMP,20,true);

        obj=ldRadialMenuInit(ID_RADIAL_MENU,ID_WIN,400,10,200,150,170,100,5);
        ldRadialMenuAddItem(obj,NOTE_PNG,48,48,0,false);
        ldRadialMenuAddItem(obj,BOOK_PNG,48,48,0,false);
        ldRadialMenuAddItem(obj,WEATHER_PNG,48,48,0,false);
        ldRadialMenuAddItem(obj,CHART_PNG,48,48,0,false);



        obj=ldDateTimeInit(ID_DATE_TIME,ID_WIN,150,10,150,30,SIMSUN_REGULAR_12);
        ldDateTimeSetBgColor(obj,LD_COLOR_LIGHT_CYAN);

        obj=ldIconSliderInit(ID_ICON_SLIDER,ID_WIN,150,50,100,65,52,2,5,1,1,SIMSUN_REGULAR_12);
//        ldIconSliderAddIcon(obj,BBG_BMP,(uint8_t*)"111");
//        ldIconSliderAddIcon(obj,ICON_BMP,(uint8_t*)"22");
//        ldIconSliderAddIcon(obj,BBG_BMP,(uint8_t*)"33");
//        ldIconSliderAddIcon(obj,ICON_BMP,(uint8_t*)"44");
//        ldIconSliderAddIcon(obj,BBG_BMP,(uint8_t*)"55");

//        obj=ldGaugeInit(ID_GAUGE,ID_WIN,150,120,121,121,TREE_BMP,false);
//        ldGaugeSetPointerImage(obj,POINTER_PNG,9,55,4,37);
//        ldGaugeSetPointerImageType(obj,withMask,0);

        obj=ldQRCodeInit(ID_QR_CODE,ID_WIN,150,250,60,60,(uint8_t*)"ldgui",LD_COLOR_LIGHT_BLUE,LD_COLOR_WHITE,eccHigh,2,2);

        obj=ldTableInit(ID_TABLE,ID_WIN,450,200,300,100,5,3,1,SIMSUN_REGULAR_12);
        ldTableSetExcelType(obj,SIMSUN_REGULAR_12);
        ldTableSetKeyboard(obj,ID_KB);

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

        obj=ldScrollSelecterInit(ID_SCROLL_SELECTER,ID_WIN,320,125,40,40,SIMSUN_REGULAR_12,5);
        ldScrollSelecterSetBgColor(obj,LD_COLOR_WHITE);
        ldScrollSelecterAddItem(obj,"1");
        ldScrollSelecterAddItem(obj,"2");
        ldScrollSelecterAddItem(obj,"3");
        ldScrollSelecterAddItem(obj,"4");
        ldScrollSelecterAddItem(obj,"5");

        // 120 280
        obj=ldLineEditInit(ID_LINE_EDIT,ID_WIN,320,280,100,40,SIMSUN_REGULAR_12,10);
        ldLineEditSetKeyboard(obj,ID_KB);


        obj=ldArcInit(ID_ARC,ID_WIN,450,350,101,101,ARC_QUARTER_PNG,ARC_QUARTER_MASK_PNG,__RGB(240,240,240));



        ldKeyboardInit(ID_KB,SIMSUN_REGULAR_12);
}

void uiWidgetLoop(uint8_t page)
{
}

void uiWidgetQuit(uint8_t page)
{
}
