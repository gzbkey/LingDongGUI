#include "uiWidget.h"
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
//   ldButton_t* btn=(ldButton_t *)msg.ptSender;

//   ldButtonSetText(btn,"222");
//   ldButtonMove(btn,150,100);
   ldImage_t *img=ldBaseGetWidget(ptScene->ptNodeRoot,1);
//   ldImageSetHidden(img,!img->use_as__ldBase_t.isHidden);
   ldBaseSetOpacity((ldBase_t *)img,128);
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

void listItemProcess(ldList_t *ptWidget,uint8_t itemNum,arm_2d_tile_t *ptTile,arm_2d_region_t *ptRegion,arm_2d_location_t clickPos,bool bIsNewFrame)
{
//    arm_2d_draw_box(ptTile,ptRegion,1,0,255);
    ptRegion->tSize.iHeight-=1;
    draw_round_corner_box(  ptTile,
                            ptRegion,
                            GLCD_COLOR_BLACK,
                            64,
                            bIsNewFrame);

    arm_lcd_text_set_target_framebuffer(ptTile);
    arm_lcd_text_set_font(&ARM_2D_FONT_6x8.use_as__arm_2d_font_t);
    arm_lcd_text_set_draw_region(ptRegion);
    arm_lcd_text_set_colour(GLCD_COLOR_BLACK, GLCD_COLOR_WHITE);
    arm_lcd_text_location(0,0);
    arm_lcd_printf_label(ARM_2D_ALIGN_CENTRE,"%d",itemNum);

    if((clickPos.iX!=-1)&&(clickPos.iY!=-1))
    {
        LOG_DEBUG("itemNum %d",ptWidget->selectItem);
        LOG_LOCATION("",clickPos);
    }
}
const uint8_t titleStr[]="title";
const uint8_t msgStr[]="12345678abcdefg\n99556";
const uint8_t *pBtnStr[]={"11","22","33"};

static const char *dayNames[7]={"Sun","Mon","Tue","Wed","Thu","Fir","Sat"};
static const char headerFormat[]="yyyy - mm - dd";

void uiWidgetInit(ld_scene_t* ptScene)
{
    void *obj,*list;

    ldBaseFocusNavigateInit();

    ldWindowInit(0, 0, 0, 0, LD_CFG_SCREEN_WIDTH, LD_CFG_SCREEN_HEIGHT);

    obj= ldImageInit(1, 0, 100, 120, 50, 80, NULL, NULL);
    ldImageSetImage(obj,IMAGE_LETTER_PAPER_BMP,NULL);
    ldBaseSetCorner(obj,true);
    ldBaseSetSelectable(obj,true);

    obj=ldButtonInit(2, 0, 10,10,79,53);
    ldButtonSetFont(obj,FONT_ARIAL_16_A8);
    ldButtonSetText(obj,(uint8_t*)"123");
    ldButtonSetTextColor(obj,GLCD_COLOR_WHITE);
    ldButtonSetImage(obj,IMAGE_KEYRELEASE_PNG,IMAGE_KEYRELEASE_PNG_Mask,IMAGE_KEYPRESS_PNG,IMAGE_KEYPRESS_PNG_Mask);
    ldBaseSetSelectable(obj,true);

    obj=ldWindowInit(3, 0, 200, 95, 20, 20);
    ldWindowSetColor(obj,GLCD_COLOR_GREEN);
    ldBaseSetCorner(obj,true);
    ldBaseSetSelectable(obj,true);


    connect(2,SIGNAL_RELEASE,slotTest);
//    connect(2,SIGNAL_RELEASE,slotTest2);

    obj=ldLabelInit(4,0,100,50, 100,50,FONT_ARIAL_12);
    ldLabelSetText(obj,(uint8_t*)"123");
    ldLabelSetBackgroundColor(obj,GLCD_COLOR_LIGHT_GREY);
    ldLabelSetAlign(obj,ARM_2D_ALIGN_BOTTOM_LEFT);
    ldBaseSetSelectable(obj,true);
    ldBaseSetCorner(obj,true);

    obj=ldCheckBoxInit(5, 0, 220, 10, 50, 20);

    ldCheckBoxSetRadioButtonGroup(obj,0);
    ldCheckBoxSetText(obj,FONT_ARIAL_12,(uint8_t*)"999");
    ldBaseSetSelectable((ldBase_t*)obj,true);


    obj=ldCheckBoxInit(6, 0, 220, 40, 50, 20);
    ldCheckBoxSetRadioButtonGroup(obj,0);
    ldBaseSetSelectable((ldBase_t*)obj,true);

    obj=ldCheckBoxInit(7, 0, 220, 70, 50, 20);
    ldBaseSetCorner(obj,true);
    ldBaseSetSelectable((ldBase_t*)obj,true);

    obj=ldProgressBarInit(8,0,10,500,300,30);
    ldProgressBarSetPercent(obj,45);
//    ldProgressBarSetHorizontal(obj,false);
    ldProgressBarSetImage(obj,IMAGE_PROGRESSBARBG_BMP,NULL,IMAGE_PROGRESSBARFG_BMP,NULL);
    ldBaseSetSelectable((ldBase_t*)obj,true);

    obj=ldTextInit(9,0,300,10,150,200,FONT_ARIAL_12,TEXT_BOX_LINE_ALIGN_LEFT,true);
    ldTextSetBackgroundImage(obj,IMAGE_LETTER_PAPER_BMP,NULL);
    ldTextSetText(obj,"123\n12333");
    ldBaseSetSelectable((ldBase_t*)obj,true);
    ldBaseSetCorner((ldBase_t*)obj,true);

    obj=ldSliderInit(10,0,50,300,317,(IMAGE_INDICATOR_PNG)->tRegion.tSize.iHeight);
    ldSliderSetPercent(obj,42);
    ldSliderSetImage(obj,IMAGE_SLIDER_PNG,IMAGE_SLIDER_PNG_Mask,IMAGE_INDICATOR_PNG,IMAGE_INDICATOR_PNG_Mask);
    ldSliderSetIndicatorWidth(obj,(IMAGE_INDICATOR_PNG)->tRegion.tSize.iWidth);


    obj=ldSliderInit(11,0,400,300,30,100);
    ldSliderSetHorizontal(obj,false);
    ldSliderSetPercent(obj,42);
    ldBaseSetSelectable((ldBase_t*)obj,true);
    ldBaseSetCorner((ldBase_t*)obj,true);

    obj=ldRadialMenuInit(12,0,500,200,150,100,100,80,5);
    ldRadialMenuAddItem(obj,IMAGE_WEATHER_PNG,IMAGE_WEATHER_PNG_Mask);
    ldRadialMenuAddItem(obj,IMAGE_NOTE_PNG,IMAGE_NOTE_PNG_Mask);
    ldRadialMenuAddItem(obj,IMAGE_WEATHER_PNG,IMAGE_WEATHER_PNG_Mask);
    ldRadialMenuAddItem(obj,IMAGE_NOTE_PNG,IMAGE_NOTE_PNG_Mask);
    ldBaseSetSelectable(obj,true);
    ldBaseSetCorner(obj,true);

    obj=ldDateTimeInit(13,0,600,100,200,50,FONT_ARIAL_12);
    ldBaseSetSelectable(obj,true);


    obj=ldIconSliderInit(14,0,500,350,150,65,48,2,5,1,1,FONT_ARIAL_12);
    ldIconSliderAddIcon(obj,IMAGE_NOTE_PNG,IMAGE_NOTE_PNG_Mask,iconName[0]);
    ldIconSliderAddIcon(obj,IMAGE_BOOK_PNG,IMAGE_BOOK_PNG_Mask,iconName[1]);
    ldIconSliderAddIcon(obj,IMAGE_WEATHER_PNG,IMAGE_WEATHER_PNG_Mask,iconName[2]);
    ldIconSliderAddIcon(obj,IMAGE_CHART_PNG,IMAGE_CHART_PNG_Mask,iconName[3]);
    ldIconSliderAddIcon(obj,IMAGE_NOTE_PNG,IMAGE_NOTE_PNG_Mask,iconName[4]);
    ldBaseSetSelectable(obj,true);
    ldBaseSetCorner(obj,true);

    obj=ldQRCodeInit(15,0,500,10,200,200,"ldgui",GLCD_COLOR_BLUE,GLCD_COLOR_WHITE,QR_ECC_7,2,5);
    ldBaseSetOpacity((ldBase_t*)obj,100);
    ldBaseSetSelectable((ldBase_t*)obj,true);

    obj=ldScrollSelecterInit(16,0,700,200,30,50,FONT_ARIAL_12);
    ldScrollSelecterSetItems(obj,pStrGroup,5);
    ldScrollSelecterSetBackgroundColor(obj,GLCD_COLOR_WHITE);
    ldBaseSetSelectable(obj,true);
    ldBaseSetCorner(obj,true);

    obj=ldGaugeInit(17,0,700,300,120,98,IMAGE_GAUGE_PNG,IMAGE_GAUGE_PNG_Mask,0,10);
    ldGaugeSetPointerImage(obj,NULL,IMAGE_GAUGEPOINTER_PNG_Mask,5,45);
    ldGaugeSetPointerColor(obj,GLCD_COLOR_BLUE);
    ldGaugeSetAngle(obj,120);
    ldBaseSetSelectable((ldBase_t*)obj,true);

    obj=ldComboBoxInit(18,0,700,420,100,30,FONT_ARIAL_12);

    ldComboBoxSetStaticItems(obj,pComboBoxStrGroup,3);
    ldBaseSetSelectable((ldBase_t*)obj,true);

    obj=ldGraphInit(19,0,830,10,100,100,2);
    ldGraphSetAxis(obj,80,80,5);
    ldGraphSetGridOffset(obj,4);
    ldGraphAddSeries(obj,GLCD_COLOR_RED,2,16);
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
    ldBaseSetSelectable((ldBase_t*)obj,true);

    obj=ldTableInit(20,0,780,150,200,100,10,6,1);
    ldTableSetExcelType(obj,FONT_ARIAL_12);
    ldTableSetKeyboard(obj,ID_KB);

    ldTableSetItemText(obj,1,1,(uint8_t*)"id");
    ldTableSetItemFont(obj,1,1,FONT_ARIAL_12);
    ldTableSetItemText(obj,1,2,(uint8_t*)"name");
    ldTableSetItemFont(obj,1,2,FONT_ARIAL_12);
    ldTableSetItemText(obj,1,3,(uint8_t*)"size");
    ldTableSetItemFont(obj,1,3,FONT_ARIAL_12);

    ldTableSetItemText(obj,2,1,(uint8_t*)"1");
    ldTableSetItemFont(obj,2,1,FONT_ARIAL_12);
    ldTableSetItemText(obj,2,2,(uint8_t*)"button");
    ldTableSetItemFont(obj,2,2,FONT_ARIAL_12);
    ldTableSetItemText(obj,2,3,(uint8_t*)"30*20");
    ldTableSetItemFont(obj,2,3,FONT_ARIAL_12);

    ldTableSetItemText(obj,3,1,(uint8_t*)"2");
    ldTableSetItemFont(obj,3,1,FONT_ARIAL_12);
    ldTableSetItemText(obj,3,2,(uint8_t*)"image");
    ldTableSetItemFont(obj,3,2,FONT_ARIAL_12);
    ldTableSetItemText(obj,3,3,(uint8_t*)"100*100");
    ldTableSetItemFont(obj,3,3,FONT_ARIAL_12);
    ldBaseSetSelectable(obj,true);

    obj=ldLineEditInit(21,0,850,400,100,50,FONT_ARIAL_12,16);
    ldLineEditSetText(obj,"123");
    ldLineEditSetKeyboard(obj,ID_KB);
    ldBaseSetSelectable(obj,true);
    ldBaseSetCorner(obj,true);


    obj=ldWindowInit(23, 0, 850, 450, 100, 100);
    ldWindowSetColor(obj,GLCD_COLOR_GREEN);

    obj=ldButtonInit(24, 23, 8,3,30,30);
    ldBase_t* pr=ldBaseGetParent(obj);

    ldButtonSetFont(obj,FONT_ARIAL_16_A8);
    ldButtonSetText(obj,(uint8_t*)"123");


    ldKeyboardInit(ID_KB,0,FONT_ARIAL_12);

    obj=ldArcInit(25,0,450,450,103,103,IMAGE_ARC_QUARTER_PNG_Mask,IMAGE_ARC_QUARTER_MASK_PNG_Mask,__RGB(240,240,240));
    ldArcSetBackgroundAngle(obj,0,350);
    ldArcSetForegroundAngle(obj,30);
    ldArcSetColor(obj,__RGB(173, 216, 230),__RGB(144, 238, 144));

    list=ldListInit(26,0,850,280,100,100);
//    ldListSetItemFunc(list,listItemProcess);
    ldListSetItemHeight(list,30);
    ldListSetText(list,pStrGroup,5, FONT_ARIAL_12);
    ldListSetAlign(list,ARM_2D_ALIGN_LEFT);
//    ldListSetItemContainer(list,5);
    ldBaseSetSelectable(list,true);

    obj=ldButtonInit(27, 26, 10,3,20,20);
     ldListSetItemWidget(list,1,obj);

     obj=ldMessageBoxInit(28,0,200,150,FONT_ARIAL_12);

     ldMessageBoxSetTitle(obj,titleStr);
     ldMessageBoxSetMsg(obj,msgStr);
     ldMessageBoxSetBtn(obj,pBtnStr,3);

     obj=ldCalendarInit(29,0,50,340,300,150,FONT_ARIAL_12,2026,1,1);
     ldCalendarSetDayNames(obj,(uint8_t**)dayNames);
     ldBaseSetCorner(obj,true);
     ldBaseSetSelectable(obj,true);
     ldBaseSetSelect(obj,true);
     ldCalendarSetHeader(obj,true);
     ldCalendarSetDateFormat(obj, YYYY_MM);

//    ldBaseNodeTreePrint(ptScene->ptNodeRoot,0);

}

void uiWidgetLoop(ld_scene_t* ptScene)
{
    static float angle=120;

    ldGauge_t *ptGauge=ldBaseGetWidgetById(17);
    if(ldTimeOut(100,true))
    {
        ldArcSetRotationAngle(ldBaseGetWidgetById(25),angle);

        ldGaugeSetAngle(ptGauge,angle);
        angle+=1;
        if(angle>=360)
        {
            angle=0;
        }
    }

    // if(xBtnGetState(KEY_NUM_UP,BTN_RELEASE))
    // {
    //     ldBaseFocusNavigate(ptScene,NAV_UP);
    // }
    // if(xBtnGetState(KEY_NUM_DOWN,BTN_RELEASE))
    // {
    //     ldBaseFocusNavigate(ptScene,NAV_DOWN);
    // }
    // if(xBtnGetState(KEY_NUM_LEFT,BTN_RELEASE))
    // {
    //     ldBaseFocusNavigate(ptScene,NAV_LEFT);
    // }
    // if(xBtnGetState(KEY_NUM_RIGHT,BTN_RELEASE))
    // {
    //     ldBaseFocusNavigate(ptScene,NAV_RIGHT);
    // }


}

void uiWidgetQuit(ld_scene_t* ptScene)
{




}
