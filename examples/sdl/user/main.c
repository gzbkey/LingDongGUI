#include <stdio.h>
#include "SDL.h"
#undef main
#include "Virtual_TFT_Port.h"
#include "arm_2d.h"
#include "arm_2d_disp_adapter_0.h"
#include "ldScene0.h"
#include "ldScene1.h"
#include "xLog.h"
//#include "xBtnAction.h"
//#include "uiWatch.h"
#include "uiWidget.h"
//#include "uiDemo.h"
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

int app_2d_main_thread (void *argument)
{
    while(1) {
        if (arm_fsm_rt_cpl == disp_adapter0_task()) {
            vtSdlFlush(1);
        }
    }

    return 0;
}

int main (void) 
{
    setbuf(stdout,NULL);

    LOG_NORMAL(
                "  _       _____    _____  _    _  _____ \n"
                " | |     |  __ \\  / ____|| |  | ||_   _|\n"
                " | |     | |  | || |  __ | |  | |  | |  \n"
                " | |     | |  | || | |_ || |  | |  | |  \n"
                " | |____ | |__| || |__| || |__| | _| |_ \n"
                " |______||_____/  \\_____| \\____/ |_____|\n"
               );

    LOG_NORMAL("====================");
    LOG_ERROR("Error");
    LOG_WARNING("Warning");
    LOG_INFO("Info");
    LOG_DEBUG("Debug");
    LOG_NORMAL("====================\n");
    vtInit();

//    xBtnInit(KEY_NUM_UP,vtGetKeyState);
//    xBtnInit(KEY_NUM_DOWN,vtGetKeyState);
//    xBtnInit(KEY_NUM_LEFT,vtGetKeyState);
//    xBtnInit(KEY_NUM_RIGHT,vtGetKeyState);
//    xBtnInit(KEY_NUM_ENTER,vtGetKeyState);
//    xBtnInit(KEY_NUM_ESC,vtGetKeyState);

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

    SDL_CreateThread(app_2d_main_thread, "arm-2d thread", NULL);

    while (1)
    {
        vtSdlRefreshTask();
        if(vtIsRequestQuit())
        {
            break;
        }
    }

    vtDeinit();
    return 0;
}

#if defined(__clang__)
#   pragma clang diagnostic pop
#endif
