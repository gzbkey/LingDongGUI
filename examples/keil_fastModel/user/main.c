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
#include "arm_2d_scene_0.h"
#include "freeRtosHeap4.h"
#include "xConnect.h"
#include "ldImage.h"
#include "ldWindow.h"
#include "ldCommon.h"

void *ldMalloc(uint32_t size)
{
    return pvPortMalloc(size);
}

void ldFree(void *p)
{
    vPortFree(p);
    p=NULL;
}

void *ldRealloc(void *ptr,uint32_t newSize)
{
    void *new_addr=NULL;
    if(ptr)
    {
        if(newSize!=0)
        {
            new_addr=ldMalloc(newSize);

            if(new_addr!=NULL)
            {
                memcpy(new_addr,ptr,newSize);
                ldFree(ptr);
            }
        }
    }
    else
    {
        if(newSize!=0)
        {
            new_addr=ldMalloc(newSize);
        }
    }
    return new_addr;
}













int main (void)
{
    init_cycle_counter(false);
#if defined(RTE_Compiler_EventRecorder) \
 && defined(RTE_Compiler_IO_STDOUT_EVR)
    EventRecorderInitialize(0, 1);
#endif
    
    printf("Hello Arm-2D \r\n");

    GLCD_Initialize();

    

    arm_irq_safe {
        arm_2d_init();
    }
    
    disp_adapter0_init();
    
    arm_2d_scene0_init(&DISP0_ADAPTER);
		
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
