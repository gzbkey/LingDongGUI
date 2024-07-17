#include "uiPageTemplate.h"
#include "uiPageLogicTemplate.h"
#include "ldGui.h"

void uiPageTemplateInit(ld_scene_t* ptScene);
void uiPageTemplateLoop(ld_scene_t* ptScene);
void uiPageTemplateQuit(ld_scene_t* ptScene);


const ldPageFuncGroup_t uiPageTemplateFunc={
    .init=uiPageTemplateInit,
    .loop=uiPageTemplateLoop,
    .quit=uiPageTemplateQuit,
#if (USE_LOG_LEVEL>=LOG_LEVEL_INFO)
    .pageName="uiPageTemplate",
#endif
};

void uiPageTemplateInit(ld_scene_t* ptScene)
{




    uiPageLogicTemplateInit(ptScene);
}

void uiPageTemplateLoop(ld_scene_t* ptScene)
{




    uiPageLogicTemplateLoop(ptScene);
}

void uiPageTemplateQuit(ld_scene_t* ptScene)
{




    uiPageLogicTemplateQuit(ptScene);
}
