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

#define ST7789V_RST_GPIO           GPIOC
#define ST7789V_RST_PIN            GPIO_Pin_15

#define ST7789V_RS_GPIO           GPIOA
#define ST7789V_RS_PIN            GPIO_Pin_4

#define ST7789V_CS_GPIO           GPIOC
#define ST7789V_CS_PIN            GPIO_Pin_13

#define ST7789V_WR_GPIO           GPIOC
#define ST7789V_WR_PIN            GPIO_Pin_14

#define ST7789V_RD_GPIO           GPIOA
#define ST7789V_RD_PIN            GPIO_Pin_3

#define ST7789V_DATA_GPIO         GPIOB

#define ST7789V_CS_L             ST7789V_CS_GPIO->BRR = ST7789V_CS_PIN
#define ST7789V_RS_L             ST7789V_RS_GPIO->BRR = ST7789V_RS_PIN
#define ST7789V_RD_L             ST7789V_RD_GPIO->BRR = ST7789V_RD_PIN
#define ST7789V_WR_L             ST7789V_WR_GPIO->BRR = ST7789V_WR_PIN
#define ST7789V_RST_L            ST7789V_RST_GPIO->BRR = ST7789V_RST_PIN

#define ST7789V_CS_H             ST7789V_CS_GPIO->BSRR = ST7789V_CS_PIN
#define ST7789V_RS_H             ST7789V_RS_GPIO->BSRR = ST7789V_RS_PIN
#define ST7789V_RD_H             ST7789V_RD_GPIO->BSRR = ST7789V_RD_PIN
#define ST7789V_WR_H             ST7789V_WR_GPIO->BSRR = ST7789V_WR_PIN
#define ST7789V_RST_H            ST7789V_RST_GPIO->BSRR = ST7789V_RST_PIN

#define ST7789V_DATA_W(dat)      ST7789V_DATA_GPIO->ODR=(dat)
#define ST7789V_DATA_R           ST7789V_DATA_GPIO->IDR


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
