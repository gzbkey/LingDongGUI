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
#include "uiWatch.h"
#include "uiWidget.h"
#include "uiDemo.h"

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

void scene0_loader(void)
{
    arm_2d_scene0_init(&DISP0_ADAPTER);
}

typedef void scene_loader_t(void);

static scene_loader_t * const c_SceneLoaders[] = {
    scene0_loader,
//    scene1_loader,
};

/* load scene one by one */
void before_scene_switching_handler(void *pTarget,
                                    arm_2d_scene_player_t *ptPlayer,
                                    arm_2d_scene_t *ptScene)
{
    static uint_fast8_t s_chIndex = 0;

    if (s_chIndex >= dimof(c_SceneLoaders)) {
        s_chIndex = 0;
    }

    /* call loader */
    c_SceneLoaders[s_chIndex]();
    s_chIndex++;
}

int app_2d_main_thread (void *argument)
{
    arm_2d_scene_player_register_before_switching_event_handler(
            &DISP0_ADAPTER,
            before_scene_switching_handler);

    arm_2d_scene_player_set_switching_mode( &DISP0_ADAPTER,
                                            ARM_2D_SCENE_SWITCH_MODE_FADE_WHITE);
    arm_2d_scene_player_set_switching_period(&DISP0_ADAPTER, 3000);

    arm_2d_scene_player_switch_to_next_scene(&DISP0_ADAPTER);

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

    printf("  _       _____    _____  _    _  _____ \n"
           " | |     |  __ \\  / ____|| |  | ||_   _|\n"
           " | |     | |  | || |  __ | |  | |  | |  \n"
           " | |     | |  | || | |_ || |  | |  | |  \n"
           " | |____ | |__| || |__| || |__| | _| |_ \n"
           " |______||_____/  \\_____| \\____/ |_____|\n\n"
           );

    printf("====================\n");
    LOG_ERROR("Error\n");
    LOG_WARNING("Warning\n");
    LOG_INFO("Info\n");
    LOG_DEBUG("Debug\n");
    printf("====================\n\n");
    vtInit();

    xBtnInit(KEY_NUM_UP,vtGetKeyState);
    xBtnInit(KEY_NUM_DOWN,vtGetKeyState);
    xBtnInit(KEY_NUM_LEFT,vtGetKeyState);
    xBtnInit(KEY_NUM_RIGHT,vtGetKeyState);
    xBtnInit(KEY_NUM_ENTER,vtGetKeyState);
    xBtnInit(KEY_NUM_ESC,vtGetKeyState);

    LD_ADD_PAGE_DEMO;

    arm_irq_safe {
        arm_2d_init();
    }

    disp_adapter0_init();

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
