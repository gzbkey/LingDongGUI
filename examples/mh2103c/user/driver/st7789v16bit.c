#include "st7789v16bit.h"


uint16_t gLcdHeight, gLcdWidth;


void st7789v_gpioInit(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    
    GPIO_InitStructure.GPIO_Pin = ST7789V_RST_PIN;
    GPIO_Init(ST7789V_RST_PORT, &GPIO_InitStructure);
    
    GPIO_InitStructure.GPIO_Pin = ST7789V_RS_PIN;
    GPIO_Init(ST7789V_RS_PORT, &GPIO_InitStructure);
    
    GPIO_InitStructure.GPIO_Pin = ST7789V_CS_PIN;
    GPIO_Init(ST7789V_CS_PORT, &GPIO_InitStructure);
    
    GPIO_InitStructure.GPIO_Pin = ST7789V_WR_PIN;
    GPIO_Init(ST7789V_WR_PORT, &GPIO_InitStructure);
    
    GPIO_InitStructure.GPIO_Pin = ST7789V_RD_PIN;
    GPIO_Init(ST7789V_RD_PORT, &GPIO_InitStructure);
    
    GPIO_InitStructure.GPIO_Pin = ST7789V_BG_PIN;
    GPIO_Init(ST7789V_BG_PORT, &GPIO_InitStructure);
    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_All;
    GPIO_Init(ST7789V_DATA_PORT, &GPIO_InitStructure);

    ST7789V_BG_OFF;
}



/* 写命令：根据时序写 */
#if 0
void st7789v_writeCmd(uint16_t cmd)
{
    ST7789V_CS_L;
    ST7789V_RS_L;
    ST7789V_DATA_W(cmd);
    ST7789V_WR_L;
    ST7789V_WR_H;
    ST7789V_CS_H;
}
#else
#define st7789v_writeCmd(cmd) ST7789V_CS_L;ST7789V_RS_L;ST7789V_DATA_W(cmd);ST7789V_WR_L;ST7789V_WR_H;ST7789V_CS_H
#endif

/* 写数据：根据时序写 */
#if 0
void st7789v_writeData(uint16_t data)
{
    ST7789V_CS_L;
    ST7789V_RS_H;
    ST7789V_DATA_W(data);
    ST7789V_WR_L;
    ST7789V_WR_H;
    ST7789V_CS_H;
}
#else
#define st7789v_writeData(data)   ST7789V_CS_L;ST7789V_RS_H;ST7789V_DATA_W(data);ST7789V_WR_L;ST7789V_WR_H;ST7789V_CS_H
#endif

void st7789v_setWindow(uint16_t Xstart,uint16_t Xend,uint16_t Ystart,uint16_t Yend)
{
    st7789v_writeCmd(0x2A);
    st7789v_writeData(Xstart>>8);
    st7789v_writeData(Xstart&0XFF);
    st7789v_writeData(Xend>>8);
    st7789v_writeData(Xend&0XFF);
    st7789v_writeCmd(0x2B);
    st7789v_writeData(Ystart>>8);
    st7789v_writeData(Ystart&0XFF);
    st7789v_writeData(Yend>>8);
    st7789v_writeData(Yend&0XFF);
}

//清屏函数
//color:要清屏的填充色
void st7789v_clear(uint16_t color)
{
    uint32_t i,n;
    st7789v_setWindow(0,gLcdWidth-1,0,gLcdHeight-1);
    st7789v_writeCmd(0x2C);

    ST7789V_CS_L;
    ST7789V_RS_H;
    ST7789V_RD_H;

	n = (gLcdHeight * gLcdWidth);

    ST7789V_DATA_W(color);
    for (i = 0; i < n; i++)
    {
	    ST7789V_WR_L;
        ST7789V_WR_H;
    }
    ST7789V_CS_H;
}

//设置光标位置
//Xpos:横坐标
//Ypos:纵坐标
void st7789v_setCursor(uint16_t Xpos, uint16_t Ypos)
{
    st7789v_writeCmd(0x2A);
    st7789v_writeData(Xpos>>8);
    st7789v_writeData(Xpos&0XFF);
    st7789v_writeData(Xpos>>8);
    st7789v_writeData(Xpos&0XFF);
    st7789v_writeCmd(0x2B);
    st7789v_writeData(Ypos>>8);
    st7789v_writeData(Ypos&0XFF);
    st7789v_writeData(Ypos>>8);
    st7789v_writeData(Ypos&0XFF);
}



#define TFT_MAD_MY  0x80
#define TFT_MAD_MX  0x40
#define TFT_MAD_MV  0x20
#define TFT_MAD_ML  0x10
#define TFT_MAD_RGB 0x00
#define TFT_MAD_BGR 0x08
#define TFT_MAD_MH  0x04
#define TFT_MAD_SS  0x02
#define TFT_MAD_GS  0x01

void st7789v_setDir(uint16_t angle)//0 90 180 270
{
	st7789v_writeCmd(0x36);
	
    switch(angle)
    {
        default:
        case 0:
        {
            st7789v_writeData(0);
            if(ST7789V_LCD_WIDTH>ST7789V_LCD_HEIGHT)
            {
                gLcdWidth = ST7789V_LCD_HEIGHT;
                gLcdHeight = ST7789V_LCD_WIDTH;
            }
            else
            {
                gLcdWidth = ST7789V_LCD_WIDTH;
                gLcdHeight = ST7789V_LCD_HEIGHT;
            }
            
            break;
        }
        case 90:
        {
            st7789v_writeData(TFT_MAD_MY|TFT_MAD_MV);
            if(ST7789V_LCD_WIDTH>ST7789V_LCD_HEIGHT)
            {
                gLcdWidth = ST7789V_LCD_WIDTH;
                gLcdHeight = ST7789V_LCD_HEIGHT;
            }
            else
            {
                gLcdWidth = ST7789V_LCD_HEIGHT;
                gLcdHeight = ST7789V_LCD_WIDTH;
            }
            break;
        }
        case 180:
        {
            st7789v_writeData(TFT_MAD_MX |TFT_MAD_MY);
            if(ST7789V_LCD_WIDTH>ST7789V_LCD_HEIGHT)
            {
                gLcdWidth = ST7789V_LCD_HEIGHT;
                gLcdHeight = ST7789V_LCD_WIDTH;
            }
            else
            {
                gLcdWidth = ST7789V_LCD_WIDTH;
                gLcdHeight = ST7789V_LCD_HEIGHT;
            }
            break;
        }
        case 270:
        {
            st7789v_writeData(TFT_MAD_MX  | TFT_MAD_MV );
            if(ST7789V_LCD_WIDTH>ST7789V_LCD_HEIGHT)
            {
                gLcdWidth = ST7789V_LCD_WIDTH;
                gLcdHeight = ST7789V_LCD_HEIGHT;
            }
            else
            {
                gLcdWidth = ST7789V_LCD_HEIGHT;
                gLcdHeight = ST7789V_LCD_WIDTH;
            }
            break;
        }   
    }
}

void st7789v_init(void)
{
    ST7789V_RCC;
    st7789v_gpioInit();
    ST7789V_CS_H;
    ST7789V_RS_H;
    ST7789V_WR_H;
    ST7789V_RD_H;
    ST7789V_DATA_W(0);

    ST7789V_RST_H;
    ST7789V_DELAY_MS(20);
    ST7789V_RST_L;
    ST7789V_DELAY_MS(20);
    ST7789V_RST_H;
    ST7789V_DELAY_MS(120);

    //---------------------------------------------------------------------------------------------------//
	st7789v_writeCmd(0x11);
	ST7789V_DELAY_MS(120);
    
    //------------------------------display and color format setting--------------------------------//
    st7789v_writeCmd(0x36);
    st7789v_writeData(0x00);
    st7789v_writeCmd(0x3a);
    st7789v_writeData(0x05);
    //--------------------------------ST7789V Frame rate setting----------------------------------//
    st7789v_writeCmd(0xb2);
    st7789v_writeData(0x0c);
    st7789v_writeData(0x0c);
    st7789v_writeData(0x00);
    st7789v_writeData(0x33);
    st7789v_writeData(0x33);
    st7789v_writeCmd(0xb7);
    st7789v_writeData(0x35);
    //---------------------------------ST7789V Power setting--------------------------------------//
    st7789v_writeCmd(0xbb);
    st7789v_writeData(0x1c);
    st7789v_writeCmd(0xc0);
    st7789v_writeData(0x2c);
    st7789v_writeCmd(0xc2);
    st7789v_writeData(0x01);
    st7789v_writeCmd(0xc3);
    st7789v_writeData(0x0b);
    st7789v_writeCmd(0xc4);
    st7789v_writeData(0x20);
    st7789v_writeCmd(0xc6);
    st7789v_writeData(0x0f);
    st7789v_writeCmd(0xd0);
    st7789v_writeData(0xa4);
    st7789v_writeData(0xa1);
    
    st7789v_setDir(ST7789V_ANGLE);
    //--------------------------------ST7789V gamma setting---------------------------------------//
    st7789v_writeCmd(0xe0);
    st7789v_writeData(0xd0);
    st7789v_writeData(0x00);
    st7789v_writeData(0x03);
    st7789v_writeData(0x09);
    st7789v_writeData(0x13);
    st7789v_writeData(0x1c);
    st7789v_writeData(0x3a);
    st7789v_writeData(0x55);
    st7789v_writeData(0x48);
    st7789v_writeData(0x18);
    st7789v_writeData(0x12);
    st7789v_writeData(0x0e);
    st7789v_writeData(0x19);
    st7789v_writeData(0x1e);
    st7789v_writeCmd(0xe1);
    st7789v_writeData(0xd0);
    st7789v_writeData(0x00);
    st7789v_writeData(0x03);
    st7789v_writeData(0x09);
    st7789v_writeData(0x05);
    st7789v_writeData(0x25);
    st7789v_writeData(0x3a);
    st7789v_writeData(0x55);
    st7789v_writeData(0x50);
    st7789v_writeData(0x3d);
    st7789v_writeData(0x1c);
    st7789v_writeData(0x1d);
    st7789v_writeData(0x1d);
    st7789v_writeData(0x1e);
    st7789v_writeCmd(0x29);

    ST7789V_BG_ON;
}

//在指定区域内填充指定颜色块
//(sx,sy),(ex,ey):填充矩形对角坐标,区域大小为:(ex-sx+1)*(ey-sy+1)
//color:要填充的颜色
void st7789v_colorFill(uint16_t sx,uint16_t sy,uint16_t ex,uint16_t ey,uint16_t *color)
{
    uint32_t count;
    count=(ex-sx+1)*(ey-sy+1);
    st7789v_setWindow(sx,ex,sy,ey);
    st7789v_writeCmd(0x2C);
    
    ST7789V_CS_L;
    ST7789V_RD_H;
    while(count--)
    {
        ST7789V_RS_H;
	
        ST7789V_DATA_W(*color++);
        ST7789V_WR_L;
        ST7789V_WR_H;
    }
    ST7789V_CS_H;
}

