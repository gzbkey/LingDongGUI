#include "spi.h"

static void spiGpioInit(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
#if USE_SPI1 == 1
    GPIO_InitStructure.GPIO_Pin = SPI1_SCK_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(SPI1_SCK_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin =SPI1_MISO_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(SPI1_MISO_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin =SPI1_MOSI_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(SPI1_MOSI_PORT, &GPIO_InitStructure);
    
    GPIO_SetBits(SPI1_SCK_PORT,SPI1_SCK_PIN);
    GPIO_SetBits(SPI1_MISO_PORT,SPI1_MISO_PIN);
    GPIO_SetBits(SPI1_MOSI_PORT,SPI1_MOSI_PIN);
#endif

#if USE_SPI2 == 1
    GPIO_InitStructure.GPIO_Pin = SPI2_SCK_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(SPI2_SCK_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin =SPI2_MISO_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(SPI2_MISO_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin =SPI2_MOSI_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(SPI2_MOSI_PORT, &GPIO_InitStructure);
#endif
}

#if USE_SPI1_DMA == 1
static uint8_t  dmaByte=0xFF;
#endif

static void spiDmaInit(void)
{
    DMA_InitTypeDef  DMA_InitStructure;
#if USE_SPI1_DMA == 1
    
    //配置DMA通道,DMA1_CH2收

    //读取SPI FLASH时多数为空数据故而数据地址无需增加
    DMA_DeInit(DMA1_Channel2);
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)(&SPI1->DR);
    DMA_InitStructure.DMA_MemoryBaseAddr = 0;
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
    DMA_InitStructure.DMA_BufferSize = 0;
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
    DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
    DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
    DMA_Init(DMA1_Channel2, &DMA_InitStructure);

    SPI_I2S_DMACmd(SPI1,SPI_I2S_DMAReq_Rx,ENABLE);                                                               
	DMA_Cmd(DMA1_Channel2, DISABLE);

 

    //配置DMA通道,DMA1_CH3发送

    DMA_DeInit(DMA1_Channel3);
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)(&SPI1->DR);
    DMA_InitStructure.DMA_MemoryBaseAddr = 0;
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;
    DMA_InitStructure.DMA_BufferSize = 0;
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
    DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
    DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
    DMA_Init(DMA1_Channel3, &DMA_InitStructure);

    SPI_I2S_DMACmd(SPI1,SPI_I2S_DMAReq_Tx,ENABLE);                                                         
	DMA_Cmd(DMA1_Channel3, DISABLE);


    DMA_ClearFlag(DMA1_FLAG_GL3|DMA1_FLAG_TC3|DMA1_FLAG_HT3|DMA1_FLAG_TE3);
    DMA_ClearFlag(DMA1_FLAG_GL2|DMA1_FLAG_TC2|DMA1_FLAG_HT2|DMA1_FLAG_TE2);
//    DMA_ITConfig(DMA1_Channel2,DMA_IT_TC,ENABLE);
    
    uint8_t temp=0xFF;
	spiDmaReadWrite(SPI1,&temp, &dmaByte, 1);
    
#endif
}

static void spiSpiInit(void)
{
    SPI_InitTypeDef SPI_InitStructure;
#if USE_SPI1 == 1
    SPI_InitStructure.SPI_Direction=SPI_Direction_2Lines_FullDuplex;
    SPI_InitStructure.SPI_Mode=SPI_Mode_Master;
    SPI_InitStructure.SPI_DataSize=SPI_DataSize_8b;
    SPI_InitStructure.SPI_CPOL=SPI_CPOL_High;//SPI_CPOL_Low;
    SPI_InitStructure.SPI_CPHA=SPI_CPHA_2Edge;//SPI_CPHA_1Edge;
    SPI_InitStructure.SPI_NSS=SPI_NSS_Soft;
    SPI_InitStructure.SPI_BaudRatePrescaler=SPI_BaudRatePrescaler_256;
    SPI_InitStructure.SPI_FirstBit=SPI_FirstBit_MSB;
    SPI_InitStructure.SPI_CRCPolynomial=7;

    SPI_Init(SPI1,&SPI_InitStructure);
    SPI_Cmd(SPI1,ENABLE);
//    spiReadWrite(SPI1,0xff);//启动传输
#endif

#if USE_SPI2 == 1
    SPI_InitStructure.SPI_Direction=SPI_Direction_2Lines_FullDuplex;
    SPI_InitStructure.SPI_Mode=SPI_Mode_Master;
    SPI_InitStructure.SPI_DataSize=SPI_DataSize_8b;
    SPI_InitStructure.SPI_CPOL=SPI_CPOL_High;//SPI_CPOL_Low;
    SPI_InitStructure.SPI_CPHA=SPI_CPHA_2Edge;//SPI_CPHA_1Edge;
    SPI_InitStructure.SPI_NSS=SPI_NSS_Soft;
    SPI_InitStructure.SPI_BaudRatePrescaler=SPI_BaudRatePrescaler_256;
    SPI_InitStructure.SPI_FirstBit=SPI_FirstBit_MSB;
    SPI_InitStructure.SPI_CRCPolynomial=7;

    SPI_Init(SPI2,&SPI_InitStructure);
    SPI_Cmd(SPI2,ENABLE);
    spiReadWrite(SPI2,0xff);//启动传输
#endif
}

void spiInit(void)
{
#if USE_SPI1 == 1
    SPI1_RCC;
#endif
#if USE_SPI2 == 1
    SPI2_RCC;
#endif
#if USE_SPI1_DMA == 1 || USE_SPI2_DMA ==1
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
#endif
    spiGpioInit();
    spiSpiInit();
    spiDmaInit();
}


uint8_t	spiReadWrite(SPI_TypeDef* SPIx,uint8_t writeData)
{
    uint8_t retry=0;

    /* Loop while DR register in not emplty */
    while(SPI_I2S_GetFlagStatus(SPIx,SPI_I2S_FLAG_TXE) == RESET)
    {
        retry++;
        if(retry>200)return 0;
    }
    retry=0;
    /* Send byte through the SPI2 peripheral */
    SPI_I2S_SendData(SPIx, writeData);

    /* Wait to receive a byte */
    while(SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_RXNE) == RESET)
    {
        retry++;
        if(retry>200)return 0;
    }
    /* Return the byte read from the SPI bus */
    return SPI_I2S_ReceiveData(SPIx);
}

//SPI 速度设置函数
//speedSet:
//SPI_BaudRatePrescaler_2   2分频   (SPI 36M@sys 72M)
//SPI_BaudRatePrescaler_8   8分频   (SPI 9M@sys 72M)
//SPI_BaudRatePrescaler_16  16分频  (SPI 4.5M@sys 72M)
//SPI_BaudRatePrescaler_256 256分频 (SPI 281.25K@sys 72M)
void spiSetSpeed(SPI_TypeDef* SPIx,uint8_t speedSet)
{
    SPIx->CR1&=~0x38;
    SPIx->CR1|=speedSet;
}

void spiDmaWrite(SPI_TypeDef* SPIx,uint8_t *pWriteData,uint16_t length)
{
#if USE_SPI1_DMA == 1
    if(SPIx==SPI1)
    {
//	DMA_Cmd(DMA1_Channel2,DISABLE);
//	DMA_Cmd(DMA1_Channel3,DISABLE);
	
	DMA_SetCurrDataCounter(DMA1_Channel2,length);
	DMA_SetCurrDataCounter(DMA1_Channel3,length);
	
//	DMA1_Channel2->CCR &= ~(1<<7);	
    DMA1_Channel2->CCR &= ~DMA_MemoryInc_Enable;
    DMA1_Channel3->CCR |= DMA_MemoryInc_Enable;
        
	DMA1_Channel2->CMAR =(u32)dmaByte;
	DMA1_Channel3->CMAR =(u32)pWriteData;
	
	SPI1->DR;
	
	while((SPI1->SR&1<<1)==0);
	
	DMA_Cmd(DMA1_Channel2,ENABLE);
	DMA_Cmd(DMA1_Channel3,ENABLE);
	
	while( DMA_GetFlagStatus(DMA1_FLAG_TC3) == RESET);
	while( DMA_GetFlagStatus(DMA1_FLAG_TC2) == RESET);
	
	DMA_Cmd(DMA1_Channel2,DISABLE);
	DMA_Cmd(DMA1_Channel3,DISABLE);
	
	DMA_ClearFlag(DMA1_FLAG_TC3);
	DMA_ClearFlag(DMA1_FLAG_TC2);
    }
#endif
}

void spiDmaRead(SPI_TypeDef* SPIx,uint8_t *pReadData,uint16_t length)
{
#if USE_SPI1_DMA == 1
    if(SPIx==SPI1)
    {
        dmaByte=0xFF;
    DMA_SetCurrDataCounter(DMA1_Channel2,length);
	DMA_SetCurrDataCounter(DMA1_Channel3,length);
		
//	DMA1_Channel3->CCR &= ~(1<<7);//0x00000080
    DMA1_Channel3->CCR &= ~DMA_MemoryInc_Enable;
    DMA1_Channel2->CCR |= DMA_MemoryInc_Enable;
        
	DMA1_Channel2->CMAR =(u32)pReadData;
	DMA1_Channel3->CMAR =(u32)&dmaByte;
	
	SPI1->DR;
	
	while((SPI1->SR&1<<1)==0);
	
	
    SPI_I2S_DMACmd (SPIx, SPI_I2S_DMAReq_Tx, ENABLE);
    SPI_I2S_DMACmd (SPIx, SPI_I2S_DMAReq_Rx, ENABLE);
    DMA_Cmd(DMA1_Channel2,ENABLE);
	DMA_Cmd(DMA1_Channel3,ENABLE);
        
	while( DMA_GetFlagStatus(DMA1_FLAG_TC3) == RESET);
	while( DMA_GetFlagStatus(DMA1_FLAG_TC2) == RESET);
	
    SPI_I2S_DMACmd (SPIx, SPI_I2S_DMAReq_Tx, DISABLE);
    SPI_I2S_DMACmd (SPIx, SPI_I2S_DMAReq_Rx, DISABLE);
	DMA_Cmd(DMA1_Channel2,DISABLE);
	DMA_Cmd(DMA1_Channel3,DISABLE);
	
	DMA_ClearFlag(DMA1_FLAG_TC3);
	DMA_ClearFlag(DMA1_FLAG_TC2);
    }
#endif
}

void spiDmaReadWrite(SPI_TypeDef* SPIx,u8 *rx_buf,u8 *tx_buf,u16 NumByte)
{
    #if USE_SPI1_DMA == 1
    if(SPIx==SPI1)
    {
//	DMA_Cmd(DMA1_Channel2,DISABLE);		//关闭DMA
//	DMA_Cmd(DMA1_Channel3,DISABLE);
	
	DMA_SetCurrDataCounter(DMA1_Channel2,NumByte);		//设定通道内存宽度
	DMA_SetCurrDataCounter(DMA1_Channel3,NumByte);
	
//	DMA1_Channel2->CCR |= (1<<7);			//打开地址自增
//	DMA1_Channel3->CCR |= (1<<7);
	DMA1_Channel3->CCR |= DMA_MemoryInc_Enable;
    DMA1_Channel2->CCR |= DMA_MemoryInc_Enable;
        
	DMA1_Channel2->CMAR =(u32)rx_buf;
	DMA1_Channel3->CMAR =(u32)tx_buf;
	
	SPI1->DR;		//清空SPI DR寄存器
	
	while((SPI1->SR&1<<1)==0);	//等待清除
	
	
    SPI_I2S_DMACmd (SPIx, SPI_I2S_DMAReq_Tx, ENABLE);
    SPI_I2S_DMACmd (SPIx, SPI_I2S_DMAReq_Rx, ENABLE);
    DMA_Cmd(DMA1_Channel2,ENABLE);	//开启DMA
	DMA_Cmd(DMA1_Channel3,ENABLE);
	
	while( DMA_GetFlagStatus(DMA1_FLAG_TC3) == RESET);	//等待传输完成
	while( DMA_GetFlagStatus(DMA1_FLAG_TC2) == RESET);
	
    SPI_I2S_DMACmd (SPIx, SPI_I2S_DMAReq_Tx, DISABLE);
    SPI_I2S_DMACmd (SPIx, SPI_I2S_DMAReq_Rx, DISABLE);
	DMA_Cmd(DMA1_Channel2,DISABLE);		//关闭DMA
	DMA_Cmd(DMA1_Channel3,DISABLE);
	
	DMA_ClearFlag(DMA1_FLAG_TC3);		//清空传输完成flag
	DMA_ClearFlag(DMA1_FLAG_TC2);
    }
    #endif
}





