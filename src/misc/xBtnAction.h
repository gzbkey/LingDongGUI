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

#ifndef _X_BTN_ACTION_H_
#define _X_BTN_ACTION_H_

#include <stdint.h>
#include <stdbool.h>

#include "stdint.h"
#include "stdbool.h"
#include "stdlib.h"
#include "ldBase.h"

#define BTN_NO_OPERATION         0 //检测按键无按下信号
#define BTN_PRESS                1 //检测按键按下信号
#define BTN_HOLD_DOWN            2 //检测按键按住不放信号
#define BTN_RELEASE              3 //检测按键释放信号
#define BTN_DOUBLE_CLICK         4 //检测按键双击信号
#define BTN_REPEAT_COUNT         5 //获取连击次数
#define BTN_HOLD_TIME            6 //获取按键按住不放的时间
#define BTN_LONG_START           7 //检测按键长按触发一次的信号
#define BTN_LONG_SHOOT           8 //检测按键长按触发一次和连续触发的信号

#ifndef XMALLOC
#define XMALLOC                  ldMalloc
#endif
#ifndef XFREE
#define XFREE                    ldFree
#endif

typedef struct {
    uint8_t FSM_State;
    uint8_t doubleClickCount;
    uint8_t repeatCount;
    uint16_t repeatTimeOutCount;
    uint16_t holdCount;
    uint16_t shootCount;
    uint16_t id;
    bool (*getBtnStateFunc)(uint16_t);
    bool btnNewState:1;
    bool btnOldState:1;
    bool isPressed:1;
    bool isReleased:1;
//    bool isClicked:1;
    bool isDoubleClicked:1;
    bool isRepeatEnd:1;
    bool isShoot:1;
}xBtnInfo_t;

void xBtnInit(uint16_t id,bool (*getBtnStateFunc)(uint16_t));
void xBtnConfig(uint8_t debounceMs,uint16_t longPressMs,uint16_t longShootMs,uint16_t clickTimeOutMs);
void xBtnTick(uint8_t cycleMs);
uint16_t xBtnGetState(uint16_t id, uint8_t state);
void xBtnClean(void);

#endif //_X_BTN_ACTION_H_
