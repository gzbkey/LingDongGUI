#include "uiWidget.h"
#include "uiWidgetLogic.h"
#include "fonts/uiFonts.h"
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
   ldImageSetOpacity(img,200);
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

    ldWindow_init(ptScene,NULL,0, 0, 0, 0, 320, 240);

    obj= ldImage_init(ptScene,NULL,1, 0, 100, 100, 50, 50, NULL, NULL,false);
    ldImageSetBgColor(obj,__RGB(0xFF,0xFF,0xFF));

    obj=ldButton_init(ptScene,NULL,2, 0, 10,10,100,50);
    ldButtonSetFont(obj,&FONT_ARIAL_16_A8);
    ldButtonSetText(obj,"123");
    LOG_DEBUG("btn 2 :%p",obj);

    win=ldWindow_init(ptScene,NULL,3, 0, 200, 95, 20, 20);
    ldWindowSetBgColor(win,GLCD_COLOR_GREEN);

//    connect(2,SIGNAL_RELEASE,slotPageJump);

    connect(2,SIGNAL_RELEASE,slotTest);
//    connect(2,SIGNAL_RELEASE,slotTest2);

    obj=ldLabel_init(ptScene,NULL,4,0,100,50, 100,50,&FONT_ARIAL_12);
    ldLabelSetText(obj,"123");
    ldLabelSetBgColor(obj,GLCD_COLOR_LIGHT_GREY);
    ldLabelSetAlign(obj,ARM_2D_ALIGN_BOTTOM_LEFT);

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
