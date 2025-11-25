/*
 * Copyright (c) 2021-2025 Ou Jianbo (59935554@qq.com). All rights reserved.
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
#include "string.h"

static xBtnInfo_t *xBtnLink=NULL;

static uint8_t btnCycle=0;

static uint8_t btnDebounceMs    = 10;   //按键滤波时间
static uint16_t btnLongPress    = 200;  //长按触发时间
static uint16_t btnLongShoot    = 10;   //长按连续触发间隔
static uint16_t btnClickTimeOut = 50;   //连击超时时间
static bool waitReset           = false;

void xBtnConfig(uint8_t debounceMs,uint16_t longPressMs,uint16_t longShootMs,uint16_t clickTimeOutMs)
{
    btnDebounceMs=debounceMs;
    btnLongPress=longPressMs/btnDebounceMs;
    btnLongShoot=longShootMs/btnDebounceMs;
    btnClickTimeOut=clickTimeOutMs/btnDebounceMs;
}

void _xBtnInit(uint16_t id, isBtnPressFunc pFunc, xBtnInfo_t *pBtnBuf)
{
    xBtnInfo_t * link = NULL;
    if(pBtnBuf==NULL)
    {
#ifdef XMALLOC
        link = (xBtnInfo_t *)XMALLOC(sizeof(xBtnInfo_t));
#endif
    }
    else
    {
        link = pBtnBuf;
    }

    if(link!=NULL)
    {
        memset(link, 0, sizeof (xBtnInfo_t));
        link->id=id;
        link->getBtnPressFunc=pFunc;
        link->pNext=xBtnLink;
        xBtnLink=link;
    }
}

static void _xBtnResetInfo(void)
{
    xBtnInfo_t *p = xBtnLink;

    p = xBtnLink;
    while (p)
    {
        if(p->getBtnPressFunc(p->id, NULL) == true)
        {
            return ;
        }
        p=p->pNext;
    }

    p=xBtnLink;

    while (p!=NULL)
    {
        p->FSM_State=BTN_NO_OPERATION;
        p->doubleClickCount=0;
        p->repeatCount=0;
        p->timeOutCount=0;
        p->holdCount=0;
        p->shootCount=0;
        p->_isNewPress=false;
        p->_isOldPress=false;
        p->isPressed=false;
        p->isReleased=false;
        p->isDoubleClicked=false;
        p->isRepeatEnd=false;
        p->isShoot=false;
        p=p->pNext;
    }
    waitReset = false;
}

static void _xBtnProcess(xBtnInfo_t *btnInfo,void *pUser)
{
    btnInfo->_isOldPress=btnInfo->_isNewPress;
    btnInfo->_isNewPress=btnInfo->getBtnPressFunc(btnInfo->id,pUser);

    switch(btnInfo->FSM_State)
    {
    case BTN_NO_OPERATION:
    {
        if((btnInfo->_isNewPress==true)&&(btnInfo->_isNewPress==btnInfo->_isOldPress))
        {
            btnInfo->holdCount=0;
            btnInfo->isPressed=true;
            btnInfo->holdCount=0;
            btnInfo->FSM_State=BTN_HOLD_DOWN;
        }
        break;
    }
    case BTN_HOLD_DOWN:
    {
        if(btnInfo->holdCount>=0)
        {
            btnInfo->holdCount++;
        }

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

        if((btnInfo->_isNewPress==false)&&(btnInfo->_isNewPress==btnInfo->_isOldPress))
        {
            btnInfo->holdCount=0;
            btnInfo->shootCount=0;
            btnInfo->isShoot=false;
            btnInfo->isReleased=true;
            btnInfo->doubleClickCount++;//单击计数
            if(btnInfo->doubleClickCount>=2)
            {
                btnInfo->isDoubleClicked=true;
                btnInfo->doubleClickCount=0;
            }
            btnInfo->repeatCount++;
            btnInfo->timeOutCount=0;//复位超时
            btnInfo->FSM_State=BTN_NO_OPERATION;
        }
        break;
    }
    default:
        break;
    }

    //连击超时处理
    if(btnInfo->repeatCount>0)//有点击才开始计算超时
    {
        btnInfo->timeOutCount++;
        if(btnInfo->timeOutCount>=btnClickTimeOut)
        {
            btnInfo->timeOutCount=0;//复位超时
            btnInfo->doubleClickCount=0;
            btnInfo->isRepeatEnd=true;
        }
    }
}

void xBtnTick(uint8_t cycleMs, void *pUser)
{
    if(xBtnLink==NULL)
    {
        return;
    }

    if(btnCycle==0)
    {
        btnCycle=cycleMs;
    }

    xBtnInfo_t *btnInfo=xBtnLink;

    while (btnInfo!=NULL)
    {
        _xBtnProcess(btnInfo,pUser);
        btnInfo=btnInfo->pNext;
    }
}

uint16_t xBtnGetState(uint16_t id,uint8_t state)
{
    uint16_t ret=0;
    xBtnInfo_t *btnInfo=NULL;

    if(waitReset)
    {
        _xBtnResetInfo();
        return 0;
    }

    btnInfo=xBtnLink;

    while (btnInfo!=NULL)
    {
        if(btnInfo->id==id)
        {
            ret=1;
            break;
        }
        btnInfo=btnInfo->pNext;
    }

    if(ret==0)
    {
        return 0;
    }

    ret=0;

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
            btnInfo->holdCount=-1;
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
        if(btnInfo->isShoot)
        {
            btnInfo->isShoot=false;
            ret=true;
        }
        break;
    }
    default:
        break;
    }
    return ret;
}

void xBtnReset(void)
{
    waitReset=true;
}

void xBtnDestroy(void)
{
    xBtnInfo_t *btnInfo = xBtnLink;

    while (btnInfo != NULL)
    {
        xBtnInfo_t *next = btnInfo->pNext;
#ifdef XFREE
        XFREE(btnInfo);
#endif
        btnInfo = next;
    }
    xBtnLink=NULL;
}
