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
#if __DISP0_CFG_DISABLE_DEFAULT_SCENE__
#include LD_DEMO_GUI_INCLUDE
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

int app_2d_main_thread (void *argument)
{
    while(1) {
        if (VT_is_request_quit()) {
            break;
        }
        ldGuiLoop();
    }
    return 0;
}

static bool __lcd_sync_handler(void *pTarget)
{
    return VT_sdl_flush(1);
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
    VT_init();

    xBtnInit(KEY_NUM_UP,vtIsKeyPress);
    xBtnInit(KEY_NUM_DOWN,vtIsKeyPress);
    xBtnInit(KEY_NUM_LEFT,vtIsKeyPress);
    xBtnInit(KEY_NUM_RIGHT,vtIsKeyPress);
    xBtnInit(KEY_NUM_ENTER,vtIsKeyPress);
    xBtnInit(KEY_NUM_ESC,vtIsKeyPress);

    ldGuiInit((ldPageFuncGroup_t *)&LD_DEMO_GUI_FUNC);

    do {
        arm_2d_helper_pfb_dependency_t tDependency = {
            .evtOnLowLevelSyncUp = {
                .fnHandler = &__lcd_sync_handler,
            },
        };
        arm_2d_helper_pfb_update_dependency(&DISP0_ADAPTER.use_as__arm_2d_helper_pfb_t,
                                            ARM_2D_PFB_DEPEND_ON_LOW_LEVEL_SYNC_UP,
                                            &tDependency);
    } while(0);

    SDL_CreateThread(app_2d_main_thread, "arm-2d thread", NULL);

    while (1)
    {
        if(!VT_sdl_refresh_task())
        {
            break;
        }
    }

    VT_deinit();
    return 0;
}

#if defined(__clang__)
#   pragma clang diagnostic pop
#endif
