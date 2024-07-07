#include <stdio.h>
#include "SDL.h"
#undef main
#include "Virtual_TFT_Port.h"
#include "arm_2d.h"
#include "arm_2d_disp_adapter_0.h"
#include "ldScene0.h"
#include "xLog.h"
//#include "xBtnAction.h"
#include "stdbool.h"
//#include "uiWatch.h"
//#include "uiWidget.h"
//#include "uiDemo.h"

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
void demoInit(arm_2d_scene_t* ptScene);

const ldPageFuncGroup_t ldGuiFuncGroup={
    .pSceneInit=__arm_2d_scene0_init,
    .init=demoInit,
#if (USE_LOG_LEVEL>=LOG_LEVEL_INFO)
    .pageName="demo",
#endif
};

static bool slotPageJump(xConnectInfo_t info)
{
    ldGuiJumpPage(&ldGuiFuncGroup,ARM_2D_SCENE_SWITCH_MODE_SLIDE_RIGHT,3000);
    return false;
}

void demoInit(arm_2d_scene_t* ptScene)
{
    void *obj;
    ldWindow_init(ptScene,NULL,0, 0, 0, 0, 320, 240);
    obj= ldImage_init(ptScene,NULL,3, 0, 100, 100, 50, 50, NULL, NULL,false);
    ldImageSetBgColor(obj,__RGB(0xFF,0xFF,0xFF));

    ldButton_init(ptScene,NULL,2, 0, 10,10,100,50);

    xConnect(2,SIGNAL_RELEASE,0,slotPageJump);
}




//void scene0_loader(void)
//{
//    __arm_2d_scene0_init(&DISP0_ADAPTER,NULL,&ldGuiFuncGroup);
//}

//typedef void scene_loader_t(void);

//static scene_loader_t * const c_SceneLoaders[] = {
//    scene0_loader,
//};

/* load scene one by one */
void before_scene_switching_handler(void *pTarget,
                                    arm_2d_scene_player_t *ptPlayer,
                                    arm_2d_scene_t *ptScene)
{
    ptGuiPageFuncGroup->pSceneInit(&DISP0_ADAPTER,NULL,&ldGuiFuncGroup);
//    static uint_fast8_t s_chIndex = 0;

//    if (s_chIndex >= dimof(c_SceneLoaders)) {
//        s_chIndex = 0;
//    }

//    /* call loader */
//    c_SceneLoaders[s_chIndex]();
//    s_chIndex++;
}

int app_2d_main_thread (void *argument)
{
#if __DISP0_CFG_DISABLE_DEFAULT_SCENE__
    arm_2d_scene_player_register_before_switching_event_handler(
            &DISP0_ADAPTER,
            before_scene_switching_handler);

    arm_2d_scene_player_set_switching_mode( &DISP0_ADAPTER,
                                            ARM_2D_SCENE_SWITCH_MODE_SLIDE_RIGHT);
    arm_2d_scene_player_set_switching_period(&DISP0_ADAPTER, 3000);

    arm_2d_scene_player_switch_to_next_scene(&DISP0_ADAPTER);
#endif
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

#if __DISP0_CFG_DISABLE_DEFAULT_SCENE__
//#if LD_PAGE_STATIC == 0
//    ldGuiSetPageMax(1);
//#endif
//    LD_ADD_PAGE_DEMO;
#endif

    ptGuiPageFuncGroup=(ldPageFuncGroup_t*)&ldGuiFuncGroup;

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
