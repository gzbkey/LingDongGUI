/*
 * Copyright (c) 2021-2024 Ou Jianbo (59935554@qq.com). All rights reserved.
 *
 * SPDX-License-Identifier: Apache-2.0
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
 * @file    xBtnAction.c
 * @author  Ou Jianbo(59935554@qq.com)
 * @brief   轻量级按键状态检测库
 *          支持实体按键、触摸按键
 */
#include "xBtnAction.h"
#include "xList.h"
#include "string.h"

//规定常规电平为高电平，按下电平为低电平，可在getBtnStateFunc匹配

#define PRESS_LEVEL          0

NEW_LIST(xBtnLink);

static uint8_t btnCycle=10;

static uint8_t btnDebounceMs    = 10;   //按键滤波时间
static uint16_t btnLongPress    = 200;  //长按触发时间
static uint16_t btnLongShoot    = 10;   //长按连续触发间隔
static uint16_t btnClickTimeOut = 50;   //连击超时时间

void xBtnConfig(uint8_t debounceMs,uint16_t longPressMs,uint16_t longShootMs,uint16_t clickTimeOutMs)
{
    btnDebounceMs=debounceMs;
    btnLongPress=longPressMs/btnDebounceMs;
    btnLongShoot=longShootMs/btnDebounceMs;
    btnClickTimeOut=clickTimeOutMs/btnDebounceMs;
}

void xBtnInit(uint16_t id,bool (*getBtnStateFunc)(uint16_t))
{
    xBtnInfo_t * link = (xBtnInfo_t *)XMALLOC(sizeof(xBtnInfo_t));
    if(link!=NULL)
    {
        memset(link,0,sizeof(xBtnInfo_t));

        link->id=id;
        link->getBtnStateFunc=getBtnStateFunc;
        xListInfoAdd(&xBtnLink,link);
    }
}

void xBtnProcess(xBtnInfo_t *btnInfo)
{

    btnInfo->btnOldState=btnInfo->btnNewState;
    btnInfo->btnNewState=btnInfo->getBtnStateFunc(btnInfo->id);

        switch(btnInfo->FSM_State)
        {
        case BTN_NO_OPERATION:
        {
            if((btnInfo->btnNewState==PRESS_LEVEL)&&(btnInfo->btnNewState==btnInfo->btnOldState))
            {
                btnInfo->holdCount=0;
                btnInfo->isPressed=true;
                btnInfo->FSM_State=BTN_HOLD_DOWN;
            }
            break;
        }
//        case EBTN_PRESSED:
//        {
//
//            break;
//        }
        case BTN_HOLD_DOWN:
        {
            btnInfo->holdCount++;

            if((btnInfo->holdCount>btnLongPress)&&(btnInfo->shootCount==0))
        {
            btnInfo->shootCount++;
            btnInfo->isShoot=true;
        }
        else
        {
            if(((btnInfo->holdCount-btnLongPress)/btnLongShoot+1==btnInfo->shootCount)&&(btnInfo->shootCount>0))
            {
                btnInfo->shootCount++;
                btnInfo->isShoot=true;
            }
        }

        
            if((btnInfo->btnNewState!=PRESS_LEVEL)&&(btnInfo->btnNewState==btnInfo->btnOldState))
            {
                            btnInfo->holdCount=0;
            btnInfo->shootCount=0;
            btnInfo->isShoot=false;
            btnInfo->isReleased=true;
//            btnInfo->isClicked=true;

                btnInfo->doubleClickCount++;//单击计数
                if(btnInfo->doubleClickCount>=2)
                {
                    btnInfo->isDoubleClicked=true;
                    btnInfo->doubleClickCount=0;
                }

                btnInfo->repeatCount++;
                btnInfo->repeatTimeOutCount=0;//复位超时

                btnInfo->FSM_State=BTN_NO_OPERATION;
            }
            break;
        }
//        case EBTN_RELEASED:
//        {
//
//            break;
//        }
        default:
            break;
        }

        //连击超时处理
        if(btnInfo->repeatCount>0)//有点击才开始计算超时
        {
            btnInfo->repeatTimeOutCount++;
            if(btnInfo->repeatTimeOutCount>=btnClickTimeOut)
            {
                btnInfo->repeatTimeOutCount=0;//复位超时
                btnInfo->isRepeatEnd=true;
            }
        }
}

static bool _btnLoop(xListNode* pEachInfo,void* pInfo)
{
    (void*)pInfo;
    xBtnProcess((xBtnInfo_t *)pEachInfo->info);
     
    return false;
}

void xBtnTick(uint8_t cycleMs)
{
    if(btnCycle==0)
    {
        btnCycle=cycleMs;
    }
    
    //循环检测所有btnEx
    xListInfoPrevTraverse(&xBtnLink,NULL,_btnLoop);
}



uint16_t xBtnGetState(uint16_t id,uint8_t state)
{
    uint16_t ret=0;
    xBtnInfo_t *btnInfo=NULL;
    xListNode *temp_pos,*safePos;

    list_for_each_prev_safe(temp_pos,safePos, &xBtnLink)
    {
        if(temp_pos->info!=NULL)
        {
            if(((xBtnInfo_t *)temp_pos->info)->id==id)
            {
                btnInfo=temp_pos->info;
                break;
            }
        }
    }
    
    if(btnInfo==NULL)
    {
        return 0;
    }
    
    switch(state)
    {
    case BTN_NO_OPERATION:
    {
        ret=(state==btnInfo->FSM_State)?true:false;
        break;
    }
    case BTN_PRESS:
    {
        if(btnInfo->isPressed)
        {
            btnInfo->isPressed=false;
            ret=true;
        }
        else
        {
            ret=false;
        }
        break;
    }
    case BTN_HOLD_DOWN:
    {
        ret=(state==btnInfo->FSM_State)?true:false;
        break;
    }
    case BTN_RELEASE:
    {
        if(btnInfo->isReleased)
        {
            btnInfo->isReleased=false;
            ret=true;
        }
        else
        {
            ret=false;
        }
        break;
    }
    case BTN_DOUBLE_CLICK:
    {
        if(btnInfo->isDoubleClicked)
        {
            btnInfo->isDoubleClicked=false;
            ret=true;
        }
        else
        {
            ret=false;
        }
        break;
    }
    case BTN_REPEAT_COUNT:
    {
        if(btnInfo->isRepeatEnd)
        {
            btnInfo->isRepeatEnd=false;
            ret=btnInfo->repeatCount;
            btnInfo->repeatCount=0;
        }
        break;
    }
    case BTN_HOLD_TIME:
    {
        ret=btnInfo->holdCount*btnCycle;
        break;
    }

    case BTN_LONG_START:
    {
        if(btnInfo->holdCount>=btnLongPress)
        {
            ret=true;
        }
        else
        {
            ret=false;
        }
        break;
    }

    case BTN_LONG_SHOOT:
    {
        if((btnInfo->holdCount>(btnLongPress))&&(btnInfo->shootCount==0))
        {
            btnInfo->shootCount++;
            ret=true;
        }
        else
        {

            if(((btnInfo->holdCount-btnLongPress)/btnLongShoot+1==btnInfo->shootCount)&&(btnInfo->shootCount>0))
            {
                btnInfo->shootCount++;
                ret=true;
            }
        }

        break;
    }
    default:
        break;
    }
    return ret;
}

void xBtnClean(void)
{
    xListNode *temp_pos,*safePos;

    list_for_each_prev_safe(temp_pos,safePos, &xBtnLink)
    {
        if(temp_pos->info!=NULL)
        {
            XFREE(xListInfoDel(temp_pos));
        }
    }
}
