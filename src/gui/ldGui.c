/*
 * Copyright 2023-2024 Ou Jianbo (59935554@qq.com)
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/**
 * @file    ldGui.c
 * @author  Ou Jianbo(59935554@qq.com)
 * @brief   ldgui的主文件
 * @version 0.1
 * @date    2023-11-03
 */
#include "ldGui.h"
#include "ldImage.h"
#include "ldButton.h"
#include "ldText.h"
#include "ldWindow.h"
#include "ldProgressBar.h"
#include "ldRadialMenu.h"
#include "ldCheckBox.h"
#include "ldLabel.h"
#include "ldScrollSelecter.h"
#include "ldDateTime.h"
#include "ldIconSlider.h"
#include "ldGauge.h"
#include "ldQRCode.h"
#include "ldTable.h"
#include "ldKeyboard.h"
#include "ldLineEdit.h"
#include "ldGraph.h"
#include "ldComboBox.h"
#include "ldArc.h"
/*============================ auto add include ==============================*/

uint8_t pageNumNow=0;
uint8_t pageTarget=0;

int64_t sysTimer=0;

#define TOUCH_NO_CLICK           0
#define TOUCH_CLICK              1

static volatile ldPoint_t pressPoint;
static volatile int16_t deltaMoveTime;
static volatile int16_t prevX,prevY;
static void *prevWidget;

void (*ldPageInitFunc[LD_PAGE_MAX])(uint8_t)={0};
void (*ldPageLoopFunc[LD_PAGE_MAX])(uint8_t)={0};
void (*ldPageQuitFunc[LD_PAGE_MAX])(uint8_t)={0};
static uint8_t pageCount=0;

void ldGuiAddPage(pFuncTypedef init,pFuncTypedef loop,pFuncTypedef quit)
{
    if(pageCount<LD_PAGE_MAX)
    {
        ldPageInitFunc[pageCount]=init;
        ldPageLoopFunc[pageCount]=loop;
        ldPageQuitFunc[pageCount]=quit;
        pageCount++;
    }
}

void ldGuiClickedAction(uint8_t touchSignal,int16_t x,int16_t y)
{
    ldCommon_t *pWidget;
    xListNode *pNode;

    switch(touchSignal)
    {
    case SIGNAL_NO_OPERATION:
    {
        break;
    }
    case SIGNAL_PRESS:
    {
        pWidget=NULL;
        if(pWidget==NULL)
        {
            pNode=ldBaseGetWidgetInfoByPos(x,y);
            if(pNode!=NULL)
            {
                pWidget=pNode->info;
                LOG_DEBUG("click widget id:%d\n",pWidget->nameId);
            }
        }
        prevX=x;
        prevY=y;
        prevWidget=pWidget;//准备数据,释放时候使用
        pressPoint.x=x;
        pressPoint.y=y;
        deltaMoveTime=arm_2d_helper_convert_ticks_to_ms(arm_2d_helper_get_system_timestamp());

        if(pWidget!=NULL)
        {
            xEmit(pWidget->nameId,touchSignal,((x<<16)&0xFFFF0000)|(y&0xFFFF));
        }
        break;
    }
    case SIGNAL_HOLD_DOWN:
    {
        if((prevX!=x)||(prevY!=y))
        {
            pWidget=prevWidget;//不可以把static变量作为函数变量调用
            if(pWidget!=NULL)
            {
                xEmit(pWidget->nameId,touchSignal,((x<<16)&0xFFFF0000)|(y&0xFFFF));
                xEmit(pWidget->nameId,SIGNAL_TOUCH_HOLD_MOVE,(((x-pressPoint.x)<<16)&0xFFFF0000)|((y-pressPoint.y)&0xFFFF));
            }
            prevX=x;
            prevY=y;
        }
        break;
    }
    case SIGNAL_RELEASE:
    {
        pWidget=prevWidget;
        if(pWidget!=NULL)
        {
            //cal speed
            deltaMoveTime=arm_2d_helper_convert_ticks_to_ms(arm_2d_helper_get_system_timestamp())-deltaMoveTime;
            pressPoint.x=(prevX-pressPoint.x);
            pressPoint.y=(prevY-pressPoint.y);
            pressPoint.x=(pressPoint.x*100)/deltaMoveTime;
            pressPoint.y=(pressPoint.y*100)/deltaMoveTime;
            xEmit(pWidget->nameId,SIGNAL_MOVE_SPEED,((pressPoint.x<<16)&0xFFFF0000)|(pressPoint.y&0xFFFF));//x speed | y speed
            xEmit(pWidget->nameId,touchSignal,((prevX<<16)&0xFFFF0000)|(prevY&0xFFFF));
        }
        break;
    }
    default:
        break;
    }
}

void ldGuiTouchProcess(void)
{
    int16_t x;
    int16_t y;
    bool nowState;
    static bool prevState=TOUCH_NO_CLICK;
    uint8_t touchSignal=SIGNAL_NO_OPERATION;

    nowState = ldCfgTouchGetPoint(&x,&y);

    if(nowState==TOUCH_CLICK)
    {
        if(prevState==TOUCH_NO_CLICK)//按下,                下降沿触发
        {
            touchSignal=SIGNAL_PRESS;
        }
        else// prevState==TOUCH_CLICK 按住                低电平
        {
            touchSignal=SIGNAL_HOLD_DOWN;
        }
    }
    else// nowState==TOUCH_NO_CLICK 无按下
    {
        if(prevState==TOUCH_NO_CLICK)//无按下                高电平
        {
            touchSignal=SIGNAL_NO_OPERATION;
        }
        else// prevState==TOUCH_CLICK 按下,上升沿触发       上降沿触发
        {
            touchSignal=SIGNAL_RELEASE;
        }
    }
    prevState=nowState;

    ldGuiClickedAction(touchSignal,x,y);
}


void ldGuiDelWidget(ldCommon_t *pWidget)
{
    switch(pWidget->widgetType)
    {
    case widgetTypeBackground:
    case widgetTypeWindow:
    {
        ldWindowDel((ldWindow_t*)pWidget);
        break;
    }
    case widgetTypeImage:
    {
        ldImageDel((ldImage_t*)pWidget);
        break;
    }
    case widgetTypeButton:
    {
        ldButtonDel((ldButton_t*)pWidget);
        break;
    }
    case widgetTypeText:
    {
        ldTextDel((ldText_t*)pWidget);
        break;
    }
    case widgetTypeProgressBar:
    {
        ldProgressBarDel((ldProgressBar_t*)pWidget);
        break;
    }
    case widgetTypeRadialMenu:
    {
        ldRadialMenuDel((ldRadialMenu_t*)pWidget);
        break;
    }
    case widgetTypeCheckBox:
    {
        ldCheckBoxDel((ldCheckBox_t*)pWidget);
        break;
    }
    case widgetTypeLabel:
    {
        ldLabelDel((ldLabel_t*)pWidget);
        break;
    }
    case widgetTypeScrollSelecter:
    {
        ldScrollSelecterDel((ldScrollSelecter_t*)pWidget);
        break;
    }
    case widgetTypeDateTime:
    {
        ldDateTimeDel((ldDateTime_t*)pWidget);
        break;
    }
    case widgetTypeIconSlider:
    {
        ldIconSliderDel((ldIconSlider_t*)pWidget);
        break;
    }
    case widgetTypeGauge:
    {
#if USE_GAUGE == 1
        ldGaugeDel((ldGauge_t*)pWidget);
#endif
        break;
    }
    case widgetTypeQRCode:
    {
#if USE_QR_CODE == 1
        ldQRCodeDel((ldQRCode_t*)pWidget);
#endif
        break;
    }
    case widgetTypeTable:
    {
        ldTableDel((ldTable_t*)pWidget);
        break;
    }
    case widgetTypeKeyboard:
    {
        ldKeyboardDel((ldKeyboard_t*)pWidget);
        break;
    }
    case widgetTypeLineEdit:
    {
        ldLineEditDel((ldLineEdit_t*)pWidget);
        break;
    }
    case widgetTypeGraph:
    {
        ldGraphDel((ldGraph_t*)pWidget);
        break;
    }
    case widgetTypeComboBox:
    {
        ldComboBoxDel((ldComboBox_t*)pWidget);
        break;
    }
    case widgetTypeArc:
    {
#if USE_ARC == 1
        ldArcDel((ldArc_t*)pWidget);
#endif
        break;
    }
/*============================ auto add del ==================================*/
    default:
        break;
    }
}

static void _widgetLoop(ldCommon_t *pWidget,const arm_2d_tile_t *ptParent,bool bIsNewFrame)
{
    switch(pWidget->widgetType)
    {
    case widgetTypeBackground:
    case widgetTypeWindow:
    case widgetTypeImage:
    {
        ldImageLoop((ldImage_t*)pWidget,ptParent,bIsNewFrame);
        break;
    }
    case widgetTypeButton:
    {
        ldButtonLoop((ldButton_t*)pWidget,ptParent,bIsNewFrame);
        break;
    }
    case widgetTypeText:
    {
        ldTextLoop((ldText_t*)pWidget,ptParent,bIsNewFrame);
        break;
    }
    case widgetTypeProgressBar:
    {
        ldProgressBarLoop((ldProgressBar_t*)pWidget,ptParent,bIsNewFrame);
        break;
    }
    case widgetTypeRadialMenu:
    {
        ldRadialMenuLoop((ldRadialMenu_t*)pWidget,ptParent,bIsNewFrame);
        break;
    }
    case widgetTypeCheckBox:
    {
        ldCheckBoxLoop((ldCheckBox_t*)pWidget,ptParent,bIsNewFrame);
        break;
    }
    case widgetTypeLabel:
    {
        ldLabelLoop((ldLabel_t*)pWidget,ptParent,bIsNewFrame);
        break;
    }
    case widgetTypeScrollSelecter:
    {
        ldScrollSelecterLoop((ldScrollSelecter_t*)pWidget,ptParent,bIsNewFrame);
        break;
    }
    case widgetTypeDateTime:
    {
        ldDateTimeLoop((ldDateTime_t*)pWidget,ptParent,bIsNewFrame);
        break;
    }
    case widgetTypeIconSlider:
    {
        ldIconSliderLoop((ldIconSlider_t*)pWidget,ptParent,bIsNewFrame);
        break;
    }
    case widgetTypeGauge:
    {
#if USE_GAUGE == 1
        ldGaugeLoop((ldGauge_t*)pWidget,ptParent,bIsNewFrame);
#endif
        break;
    }
    case widgetTypeQRCode:
    {
#if USE_QR_CODE == 1
        ldQRCodeLoop((ldQRCode_t*)pWidget,ptParent,bIsNewFrame);
#endif
        break;
    }
    case widgetTypeTable:
    {
        ldTableLoop((ldTable_t*)pWidget,ptParent,bIsNewFrame);
        break;
    }
    case widgetTypeKeyboard:
    {
        ldKeyboardLoop((ldKeyboard_t*)pWidget,ptParent,bIsNewFrame);
        break;
    }
    case widgetTypeLineEdit:
    {
        ldLineEditLoop((ldLineEdit_t*)pWidget,ptParent,bIsNewFrame);
        break;
    }
    case widgetTypeGraph:
    {
        ldGraphLoop((ldGraph_t*)pWidget,ptParent,bIsNewFrame);
        break;
    }
    case widgetTypeComboBox:
    {
        ldComboBoxLoop((ldComboBox_t*)pWidget,ptParent,bIsNewFrame);
        break;
    }
    case widgetTypeArc:
    {
#if USE_ARC == 1
        ldArcLoop((ldArc_t*)pWidget,ptParent,bIsNewFrame);
#endif
        break;
    }
/*============================ auto add loop =================================*/
    default:
        break;
    }
}

static void _ldGuiLoop(xListNode* pLink,const arm_2d_tile_t *ptParent,bool bIsNewFrame)
{
    xListNode *temp_pos,*safePos;

    list_for_each_safe(temp_pos,safePos, pLink)
    {
        if(temp_pos->info!=NULL)
        {
            _widgetLoop(temp_pos->info,ptParent,bIsNewFrame);
            
            if(((ldCommon_t *)temp_pos->info)->childList!=NULL)
            {
                _ldGuiLoop(((ldCommon_t *)temp_pos->info)->childList,ptParent,bIsNewFrame);
            }
        }
    }
}

static void ldGuiSetDirtyRegion(xListNode* pLink,arm_2d_scene_t *pSence)
{
    xListNode *temp_pos,*safePos;

    list_for_each_safe(temp_pos,safePos, pLink)
    {
        if(temp_pos->info!=NULL)
        {
            ldBaseAddDirtyRegion(&((ldCommon_t *)temp_pos->info)->dirtyRegionListItem,&pSence->ptDirtyRegion);

            if(((ldCommon_t *)temp_pos->info)->childList!=NULL)
            {
                ldGuiSetDirtyRegion(((ldCommon_t *)temp_pos->info)->childList,pSence);
            }
        }
    }
}

/**
 * @brief   ldgui的初始化函数
 * 
 * @author  Ou Jianbo(59935554@qq.com)
 * @date    2023-11-07
 */
void ldGuiInit(arm_2d_scene_t *pSence)
{
    xEmitInit();

#if LD_PAGE_MAX > 1
    if(ldPageInitFunc[pageNumNow])
    {
        ldPageInitFunc[pageNumNow](pageNumNow);
    }
#else
    ldPageInitFunc[0](pageNumNow);
#endif
    LOG_INFO("[sys] page %d init\n",pageNumNow);

#if USE_DIRTY_REGION == 1
    ldGuiSetDirtyRegion(&ldWidgetLink,pSence);
    LOG_INFO("[sys] set dirty region\n");
#endif
}


static void _frameUpdate(ldCommon_t *pWidget)
{
    switch(pWidget->widgetType)
    {
    case widgetTypeBackground:
    case widgetTypeWindow:
    case widgetTypeImage:
    {
        ldImageFrameUpdate((ldImage_t*)pWidget);
        break;
    }
    case widgetTypeButton:
    {
        ldButtonFrameUpdate((ldButton_t*)pWidget);
        break;
    }
    case widgetTypeText:
    {
        ldTextFrameUpdate((ldText_t*)pWidget);
        break;
    }
    case widgetTypeProgressBar:
    {
        ldProgressBarFrameUpdate((ldProgressBar_t*)pWidget);
        break;
    }
    case widgetTypeRadialMenu:
    {
        ldRadialMenuFrameUpdate((ldRadialMenu_t*)pWidget);
        break;
    }
    case widgetTypeCheckBox:
    {
        ldCheckBoxFrameUpdate((ldCheckBox_t*)pWidget);
        break;
    }
    case widgetTypeLabel:
    {
        ldLabelFrameUpdate((ldLabel_t*)pWidget);
        break;
    }
    case widgetTypeScrollSelecter:
    {
        ldScrollSelecterFrameUpdate((ldScrollSelecter_t*)pWidget);
        break;
    }
    case widgetTypeDateTime:
    {
        ldDateTimeFrameUpdate((ldDateTime_t*)pWidget);
        break;
    }
    case widgetTypeIconSlider:
    {
        ldIconSliderFrameUpdate((ldIconSlider_t*)pWidget);
        break;
    }
    case widgetTypeGauge:
    {
#if USE_GAUGE == 1
        ldGaugeFrameUpdate((ldGauge_t*)pWidget);
#endif
        break;
    }
    case widgetTypeQRCode:
    {
#if USE_QR_CODE == 1
        ldQRCodeFrameUpdate((ldQRCode_t*)pWidget);
#endif
        break;
    }
    case widgetTypeTable:
    {
        ldTableFrameUpdate((ldTable_t*)pWidget);
        break;
    }
    case widgetTypeKeyboard:
    {
        ldKeyboardFrameUpdate((ldKeyboard_t*)pWidget);
        break;
    }
    case widgetTypeLineEdit:
    {
        ldLineEditFrameUpdate((ldLineEdit_t*)pWidget);
        break;
    }
    case widgetTypeGraph:
    {
        ldGraphFrameUpdate((ldGraph_t*)pWidget);
        break;
    }
    case widgetTypeComboBox:
    {
        ldComboBoxFrameUpdate((ldComboBox_t*)pWidget);
        break;
    }
    case widgetTypeArc:
    {
#if USE_ARC == 1
        ldArcFrameUpdate((ldArc_t*)pWidget);
#endif
        break;
    }
/*============================ auto add start =================================*/
    default:
        break;
    }
}

static void _ldGuiFrameUpdate(xListNode* pLink)
{
    xListNode *temp_pos,*safePos;

    list_for_each_safe(temp_pos,safePos, pLink)
    {
        if(temp_pos->info!=NULL)
        {
            _frameUpdate((ldCommon_t *)temp_pos->info);

            if(((ldCommon_t *)temp_pos->info)->childList!=NULL)
            {
                _ldGuiFrameUpdate(((ldCommon_t *)temp_pos->info)->childList);
            }
        }
    }
}

void ldGuiFrameStart(void)
{
    _ldGuiFrameUpdate(&ldWidgetLink);

    //检查按键
    if(ldTimeOut(10,&sysTimer,true))
    {
        xBtnTick(10);
    }

    //检查触摸
    ldGuiTouchProcess();

    xConnectProcess();
}

/**
 * @brief   ldgui的逻辑处理函数
 * 
 * @author  Ou Jianbo(59935554@qq.com)
 * @date    2023-11-07
 */
void ldGuiLogicLoop(void)
{
#if LD_PAGE_MAX > 1
    if(ldPageLoopFunc[pageNumNow])
    {
        ldPageLoopFunc[pageNumNow](pageNumNow);
    }
#else
    ldPageLoopFunc[0](pageNumNow);
#endif
}

/**
 * @brief   ldgui的界面处理函数
 * 
 * @param   ptParent        arm2d的tile对象
 * @param   bIsNewFrame     新的一帧开始标志
 * @author  Ou Jianbo(59935554@qq.com)
 * @date    2023-11-07
 */
void ldGuiLoop(arm_2d_scene_t *pSence,arm_2d_tile_t *ptParent,bool bIsNewFrame)
{
    //遍历控件
    _ldGuiLoop(&ldWidgetLink,ptParent,bIsNewFrame);
}

/**
 * @brief   ldgui的页面退出函数
 * 
 * @author  Ou Jianbo(59935554@qq.com)
 * @date    2023-11-07
 */
void ldGuiQuit(void)
{
#if LD_PAGE_MAX > 1
    if(ldPageQuitFunc[pageNumNow])
    {
        ldPageQuitFunc[pageNumNow](pageNumNow);
    }
#else
    ldPageQuitFunc[0](pageNumNow);
#endif
    LOG_INFO("[sys] page %d quit\n",pageNumNow);
}

/**
 * @brief   ldgui页面跳转函数
 * 
 * @param   pageNum         目标页面序号
 * @author  Ou Jianbo(59935554@qq.com)
 * @date    2023-11-07
 */
void ldGuiJumpPage(uint8_t pageNum)
{
    pageTarget=pageNum;
}
