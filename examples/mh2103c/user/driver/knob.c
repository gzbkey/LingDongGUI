#include "knob.h"
#include "stdbool.h"

void knobGpioInit(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
    EXTI_InitTypeDef  EXTI_InitStructure;
    
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    
//    GPIO_InitStructure.GPIO_Pin = KNOB_DIR_PIN;
//    GPIO_Init(KNOB_DIR_GPIO, &GPIO_InitStructure);
//    
//    GPIO_InitStructure.GPIO_Pin = KNOB_INT_PIN;
//    GPIO_Init(KNOB_INT_GPIO, &GPIO_InitStructure);
    
    GPIO_InitStructure.GPIO_Pin = KNOB_KEY_PIN;
    GPIO_Init(KNOB_KEY_GPIO, &GPIO_InitStructure);
    
    
    
    
    
//    GPIO_InitStructure.GPIO_Pin = KNOB_INT_PIN;
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;	
//	GPIO_Init(KNOB_INT_GPIO, &GPIO_InitStructure);
//	
//	GPIO_EXTILineConfig(KNOB_INT_PortSource, KNOB_INT_PinSource);
//	
//	NVIC_InitStructure.NVIC_IRQChannel = KNOB_INT_EXTI_IRQ;
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0;
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority =0;
//	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//	NVIC_Init(&NVIC_InitStructure);
//	
//	EXTI_InitStructure.EXTI_Line=KNOB_INT_EXTI_Line;
//	EXTI_InitStructure.EXTI_Mode=EXTI_Mode_Interrupt;
//	EXTI_InitStructure.EXTI_Trigger=EXTI_Trigger_Falling;
//	EXTI_InitStructure.EXTI_LineCmd=ENABLE;
//	EXTI_Init(&EXTI_InitStructure);
}


void knobInit(void)
{
    KNOB_RCC;
    knobGpioInit();
}

//extern uint8_t changeFleg;
//void EXTI2_IRQHandler(void)
//{
//	if(EXTI_GetITStatus(EXTI_Line2)==SET)
//	{
//		if(KNOB_INT_R==Bit_SET)
//		{
//            if(changeFleg>=0x80)
//            {
//                changeFleg-=0x80;
//            }
//			changeFleg++;
//		}	
//	}
//	EXTI_ClearITPendingBit(EXTI_Line2);
//}


    
	
    
bool knobKeyRead(bool isUp)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Pin = KNOB_KEY_PIN;

    if(isUp)
    {
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    }
    else
    {
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    }
    GPIO_Init(KNOB_KEY_GPIO, &GPIO_InitStructure);
    
    if(isUp==KNOB_KEY_R)
    {
        return true;
    }
    return false;
}


