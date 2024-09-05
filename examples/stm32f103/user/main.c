#include "stm32f10x.h"                  // Device header
#include "perf_counter.h"
#include "arm_2d.h"
#include "arm_2d_disp_adapter_0.h"
#include "ldScene0.h"
#include "ldScene1.h"
#include "xLog.h"
#if __DISP0_CFG_DISABLE_DEFAULT_SCENE__
#if USE_DEMO > 0
#include LD_DEMO_GUI_INCLUDE
#endif
#endif
#include "freeRtosHeap4.h"
#include "ldGui.h"

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
	RCC_PLLConfig(RCC_PLLSource_HSE_Div1,RCC_PLLMul_9);
	
	RCC_PLLCmd(ENABLE);
	while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET);
	
	RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
	
	RCC_HCLKConfig(RCC_SYSCLK_Div1);
	RCC_PCLK1Config(RCC_HCLK_Div2);
	RCC_PCLK2Config(RCC_HCLK_Div1);
	
	RCC_LSICmd(ENABLE);
	while(RCC_GetFlagStatus(RCC_FLAG_LSIRDY) == RESET)
    {
    }
	RCC_HSICmd(ENABLE);
	while(RCC_GetFlagStatus(RCC_FLAG_HSIRDY) == RESET)
    {
    }
}

int main(void)
{
    rccInit();
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_3);

    SysTick_Config(SystemCoreClock/100);
    init_cycle_counter(true);
    
    arm_irq_safe {
        arm_2d_init();
    }

    disp_adapter0_init();
    
    #if __DISP0_CFG_DISABLE_DEFAULT_SCENE__
#if USE_DEMO == 0
    //user gui page init

#else
    ldGuiInit(LD_DEMO_GUI_FUNC);
#endif
#endif
    
    while(1)
    {
        disp_adapter0_task();
    }
}

__attribute__((used))
void SysTick_Handler(void)
{
}
