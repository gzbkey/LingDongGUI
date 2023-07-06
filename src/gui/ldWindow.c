#include "ldWindow.h"

//#include "global.h"
//#include "jfif_parser.h"

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
    ldDelWidget(pEachInfo->info);
    return false;
}

void pWindowDel(ldWindow_t *widget)
{
    xListNode *listInfo;
    
    if(widget->widgetType!=widgetTypeWindow)
    {
        return;
    }
    listInfo=ldGetWidgetInfoById(widget->nameId);
    
    if(listInfo!=NULL)
    {
        xListInfoPrevTraverse(widget->childList,NULL,_windowDel);
        xListFreeNode(widget->childList);
        
        pImageDel((ldImage_t *)widget);
        
    }
}

ldWindow_t* ldWindowInit(uint16_t nameId, uint16_t parentNameId, int16_t x,int16_t y,int16_t width,int16_t height,ldColor bgColor,uint32_t imageAddr,uint16_t maxImageNum,bool isWithMask,bool isTransparent,bool isHidden)
{
    ldWindow_t * pNewWidget = NULL;
    
    pNewWidget = ldImageInit(nameId,parentNameId,x,y,width,height,bgColor,imageAddr,maxImageNum,isWithMask,0);
    if(pNewWidget!=NULL)
    {            
        if(xListMallocNode(&pNewWidget->childList)!=NULL)
        {
            pNewWidget->isTransparent=isTransparent;
            pNewWidget->widgetType=widgetTypeWindow;
        }
        else
        {
            ldFree(pNewWidget->childList);
            pImageDel(pNewWidget);
            pNewWidget=NULL;
        }
    }
    return pNewWidget;
}

void pWindowSetHidden(bool isHidden)
{
    
}

#if defined(__clang__)
#   pragma clang diagnostic pop
#endif

