#include "xConnect.h"
#include "xQueue.h"
#include "xList.h"
#include "ldCommon.h"


xQueue_t *emitQueue=NULL;
NEW_LIST(listConnect);

#define X_CONNECT_ADD(pInfo)    xListInfoAdd(&listConnect,pInfo)

static bool xEmitInit(void)
{
    emitQueue=xQueueCreate(EMIT_QUEUE_SIZE,sizeof (emitInfo_t));
    if(emitQueue)
    {
        return true;
    }
    return false;
}

bool xEmit(void *widget,uint8_t signal)
{
    emitInfo_t emitInfo;
    
    emitInfo.pSender=widget;
    emitInfo.signalType=signal;
    return xQueueEnqueue(emitQueue,&emitInfo,sizeof (emitInfo_t));
}

static bool _isConnectSame(xListNode* pEachInfo,void* pRelationInfo)
{
    if(((((relationInfo_t*)(pEachInfo->info))->pSender) ==((relationInfo_t*)pRelationInfo)->pSender)&&
       (((relationInfo_t*)(pEachInfo->info))->signalType ==((relationInfo_t*)pRelationInfo)->signalType)&&
       ((((relationInfo_t*)(pEachInfo->info))->pReceiver) ==((relationInfo_t*)pRelationInfo)->pReceiver)&&
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

    if(emitQueue==NULL)//自动申请信号储存空间
    {
        if(xEmitInit()==false)
        {
            return false;
        }
    }
    
    pRelation=XMALLOC(sizeof(relationInfo_t));
    
    if(pRelation!=NULL)
    {
        //根据ID查找widget
    pRelation->pSender=ldGetWidgetInfoById(senderId)->info;
    if(pRelation->pSender==NULL)
    {
        XFREE(pRelation);
        return false;
    }

    pRelation->pReceiver=ldGetWidgetInfoById(receiverId)->info;
    if(pRelation->pReceiver==NULL)
    {
        XFREE(pRelation);
        return false;
    }

    pRelation->signalType=siganl;
    pRelation->receiverFunc=func;

    //确定是否重复添加链接
    if(IS_CONNECT_SAME(pRelation))
    {
        XFREE(pRelation);
        return false;
    }
    
    //添加链表
    if(X_CONNECT_ADD(pRelation)==NULL)
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
    if(((((relationInfo_t*)(pEachInfo->info))->pSender) ==((relationInfo_t*)pRelationInfo)->pSender)&&
       (((relationInfo_t*)(pEachInfo->info))->signalType ==((relationInfo_t*)pRelationInfo)->signalType)&&
       ((((relationInfo_t*)(pEachInfo->info))->pReceiver) ==((relationInfo_t*)pRelationInfo)->pReceiver)&&
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

    info.pSender =ldGetWidgetInfoById(senderId)->info;
    info.signalType=siganl;
    info.pReceiver=ldGetWidgetInfoById(receiverId)->info;
    info.receiverFunc=func;

    return xListInfoPrevTraverse(&listConnect,&info,_disconnect);
}

static bool _delConnect(xListNode* pEachInfo,void* pRelationInfo)
{
    if(((((relationInfo_t*)(pEachInfo->info))->pSender) ==((relationInfo_t*)pRelationInfo)->pSender)||
       ((((relationInfo_t*)(pEachInfo->info))->pReceiver) ==((relationInfo_t*)pRelationInfo)->pReceiver))
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

    info.pSender =ldGetWidgetInfoById(nameId)->info;
    info.signalType=0;
    info.pReceiver=info.pSender;
    info.receiverFunc=0;

    xListInfoPrevTraverse(&listConnect,&info,_delConnect);
}





