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

#ifndef _LL_TIMER_H_
#define _LL_TIMER_H_

#include <stdint.h>
#include <stdbool.h>

/**
 * @brief 放在定时器中断内
 * 
 * @param cycleMs 定时器周期，单位：毫秒
 */
void xTimerTick(uint16_t cycleMs);

/**
 * @brief 启动定时器
 * 
 * @param pTimer 计数器指针
 */
void xTimerStart(uint32_t *pTimer);

/**
 * @brief 停止定时器
 * 
 * @param pTimer  计数器指针
 */
void xTimerStop(uint32_t *pTimer);

/**
 * @brief 软件定时器超时检测
 * 
 * @param  pTimer        计数变量
 * @param  msec          定时时间，单位：毫秒
 * @param  isRestart     超时触发后，是否自动重新启动
 * @return true         到设定时间
 * @return false        没到设定时间
 * @date   2021-01-10
 */
bool xTimerTimeout(uint32_t *pTimer,uint32_t msec,bool isRestart);

/**
 * @brief 软件循环等待延时
 * 
 * @param delayMs 延时时间，单位：毫秒
 */
void xTimerDelayMs(uint32_t delayMs);














#endif //_LL_TIMER_H_
