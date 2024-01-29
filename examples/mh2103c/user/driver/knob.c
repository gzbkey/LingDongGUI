#include "knob.h"
#include "stdbool.h"
#include "perf_counter.h"

static uint8_t knobTickCount=0;

void knobGpioInit(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
    EXTI_InitTypeDef  EXTI_InitStructure;
    ADC_InitTypeDef ADC_InitStructure;
    
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
    
    GPIO_InitStructure.GPIO_Pin = KNOB_KEY_PIN;
    GPIO_Init(KNOB_KEY_GPIO, &GPIO_InitStructure);
    
    GPIO_InitStructure.GPIO_Pin = KNOB_A_PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;	
    GPIO_Init(KNOB_A_GPIO, &GPIO_InitStructure);
    
    GPIO_InitStructure.GPIO_Pin = KNOB_B_PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;	
    GPIO_Init(KNOB_B_GPIO, &GPIO_InitStructure);
    
    EXTI_InitStructure.EXTI_Line=KNOB_A_EXTI_Line;
    EXTI_InitStructure.EXTI_Mode=EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger=EXTI_Trigger_Falling;
    EXTI_InitStructure.EXTI_LineCmd=ENABLE;
    EXTI_Init(&EXTI_InitStructure);
    GPIO_EXTILineConfig(KNOB_A_PortSource, KNOB_A_PinSource);

    NVIC_InitStructure.NVIC_IRQChannel = KNOB_A_EXTI_IRQ;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority =0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    
	RCC_ADCCLKConfig(RCC_PCLK2_Div8);   				
	ADC_DeInit(ADC1); 

	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;	
	ADC_InitStructure.ADC_ScanConvMode = ENABLE;		
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;	
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;	
	ADC_InitStructure.ADC_NbrOfChannel = 1;
	ADC_Init(ADC1, &ADC_InitStructure);	
	
    ADC_RegularChannelConfig(ADC1 , ADC_Channel_2 , 1 , ADC_SampleTime_239Cycles5);

	ADC_SoftwareStartConvCmd(ADC1, ENABLE);
	
	ADC_Cmd(ADC1, ENABLE);	
	
	ADC_ResetCalibration(ADC1);	
	while(ADC_GetResetCalibrationStatus(ADC1));
	ADC_StartCalibration(ADC1);	 
	while(ADC_GetCalibrationStatus(ADC1));
}

void knobInit(void)
{
    KNOB_RCC;
    knobGpioInit();
}

__WEAK void knobAdd(void)
{
}

__WEAK void knobSub(void)
{
}

static void knobDelay(void)
{
    volatile uint8_t i=250;
    while(i--);
}

void EXTI15_10_IRQHandler(void)
{
	if(EXTI_GetITStatus(KNOB_A_EXTI_Line)==SET)
	{
        if(knobTickCount==0)
        {
            knobDelay();
            if(KNOB_A_R==RESET)
            {
                if(KNOB_B_R)
                {
                    knobAdd();
                }
                else
                {
                    knobSub();
                }
            }
            knobTickCount=5;
        }
		EXTI_ClearITPendingBit(KNOB_A_EXTI_Line);
	}
	
}

void knobTick(uint8_t cycleMs)
{
    if(knobTickCount!=0)
    {
        knobTickCount--;
    }
}

uint8_t knobKeyRead(void)
{
    uint16_t value;
    ADC_SoftwareStartConvCmd(ADC1, ENABLE);
    while(ADC_GetFlagStatus(ADC1,ADC_FLAG_EOC)==RESET);
	value= ADC_GetConversionValue(ADC1);
    
    if(value>4000)
    {
        return 0;
    }
    else
    {
        if(value>2000)
        {
            return 2;
        }
        else
        {
            return 1;
        }
    }
}


