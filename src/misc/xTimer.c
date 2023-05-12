/*
 * Copyright 2021-2023 Ou Jianbo 59935554@qq.com
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

#include "xTimer.h"

static volatile uint32_t ticksCount=1;
static uint16_t timerCycle=0;

#define TIMER_TICK_MAX    0xFFFFFFFF

/**
 * @brief 
 * 
 * @param cycleMs 
 */
void xTimerTick(uint16_t cycleMs)
{
    if(timerCycle==0)
    {
        timerCycle=cycleMs;
    }
    if(ticksCount<TIMER_TICK_MAX)
    {
        ticksCount++;
    }
    else
    {
        ticksCount=0;
    }
}

void xTimerStart(uint32_t *pTimer)
{
    if(ticksCount==0)
    {
        *pTimer=1;
    }
    else
    {
        *pTimer=ticksCount;
    }
}

void xTimerStop(uint32_t *pTimer)
{
    *pTimer=0;
}

// return 1 time out
// msec 必须大于定时器触发周期
bool xTimerTimeout(uint32_t *pTimer,uint32_t msec,bool isRestart)
{
    uint64_t targetTickValue;
    uint64_t ticksCountTemp;
    uint32_t tickIncrement;
    
    if(timerCycle==0)
    {
        return false;
    }

    tickIncrement=msec/timerCycle;
    
    if(tickIncrement!=0)
    {
        if(tickIncrement>=TIMER_TICK_MAX)
        {
            *pTimer=0;
            return false;
        }
        if(*pTimer!=0)
        {
            targetTickValue=*pTimer+tickIncrement;

            if(ticksCount<*pTimer)
            {
                ticksCountTemp=TIMER_TICK_MAX+ticksCount;
            }
            else
            {
                ticksCountTemp=ticksCount;
            }
            if(ticksCountTemp>=targetTickValue)
            {
                if(isRestart)
                {
                    xTimerStart(pTimer);
                }
                else
                {
                    *pTimer=0;
                }
                
                return true;
            }
        }
        return false;
    }
    else
    {
        return true;
    }
}

void xTimerDelayMs(uint32_t delayMs)
{
    uint32_t delayCounter=0;
    uint8_t flag=1;
    
    xTimerStart(&delayCounter);
    while(flag)
    {
        while(xTimerTimeout(&delayCounter,delayMs,false))
        {
            flag=0;
            break;
        }
    }
}
