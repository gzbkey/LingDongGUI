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

const uint8_t *pComboBoxStrGroup[3]={"11","22","00"};
#define ID_KB  22
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

    obj=ldQRCodeInit(15,0,500,10,200,200,"ldgui",GLCD_COLOR_BLUE,GLCD_COLOR_WHITE,QR_ECC_7,2,5);
    ldQRCodeSetOpacity(obj,100);


    obj=ldScrollSelecterInit(16,0,700,200,30,50,FONT_ARIAL_12);
    ldScrollSelecterSetItems(obj,pStrGroup,5);
    ldScrollSelecterSetBgColor(obj,GLCD_COLOR_WHITE);

    obj=ldGaugeInit(17,0,700,300,120,98,IMAGE_GAUGE_PNG,IMAGE_GAUGE_PNG_Mask,0,10);
    ldGaugeSetPointerImage(obj,NULL,IMAGE_GAUGEPOINTER_PNG_Mask,5,45);
    ldGaugeSetPointerColor(obj,GLCD_COLOR_BLUE);
    ldGaugeSetAngle(obj,120);

    obj=ldComboBoxInit(18,0,700,420,100,30,FONT_ARIAL_12);

    ldComboBoxSetItems(obj,pComboBoxStrGroup,3);

    obj=ldGraphInit(19,0,830,10,100,100,2);
    ldGraphSetAxis(obj,80,80,5);
    ldGraphSetGridOffset(obj,4);
    ldGraphAddSeries(obj,GLCD_COLOR_NIXIE_TUBE,2,16);
    srand(10);
    for (int i = 0; i < 16; i++)
    {
        ldGraphSetValue(obj,0,i,rand() % 81);
    }
    ldGraphAddSeries(obj,GLCD_COLOR_LIGHT_GREY,2,16);
    for (int i = 0; i < 16; i++)
    {
        ldGraphSetValue(obj,1,i,rand() % 81);
    }

    obj=ldTableInit(20,0,780,150,200,100,10,6,1,FONT_ARIAL_12);
    ldTableSetExcelType(obj,FONT_ARIAL_12);
    ldTableSetKeyboard(obj,ID_KB);

    ldTableSetItemText(obj,1,1,(uint8_t*)"id",FONT_ARIAL_12);
    ldTableSetItemText(obj,1,2,(uint8_t*)"name",FONT_ARIAL_12);
    ldTableSetItemText(obj,1,3,(uint8_t*)"size",FONT_ARIAL_12);

    ldTableSetItemText(obj,2,1,(uint8_t*)"1",FONT_ARIAL_12);
    ldTableSetItemText(obj,2,2,(uint8_t*)"button",FONT_ARIAL_12);
    ldTableSetItemText(obj,2,3,(uint8_t*)"30*20",FONT_ARIAL_12);

    ldTableSetItemText(obj,3,1,(uint8_t*)"2",FONT_ARIAL_12);
    ldTableSetItemText(obj,3,2,(uint8_t*)"image",FONT_ARIAL_12);
    ldTableSetItemText(obj,3,3,(uint8_t*)"100*100",FONT_ARIAL_12);

    obj=ldLineEditInit(21,0,850,400,100,50,FONT_ARIAL_12,16);
    ldLineEditSetText(obj,"123");
    ldLineEditSetKeyboard(obj,ID_KB);



    win=ldWindowInit(23, 0, 850, 450, 100, 100);
    ldWindowSetBgColor(win,GLCD_COLOR_GREEN);

    obj=ldButtonInit(24, 23, 8,3,30,30);
    ldBase_t* pr=ldBaseGetParent(obj);

    ldButtonSetFont(obj,FONT_ARIAL_16_A8);
    ldButtonSetText(obj,(uint8_t*)"123");

//    ldBaseNodeTreePrint(ptScene->ptNodeRoot,0);

    ldKeyboardInit(ID_KB,0,FONT_ARIAL_12);
//ldBaseBgMove(ptScene,LD_CFG_SCEEN_WIDTH,LD_CFG_SCEEN_HEIGHT,0,-200);

    obj=ldArcInit(25,0,450,450,101,101,IMAGE_ARC_QUARTER_PNG_Mask,IMAGE_ARC_QUARTER_MASK_PNG_Mask,__RGB(240,240,240));
    ldArcSetBgAngle(obj,0,350);
    ldArcSetFgAngle(obj,30);
    ldArcSetColor(obj,__RGB(173, 216, 230),__RGB(144, 238, 144));

    uiWidgetLogicInit(ptScene);
}

void uiWidgetLoop(ld_scene_t* ptScene)
{
    static int64_t timer=0;
    static float angle=120;

    ldGauge_t *ptGauge=ldBaseGetWidgetById(17);
    if(ldTimeOut(100,&timer,true))
    {
//        ldArcSetRotationAngle(ldBaseGetWidgetById(25),angle);

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
