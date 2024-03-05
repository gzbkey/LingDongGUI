#ifndef _VIRTUAL_NOR_H_
#define _VIRTUAL_NOR_H_

#include "stdint.h"

#define VIRTUAL_NOR_LOG     printf

void norSetBin(char *path);

void norRead(uint8_t *buf,uint32_t addr,uint16_t length);

void norWrite(uint8_t *buf,uint32_t addr,uint16_t length);

void norInit(uint32_t maxSize);

void norWritePage(uint8_t *buf,uint32_t addr,uint16_t length);

void norErasePage(uint16_t pageNum);

#endif //_VIRTUAL_NOR_H_
