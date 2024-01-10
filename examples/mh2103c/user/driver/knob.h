#ifndef _KNOB_H_
#define _KNOB_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "mh210x.h"
#include "stdbool.h"

#define KNOB_RCC                RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); \
                                RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE)

#define KNOB_DIR_GPIO           GPIOC
#define KNOB_DIR_PIN            GPIO_Pin_13

#define KNOB_INT_GPIO           GPIOA
#define KNOB_INT_PIN            GPIO_Pin_2
#define KNOB_INT_PortSource     GPIO_PortSourceGPIOA   
#define KNOB_INT_PinSource      GPIO_PinSource2
#define KNOB_INT_EXTI_Line      EXTI_Line2
#define KNOB_INT_EXTI_IRQ       EXTI2_IRQn


#define KNOB_KEY_GPIO           GPIOA
#define KNOB_KEY_PIN            GPIO_Pin_15

#define KNOB_DIR_R              GPIO_ReadInputDataBit(KNOB_DIR_GPIO,KNOB_DIR_PIN)
#define KNOB_INT_R              GPIO_ReadInputDataBit(KNOB_INT_GPIO,KNOB_INT_PIN)
#define KNOB_KEY_R              GPIO_ReadInputDataBit(KNOB_KEY_GPIO,KNOB_KEY_PIN)

void knobInit(void);

bool knobKeyRead(bool isUp);

#ifdef __cplusplus
}
#endif

#endif //_KNOB_H_
