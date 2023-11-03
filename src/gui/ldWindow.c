/*
 * Copyright 2023-2024 Ou Jianbo (59935554@qq.com)
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/**
 * @file    ldWindow.c
 * @author  Ou Jianbo(59935554@qq.com)
 * @brief   window widget
 * @version 0.1
 * @date    2023-11-03
 */
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

void ldWindowDel(ldWindow_t *pWidget)
{
    xListNode *listInfo;
    
    if (pWidget == NULL)
    {
        return;
    }

    if((pWidget->widgetType!=widgetTypeWindow)&&(pWidget->widgetType!=widgetTypeBackground))
    {
        return;
    }

    LOG_INFO("[window] del,id:%d\n",pWidget->nameId);

    listInfo=ldGetWidgetInfoById(pWidget->nameId);

    if(listInfo!=NULL)
    {
        xListInfoPrevTraverse(pWidget->childList,NULL,_windowDel);
        xListFreeNode(pWidget->childList);
        ldImageDel((ldImage_t *)pWidget);
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
            if(pNewWidget->parentWidget==NULL)
            {
                pNewWidget->isTransparent=false;
                pNewWidget->widgetType=widgetTypeBackground;
            }
            else
            {
                pNewWidget->isTransparent=true;
                pNewWidget->widgetType=widgetTypeWindow;
            }

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

void ldWindowSetTransparent(ldWindow_t *pWidget,bool isTransparent)
{
    if (pWidget == NULL)
    {
        return;
    }
    pWidget->isTransparent=isTransparent;
}



#if defined(__clang__)
#   pragma clang diagnostic pop
#endif

