#include "mh210x.h"
#include "mh.h"
#include "perf_counter.h"
#include "st7789v16bit.h"
#include "arm_2d.h"
#include "arm_2d_disp_adapter_0.h"
#include "ldScene0.h"
#include "uiDemo.h"
#include "knob.h"

#if defined(__clang__)
#   pragma clang diagnostic push
#   pragma clang diagnostic ignored "-Wunknown-warning-option"
#   pragma clang diagnostic ignored "-Wreserved-identifier"
#   pragma clang diagnostic ignored "-Wsign-conversion"
#   pragma clang diagnostic ignored "-Wpadded"
#   pragma clang diagnostic ignored "-Wcast-qual"
#   pragma clang diagnostic ignored "-Wcast-align"
#   pragma clang diagnostic ignored "-Wmissing-field-initializers"
#   pragma clang diagnostic ignored "-Wgnu-zero-variadic-macro-arguments"
#   pragma clang diagnostic ignored "-Wmissing-prototypes"
#   pragma clang diagnostic ignored "-Wunused-variable"
#   pragma clang diagnostic ignored "-Wunused-parameter"
#   pragma clang diagnostic ignored "-Wgnu-statement-expression"
#elif __IS_COMPILER_ARM_COMPILER_5__
#elif __IS_COMPILER_GCC__
#   pragma GCC diagnostic push
#   pragma GCC diagnostic ignored "-Wformat="
#   pragma GCC diagnostic ignored "-Wpedantic"
#endif
void rccInit(void)
{
	RCC_DeInit();

	RCC_HSEConfig(RCC_HSE_ON);
	while(RCC_GetFlagStatus(RCC_FLAG_HSERDY) == RESET);
	
	RCC_PLLCmd(DISABLE);
	MH_RCC_PLLConfig(RCC_PLLSource_HSE_Div1,RCC_PLLMul_27,1);
	
	RCC_PLLCmd(ENABLE);
	while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET);
	
	RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
	
	RCC_HCLKConfig(RCC_SYSCLK_Div1);
	RCC_PCLK1Config(RCC_HCLK_Div2);
	RCC_PCLK2Config(RCC_HCLK_Div1);
	
	RCC_LSICmd(ENABLE);
	while(RCC_GetFlagStatus(RCC_FLAG_LSIRDY) == RESET);
	RCC_HSICmd(ENABLE);
	while(RCC_GetFlagStatus(RCC_FLAG_HSIRDY) == RESET);
    
    SystemCoreClock=216000000;
}

void gpioDeInit(void)
{
#if 0
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
    GPIO_PinRemapConfig(GPIO_Remap_SWJ_Disable,ENABLE);
#else
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
    GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);
#endif

    PWR_BackupAccessCmd(ENABLE);
    RCC_LSEConfig(RCC_LSE_OFF);
    BKP_TamperPinCmd(DISABLE);
    PWR_BackupAccessCmd(DISABLE);
}

#define BTN_ID_KNOB 0
#define BTN_ID_POW  1

bool getBtnState(uint16_t id)
{
    uint8_t value=knobKeyRead();
    
    switch(id)
    {
        case BTN_ID_KNOB:
        {
            return GETBIT(value,0);
            break;
        }
        case BTN_ID_POW:
        {
            return GETBIT(value,1);
            break;
        }
        default:
            break;
    }
    return 0;
}

volatile int16_t encoderCount;

int16_t add=0,sub=0;
int main(void)
{
	RCC_ClocksTypeDef clocks;
	
	rccInit();
	RCC_GetClocksFreq(&clocks);
    
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_3);

    SysTick_Config(SystemCoreClock/100);
    init_cycle_counter(true);

    gpioDeInit();
    
    st7789v_init();
    
    knobInit();
    
    xBtnInit(BTN_ID_POW,getBtnState);
    xBtnInit(BTN_ID_KNOB,getBtnState);

    LD_ADD_PAGE(uiDemo);

    arm_irq_safe {
        arm_2d_init();
    }

    disp_adapter0_init();
    
    arm_2d_scene0_init(&DISP0_ADAPTER);

    while(1)
    {
        disp_adapter0_task();

        encoderCount=knobGetEncoder();
        if(encoderCount>0)
        {
            add+=encoderCount;
        }
        else
        {
            sub+=encoderCount;
        }
    }
}

__attribute__((used))    //!< 避免下面的处理程序被编译器优化掉
void SysTick_Handler(void)
{
    knobTick(10);
}



#if defined(__clang__)
#   pragma clang diagnostic pop
#endif
