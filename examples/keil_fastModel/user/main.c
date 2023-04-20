#include <stdio.h>
#include "Board_GLCD.h"
#include "perf_counter.h"
#include <stdio.h>
#include <RTE_Components.h>
#if defined(RTE_Compiler_EventRecorder)
#include <EventRecorder.h>
#endif
#include "arm_2d.h"
#include "arm_2d_disp_adapter_0.h"

int main (void)
{
    init_cycle_counter(false);
#if defined(RTE_Compiler_EventRecorder) \
 && defined(RTE_Compiler_IO_STDOUT_EVR)
    EventRecorderInitialize(0, 1);
#endif
    
    printf("Hello Arm-2D \r\n");

    GLCD_Initialize();
    GLCD_SetForegroundColor(0xFFFF);
    GLCD_SetBackgroundColor(0);
    GLCD_ClearScreen();
    
    GLCD_DrawRectangle(10,10,200,50);
    arm_irq_safe {
        arm_2d_init(); // ≥ı ºªØ arm-2d
    }
    
    disp_adapter0_init();
    
    while(1) {
        disp_adapter0_task();
    }
}

__attribute__((used))
void SysTick_Handler(void)
{

}

int32_t Disp0_DrawBitmap(int16_t x,int16_t y,int16_t width,int16_t height,const uint8_t *bitmap)
{
    return GLCD_DrawBitmap((uint32_t)x,(uint32_t)y, (uint32_t)width, (uint32_t)height, bitmap);
}
