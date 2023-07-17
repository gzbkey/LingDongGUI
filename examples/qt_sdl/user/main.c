#include <stdio.h>
#include "SDL.h"
#undef main
#include "Virtual_TFT_Port.h"
#include "arm_2d.h"
#include "arm_2d_disp_adapter_0.h"
#include "ldScene0.h"
#include "xLog.h"
#include "xBtnAction.h"
#include "stdbool.h"

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





int main (void) 
{
    setbuf(stdout,NULL);
    printf("ldgui\n");
    vtInit();

    X_BTN_KEY_INIT(KEY_NUM_UP,vtGetKeyState);
    X_BTN_KEY_INIT(KEY_NUM_DOWN,vtGetKeyState);
    X_BTN_KEY_INIT(KEY_NUM_LEFT,vtGetKeyState);
    X_BTN_KEY_INIT(KEY_NUM_RIGHT,vtGetKeyState);
    X_BTN_KEY_INIT(KEY_NUM_ENTER,vtGetKeyState);
    X_BTN_KEY_INIT(KEY_NUM_ESC,vtGetKeyState);

    arm_irq_safe {
        arm_2d_init();
    }

    disp_adapter0_init();

    arm_2d_scene0_init(&DISP0_ADAPTER);

    while (1)
    {
        if (arm_fsm_rt_cpl == disp_adapter0_task())
        {
            lcdFlush(1);
        }
    }
}

#if defined(__clang__)
#   pragma clang diagnostic pop
#endif
