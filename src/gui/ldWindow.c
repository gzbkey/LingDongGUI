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

void ldWindowDel(ldWindow_t *pWidget);
void ldImageFrameUpdate(ldWindow_t* pWidget);
void ldImageLoop(arm_2d_scene_t *pScene,ldWindow_t *pWidget,const arm_2d_tile_t *pParentTile,bool bIsNewFrame);
const ldGuiCommonFunc_t ldWindowCommonFunc={
    (ldDelFunc_t)ldWindowDel,
    (ldLoopFunc_t)ldImageLoop,
    (ldUpdateFunc_t)ldImageFrameUpdate,
};

static bool _windowDel(xListNode* pEachInfo,void* pTarget)
{
    ((ldCommon_t*)pEachInfo->info)->pFunc->del(pEachInfo->info);
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

    LOG_INFO("[window] del,id:%d",pWidget->nameId);

    listInfo=ldBaseGetWidgetInfoById(pWidget->nameId);

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
ldWindow_t* ldWindowInit(arm_2d_scene_t *pScene,uint16_t nameId, uint16_t parentNameId, int16_t x,int16_t y,int16_t width,int16_t height)
{
    ldWindow_t * pNewWidget = NULL;
    pNewWidget = ldImageInit(pScene,nameId,parentNameId,x,y,width,height,0,false);
    if(pNewWidget!=NULL)
    {
        if(xListMallocNode(&pNewWidget->childList)!=NULL)
        {
            if(pNewWidget->parentWidget==NULL)
            {
                pNewWidget->widgetType=widgetTypeBackground;
                ldImageSetBgColor(pNewWidget,__RGB(240,240,240));
            }
            else
            {
                pNewWidget->isTransparent=true;
                pNewWidget->widgetType=widgetTypeWindow;
            }
            pNewWidget->pFunc=&ldWindowCommonFunc;

            LOG_INFO("[window] init,id:%d",nameId);
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

#if defined(__clang__)
#   pragma clang diagnostic pop
#endif

