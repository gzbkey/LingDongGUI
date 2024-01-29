#ifndef _KNOB_H_
#define _KNOB_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "mh210x.h"
#include "stdbool.h"

#define KNOB_RCC                 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); \
                                 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE); \
                                 RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE); \
                                 RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE)
                                 

#define KNOB_B_GPIO              GPIOC
#define KNOB_B_PIN               GPIO_Pin_13
#define KNOB_B_PortSource        GPIO_PortSourceGPIOC   
#define KNOB_B_PinSource         GPIO_PinSource13
#define KNOB_B_EXTI_Line         EXTI_Line13
#define KNOB_B_EXTI_IRQ          EXTI15_10_IRQn

#define KNOB_A_GPIO              GPIOA
#define KNOB_A_PIN               GPIO_Pin_15
#define KNOB_A_PortSource        GPIO_PortSourceGPIOA   
#define KNOB_A_PinSource         GPIO_PinSource15
#define KNOB_A_EXTI_Line         EXTI_Line15
#define KNOB_A_EXTI_IRQ          EXTI15_10_IRQn

#define KNOB_KEY_GPIO            GPIOA
#define KNOB_KEY_PIN             GPIO_Pin_2

#define KNOB_A_R                 GPIO_ReadInputDataBit(KNOB_A_GPIO,KNOB_A_PIN)
#define KNOB_B_R                 GPIO_ReadInputDataBit(KNOB_B_GPIO,KNOB_B_PIN)
#define KNOB_KEY_R               GPIO_ReadInputDataBit(KNOB_KEY_GPIO,KNOB_KEY_PIN)

void knobInit(void);



void knobTick(uint8_t cycleMs);

uint8_t knobKeyRead(void);

#ifdef __cplusplus
}
#endif

#endif //_KNOB_H_
