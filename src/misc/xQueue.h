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

#ifndef _X_QUEUE_H_
#define _X_QUEUE_H_

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "ldCommon.h"

#define X_QUEUE_CALLOC    XCALLOC
//#define X_QUEUE_FREE      free

typedef struct
{
    void *pDataBase;
    uint32_t head;
    uint32_t tail;
    uint32_t maxsize;
}xQueue_t;


xQueue_t* xQueueCreate(uint32_t length, uint32_t itemSize);

bool xQueueEnqueue(xQueue_t *queue ,void * pInItem,uint32_t itemSize);

bool xQueueDequeue(xQueue_t *queue ,void * pOutItem,uint32_t itemSize);

uint32_t xQueueGetLength(xQueue_t *queue);

void xQueueClear(xQueue_t* queue);

#endif //_X_QUEUE_H_
