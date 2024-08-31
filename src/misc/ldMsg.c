#include "ldMsg.h"
#include "xQueue.h"
#include "ldBase.h"
bool ldMsgInit(xQueue_t **pptQueue, uint8_t size)
{
    if (*pptQueue == NULL)
    {
        *pptQueue = xQueueCreate(size, sizeof(ldMsg_t));
        if (*pptQueue)
        {
            return true;
        }
    }
    return false;
}

bool ldMsgEmit(xQueue_t *ptQueue, void *ptSender, uint8_t signal, uint64_t value)
{
    ldMsg_t tMsg;

    tMsg.ptSender = ptSender;
    tMsg.signal = signal;
    tMsg.value = value;
    return xQueueEnqueue(ptQueue, &tMsg, sizeof(ldMsg_t));
}

bool ldMsgConnect(void *ptSender, uint8_t signal, assnFunc pFunc)
{
    ldAssn_t *ptAssn = NULL, *ptNext = NULL;

    ptNext = ((ldBase_t *)ptSender)->ptAssn;

    while (ptNext != NULL)
    {
        if ((ptNext->signal == signal) && (ptNext->pFunc == pFunc))
        {
            return false;
        }
        ptNext = ptNext->ptNext;
    }

    ptAssn = (ldAssn_t *)ldCalloc(1, sizeof(ldAssn_t));
    if (ptAssn != NULL)
    {
        ptAssn->pFunc = pFunc;
        ptAssn->signal = signal;
        ptAssn->ptNext = ((ldBase_t *)ptSender)->ptAssn; // 将新节点添加到链表头部
        ((ldBase_t *)ptSender)->ptAssn = ptAssn; // 更新链表头指针
        return true;
    }

    return false;
}

void ldMsgDelConnect(void *ptSender)
{
    ldAssn_t *ptAssn=((ldBase_t *)ptSender)->ptAssn;

    while (ptAssn != NULL)
    {
        ldFree(ptAssn);
        ptAssn = ptAssn->ptNext;
    }
}

void ldMsgProcess(void *ptScene)
{
    ldMsg_t tMsg;
    ldBase_t *ptWidget;
    ldAssn_t *ptNext = NULL;
//    bool isOk=0;
    while(xQueueDequeue(((ld_scene_t*)ptScene)->ptMsgQueue,&tMsg,sizeof (ldMsg_t)))
    {
        ptWidget=(ldBase_t*)tMsg.ptSender;
        ptNext=ptWidget->ptAssn;

//        do{
            while(ptNext!=NULL)
            {
                if(ptNext->signal==tMsg.signal)
                {
                    if(ptNext->pFunc(ptScene,tMsg))
                    {
//                        isOk=true;
                        break;
                    }
                }
                ptNext=ptNext->ptNext;
            }
//            if(isOk)
//            {
//                break;
//            }
//            else
//            {
//                ptWidget=ptWidget->
//            }
//        }
//        while(true);

    }
}
