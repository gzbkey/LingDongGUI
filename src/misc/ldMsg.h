#ifndef __LD_MSG_H__
#define __LD_MSG_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "stdbool.h"
#include "stdint.h"
#include "xQueue.h"

typedef struct ldAssn ldAssn_t;
typedef struct ldMsg ldMsg_t;
typedef bool (*assnFunc)(void* ptScene,ldMsg_t msg);

struct ldMsg
{
    uint64_t value;
    void *ptSender;
    uint8_t signal;
};//message

struct ldAssn
{
    assnFunc pFunc;
    ldAssn_t* ptNext;
    uint8_t signal;
};//association

bool ldMsgInit(xQueue_t **pptQueue, uint8_t size);
bool ldMsgEmit(xQueue_t *ptQueue, void *ptSender, uint8_t signal, uint64_t value);
bool ldMsgConnect(void *ptSender, uint8_t signal, assnFunc pFunc);
void ldMsgDelConnect(void *ptSender);
void ldMsgProcess(void *ptScene);
#define connect(senderId,signal,func)      ldMsgConnect(ldBaseGetWidget(ptScene->ptNodeRoot,senderId),signal,func)

#define emit(senderId,signal,value)                   ldMsgEmit(ptScene->ptMsgQueue,ldBaseGetWidget(ptScene->ptNodeRoot,senderId),signal,value)

#ifdef __cplusplus
}
#endif

#endif
