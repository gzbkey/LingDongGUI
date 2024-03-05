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
uint8_t cursorBlinkCount=0;
bool cursorBlinkFlag=false;
#define TOUCH_NO_CLICK           0
#define TOUCH_CLICK              1

#ifndef LD_EMIT_SIZE
#define LD_EMIT_SIZE             8
#endif

static volatile ldPoint_t pressPoint;
static volatile int16_t deltaMoveTime;
static volatile int16_t prevX,prevY;
static void *prevWidget;

void (*ldPageInitFunc[LD_PAGE_MAX])(uint8_t)={0};
void (*ldPageLoopFunc[LD_PAGE_MAX])(uint8_t)={0};
void (*ldPageQuitFunc[LD_PAGE_MAX])(uint8_t)={0};

#if LD_PAGE_MAX > 1
static uint8_t pageCount=0;
#endif

void ldGuiAddPage(pFuncTypedef init,pFuncTypedef loop,pFuncTypedef quit)
{
#if LD_PAGE_MAX > 1
    if(pageCount<LD_PAGE_MAX)
    {
        ldPageInitFunc[pageCount]=init;
        ldPageLoopFunc[pageCount]=loop;
        ldPageQuitFunc[pageCount]=quit;
        pageCount++;
    }
#else
    ldPageInitFunc[0]=init;
    ldPageLoopFunc[0]=loop;
    ldPageQuitFunc[0]=quit;
#endif
}

void ldGuiClickedAction(uint8_t touchSignal,int16_t x,int16_t y)
{
    ldCommon_t *pWidget;
    xListNode *pNode;
    uint64_t u64Temp=0;

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
            u64Temp=x;
            u64Temp<<=16;
            u64Temp+=y;
            xEmit(pWidget->nameId,touchSignal,u64Temp);
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
                u64Temp=x-pressPoint.x;
                u64Temp<<=16;
                u64Temp+=y-pressPoint.y;
                u64Temp<<=16;
                u64Temp+=x;
                u64Temp<<=16;
                u64Temp+=y;

                xEmit(pWidget->nameId,touchSignal,u64Temp);
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

            // x speed,y speed,x,y
            u64Temp=pressPoint.x;
            u64Temp<<=16;
            u64Temp+=pressPoint.y;
            u64Temp<<=16;
            u64Temp+=prevX;
            u64Temp<<=16;
            u64Temp+=prevY;

            xEmit(pWidget->nameId,touchSignal,u64Temp);
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

static void _ldGuiLoop(xListNode* pLink,arm_2d_tile_t *ptParent,bool bIsNewFrame)
{
    xListNode *tempPos,*safePos;

    list_for_each_safe(tempPos,safePos, pLink)
    {
        if(tempPos->info!=NULL)
        {
            (((ldCommon_t *)tempPos->info)->pFunc)->loop(tempPos->info,ptParent,bIsNewFrame);
            
            if(((ldCommon_t *)tempPos->info)->childList!=NULL)
            {
                _ldGuiLoop(((ldCommon_t *)tempPos->info)->childList,ptParent,bIsNewFrame);
            }
        }
    }
}

static void ldGuiSetDirtyRegion(xListNode* pLink,arm_2d_scene_t *pSence)
{
    xListNode *tempPos,*safePos;

    list_for_each_safe(tempPos,safePos, pLink)
    {
        if(tempPos->info!=NULL)
        {
            ldBaseAddDirtyRegion(&((ldCommon_t *)tempPos->info)->dirtyRegionListItem,&pSence->ptDirtyRegion);

            if(((ldCommon_t *)tempPos->info)->childList!=NULL)
            {
                ldGuiSetDirtyRegion(((ldCommon_t *)tempPos->info)->childList,pSence);
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
    xEmitInit(LD_EMIT_SIZE);

#if LD_PAGE_MAX > 1
    if(ldPageInitFunc[pageNumNow])
    {
        ldPageInitFunc[pageNumNow](pageNumNow);
    }
#else
    if(ldPageInitFunc[0])
    {
        ldPageInitFunc[0](pageNumNow);
    }
#endif
    LOG_INFO("[sys] page %d init\n",pageNumNow);

#if USE_DIRTY_REGION == 1
    ldGuiSetDirtyRegion(&ldWidgetLink,pSence);
    LOG_INFO("[sys] set dirty region\n");
#endif
}

static void _ldGuiFrameUpdate(xListNode* pLink)
{
    xListNode *tempPos,*safePos;

    list_for_each_safe(tempPos,safePos, pLink)
    {
        if(tempPos->info!=NULL)
        {
            (((ldCommon_t *)tempPos->info)->pFunc)->update(tempPos->info);

            if(((ldCommon_t *)tempPos->info)->childList!=NULL)
            {
                _ldGuiFrameUpdate(((ldCommon_t *)tempPos->info)->childList);
            }
        }
    }
}

void ldGuiFrameStart(void)
{
    _ldGuiFrameUpdate(&ldWidgetLink);

    //检查按键
    if(ldTimeOut(SYS_TICK_CYCLE_MS,&sysTimer,true))
    {
        xBtnTick(SYS_TICK_CYCLE_MS);
        cursorBlinkCount++;
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
    if(ldPageLoopFunc[0])
    {
        ldPageLoopFunc[0](pageNumNow);
    }
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
    (void*)pSence;
    //遍历控件
    _ldGuiLoop(&ldWidgetLink,ptParent,bIsNewFrame);
}

/**
 * @brief   ldgui的页面退出函数
 * 
 * @author  Ou Jianbo(59935554@qq.com)
 * @date    2023-11-07
 */
void ldGuiQuit(arm_2d_scene_t *pSence)
{
#if LD_PAGE_MAX > 1
    if(ldPageQuitFunc[pageNumNow])
    {
        ldPageQuitFunc[pageNumNow](pageNumNow);
    }
#else
    if(ldPageQuitFunc[0])
    {
        ldPageQuitFunc[0](pageNumNow);
    }
#endif
    pSence->ptDirtyRegion=NULL;
    ldWindowDel(ldBaseGetWidgetById(0));
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
