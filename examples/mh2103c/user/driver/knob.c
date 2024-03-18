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
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;  
	TIM_ICInitTypeDef TIM_ICInitStructure; 
    
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_InitStructure.GPIO_Pin = KNOB_A_PIN;
    GPIO_Init(KNOB_A_GPIO, &GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin = KNOB_B_PIN;
    GPIO_Init(KNOB_B_GPIO, &GPIO_InitStructure);
    
	TIM_TimeBaseInitStructure.TIM_Prescaler = 0;
	TIM_TimeBaseInitStructure.TIM_Period = 0xffff;
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up; 
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStructure);

	TIM_EncoderInterfaceConfig(TIM2, TIM_EncoderMode_TI12, TIM_ICPolarity_Falling, TIM_ICPolarity_Falling);
	TIM_ICStructInit(&TIM_ICInitStructure);
    TIM_ICInitStructure.TIM_Channel = TIM_Channel_1;
	TIM_ICInitStructure.TIM_ICFilter = 0x0F;
	TIM_ICInit(TIM2, &TIM_ICInitStructure);
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_2;
	TIM_ICInitStructure.TIM_ICFilter = 0x0F;
	TIM_ICInit(TIM2, &TIM_ICInitStructure);

	//Reset counter
	TIM_SetCounter(TIM2,0x7fff);
    TIM_ClearFlag(TIM2, TIM_FLAG_Update);
	TIM_Cmd(TIM2, ENABLE);
    
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
    GPIO_InitStructure.GPIO_Pin = KNOB_KEY_PIN;
    GPIO_Init(KNOB_KEY_GPIO, &GPIO_InitStructure);
    
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

int16_t knobGetEncoder(void)
{
	int16_t temp = TIM_GetCounter(TIM2)-0x7fff;
	TIM_SetCounter(TIM2, 0x7fff);
    return temp;
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


