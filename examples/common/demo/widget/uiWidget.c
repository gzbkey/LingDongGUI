#include "uiWidget.h"

#if (USE_VIRTUAL_RESOURCE == 1) && ( __x86_64__ || __i386__ || __APPLE__ )
#include "virtualNor.h"
static bool isWaitNorInit = true;
#endif

void uiWidgetInit(uint8_t page)
{
#if (USE_VIRTUAL_RESOURCE == 1) && ( __x86_64__ || __i386__ || __APPLE__ )
    if(isWaitNorInit)
    {
        norSetBin("../common/demo/widget/srcWidget.bin");
        isWaitNorInit=false;
    }
#endif
    void *obj;

}

void uiWidgetLoop(uint8_t page)
{
}

void uiWidgetQuit(uint8_t page)
{
}
