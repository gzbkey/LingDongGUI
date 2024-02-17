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
