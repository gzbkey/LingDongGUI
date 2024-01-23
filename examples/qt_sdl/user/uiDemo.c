#include "uiDemo.h"
#include "image.h"
#include "xString.h"

#define ID_BG       0

#define ID_ARC              1
#define ID_BUTTON           2
#define ID_CHECK_BOX        3
#define ID_COMBO_BOX        4
#define ID_DATE_TIME        5
#define ID_GAUGE            6
#define ID_GRAPH            7
#define ID_ICON_SLIDER      8
#define ID_IMAGE            9
#define ID_KB               10
#define ID_LABEL            11
#define ID_LINE_EDIT        12
#define ID_PROGRESS_BAR     13
#define ID_QR_CODE          14
#define ID_RADIAL_MENU      15
#define ID_TABLE            16
#define ID_TEXT             17
#define ID_WIN              18

#define ID_LABEL_2          19
#define ID_LABEL_TEMP_VALUE          20
#define ID_LABEL_INFO          21

#define ID_CHECK_BOX_2        22
#define ID_CHECK_BOX_3        23
#define ID_CHECK_BOX_4        24
#define ID_CHECK_BOX_5        25
#define ID_CHECK_BOX_6        26

#define ID_LINE_EDIT_2        27

int64_t timer=0;
int32_t tempValue;
uint8_t tempBuf[16]={0};

bool slotJumpProess(xConnectInfo_t info)
{
    if(info.signalType==SIGNAL_CLICKED_ITEM)
    {
        ldGuiJumpPage(info.value+1);
    }
    return false;
}

bool slotJumpPage0(xConnectInfo_t info)
{
    ldGuiJumpPage(0);
    return false;
}

bool slotChangeItem(xConnectInfo_t info)
{
    ldComboBox_t *pCb=ldBaseGetWidgetById(info.senderId);
    ldLabel_t *pl=ldBaseGetWidgetById(info.receiverId);

    if(info.signalType==SIGNAL_CLICKED_ITEM)
    {
        ldLabelSetText(pl,pCb->ppItemStrGroup[pCb->itemSelect]);
    }
    return false;
}
extern size_t xFreeBytesRemaining;
void uiDemoInit(uint8_t page)
{
    void* obj;

    timer=0;
    tempValue=0;
LOG_DEBUG("before ========================== %d\n",xFreeBytesRemaining);
    switch (page)
    {
    case 0:
    {
        obj=ldWindowInit(ID_BG, ID_BG, 0,0,LD_CFG_SCEEN_WIDTH,LD_CFG_SCEEN_HEIGHT);

        obj=ldLabelInit(ID_LABEL,ID_BG,0,0,LD_CFG_SCEEN_WIDTH,30,SIMSUN_REGULAR_12);
        ldLabelSetText(obj,(uint8_t*)"ldgui demo");
        ldLabelSetBgColor(obj,LD_COLOR_BEIGE);

        obj=ldTextInit(ID_TEXT,ID_BG,30,70,150,90,SIMSUN_REGULAR_12);
        ldTextSetAlign(obj,LD_ALIGN_LEFT|LD_ALIGN_TOP);
        ldTextSetScroll(obj,true);
        ldTextSetText(obj,(uint8_t*)"LDGUI is developed based on ARM-2D,greatly reducing the difficulty of using ARM-2D.\nSlide or click on the icon below.");

        obj=ldDateTimeInit(ID_DATE_TIME,ID_BG,0,40,LD_CFG_SCEEN_WIDTH,20,SIMSUN_REGULAR_12);
        ldDateTimeSetDate(obj,2024,1,15);
        ldDateTimeSetTextColor(obj,LD_COLOR_DARK_BLUE);

//        ldQRCodeInit(ID_QR_CODE,ID_BG,205,70,100,100,(uint8_t*)"https://gitee.com/gzbkey/LingDongGUI",LD_COLOR_LIGHT_BLUE,LD_COLOR_DARK_MAGENTA,eccLow,7,2);


        obj=ldLabelInit(ID_LABEL_INFO,ID_BG,200,160,100,20,SIMSUN_REGULAR_12);
        ldLabelSetAlign(obj,LD_ALIGN_LEFT);
        ldLabelSetText(obj,(uint8_t*)"Get more information");

        obj=ldIconSliderInit(ID_ICON_SLIDER,ID_BG,0,(LD_CFG_SCEEN_HEIGHT-60),LD_CFG_SCEEN_WIDTH,60,48,25,4,1,2,SIMSUN_REGULAR_12);
        ldIconSliderAddIcon(obj,BBG_BMP,(uint8_t*)"icon 1");
        ldIconSliderAddIcon(obj,BBG_BMP,(uint8_t*)"icon 2");
        ldIconSliderAddIcon(obj,BBG_BMP,(uint8_t*)"icon 3");
        ldIconSliderAddIcon(obj,BBG_BMP,(uint8_t*)"icon 4");
        ldIconSliderAddIcon(obj,BBG_BMP,(uint8_t*)"icon 5");
        ldIconSliderAddIcon(obj,BBG_BMP,(uint8_t*)"icon 6");
        ldIconSliderAddIcon(obj,BBG_BMP,(uint8_t*)"icon 7");
        ldIconSliderAddIcon(obj,BBG_BMP,(uint8_t*)"icon 8");

        xConnect(ID_ICON_SLIDER,SIGNAL_CLICKED_ITEM,ID_BG,slotJumpProess);
        break;
    }
    case 1:
    {
        obj=ldWindowInit(ID_BG, ID_BG, 0,0,LD_CFG_SCEEN_WIDTH,LD_CFG_SCEEN_HEIGHT);

        obj=ldButtonInit(ID_BUTTON,ID_BG,(LD_CFG_SCEEN_WIDTH-70),10,60,30);
        ldButtonSetRoundCorner(obj,true);
        ldButtonSetFont(obj,SIMSUN_REGULAR_12);
        ldButtonSetText(obj,(uint8_t*)"return");
        xConnect(ID_BUTTON,SIGNAL_RELEASE,ID_BG,slotJumpPage0);

        obj=ldCheckBoxInit(ID_CHECK_BOX,ID_BG,10,50,80,20);
        ldCheckBoxSetText(obj,SIMSUN_REGULAR_12,(uint8_t*)"check1");

        obj=ldCheckBoxInit(ID_CHECK_BOX_2,ID_BG,10,80,80,20);
        ldCheckBoxSetText(obj,SIMSUN_REGULAR_12,(uint8_t*)"check2");

        obj=ldCheckBoxInit(ID_CHECK_BOX_3,ID_BG,10,110,80,20);
        ldCheckBoxSetText(obj,SIMSUN_REGULAR_12,(uint8_t*)"check3");

        obj=ldCheckBoxInit(ID_CHECK_BOX_4,ID_BG,100,50,80,20);
        ldCheckBoxSetText(obj,SIMSUN_REGULAR_12,(uint8_t*)"radio1");
        ldCheckBoxSetCorner(obj,true);
        ldCheckBoxSetRadioButtonGroup(obj,0);

        obj=ldCheckBoxInit(ID_CHECK_BOX_5,ID_BG,100,80,80,20);
        ldCheckBoxSetText(obj,SIMSUN_REGULAR_12,(uint8_t*)"radio2");
        ldCheckBoxSetCorner(obj,true);
        ldCheckBoxSetRadioButtonGroup(obj,0);

        obj=ldCheckBoxInit(ID_CHECK_BOX_6,ID_BG,100,110,80,20);
        ldCheckBoxSetText(obj,SIMSUN_REGULAR_12,(uint8_t*)"radio3");
        ldCheckBoxSetCorner(obj,true);
        ldCheckBoxSetRadioButtonGroup(obj,0);

        ldComboBox_t* pCB=ldComboBoxInit(ID_COMBO_BOX,ID_BG,180,80,120,30,SIMSUN_REGULAR_12,4);
        ldComboBoxAddItem(pCB,(uint8_t*)"item 1");
        ldComboBoxAddItem(pCB,(uint8_t*)"item 2");
        ldComboBoxAddItem(pCB,(uint8_t*)"item 3");
        ldComboBoxAddItem(pCB,(uint8_t*)"item 4");

        obj=ldLabelInit(ID_LABEL,ID_BG,180,50,50,30,SIMSUN_REGULAR_12);
        ldLabelSetText(obj,pCB->ppItemStrGroup[pCB->itemSelect]);

        xConnect(ID_COMBO_BOX,SIGNAL_CLICKED_ITEM,ID_LABEL,slotChangeItem);
        break;
    }
    case 2:
    {
        obj=ldWindowInit(ID_BG, ID_BG, 0,0,LD_CFG_SCEEN_WIDTH,LD_CFG_SCEEN_HEIGHT);

        obj=ldButtonInit(ID_BUTTON,ID_BG,(LD_CFG_SCEEN_WIDTH-70),10,60,30);
        ldButtonSetRoundCorner(obj,true);
        ldButtonSetFont(obj,SIMSUN_REGULAR_12);
        ldButtonSetText(obj,(uint8_t*)"return");
        xConnect(ID_BUTTON,SIGNAL_RELEASE,ID_BG,slotJumpPage0);

        ldLineEditInit(ID_LINE_EDIT,ID_BG,10,50,100,30,SIMSUN_REGULAR_12,20);

        ldLineEditInit(ID_LINE_EDIT_2,ID_BG,10,150,100,30,SIMSUN_REGULAR_12,20);

        ldKeyboardInit(ID_KB,SIMSUN_REGULAR_12);

        ldLineEditSetKeyboard(ldBaseGetWidgetById(ID_LINE_EDIT),ID_KB);

        ldLineEditSetKeyboard(ldBaseGetWidgetById(ID_LINE_EDIT_2),ID_KB);

        break;
    }
    case 3:
    {
        obj=ldWindowInit(ID_BG, ID_BG, 0,0,LD_CFG_SCEEN_WIDTH,LD_CFG_SCEEN_HEIGHT);

        obj=ldButtonInit(ID_BUTTON,ID_BG,(LD_CFG_SCEEN_WIDTH-70),10,60,30);
        ldButtonSetRoundCorner(obj,true);
        ldButtonSetFont(obj,SIMSUN_REGULAR_12);
        ldButtonSetText(obj,(uint8_t*)"return");
        xConnect(ID_BUTTON,SIGNAL_RELEASE,ID_BG,slotJumpPage0);

        ldTable_t *pTable=ldTableInit(ID_TABLE,ID_BG,10,50,LD_CFG_SCEEN_WIDTH-20,LD_CFG_SCEEN_HEIGHT-60,10,10,1,SIMSUN_REGULAR_12);

        ldTableSetExcelType(pTable,SIMSUN_REGULAR_12);

        ldKeyboardInit(ID_KB,SIMSUN_REGULAR_12);

        ldTableSetKeyboard(pTable,ID_KB);

        ldTableSetItemButton(pTable,1,1,20,5,52,52,ICON_BMP,BBG_BMP,false);
        break;
    }
    case 4:
    {
        obj=ldWindowInit(ID_BG, ID_BG, 0,0,LD_CFG_SCEEN_WIDTH,LD_CFG_SCEEN_HEIGHT);

        obj=ldButtonInit(ID_BUTTON,ID_BG,(LD_CFG_SCEEN_WIDTH-70),10,60,30);
        ldButtonSetRoundCorner(obj,true);
        ldButtonSetFont(obj,SIMSUN_REGULAR_12);
        ldButtonSetText(obj,(uint8_t*)"return");
        xConnect(ID_BUTTON,SIGNAL_RELEASE,ID_BG,slotJumpPage0);

        obj=ldRadialMenuInit(ID_RADIAL_MENU,ID_BG,10,40,300,200,220,100,5);

        ldRadialMenuAddItem(obj,ICON_BMP,52,52,0,false);
        ldRadialMenuAddItem(obj,BBG_BMP,52,52,0,false);
        ldRadialMenuAddItem(obj,ICON_BMP,52,52,0,false);
        ldRadialMenuAddItem(obj,BBG_BMP,52,52,0,false);
        ldRadialMenuAddItem(obj,ICON_BMP,52,52,0,false);
//        ldRadialMenuAddItem(obj,BBG_BMP,52,52,0,false);


        break;
    }
    case 5:
    {
        obj=ldWindowInit(ID_BG, ID_BG, 0,0,LD_CFG_SCEEN_WIDTH,LD_CFG_SCEEN_HEIGHT);
        ldWindowSetBgColor(obj,LD_COLOR_LAVENDER_BLUSH);

        obj=ldButtonInit(ID_BUTTON,ID_BG,(LD_CFG_SCEEN_WIDTH-70),10,60,30);
        ldButtonSetRoundCorner(obj,true);
        ldButtonSetFont(obj,SIMSUN_REGULAR_12);
        ldButtonSetText(obj,(uint8_t*)"return");
        xConnect(ID_BUTTON,SIGNAL_RELEASE,ID_BG,slotJumpPage0);

        obj=ldGraphInit(ID_GRAPH,ID_BG,10,20,120,120,2);
        ldGraphSetAxis(obj,100,100,10);
        ldGraphAddSeries(obj,LD_COLOR_DARK_BLUE,1,10);
        ldGraphSetValue(obj,0,0,10);
        ldGraphSetValue(obj,0,1,50);
        ldGraphSetValue(obj,0,2,80);
        ldGraphSetValue(obj,0,3,60);
        ldGraphSetValue(obj,0,4,10);
        ldGraphSetValue(obj,0,5,70);
        ldGraphSetValue(obj,0,6,85);
        ldGraphSetValue(obj,0,7,72);
        ldGraphSetValue(obj,0,8,95);
        ldGraphSetValue(obj,0,9,50);

        ldGraphAddSeries(obj,LD_COLOR_CYAN,5,10);
        ldGraphSetValue(obj,1,0,30);
        ldGraphSetValue(obj,1,1,20);
        ldGraphSetValue(obj,1,2,50);
        ldGraphSetValue(obj,1,3,40);
        ldGraphSetValue(obj,1,4,60);
        ldGraphSetValue(obj,1,5,50);
        ldGraphSetValue(obj,1,6,55);
        ldGraphSetValue(obj,1,7,12);
        ldGraphSetValue(obj,1,8,45);
        ldGraphSetValue(obj,1,9,30);

        obj=ldArcInit(ID_ARC,ID_BG,160,40,100,100,ARC51_PNG,ARC51_PNG,LD_COLOR_BLUE);

        ldArcSetBgAngle(obj,30,330);
        ldArcSetFgAngle(obj,270);

        break;
    }
    default:
        break;
    }
LOG_DEBUG("after ========================== %d\n",xFreeBytesRemaining);
}

#define BTN_ID_POW 0
#define BTN_ID_KNOB 1

int8_t page=0;
uint8_t counter=0;
int64_t p5Timer=0;
float arcAngle=0;
void uiDemoLoop(uint8_t page)
{
//    if(ldTimeOut(5000,&timer,true))
////    if(xBtnGetState(BTN_ID_POW,BTN_PRESS))
//    {
//        if(page==4)
//        {
//            if(counter<4)
//            {
//                counter++;
//                ldRadialMenuSelectItem(ldBaseGetWidgetById(ID_RADIAL_MENU),counter+1);
//            }
//            else
//            {
//                counter=0;
//                page++;
//                ldGuiJumpPage(page);
//            }
//        }
//        else
//        {
//            page++;
//            if(page>=5)
//            {
//                page=0;
//            }
//        ldGuiJumpPage(page);
//        }




//    }

//    if(page==5)
//    {
//        if(ldTimeOut(10,&p5Timer,true))
//        {
//            ldArcSetRotationAngle(ldBaseGetWidgetById(ID_ARC),arcAngle++);
//        }
//    }
//    else
//    {
//        p5Timer=0;
//    }
//    if(xBtnGetState(BTN_ID_POW,BTN_PRESS))
//    {
//        page--;
//        if(page<0)
//        {
//            page=3;
//        }
//         ldGuiJumpPage(page);
//
//
//    }
}

void uiDemoQuit(uint8_t page)
{
}
