/*
 * Copyright 2021-2024 Ou Jianbo 59935554@qq.com
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
 * @file    xQueue.c
 * @author  Ou Jianbo(59935554@qq.com)
 * @brief   队列库
 *          支持16位、32位、64位芯片
 */
#include "xQueue.h"
#include "string.h"

#if SIZE_MAX == 18446744073709551615ull
#define MCU_BITS 64
#elif SIZE_MAX == 4294967295
#define MCU_BITS 32
#elif SIZE_MAX == 65535
#define MCU_BITS 16
#else
#error "unknown"
#endif

xQueue_t* xQueueCreate(uint32_t length, uint32_t itemSize)
{
    xQueue_t *pNewQueue;
    uint32_t  Queue_Data_Buff_Size;

    Queue_Data_Buff_Size=length*itemSize;
    //申请队列总空间
    pNewQueue=( xQueue_t * ) X_QUEUE_CALLOC ( sizeof( xQueue_t ) + Queue_Data_Buff_Size ) ;

    if(pNewQueue!=NULL)
    {

        memset(pNewQueue,0,sizeof( xQueue_t ) + Queue_Data_Buff_Size);
        //分配内存成功
        pNewQueue->head=0;
        pNewQueue->tail=0;
        pNewQueue->maxsize=length;

#if MCU_BITS == 16
        pNewQueue->pDataBase=(void *)( (uint16_t)pNewQueue + (uint16_t)sizeof( xQueue_t )); //s12
#endif
#if MCU_BITS == 32
        pNewQueue->pDataBase=(void *)( (uint32_t)pNewQueue + sizeof( xQueue_t )); //M3
#endif
#if MCU_BITS == 64
        pNewQueue->pDataBase=(void *)( (uint64_t)pNewQueue + sizeof( xQueue_t )); //M3
#endif
    }
    return pNewQueue;
}

void xQueueClear(xQueue_t* queue)
{
    queue->head=queue->tail;
}

bool xQueueIsFull(xQueue_t *queue)
{
    if(queue->head==(queue->tail+1)%queue->maxsize)
        return true;
    else
        return false;
}

bool xQueueIsEmpty(xQueue_t *queue)
{
    if(queue->head==queue->tail)
        return true;
    else
    {
        return false;
    }
}

uint32_t xQueueGetLength(xQueue_t *queue)
{
    if(xQueueIsEmpty(queue)==true)
    {
        return 0;
    }
    else
    {
        if(xQueueIsFull(queue)==true)
        {
            return queue->maxsize-1;
        }
        else
        {
            if(queue->tail<queue->head)
            {
                return (queue->maxsize-queue->head)+queue->tail;
            }
            else
            {
                return queue->tail-queue->head;
            }
        }
    }
}

bool xQueueEnqueue(xQueue_t *queue,void * pInItem,uint32_t itemSize)
{
    // 数据首地址 queue->pDataBase+uiItemSize*(queue->tail)

    //判断是否满
    if(xQueueIsFull(queue)==false)
    {
        memcpy((uint8_t*)queue->pDataBase+itemSize*(queue->tail),pInItem,itemSize);
        queue->tail=(queue->tail+1)%queue->maxsize;
        return true;
    }
    else
    {
        return false;
    }
}

bool xQueueDequeue(xQueue_t *queue,void * pOutItem,uint32_t itemSize)
{
    //判断是否空
    if(xQueueIsEmpty(queue)==false)
    {
        memcpy(pOutItem,(uint8_t*)queue->pDataBase+itemSize*(queue->head),itemSize);

        queue->head=(queue->head+1)%queue->maxsize;

        return true;
    }
    else
    {
        return false;
    }
}

