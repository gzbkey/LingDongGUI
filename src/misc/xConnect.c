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
 * @file    xConnect.c
 * @author  Ou Jianbo(59935554@qq.com)
 * @brief   信号动作连接库，类似qt的信号槽
 * @version 0.1
 * @date    2023-11-03
 */
#include "xConnect.h"
#include "xQueue.h"
#include "xList.h"
#include "ldCommon.h"


xQueue_t *emitQueue=NULL;
NEW_LIST(listConnect);

bool xEmitInit(uint8_t size)
{
    if(emitQueue==NULL)
    {
        emitQueue=xQueueCreate(size,sizeof (emitInfo_t));
        if(emitQueue)
        {
            return true;
        }
    }
    return false;
}

bool xEmit(uint16_t senderId,uint8_t signal,uint64_t value)
{
    emitInfo_t emitInfo;

    emitInfo.senderId=senderId;
    emitInfo.signalType=signal;
    emitInfo.value=value;
    return xQueueEnqueue(emitQueue,&emitInfo,sizeof (emitInfo_t));
}

static bool _isConnectSame(xListNode* pEachInfo,void* pRelationInfo)
{
    if(((((relationInfo_t*)(pEachInfo->info))->senderId) ==((relationInfo_t*)pRelationInfo)->senderId)&&
       (((relationInfo_t*)(pEachInfo->info))->signalType ==((relationInfo_t*)pRelationInfo)->signalType)&&
       ((((relationInfo_t*)(pEachInfo->info))->receiverId) ==((relationInfo_t*)pRelationInfo)->receiverId)&&
       (((relationInfo_t*)(pEachInfo->info))->receiverFunc ==((relationInfo_t*)pRelationInfo)->receiverFunc))
    {
        return true;
    }

    return false;
}

bool xConnect(uint16_t senderId,uint8_t signal,uint16_t receiverId,connectFunc func)
{
    relationInfo_t* pRelation;

    pRelation=XCALLOC(sizeof(relationInfo_t));

    if(pRelation!=NULL)
    {
    pRelation->senderId=senderId;
    pRelation->receiverId=receiverId;
    pRelation->signalType=signal;
    pRelation->receiverFunc=func;

    //确定是否重复添加链接
    if(xListInfoPrevTraverse(&listConnect,pRelation,_isConnectSame))
    {
        XFREE(pRelation);
        return false;
    }

    //添加链表
    if(xListInfoAdd(&listConnect,pRelation)==NULL)
    {
        XFREE(pRelation);
        return false;
    }

        return true;
    }

    return false;

}

static bool _disconnect(xListNode* pEachInfo,void* pRelationInfo)
{
    if(((((relationInfo_t*)(pEachInfo->info))->senderId) ==((relationInfo_t*)pRelationInfo)->senderId)&&
       (((relationInfo_t*)(pEachInfo->info))->signalType ==((relationInfo_t*)pRelationInfo)->signalType)&&
       ((((relationInfo_t*)(pEachInfo->info))->receiverId) ==((relationInfo_t*)pRelationInfo)->receiverId)&&
       (((relationInfo_t*)(pEachInfo->info))->receiverFunc ==((relationInfo_t*)pRelationInfo)->receiverFunc))
    {
        XFREE((pEachInfo->info));
        xListInfoDel(pEachInfo);
        return true;
    }

    return false;
}

bool xDisconnect(uint16_t senderId,uint8_t signal,uint16_t receiverId,connectFunc func)
{
    relationInfo_t info;

    info.senderId =senderId;
    info.signalType=signal;
    info.receiverId=receiverId;
    info.receiverFunc=func;

    return xListInfoPrevTraverse(&listConnect,&info,_disconnect);
}

static bool _delConnect(xListNode* pEachInfo,void* pRelationInfo)
{
    if(((((relationInfo_t*)(pEachInfo->info))->senderId) ==((relationInfo_t*)pRelationInfo)->senderId)||
       ((((relationInfo_t*)(pEachInfo->info))->receiverId) ==((relationInfo_t*)pRelationInfo)->receiverId))
    {
        XFREE((pEachInfo->info));
        xListInfoDel(pEachInfo);
    }
    //为了遍历所有对象，必须返回false
    return false;
}

void xDeleteConnect(uint16_t nameId)
{
    relationInfo_t info;

    info.senderId =nameId;
    info.signalType=0;
    info.receiverId=nameId;
    info.receiverFunc=0;

    xListInfoPrevTraverse(&listConnect,&info,_delConnect);
}

void xConnectProcess(void)
{
    emitInfo_t emitInfo;
    xListNode *temp_pos,*safePos;
    relationInfo_t *pRelationInfo;
    xConnectInfo_t connectInfo;
    bool ignoreSignal=false;//忽略相同信号的其他操作
    if(xQueueGetLength(emitQueue)>0)
    {
        if(xQueueDequeue(emitQueue,&emitInfo,sizeof (emitInfo)))
        {
            //此处轮训 连接表 所有数据
            //正向开始遍历list_for_each_safe(temp_pos,safePos, pList)
            list_for_each_safe(temp_pos,safePos, &listConnect)
            {
                    pRelationInfo=(relationInfo_t*)temp_pos->info;

                    if((pRelationInfo->senderId==emitInfo.senderId)&&(pRelationInfo->signalType==emitInfo.signalType))
                    {
                        connectInfo.senderId=pRelationInfo->senderId;
                        connectInfo.signalType=pRelationInfo->signalType;
                        connectInfo.receiverId=pRelationInfo->receiverId;
                        connectInfo.value=emitInfo.value;
                        ignoreSignal=pRelationInfo->receiverFunc(connectInfo);

                        if(ignoreSignal==true)
                        {
                            break;
                        }
                    }
            }
        }
    }
}


