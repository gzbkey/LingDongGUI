#include "knob.h"
#include "stdbool.h"

void knobGpioInit(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
    EXTI_InitTypeDef  EXTI_InitStructure;
    
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    
    GPIO_InitStructure.GPIO_Pin = KNOB_KEY_PIN;
    GPIO_Init(KNOB_KEY_GPIO, &GPIO_InitStructure);
    
    
//    
//    
//    
//    GPIO_InitStructure.GPIO_Pin = KNOB_A_PIN;
//    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;	
//    GPIO_Init(KNOB_A_GPIO, &GPIO_InitStructure);
//    
//    GPIO_InitStructure.GPIO_Pin = KNOB_B_PIN;
//    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;	
//    GPIO_Init(KNOB_B_GPIO, &GPIO_InitStructure);
//    
//	
//    GPIO_EXTILineConfig(KNOB_A_PortSource, KNOB_A_PinSource);
//    
//    EXTI_InitStructure.EXTI_Line=KNOB_A_EXTI_Line;
//    EXTI_InitStructure.EXTI_Mode=EXTI_Mode_Interrupt;
//    EXTI_InitStructure.EXTI_Trigger=EXTI_Trigger_Rising;
//    EXTI_InitStructure.EXTI_LineCmd=ENABLE;
//    EXTI_Init(&EXTI_InitStructure);
//    
//    NVIC_InitStructure.NVIC_IRQChannel = KNOB_A_EXTI_IRQ;
//    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0;
//    NVIC_InitStructure.NVIC_IRQChannelSubPriority =0;
//    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//    NVIC_Init(&NVIC_InitStructure);
}


void knobInit(void)
{
    KNOB_RCC;
    knobGpioInit();
}

//uint8_t changeFleg;
//uint8_t A[10]={9,9,9,9,9,9,9,9,9,9},B[10]={9,9,9,9,9,9,9,9,9,9};
//uint8_t count=0;
//void EXTI2_IRQHandler(void)
//{
//	if(EXTI_GetITStatus(EXTI_Line2)==SET)
//	{
//        if(count<10)
//        {
//           A[count]=KNOB_A_R;
//        B[count]=KNOB_B_R;
//        count++;
//        }
//        
//        
//		
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


