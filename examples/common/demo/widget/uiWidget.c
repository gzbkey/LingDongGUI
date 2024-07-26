#include "uiWidget.h"
#include "uiWidgetLogic.h"
#include "fonts/uiFonts.h"
#include "images/uiImages.h"
#include "ldGui.h"

void uiWidgetInit(ld_scene_t* ptScene);
void uiWidgetLoop(ld_scene_t* ptScene);
void uiWidgetQuit(ld_scene_t* ptScene);


const ldPageFuncGroup_t uiWidgetFunc={
    .init=uiWidgetInit,
    .loop=uiWidgetLoop,
    .quit=uiWidgetQuit,
#if (USE_LOG_LEVEL>=LOG_LEVEL_INFO)
    .pageName="uiWidget",
#endif
};

static bool slotTest(ld_scene_t *ptScene,ldMsg_t msg)
{
   LOG_DEBUG("test 1111");

//   ldButton_t* btn=(ldButton_t *)msg.ptSender;

//   ldButtonSetText(btn,"222");
//   ldButtonMove(btn,150,100);
   ldImage_t*img=ldBaseGetWidget(ptScene->ptNodeRoot,1);
//   ldImageSetHidden(img,!img->use_as__ldBase_t.isHidden);
   ldImageSetOpacity(img,128);
   return false;
}

static bool slotTest2(ld_scene_t *ptScene,ldMsg_t msg)
{
   LOG_DEBUG("test 22222");

   return false;
}
const uint8_t *pStrGroup[]={"1","10","123","99","7"};

const uint8_t *iconName[5]={"11","22","33","44","55"};

void uiWidgetInit(ld_scene_t* ptScene)
{
    void *obj,*win;

    ldWindowInit(0, 0, 0, 0, LD_CFG_SCEEN_WIDTH, LD_CFG_SCEEN_HEIGHT);

    obj= ldImageInit(1, 0, 100, 100, 50, 50, NULL, NULL,false);
    ldImageSetBgColor(obj,__RGB(0xFF,0xFF,0xFF));

    obj=ldButtonInit(2, 0, 10,10,79,53);
    ldButtonSetFont(obj,FONT_ARIAL_16_A8);
    ldButtonSetText(obj,(uint8_t*)"123");
    ldButtonSetTextColor(obj,GLCD_COLOR_WHITE);
    ldButtonSetImage(obj,IMAGE_KEYRELEASE_PNG,IMAGE_KEYRELEASE_PNG_Mask,IMAGE_KEYPRESS_PNG,IMAGE_KEYPRESS_PNG_Mask);

    win=ldWindowInit(3, 0, 200, 95, 20, 20);
    ldWindowSetBgColor(win,GLCD_COLOR_GREEN);

//    connect(2,SIGNAL_RELEASE,slotPageJump);

    connect(2,SIGNAL_RELEASE,slotTest);
//    connect(2,SIGNAL_RELEASE,slotTest2);

    obj=ldLabelInit(4,0,100,50, 100,50,FONT_ARIAL_12);
    ldLabelSetText(obj,(uint8_t*)"123");
    ldLabelSetBgColor(obj,GLCD_COLOR_LIGHT_GREY);
    ldLabelSetAlign(obj,ARM_2D_ALIGN_BOTTOM_LEFT);

    obj=ldCheckBoxInit(5, 0, 220, 10, 50, 20);

    ldCheckBoxSetRadioButtonGroup(obj,0);
    ldCheckBoxSetText(obj,FONT_ARIAL_12,(uint8_t*)"999");


    obj=ldCheckBoxInit(6, 0, 220, 40, 50, 20);
    ldCheckBoxSetRadioButtonGroup(obj,0);

    obj=ldCheckBoxInit(7, 0, 220, 70, 50, 20);
    ldCheckBoxSetCorner(obj,true);

    obj=ldProgressBarInit(8,0,10,500,300,30);
    ldProgressBarSetPercent(obj,45);
//    ldProgressBarSetHorizontal(obj,false);
    ldProgressBarSetImage(obj,IMAGE_PROGRESSBARBG_BMP,IMAGE_PROGRESSBARFG_BMP);

    obj=ldTextInit(9,0,300,10,150,200,FONT_ARIAL_12,true);
    ldTextSetBgImage(obj,IMAGE_LETTER_PAPER_BMP);
    ldTextSetText(obj,"123\n12333");

    obj=ldSliderInit(10,0,50,300,317,(IMAGE_INDICATOR_PNG)->tRegion.tSize.iHeight);
    ldSliderSetPercent(obj,42);
    ldSliderSetImage(obj,IMAGE_SLIDER_PNG,IMAGE_SLIDER_PNG_Mask,IMAGE_INDICATOR_PNG,IMAGE_INDICATOR_PNG_Mask);
    ldSliderSetIndicatorWidth(obj,(IMAGE_INDICATOR_PNG)->tRegion.tSize.iWidth);

    obj=ldSliderInit(11,0,400,300,30,100);
    ldSliderSetHorizontal(obj,false);
    ldSliderSetPercent(obj,42);

    obj=ldRadialMenuInit(12,0,500,200,150,100,100,80,5);
    ldRadialMenuAddItem(obj,IMAGE_WEATHER_PNG,IMAGE_WEATHER_PNG_Mask);
    ldRadialMenuAddItem(obj,IMAGE_NOTE_PNG,IMAGE_NOTE_PNG_Mask);
    ldRadialMenuAddItem(obj,IMAGE_WEATHER_PNG,IMAGE_WEATHER_PNG_Mask);
    ldRadialMenuAddItem(obj,IMAGE_NOTE_PNG,IMAGE_NOTE_PNG_Mask);

    obj=ldDateTimeInit(13,0,600,100,200,50,FONT_ARIAL_12);


    obj=ldIconSliderInit(14,0,500,350,150,65,48,2,5,1,1,FONT_ARIAL_12);
    ldIconSliderAddIcon(obj,IMAGE_NOTE_PNG,IMAGE_NOTE_PNG_Mask,iconName[0]);
    ldIconSliderAddIcon(obj,IMAGE_BOOK_PNG,IMAGE_BOOK_PNG_Mask,iconName[1]);
    ldIconSliderAddIcon(obj,IMAGE_WEATHER_PNG,IMAGE_WEATHER_PNG_Mask,iconName[2]);
    ldIconSliderAddIcon(obj,IMAGE_CHART_PNG,IMAGE_CHART_PNG_Mask,iconName[3]);
    ldIconSliderAddIcon(obj,IMAGE_NOTE_PNG,IMAGE_NOTE_PNG_Mask,iconName[4]);

    obj=ldQRCodeInit(15,0,600,10,200,200,"ldgui",GLCD_COLOR_BLUE,GLCD_COLOR_WHITE,QR_ECC_7,2,5);
    ldQRCodeSetOpacity(obj,100);


    obj=ldScrollSelecterInit(16,0,700,200,30,50,FONT_ARIAL_12);
    ldScrollSelecterSetItems(obj,pStrGroup,5);
    ldScrollSelecterSetBgColor(obj,GLCD_COLOR_WHITE);

    obj=ldGaugeInit(17,0,700,300,120,98,IMAGE_GAUGE_PNG,IMAGE_GAUGE_PNG_Mask,0,10);
    ldGaugeSetPointerImage(obj,NULL,IMAGE_GAUGEPOINTER_PNG_Mask,5,45);
    ldGaugeSetPointerColor(obj,GLCD_COLOR_BLUE);
    ldGaugeSetAngle(obj,120);

    uiWidgetLogicInit(ptScene);
}

void uiWidgetLoop(ld_scene_t* ptScene)
{
    static int64_t timer=0;
    static float angle=120;

    ldGauge_t *ptGauge=ldBaseGetWidgetById(17);
    if(ldTimeOut(100,&timer,true))
    {
        ldGaugeSetAngle(ptGauge,angle);
        angle+=1;
        if(angle>=360)
        {
            angle=0;
        }
    }



    uiWidgetLogicLoop(ptScene);
}

void uiWidgetQuit(ld_scene_t* ptScene)
{




    uiWidgetLogicQuit(ptScene);
}
