#ifndef _W25QXX_H_
#define _W25QXX_H_

#include "stdint.h"
#include "stdbool.h"
#include "spi.h"

#define USE_W25QXX_DMA           1

#define W25QXX_SPI_X             SPI1



#define W25QXX_CS_RCC            RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE)

#define W25QXX_CS_PIN            GPIO_Pin_8
#define W25QXX_CS_PORT           GPIOA

#define W25QXX_CS_H              GPIO_SetBits(W25QXX_CS_PORT, W25QXX_CS_PIN)
#define W25QXX_CS_L              GPIO_ResetBits(W25QXX_CS_PORT, W25QXX_CS_PIN)

#define W25QXX_CS_ON             W25QXX_CS_L
#define W25QXX_CS_OFF            W25QXX_CS_H

#define W25QXX_SET_SPEED_MAX     spiSetSpeed(W25QXX_SPI_X,SPI_BaudRatePrescaler_2)

void w25qxxInit(void);
uint16_t w25qxxReadID(void);
uint8_t w25qxxReadDeviceId(void);
uint64_t w25qxxReadUniqueId(void);
uint16_t w25qxxGetSectorSize(void);
void w25qxxEraseChip(void);
void w25qxxEraseSector(uint16_t sectorNum);
void w25qxxWritePage(uint8_t* pBuffer,uint32_t WriteAddr,uint16_t NumByteToWrite);

void w25qxxRead(uint8_t* pBuffer,uint32_t ReadAddr,uint16_t NumByteToRead);
//void w25qxxWrite(uint8_t* pBuffer,uint32_t WriteAddr,uint16_t NumByteToWrite);
void W25QXX_Write_NoCheck(uint8_t* pBuffer,uint32_t WriteAddr,uint16_t NumByteToWrite);

void w25qxxEraseBlock64k(uint32_t addr);

#endif //_W25QXX_H_
