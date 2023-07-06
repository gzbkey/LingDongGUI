#include <stdio.h>
#include "SDL.h"
#undef main
#include "Virtual_TFT_Port.h"
#include "arm_2d.h"
#include "arm_2d_benchmark.h"

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

extern void lcd_flush(int32_t nMS);
int main (void) 
{
    printf("arm-2d sdl\n");
    VT_Init();

//    SDL_GetTicks();
//    SDL_AddTimer(10, VT_timerCallback, NULL);

    arm_irq_safe {
        arm_2d_init();
    }

    disp_adapter0_init();

    arm_2d_run_benchmark();
//    arm_2d_scene0_init(&DISP0_ADAPTER);

    while (1) {
        if (arm_fsm_rt_cpl == disp_adapter0_task()) {
            lcd_flush(1);
        }
    }
}

#if defined(__clang__)
#   pragma clang diagnostic pop
#endif
