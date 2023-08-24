#include "ldWindow.h"
#include "ldGui.h"

#if defined(__clang__)
#   pragma clang diagnostic push
#   pragma clang diagnostic ignored "-Wunknown-warning-option"
#   pragma clang diagnostic ignored "-Wreserved-identifier"
#   pragma clang diagnostic ignored "-Wdeclaration-after-statement"
#   pragma clang diagnostic ignored "-Wsign-conversion"
#   pragma clang diagnostic ignored "-Wpadded"
#   pragma clang diagnostic ignored "-Wcast-qual"
#   pragma clang diagnostic ignored "-Wcast-align"
#   pragma clang diagnostic ignored "-Wmissing-field-initializers"
#   pragma clang diagnostic ignored "-Wgnu-zero-variadic-macro-arguments"
#   pragma clang diagnostic ignored "-Wmissing-braces"
#   pragma clang diagnostic ignored "-Wunused-const-variable"
#   pragma clang diagnostic ignored "-Wmissing-declarations"
#   pragma clang diagnostic ignored "-Wmissing-variable-declarations"
#endif

static bool _windowDel(xListNode* pEachInfo,void* pTarget)
{
    ldGuiDelWidget(pEachInfo->info);
    return false;
}

void ldWindowDel(ldWindow_t *widget)
{
    xListNode *listInfo;
    
    if (widget == NULL)
    {
        return;
    }

    if(widget->widgetType!=widgetTypeWindow)
    {
        return;
    }

    LOG_DEBUG("[window] del,id:%d\n",widget->nameId);

    listInfo=ldGetWidgetInfoById(widget->nameId);

    if(listInfo!=NULL)
    {
        xListInfoPrevTraverse(widget->childList,NULL,_windowDel);
        xListFreeNode(widget->childList);
        ldImageDel((ldImage_t *)widget);
    }
}

ldWindow_t* ldWindowInit(uint16_t nameId, uint16_t parentNameId, int16_t x,int16_t y,int16_t width,int16_t height)
{
    ldWindow_t * pNewWidget = NULL;
    pNewWidget = ldImageInit(nameId,parentNameId,x,y,width,height,0,false);
    if(pNewWidget!=NULL)
    {
        if(xListMallocNode(&pNewWidget->childList)!=NULL)
        {
            pNewWidget->isTransparent=true;
            pNewWidget->widgetType=widgetTypeWindow;
            LOG_INFO("[window] init,id:%d\n",nameId);
        }
        else
        {
            ldFree(pNewWidget->childList);
            ldImageDel(pNewWidget);
            pNewWidget=NULL;
        }
    }
    return pNewWidget;
}

void ldWindowSetTransparent(ldWindow_t *widget,bool isTransparent)
{
    if (widget == NULL)
    {
        return;
    }
    widget->isTransparent=isTransparent;
}



#if defined(__clang__)
#   pragma clang diagnostic pop
#endif

