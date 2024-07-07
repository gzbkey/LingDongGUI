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

#ifndef _X_CONNECT_H_
#define _X_CONNECT_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "stdint.h"
#include "stdbool.h"
#include "stddef.h"

typedef struct{
    uint16_t senderId;
    uint8_t signalType;
    uint64_t value;
}emitInfo_t;

typedef struct{
    uint16_t senderId;
    uint8_t signalType;
    uint16_t receiverId;
    uint64_t value;
}xConnectInfo_t;

typedef bool (*connectFunc)(xConnectInfo_t);

typedef struct{
    uint16_t senderId;
    uint8_t signalType;
    uint16_t receiverId;
    connectFunc receiverFunc;
}relationInfo_t;

bool xEmitInit(uint8_t size);
bool xEmit(uint16_t senderId,uint8_t signal,uint64_t value);
bool xConnect(uint16_t senderId,uint8_t signal,uint16_t receiverId,connectFunc func);
bool xDisconnect(uint16_t senderId,uint8_t signal,uint16_t receiverId,connectFunc func);
void xDeleteConnect(uint16_t nameId);

void xConnectProcess(void);

#ifdef __cplusplus
}
#endif

#endif //_X_CONNECT_H_
