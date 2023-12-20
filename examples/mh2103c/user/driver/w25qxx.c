#include "w25qxx.h"


#define FM25Q80A    0XA113
#define W25Q80      0XEF13
#define W25Q16      0XEF14
#define W25Q32      0XEF15
#define W25Q64      0XEF16
#define W25Q128     0XEF17

//指令表
#define W25X_WriteEnable		0x06
#define W25X_WriteDisable		0x04
#define W25X_ReadStatusReg1		0x05
#define W25X_ReadStatusReg2		0x35
#define W25X_ReadStatusReg3		0x15
#define W25X_WriteStatusReg1		0x01
#define W25X_WriteStatusReg2		0x31
#define W25X_WriteStatusReg3		0x11
#define W25X_ReadData			0x03
#define W25X_FastReadData		0x0B
#define W25X_FastReadDual		0x3B
#define W25X_PageProgram		0x02
#define W25X_BlockErase			0xD8
#define W25X_SectorErase		0x20
#define W25X_ChipErase			0xC7
#define W25X_PowerDown			0xB9
#define W25X_ReleasePowerDown	0xAB
#define W25X_DeviceID			0xAB
#define W25X_ManufactDeviceID	0x90
#define W25X_JedecDeviceID		0x9F
#define W25X_FastReadQuadOutput		0x6B

//--------------------------- Flash 状态寄存器配置------------------------//
#define W25X_STATUS_WIP_Pos		0		// Write In Progress
#define W25X_STATUS_WIP_Msk		(0x01 << W25X_STATUS_WIP_Pos)
#define W25X_STATUS_WEL_Pos		1		// Write Enable Latch
#define W25X_STATUS_WEL_Msk		(0x01 << W25X_STATUS_WEL_Pos)
#define W25X_STATUS_SBP_Pos		2		// Software Block Protect
#define W25X_STATUS_SBP_Msk		(0x1F << W25X_STATUS_SBP_Pos)
#define W25X_STATUS_SRP_Pos		7		// Status Register Protect
#define W25X_STATUS_SRP_Msk		(0x03 << W25X_STATUS_SRP_Pos)
#define W25X_STATUS_QE_Pos		9		// Quad Enable
#define W25X_STATUS_QE_Msk		(0x01 << W25X_STATUS_QE_Pos)
#define W25X_STATUS_SUS_Pos		15		// Erase/Program Suspend
#define W25X_STATUS_SUS_Msk		(0x01 << W25X_STATUS_SUS_Pos)

//4Kbytes为一个Sector
//16个扇区为1个Block
//W25Q128
//容量为16M字节,共有128个Block,4096个Sector


//读取W25QXX的状态寄存器
//BIT7  6   5   4   3   2   1   0
//SPR   RV  TB BP2 BP1 BP0 WEL BUSY
//SPR:默认0,状态寄存器保护位,配合WP使用
//TB,BP2,BP1,BP0:FLASH区域写保护设置
//WEL:写使能锁定
//BUSY:忙标记位(1,忙;0,空闲)
//默认:0x00
static uint8_t W25QXX_ReadSR1(void)
{
    uint8_t byte=0;
    W25QXX_CS_ON;
    spiReadWrite(W25QXX_SPI_X,W25X_ReadStatusReg1); //发送读取状态寄存器命令
    byte=spiReadWrite(W25QXX_SPI_X,0Xff);          //读取一个字节
    W25QXX_CS_OFF;
    return byte;
}

static uint8_t W25QXX_ReadSR2(void)
{
    uint8_t byte=0;
    W25QXX_CS_ON;
    spiReadWrite(W25QXX_SPI_X,W25X_ReadStatusReg2); //发送读取状态寄存器命令
    byte=spiReadWrite(W25QXX_SPI_X,0Xff);          //读取一个字节
    W25QXX_CS_OFF;
    return byte;
}

static uint32_t W25X_ReadStatus(void)
{
	uint32_t status = 0;

	//Status Register-1
	W25QXX_CS_ON;
	spiReadWrite(W25QXX_SPI_X, W25X_ReadStatusReg1); //05h
	status = spiReadWrite(W25QXX_SPI_X, 0Xff);	//发送 Dummy_Byte 获取寄存器值
	W25QXX_CS_OFF;
	//printf("read_status_1 = [0X%x]\r\n", status);

	//Status Register-2
	W25QXX_CS_ON;
	spiReadWrite(W25QXX_SPI_X, W25X_ReadStatusReg2); //35h
	status |= spiReadWrite(W25QXX_SPI_X, 0Xff) << 8;
	W25QXX_CS_OFF;
	//printf("read_status_2 = [0X%x]\r\n", status);

	/* Status Register-3 暂时未使用
	SPIFlash_CSn_Clr;
	spiReadWrite(SPI_PORT_W25X, W25X_ReadStatusReg_3);//FFh
	status |= spiReadWrite(SPI_PORT_W25X, Dummy_Byte) << 16;
	SPIFlash_CSn_Set;
	printf("read_status_3 = [0X%x]\r\n", status);
	*/
	return status;
}

//写W25QXX状态寄存器
//只有SPR,TB,BP2,BP1,BP0(bit 7,5,4,3,2)可以写!!!
static void W25QXX_Write_SR1(uint8_t sr)
{
	W25QXX_CS_ON;
	spiReadWrite(W25QXX_SPI_X,W25X_WriteStatusReg1);//发送写取状态寄存器命令
	spiReadWrite(W25QXX_SPI_X,sr);               	//写入一个字节
	W25QXX_CS_OFF;
}

static void W25QXX_Write_SR2(uint8_t sr)
{
	W25QXX_CS_ON;
	spiReadWrite(W25QXX_SPI_X,W25X_WriteStatusReg2);//发送写取状态寄存器命令
	spiReadWrite(W25QXX_SPI_X,sr);               	//写入一个字节
	W25QXX_CS_OFF;
}



//W25QXX写使能
//将WEL置位
static void W25QXX_Write_Enable(void)
{
    W25QXX_CS_ON;
    spiReadWrite(W25QXX_SPI_X,W25X_WriteEnable); 	//发送写使能
    W25QXX_CS_OFF;
}

////W25QXX写禁止
////将WEL清零
//static void W25QXX_Write_Disable(void)
//{
//	W25QXX_CS_ON;
//    spiReadWrite(W25QXX_SPI_X,W25X_WriteDisable);  //发送写禁止指令
//	W25QXX_CS_OFF;
//}
//等待空闲
static void W25QXX_Wait_Busy(void)
{
    while((W25QXX_ReadSR1()&W25X_STATUS_WIP_Msk)==0x01);  		// 等待BUSY位清空
}

void W25QXX_QuadEnable(bool isEnable)
{
    uint32_t status;
    
    W25QXX_Write_Enable();
    
	status = W25X_ReadStatus();
	
	if(isEnable)
    {
		status |= (1 << W25X_STATUS_QE_Pos);
    }
	else
    {
		status &= ~(1 << W25X_STATUS_QE_Pos);
    }
    W25QXX_CS_ON;
	spiReadWrite(W25QXX_SPI_X, W25X_WriteStatusReg1);
	spiReadWrite(W25QXX_SPI_X, (status >> 0) & 0xFF); //Status Register-1 <=> status 低 8 bit
	spiReadWrite(W25QXX_SPI_X, (status >> 8) & 0xFF); //Status Register-2 <=> status 中 8 bit
    W25QXX_CS_OFF;
    
    W25QXX_Wait_Busy();
}

//擦除整个芯片
//等待时间超长...
void w25qxxEraseChip(void)
{
    W25QXX_SET_SPEED_MAX;
    W25QXX_Write_Enable();                 	 	//SET WEL
    W25QXX_Wait_Busy();
    W25QXX_CS_ON;
    spiReadWrite(W25QXX_SPI_X,W25X_ChipErase);        	//发送片擦除命令
    W25QXX_CS_OFF;
    W25QXX_Wait_Busy();   				   		//等待芯片擦除结束
}

//擦除一个扇区
//Dst_Addr:扇区地址 根据实际容量设置
//擦除一个扇区的最少时间:150ms
void w25qxxEraseSector(uint16_t sectorNum)
{
    uint32_t Dst_Addr;
    W25QXX_SET_SPEED_MAX;
    Dst_Addr=sectorNum*4096;
    W25QXX_Write_Enable();                  	//SET WEL
    W25QXX_Wait_Busy();
    W25QXX_CS_ON;
    spiReadWrite(W25QXX_SPI_X,W25X_SectorErase);      	//发送扇区擦除指令
    spiReadWrite(W25QXX_SPI_X,(uint8_t)((Dst_Addr)>>16));  	//发送24bit地址
    spiReadWrite(W25QXX_SPI_X,(uint8_t)((Dst_Addr)>>8));
    spiReadWrite(W25QXX_SPI_X,(uint8_t)Dst_Addr);
    W25QXX_CS_OFF;
    W25QXX_Wait_Busy();   				   		//等待擦除完成
}

//读取芯片ID
//返回值如下:
//0XA113,表示芯片型号为FM25Q80A
//0XEF13,表示芯片型号为W25Q80
//0XEF14,表示芯片型号为W25Q16
//0XEF15,表示芯片型号为W25Q32
//0XEF16,表示芯片型号为W25Q64
//0XEF17,表示芯片型号为W25Q128
uint16_t w25qxxReadID(void)
{
    uint16_t Temp = 0;
    W25QXX_SET_SPEED_MAX;
    W25QXX_CS_ON;
    spiReadWrite(W25QXX_SPI_X,0x90);//发送读取ID命令
    spiReadWrite(W25QXX_SPI_X,0x00);
    spiReadWrite(W25QXX_SPI_X,0x00);
    spiReadWrite(W25QXX_SPI_X,0x00);
    Temp|=spiReadWrite(W25QXX_SPI_X,0xFF)<<8;
    Temp|=spiReadWrite(W25QXX_SPI_X,0xFF);
    W25QXX_CS_OFF;
    return Temp;
}

uint8_t w25qxxReadDeviceId(void)
{
    uint8_t u8Temp = 0;
    W25QXX_SET_SPEED_MAX;
    W25QXX_CS_ON;
    spiReadWrite(W25QXX_SPI_X,0xAB);//发送读取ID命令
    spiReadWrite(W25QXX_SPI_X,0x00);
    spiReadWrite(W25QXX_SPI_X,0x00);
    spiReadWrite(W25QXX_SPI_X,0x00);
    u8Temp=spiReadWrite(W25QXX_SPI_X,0xFF);
    W25QXX_CS_OFF;
    return u8Temp;
}

uint16_t w25qxxGetSectorSize(void)
{
    uint8_t norDeviceId;
    
    norDeviceId= w25qxxReadDeviceId();
//0X13,表示芯片型号为FM25Q80A
//0X13,表示芯片型号为W25Q80
//0X14,表示芯片型号为W25Q16
//0X15,表示芯片型号为W25Q32
//0X16,表示芯片型号为W25Q64
//0X17,表示芯片型号为W25Q128
    switch(norDeviceId)
    {
        case 0x13:
        {
            //1MByte
            return 256;
        }
        case 0x14:
        {
            //2MByte
            return 512;
        }
        case 0x15:
        {
            //4MByte
            return 1024;
        }
        case 0x16:
        {
            //8MByte
            return 2048;
        }
        case 0x17:
        {
            //16MByte
            return 4096;
        }
        case 0x18:
        {
            //32MByte
            return 8192;
        }
        default:
            return 0;
    }
}



//读取SPI FLASH
//在指定地址开始读取指定长度的数据
//pBuffer:数据存储区
//ReadAddr:开始读取的地址(24bit)
//NumByteToRead:要读取的字节数(最大65535)
void w25qxxRead_1bit(uint8_t* pBuffer,uint32_t ReadAddr,uint16_t NumByteToRead)
{
    uint16_t i;
    W25QXX_SET_SPEED_MAX;
    W25QXX_CS_ON;
    spiReadWrite(W25QXX_SPI_X,W25X_ReadData);         	//发送读取命令
    spiReadWrite(W25QXX_SPI_X,(uint8_t)((ReadAddr)>>16));  	//发送24bit地址
    spiReadWrite(W25QXX_SPI_X,(uint8_t)((ReadAddr)>>8));
    spiReadWrite(W25QXX_SPI_X,(uint8_t)ReadAddr);
    for(i=0; i<NumByteToRead; i++)
    {
        pBuffer[i]=spiReadWrite(W25QXX_SPI_X,0XFF);   	//循环读数
    }
    W25QXX_CS_OFF;
}

void w25qxxReadFast_1bit(uint8_t* pBuffer,uint32_t ReadAddr,uint16_t NumByteToRead)
{
    uint16_t i;
    W25QXX_SET_SPEED_MAX;
    W25QXX_CS_ON;
    spiReadWrite(W25QXX_SPI_X,W25X_FastReadData);         	//发送读取命令
    spiReadWrite(W25QXX_SPI_X,(uint8_t)((ReadAddr)>>16));  	//发送24bit地址
    spiReadWrite(W25QXX_SPI_X,(uint8_t)((ReadAddr)>>8));
    spiReadWrite(W25QXX_SPI_X,(uint8_t)ReadAddr);
    spiReadWrite(W25QXX_SPI_X, 0xFF);

    while (NumByteToRead--) // 读取数据
	{
		*pBuffer = spiReadWrite(W25QXX_SPI_X, 0XFF); // 读取一个字节
		pBuffer++;											  // 指向下一个字节缓冲区
	}
    W25QXX_CS_OFF;
}

void w25qxxRead_1bitDma(uint8_t* pBuffer,uint32_t ReadAddr,uint16_t NumByteToRead)
{
    uint8_t cmdBuf[4];
    if (NumByteToRead > 0xFFFF)
    {
        return ;
    }
    W25QXX_SET_SPEED_MAX;
    W25QXX_CS_ON;
    cmdBuf[0]=W25X_ReadData;
    cmdBuf[1]=(ReadAddr >> 16) & 0xFF;
    cmdBuf[2]=(ReadAddr >> 8) & 0xFF;
    cmdBuf[3]=(ReadAddr >> 0) & 0xFF;
    spiDmaWrite(W25QXX_SPI_X,cmdBuf,4);
    spiDmaRead(W25QXX_SPI_X,pBuffer,NumByteToRead);
    W25QXX_CS_OFF;
}


//读取SPI FLASH
//在指定地址开始读取指定长度的数据
//pBuffer:数据存储区
//ReadAddr:开始读取的地址(24bit)
//NumByteToRead:要读取的字节数(最大65535)
void w25qxxRead(uint8_t* pBuffer,uint32_t ReadAddr,uint16_t NumByteToRead)
{
#if USE_W25QXX_DMA == 1
    w25qxxRead_1bitDma(pBuffer,ReadAddr,NumByteToRead);
#else
    w25qxxReadFast_1bit(pBuffer,ReadAddr,NumByteToRead);
#endif 
}

//SPI在一页(0~65535)内写入少于256个字节的数据
//在指定地址开始写入最大256字节的数据
//pBuffer:数据存储区
//WriteAddr:开始写入的地址(24bit)
//NumByteToWrite:要写入的字节数(最大256),该数不应该超过该页的剩余字节数!!!
void w25qxxWritePage(uint8_t* pBuffer,uint32_t WriteAddr,uint16_t NumByteToWrite)
{
    uint16_t i;
    uint8_t cmdBuf[4];
    W25QXX_SET_SPEED_MAX;
    W25QXX_Write_Enable();                  	//SET WEL
    W25QXX_CS_ON;
    
#if USE_W25QXX_DMA == 1
    cmdBuf[0]=W25X_PageProgram;
    cmdBuf[1]=(WriteAddr >> 16) & 0xFF;
    cmdBuf[2]=(WriteAddr >> 8) & 0xFF;
    cmdBuf[3]=(WriteAddr >> 0) & 0xFF;
    spiDmaWrite(W25QXX_SPI_X,cmdBuf,4);
    spiDmaWrite(W25QXX_SPI_X,pBuffer,NumByteToWrite);
#else
    spiReadWrite(W25QXX_SPI_X,W25X_PageProgram);      	//发送写页命令
    spiReadWrite(W25QXX_SPI_X,(uint8_t)((WriteAddr)>>16)); 	//发送24bit地址
    spiReadWrite(W25QXX_SPI_X,(uint8_t)((WriteAddr)>>8));
    spiReadWrite(W25QXX_SPI_X,(uint8_t)WriteAddr);
    for(i=0; i<NumByteToWrite; i++)spiReadWrite(W25QXX_SPI_X,pBuffer[i]); //循环写数
#endif
    W25QXX_CS_OFF;
    W25QXX_Wait_Busy();					   		//等待写入结束
}

void W25QXX_Write_NoCheck(uint8_t* pBuffer,uint32_t WriteAddr,uint16_t NumByteToWrite)
{
    uint16_t pageremain;
    pageremain=256-WriteAddr%256; //单页剩余的字节数
    if(NumByteToWrite<=pageremain)pageremain=NumByteToWrite;//不大于256个字节
    while(1)
    {
        w25qxxWritePage(pBuffer,WriteAddr,pageremain);
        if(NumByteToWrite==pageremain)break;//写入结束了
        else //NumByteToWrite>pageremain
        {
            pBuffer+=pageremain;
            WriteAddr+=pageremain;

            NumByteToWrite-=pageremain;			  //减去已经写入了的字节数
            if(NumByteToWrite>256)pageremain=256; //一次可以写入256个字节
            else pageremain=NumByteToWrite; 	  //不够256个字节了
        }
    };
}

uint64_t w25qxxReadUniqueId(void)
{
    uint64_t Temp = 0;
    W25QXX_CS_ON;
    spiReadWrite(W25QXX_SPI_X,0x4B);//发送读取UID命令	    
    spiReadWrite(W25QXX_SPI_X,0x00);
    spiReadWrite(W25QXX_SPI_X,0x00);
    spiReadWrite(W25QXX_SPI_X,0x00);
    spiReadWrite(W25QXX_SPI_X,0x00);
    Temp+=spiReadWrite(W25QXX_SPI_X,0xFF);
    Temp=Temp<<8;
    Temp+=spiReadWrite(W25QXX_SPI_X,0xFF);
    Temp=Temp<<8;
    Temp+=spiReadWrite(W25QXX_SPI_X,0xFF);
    Temp=Temp<<8;
    Temp+=spiReadWrite(W25QXX_SPI_X,0xFF);
    Temp=Temp<<8;
    Temp+=spiReadWrite(W25QXX_SPI_X,0xFF);
    Temp=Temp<<8;
    Temp+=spiReadWrite(W25QXX_SPI_X,0xFF);
    Temp=Temp<<8;
    Temp+=spiReadWrite(W25QXX_SPI_X,0xFF);
    Temp=Temp<<8;
    Temp+=spiReadWrite(W25QXX_SPI_X,0xFF);
    W25QXX_CS_OFF;
    return Temp;
}

void w25qxxGpioInit(void)
{
    GPIO_InitTypeDef GPIO_InitStruct;
    
    GPIO_InitStruct.GPIO_Pin = W25QXX_CS_PIN;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(W25QXX_CS_PORT, &GPIO_InitStruct);
    
    W25QXX_CS_OFF;
}

void w25qxxInit(void)
{
    W25QXX_CS_RCC;
    w25qxxGpioInit();
    spiInit();

}

void w25qxxEraseBlock64k(uint32_t addr)
{
    W25QXX_SET_SPEED_MAX;
    W25QXX_Write_Enable();                  	//SET WEL
    W25QXX_Wait_Busy();
    W25QXX_CS_ON;
    spiReadWrite(W25QXX_SPI_X,W25X_BlockErase);      	//发送扇区擦除指令
    spiReadWrite(W25QXX_SPI_X,(uint8_t)((addr)>>16));  	//发送24bit地址
    spiReadWrite(W25QXX_SPI_X,(uint8_t)((addr)>>8));
    spiReadWrite(W25QXX_SPI_X,(uint8_t)addr);
    W25QXX_CS_OFF;
    W25QXX_Wait_Busy();   				   		//等待擦除完成
}
