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
 *          窗体控件可包含其他的控件，实现复合型控件或者控件组合模块
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

/**
 * @brief   window初始化函数
 * 
 * @param   nameId          控件id
 * @param   parentNameId    父控件id
 * @param   x               相对坐标x轴
 * @param   y               相对坐标y轴
 * @param   width           控件宽度
 * @param   height          控件高度
 * @return  ldWindow_t*     新控件指针
 * @author  Ou Jianbo(59935554@qq.com)
 * @date    2023-11-07
 */
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
            pNewWidget->dirtyRegion.bIgnore = true;

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

/**
 * @brief   window控件设置透明
 * 
 * @param   pWidget         window控件指针
 * @param   isTransparent   true=透明 false=不透明
 * @author  Ou Jianbo(59935554@qq.com)
 * @date    2023-11-07
 */
void ldWindowSetTransparent(ldWindow_t *pWidget,bool isTransparent)
{
    if (pWidget == NULL)
    {
        return;
    }
    pWidget->isTransparent=isTransparent;
}

/**
 * @brief   window控件设置为静态
 *
 * @param   pWidget         window控件指针
 * @param   Static          true=不修改颜色或图片 false=需要动态刷图或修改颜色
 * @author  Ou Jianbo(59935554@qq.com)
 * @date    2023-12-06
 */
void ldWindowSetStatic(ldWindow_t *pWidget,bool isStatic)
{
    if (pWidget == NULL)
    {
        return;
    }
    pWidget->isStatic=isStatic;
}

#if defined(__clang__)
#   pragma clang diagnostic pop
#endif

