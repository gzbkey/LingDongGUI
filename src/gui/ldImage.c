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
 * @file    ldImage.c
 * @author  Ou Jianbo(59935554@qq.com)
 * @brief   image widget
 *          background、window、image共用控件
 */
#include "ldImage.h"
#include "ldGui.h"

#if defined(__clang__)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunknown-warning-option"
#pragma clang diagnostic ignored "-Wreserved-identifier"
#pragma clang diagnostic ignored "-Wdeclaration-after-statement"
#pragma clang diagnostic ignored "-Wsign-conversion"
#pragma clang diagnostic ignored "-Wpadded"
#pragma clang diagnostic ignored "-Wcast-qual"
#pragma clang diagnostic ignored "-Wcast-align"
#pragma clang diagnostic ignored "-Wmissing-field-initializers"
#pragma clang diagnostic ignored "-Wgnu-zero-variadic-macro-arguments"
#pragma clang diagnostic ignored "-Wmissing-braces"
#pragma clang diagnostic ignored "-Wunused-const-variable"
#pragma clang diagnostic ignored "-Wmissing-declarations"
#pragma clang diagnostic ignored "-Wmissing-variable-declarations"
#endif

void ldImageDel(ldImage_t *pWidget);
void ldImageFrameUpdate(ldImage_t* pWidget);
void ldImageLoop(arm_2d_scene_t *pScene,ldImage_t *pWidget,const arm_2d_tile_t *pParentTile,bool bIsNewFrame);
const ldGuiCommonFunc_t ldImageCommonFunc={
    (ldDelFunc_t)ldImageDel,
    (ldLoopFunc_t)ldImageLoop,
    (ldUpdateFunc_t)ldImageFrameUpdate,
};

static bool _imageDel(xListNode *pEachInfo, void *pTarget)
{
    if (pEachInfo->info == pTarget)
    {
        ldFree(((ldImage_t *)pTarget));
        xListInfoDel(pEachInfo);
    }
    return false;
}

void ldImageDel(ldImage_t *pWidget)
{
    xListNode *listInfo;

    if (pWidget == NULL)
    {
        return;
    }

    if((pWidget->widgetType!=widgetTypeImage)&&(pWidget->widgetType!=widgetTypeWindow)&&(pWidget->widgetType!=widgetTypeBackground))
    {
        return;
    }

    LOG_INFO("[image] del,id:%d",pWidget->nameId);

    if (pWidget->parentWidget == NULL)
    {
        listInfo = &ldWidgetLink;
    }
    else
    {
        listInfo = ldBaseGetWidgetInfoById(((ldCommon_t *)pWidget->parentWidget)->nameId);
        listInfo = ((ldCommon_t *)listInfo->info)->childList;
    }

    if (listInfo != NULL)
    {
        xListInfoPrevTraverse(listInfo, pWidget, _imageDel);
    }
}
/**
 * @brief   图片初始化
 * 
 * @param   pScene          场景指针
 * @param   nameId          新控件id
 * @param   parentNameId    父控件id
 * @param   x               相对坐标x轴
 * @param   y               相对坐标y轴
 * @param   width           控件宽度
 * @param   height          控件高度
 * @param   imageAddr       图片地址
 * @param   isWithMask      图片是否带mask
 * @return  ldImage_t*      新控件指针
 * @author  Ou Jianbo(59935554@qq.com)
 * @date    2023-12-21
 */
ldImage_t *ldImageInit(arm_2d_scene_t *pScene,uint16_t nameId, uint16_t parentNameId, int16_t x, int16_t y, int16_t width, int16_t height, uintptr_t imageAddr, bool isWithMask)
{
    ldImage_t *pNewWidget = NULL;
    xListNode *parentInfo;
    xListNode *parent_link;
    arm_2d_tile_t *tResTile;

    parentInfo = ldBaseGetWidgetInfoById(parentNameId);
    pNewWidget = LD_CALLOC_WIDGET_INFO(ldImage_t);
    if (pNewWidget != NULL)
    {
        pNewWidget->isParentHidden=false;
        if (parentInfo)
        {
            parent_link = ((ldCommon_t *)parentInfo->info)->childList;
            if(((ldCommon_t *)parentInfo->info)->isHidden||((ldCommon_t *)parentInfo->info)->isParentHidden)
            {
                pNewWidget->isParentHidden=true;
            }
        }
        else
        {
            parent_link = &ldWidgetLink;
        }
        pNewWidget->nameId = nameId;
        pNewWidget->childList = NULL;
        pNewWidget->widgetType = widgetTypeImage;
        xListInfoAdd(parent_link, pNewWidget);
        if (parent_link == &ldWidgetLink) // 自身为bg
        {
            pNewWidget->parentWidget = NULL;
        }
        else
        {
            pNewWidget->parentWidget = parentInfo->info;
        }
        pNewWidget->bgColor = 0;
        pNewWidget->isColor = false;
        pNewWidget->isWithMask = isWithMask;
        pNewWidget->isHidden = false;
        pNewWidget->isTransparent=false;
#if USE_OPACITY == 1
        pNewWidget->opacity = 255;
#endif
        tResTile=(arm_2d_tile_t*)&pNewWidget->resource;
        tResTile->tRegion.tLocation.iX=x;
        tResTile->tRegion.tLocation.iY=y;
        tResTile->tRegion.tSize.iWidth=width;
        tResTile->tRegion.tSize.iHeight=height;
        tResTile->tInfo.bIsRoot = true;
        tResTile->tInfo.bHasEnforcedColour = true;
        tResTile->tInfo.tColourInfo.chScheme = ARM_2D_COLOUR;
        tResTile->pchBuffer = (uint8_t *)imageAddr;
#if USE_VIRTUAL_RESOURCE == 1
        tResTile->tInfo.bVirtualResource = true;
        
        ((arm_2d_vres_t*)tResTile)->pTarget=imageAddr;
        ((arm_2d_vres_t*)tResTile)->Load = &__disp_adapter0_vres_asset_loader;
        ((arm_2d_vres_t*)tResTile)->Depose = &__disp_adapter0_vres_buffer_deposer;
#endif
        pNewWidget->dirtyRegionState=waitChange;
        pNewWidget->pFunc=&ldImageCommonFunc;

        arm_2d_scene_player_dynamic_dirty_region_init(&pNewWidget->dirtyRegionListItem,pScene);

        LOG_INFO("[image] init,id:%d",nameId);
    }
    else
    {
        ldFree(pNewWidget);

        LOG_ERROR("[image] init failed,id:%d",nameId);
    }

    return pNewWidget;
}

/**
 * @brief   设置背景色，window专用
 * 
 * @param   pWidget         目标控件指针
 * @param   bgColor         背景颜色
 * @author  Ou Jianbo(59935554@qq.com)
 * @date    2023-12-21
 */
void ldImageSetBgColor(ldImage_t *pWidget,ldColor bgColor)
{
    if (pWidget == NULL)
    {
        return;
    }
    pWidget->isTransparent=false;
    pWidget->isColor=true;
    pWidget->bgColor=bgColor;
    pWidget->dirtyRegionState=waitChange;
}

void ldImageFrameUpdate(ldImage_t* pWidget)
{
    pWidget->dirtyRegionState=ldBaseUpdateDirtyRegionState(pWidget->dirtyRegionState);
    arm_2d_dynamic_dirty_region_on_frame_start(&pWidget->dirtyRegionListItem,waitChange);
}

void ldImageLoop(arm_2d_scene_t *pScene,ldImage_t *pWidget, const arm_2d_tile_t *pParentTile, bool bIsNewFrame)
{
#if USE_OPACITY == 1
#define IMG_OPACITY        pWidget->opacity
#else
#define IMG_OPACITY        255
#endif

    arm_2d_tile_t *pResTile=(arm_2d_tile_t*)&pWidget->resource;
#if USE_VIRTUAL_RESOURCE == 0
    arm_2d_tile_t tempRes=*pResTile;
#else
    arm_2d_vres_t tempRes=*((arm_2d_vres_t*)pResTile);
#endif
    ((arm_2d_tile_t*)&tempRes)->tRegion.tLocation.iX=0;
    ((arm_2d_tile_t*)&tempRes)->tRegion.tLocation.iY=0;

    if (pWidget == NULL)
    {
        return;
    }

    if((pWidget->isParentHidden)||(pWidget->isHidden)||(pWidget->isTransparent))
    {
        return;
    }

    arm_2d_region_t newRegion=ldBaseGetGlobalRegion((ldCommon_t*)pWidget,&pResTile->tRegion);

    arm_2d_container(pParentTile,tTarget , &newRegion)
    {
        if(ldBaseDirtyRegionUpdate((ldCommon_t*)pWidget,&tTarget_canvas,&pWidget->dirtyRegionListItem,pWidget->dirtyRegionState))
        {
            pWidget->dirtyRegionState=waitEnd;
        }

        if(pWidget->dirtyRegionState<waitRefresh)
        {
            return;
        }

        if (pWidget->isColor)
        {
            ldBaseColor(&tTarget,pWidget->bgColor,IMG_OPACITY);
        }
        else
        {
            if(pResTile->tInfo.tColourInfo.chScheme <= ARM_2D_COLOUR_MASK_A8)
            {
                ldBaseMaskImage(&tTarget,&tempRes,pWidget->specialColor,IMG_OPACITY);
            }
            else
            {
                ldBaseImage(&tTarget,&tempRes,pWidget->isWithMask,IMG_OPACITY);
            }
        }
        arm_2d_op_wait_async(NULL);


    }
}

/**
 * @brief   设置不透明度
 * 
 * @param   pWidget         目标控件指针
 * @param   opacity         0(透明)-255(不透明)
 * @author  Ou Jianbo(59935554@qq.com)
 * @date    2023-12-21
 */
void ldImageSetOpacity(ldImage_t *pWidget, uint8_t opacity)
{
    if (pWidget == NULL)
    {
        return;
    }
#if USE_OPACITY == 1
    pWidget->opacity=opacity;
#endif
    pWidget->dirtyRegionState=waitChange;
}

/**
 * @brief   设置图片
 * 
 * @param   pWidget         目标控件指针
 * @param   imageAddr       图片地址
 * @param   isWithMask      是否带mask
 * @author  Ou Jianbo(59935554@qq.com)
 * @date    2023-12-21
 */
void ldImageSetImage(ldImage_t *pWidget, uintptr_t imageAddr, bool isWithMask)
{
    if (pWidget == NULL)
    {
        return;
    }
    pWidget->isTransparent=false;
    pWidget->isWithMask=isWithMask;
#if USE_VIRTUAL_RESOURCE == 1
        ((arm_2d_vres_t*)(&pWidget->resource))->pTarget=imageAddr;
#endif
        ((arm_2d_tile_t*)(&pWidget->resource))->pchBuffer = (uint8_t *)imageAddr;
    pWidget->dirtyRegionState=waitChange;
}

// grayBit :1 2 4 8
void ldImageSetGrayscale(ldImage_t *pWidget, uint8_t grayBit, ldColor writeColor)
{
    uint8_t colorType=0;
    if (pWidget == NULL)
    {
        return;
    }
    
    switch(grayBit)
    {
        case 1:
        {
            colorType=ARM_2D_COLOUR_1BIT;
            break;
        }
        case 2:
        {
            colorType=ARM_2D_COLOUR_MASK_A2;
            break;
        }
        case 4:
        {
            colorType=ARM_2D_COLOUR_MASK_A4;
            break;
        }
        case 8:
        {
            colorType=ARM_2D_COLOUR_MASK_A8;
            break;
        }
        default:
        {
            return ;
        }
    }
    pWidget->isTransparent=false;
    ((arm_2d_tile_t*)(&pWidget->resource))->tColourInfo.chScheme = colorType;
    pWidget->specialColor = writeColor;
}



#if defined(__clang__)
#pragma clang diagnostic pop
#endif
