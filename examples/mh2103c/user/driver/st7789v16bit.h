#ifndef _ST7789V_16BIT_H_
#define _ST7789V_16BIT_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "mh210x.h"
#include "stdint.h"
#include "perf_counter.h"

#define ST7789V_RCC              RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); \
                                 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE); \
                                 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE)

#define ST7789V_RST_PORT           GPIOC
#define ST7789V_RST_PIN            GPIO_Pin_15

#define ST7789V_RS_PORT           GPIOA
#define ST7789V_RS_PIN            GPIO_Pin_3

#define ST7789V_CS_PORT           GPIOA
#define ST7789V_CS_PIN            GPIO_Pin_4

#define ST7789V_WR_PORT           GPIOC
#define ST7789V_WR_PIN            GPIO_Pin_14

#define ST7789V_RD_PORT           GPIOA
#define ST7789V_RD_PIN            GPIO_Pin_1

#define ST7789V_DATA_PORT         GPIOB

#define ST7789V_BG_PORT           GPIOA
#define ST7789V_BG_PIN            GPIO_Pin_0

#if 1
#define ST7789V_CS_L             ST7789V_CS_PORT->BRR = ST7789V_CS_PIN
#define ST7789V_RS_L             ST7789V_RS_PORT->BRR = ST7789V_RS_PIN
#define ST7789V_RD_L             ST7789V_RD_PORT->BRR = ST7789V_RD_PIN
#define ST7789V_WR_L             ST7789V_WR_PORT->BRR = ST7789V_WR_PIN
#define ST7789V_RST_L            ST7789V_RST_PORT->BRR = ST7789V_RST_PIN

#define ST7789V_CS_H             ST7789V_CS_PORT->BSRR = ST7789V_CS_PIN
#define ST7789V_RS_H             ST7789V_RS_PORT->BSRR = ST7789V_RS_PIN
#define ST7789V_RD_H             ST7789V_RD_PORT->BSRR = ST7789V_RD_PIN
#define ST7789V_WR_H             ST7789V_WR_PORT->BSRR = ST7789V_WR_PIN
#define ST7789V_RST_H            ST7789V_RST_PORT->BSRR = ST7789V_RST_PIN

#define ST7789V_DATA_W(dat)      ST7789V_DATA_PORT->ODR=(dat)
#define ST7789V_DATA_R           ST7789V_DATA_PORT->IDR
#else

#define ST7789V_CS_L             GPIO_ResetBits(ST7789V_CS_PORT,ST7789V_CS_PIN)
#define ST7789V_RS_L             GPIO_ResetBits(ST7789V_RS_PORT,ST7789V_RS_PIN)
#define ST7789V_RD_L             GPIO_ResetBits(ST7789V_RD_PORT,ST7789V_RD_PIN)
#define ST7789V_WR_L             GPIO_ResetBits(ST7789V_WR_PORT,ST7789V_WR_PIN)
#define ST7789V_RST_L            GPIO_ResetBits(ST7789V_RST_PORT,ST7789V_RST_PIN)

#define ST7789V_CS_H             GPIO_SetBits(ST7789V_CS_PORT,ST7789V_CS_PIN)
#define ST7789V_RS_H             GPIO_SetBits(ST7789V_RS_PORT,ST7789V_RS_PIN)
#define ST7789V_RD_H             GPIO_SetBits(ST7789V_RD_PORT,ST7789V_RD_PIN)
#define ST7789V_WR_H             GPIO_SetBits(ST7789V_WR_PORT,ST7789V_WR_PIN)
#define ST7789V_RST_H            GPIO_SetBits(ST7789V_RST_PORT,ST7789V_RST_PIN)

#define ST7789V_DATA_W(dat)      GPIO_Write(ST7789V_DATA_PORT,dat)
//#define ST7789V_DATA_R           GPIO_ReadInputData(ST7789V_DATA_PORT)

#endif

#define ST7789V_BG_H             GPIO_SetBits(ST7789V_BG_PORT, ST7789V_BG_PIN)
#define ST7789V_BG_L             GPIO_ResetBits(ST7789V_BG_PORT, ST7789V_BG_PIN)

#define ST7789V_BG_ON            ST7789V_BG_H
#define ST7789V_BG_OFF           ST7789V_BG_L


#define ST7789V_ANGLE             90
#define ST7789V_LCD_WIDTH         320
#define ST7789V_LCD_HEIGHT        240


extern uint16_t gLcdHeight, gLcdWidth;


void st7789v_init(void);
void st7789v_clear(uint16_t color);

void st7789v_colorFill(uint16_t sx,uint16_t sy,uint16_t ex,uint16_t ey,uint16_t *color);
void st7789v_setDir(uint16_t angle);


#define ST7789V_DELAY_MS(t)    delay_ms(t)

#ifdef __cplusplus
}
#endif

#endif //_ST7789V_16BIT_H_
