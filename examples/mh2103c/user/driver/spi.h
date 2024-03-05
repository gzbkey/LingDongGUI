#ifndef _SPI_H_
#define _SPI_H_

#include "mh210x.h"
#include "stdbool.h"

#define USE_SPI1               1
#define USE_SPI2               0

#define USE_SPI1_DMA               1

#if USE_SPI1 == 1
#define SPI1_RCC               RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_SPI1, ENABLE)

#define SPI1_SCK_PIN           GPIO_Pin_5
#define SPI1_SCK_PORT          GPIOA

#define SPI1_MISO_PIN          GPIO_Pin_6
#define SPI1_MISO_PORT         GPIOA

#define SPI1_MOSI_PIN          GPIO_Pin_7
#define SPI1_MOSI_PORT         GPIOA
#endif

#if USE_SPI2 == 1
#define SPI2_RCC               RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE); \
                               RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2,ENABLE)

#define SPI2_SCK_PIN           GPIO_Pin_13
#define SPI2_SCK_PORT          GPIOB

#define SPI2_MISO_PIN          GPIO_Pin_14
#define SPI2_MISO_PORT         GPIOB

#define SPI2_MOSI_PIN          GPIO_Pin_15
#define SPI2_MOSI_PORT         GPIOB
#endif

void spiInit(void);
uint8_t	spiReadWrite(SPI_TypeDef* SPIx,uint8_t writeData);
void spiSetSpeed(SPI_TypeDef* SPIx,uint8_t speedSet);

void spiDmaWrite(SPI_TypeDef* SPIx,uint8_t *pWriteData,uint16_t length);
void spiDmaRead(SPI_TypeDef* SPIx,uint8_t *pReadData,uint16_t length);
void spiDmaReadWrite(SPI_TypeDef* SPIx,u8 *rx_buf,u8 *tx_buf,u16 NumByte);

#endif //_SPI_H_
