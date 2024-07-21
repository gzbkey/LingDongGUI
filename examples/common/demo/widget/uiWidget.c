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

    uiWidgetLogicInit(ptScene);
}

void uiWidgetLoop(ld_scene_t* ptScene)
{




    uiWidgetLogicLoop(ptScene);
}

void uiWidgetQuit(ld_scene_t* ptScene)
{




    uiWidgetLogicQuit(ptScene);
}
