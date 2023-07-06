#include "xConnect.h"
#include "xQueue.h"
#include "xList.h"
#include "ldCommon.h"


xQueue_t *emitQueue=NULL;
NEW_LIST(listConnect);

bool xEmitInit(void)
{
    emitQueue=xQueueCreate(EMIT_QUEUE_SIZE,sizeof (emitInfo_t));
    if(emitQueue)
    {
        return true;
    }
    return false;
}

bool xEmit(uint16_t senderId,uint8_t signal)
{
    emitInfo_t emitInfo;
    
    emitInfo.senderId=senderId;
    emitInfo.signalType=signal;
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

#define IS_CONNECT_SAME(pRelation)   xListInfoPrevTraverse(&listConnect,pRelation,_isConnectSame)

bool xConnect(uint16_t senderId,uint8_t siganl,uint16_t receiverId,connectFunc func)
{
    relationInfo_t* pRelation;
    
    pRelation=XMALLOC(sizeof(relationInfo_t));
    
    if(pRelation!=NULL)
    {
    pRelation->senderId=senderId;
    pRelation->receiverId=receiverId;
    pRelation->signalType=siganl;
    pRelation->receiverFunc=func;

    //确定是否重复添加链接
    if(IS_CONNECT_SAME(pRelation))
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

bool xDisconnect(uint16_t senderId,uint8_t siganl,uint16_t receiverId,connectFunc func)
{
    relationInfo_t info;

    info.senderId =senderId;
    info.signalType=siganl;
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
    connectInfo_t connectInfo;
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


