#ifndef _VIRTUAL_NOR_H_
#define _VIRTUAL_NOR_H_

#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>

#define VIRTUAL_NOR_LOG     printf

void norSetBin(char *path);

void norRead(uint8_t *buf,uint32_t addr,uint16_t length);

void norWrite(uint8_t *buf,uint32_t addr,uint16_t length);

void norInit(uint32_t maxSize);

void norWritePage(uint8_t *buf,uint32_t addr,uint16_t length);

void norErasePage(uint16_t pageNum);

void norReadWrite_fast(FILE *fp_fast,uint8_t *path,bool isWrite,uint8_t *buf, uint32_t addr, uint16_t length);

#endif //_VIRTUAL_NOR_H_
