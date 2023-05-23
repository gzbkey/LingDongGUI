#ifndef _CONNECT_H_
#define _CONNECT_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "stdint.h"
#include "stdbool.h"


typedef struct{
    void *pSender;
    uint8_t signalType;
}emitInfo_t;

typedef struct{
    void *pSender;
    uint8_t signalType;
    void *pReceiver;
}connectInfo_t;

typedef bool (*connectFunc)(connectInfo_t);

typedef struct{
    void *pSender;
    uint8_t signalType;
    void *pReceiver;
    connectFunc receiverFunc;
}relationInfo_t;

#define EMIT_QUEUE_SIZE                 (4)

bool xEmit(void *widget,uint8_t signal);
bool xConnect(uint16_t senderId,uint8_t siganl,uint16_t receiverId,connectFunc func);
bool xDisconnect(uint16_t senderId,uint8_t siganl,uint16_t receiverId,connectFunc func);
void xDeleteConnect(uint16_t nameId);

#ifdef __cplusplus
}
#endif

#endif //_CONNECT_H_
