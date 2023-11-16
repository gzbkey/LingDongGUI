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
 * @file    gauge.c
 * @author  Ou Jianbo(59935554@qq.com)
 * @brief   仪表盘控件
 * @version 0.1
 * @date    2023-11-09
 */
#include "ldGauge.h"
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

#define ANGLE_OFFSET       180 //调整指针起始位置

static bool _gaugeDel(xListNode *pEachInfo, void *pTarget)
{
    if (pEachInfo->info == pTarget)
    {
        //del user object

        ldFree(((ldGauge_t *)pTarget));
        xListInfoDel(pEachInfo);
    }
    return false;
}

void ldGaugeDel(ldGauge_t *pWidget)
{
    xListNode *listInfo;

    if (pWidget == NULL)
    {
        return;
    }

    if(pWidget->widgetType!=widgetTypeGauge)
    {
        return;
    }

    LOG_INFO("[gauge] del,id:%d\n",pWidget->nameId);

    xDeleteConnect(pWidget->nameId);

    listInfo = ldGetWidgetInfoById(((ldCommon_t *)pWidget->parentWidget)->nameId);
    listInfo = ((ldCommon_t *)listInfo->info)->childList;

    if (listInfo != NULL)
    {
        xListInfoPrevTraverse(listInfo, pWidget, _gaugeDel);
    }
}

/**
 * @brief   gauge初始化函数
 *
 * @param   nameId          新控件id
 * @param   parentNameId    父控件id
 * @param   x               相对坐标x轴
 * @param   y               相对坐标y轴
 * @param   width           控件宽度
 * @param   height          控件高度
 * @return  ldGauge_t*     新控件指针
 * @author  Ou Jianbo(59935554@qq.com)
 * @date    2023-11-09
 */
ldGauge_t *ldGaugeInit(uint16_t nameId, uint16_t parentNameId, int16_t x, int16_t y, int16_t width, int16_t height,uint32_t bgImgAddr,bool isBgMask)
{
    ldGauge_t *pNewWidget = NULL;
    xListNode *parentInfo;
    xListNode *parentList = NULL;
    arm_2d_tile_t *tResTile;

    parentInfo = ldGetWidgetInfoById(parentNameId);
    pNewWidget = LD_MALLOC_WIDGET_INFO(ldGauge_t);
    if (pNewWidget != NULL)
    {
        pNewWidget->isParentHidden=false;
        
        parentList = ((ldCommon_t *)parentInfo->info)->childList;
        if(((ldCommon_t *)parentInfo->info)->isHidden||((ldCommon_t *)parentInfo->info)->isParentHidden)
        {
            pNewWidget->isParentHidden=true;
        }
        pNewWidget->nameId = nameId;
        pNewWidget->childList = NULL;
        pNewWidget->widgetType = widgetTypeGauge;
        xListInfoAdd(parentList, pNewWidget);
        pNewWidget->parentWidget = parentInfo->info;
        pNewWidget->isHidden = false;

        tResTile=(arm_2d_tile_t*)&pNewWidget->resource;
        tResTile->tRegion.tLocation.iX=x;
        tResTile->tRegion.tLocation.iY=y;
        tResTile->tRegion.tSize.iWidth=width;
        tResTile->tRegion.tSize.iHeight=height;
        tResTile->tInfo.bIsRoot = true;
        tResTile->tInfo.bHasEnforcedColour = true;
        tResTile->tInfo.tColourInfo.chScheme = ARM_2D_COLOUR;
        tResTile->pchBuffer = (uint8_t*)LD_ADDR_NONE;
#if USE_VIRTUAL_RESOURCE == 1
        tResTile->tInfo.bVirtualResource = true;
        ((arm_2d_vres_t*)tResTile)->pTarget = LD_ADDR_NONE;
        ((arm_2d_vres_t*)tResTile)->Load = &__disp_adapter0_vres_asset_loader;
        ((arm_2d_vres_t*)tResTile)->Depose = &__disp_adapter0_vres_buffer_deposer;
#endif

        pNewWidget->pointerImgAddr=LD_ADDR_NONE;
        pNewWidget->bgImgAddr=bgImgAddr;
        pNewWidget->isWithBgMask=isBgMask;
        pNewWidget->pointerImgType=0;
        pNewWidget->centreOffsetX=0;
        pNewWidget->centreOffsetY=0;
        pNewWidget->pointerOriginOffsetX=0;
        pNewWidget->pointerOriginOffsetY=0;
        pNewWidget->angle_x10=0;

        memset(&pNewWidget->op, 0, sizeof (pNewWidget->op));
//        ARM_2D_OP_INIT(pNewWidget->op);
//        arm_2d_helper_transform_init(&pNewWidget->helper,
//           (arm_2d_op_t *)&pNewWidget->op,
//           0.01f,
//           0.1f,
//           &this.use_as__arm_2d_scene_t.ptDirtyRegion);

        LOG_INFO("[gauge] init,id:%d\n",nameId);
    }
    else
    {
        ldFree(pNewWidget);

        LOG_ERROR("[gauge] init failed,id:%d\n",nameId);
    }

    return pNewWidget;
}

/**
 * @brief   gauge循环处理函数
 *
 * @param   pWidget         目标控件指针
 * @param   pParentTile     父控件tile对象
 * @param   bIsNewFrame     新的一帧开始标志
 * @author  Ou Jianbo(59935554@qq.com)
 * @date    2023-11-09
 */
void ldGaugeLoop(ldGauge_t *pWidget,const arm_2d_tile_t *pParentTile,bool bIsNewFrame)
{
    arm_2d_tile_t *pResTile=(arm_2d_tile_t*)&pWidget->resource;

    if (pWidget == NULL)
    {
        return;
    }

    if((pWidget->isParentHidden)||(pWidget->isHidden))
    {
        return;
    }

    arm_2d_region_t newRegion=ldBaseGetGlobalRegion((ldCommon_t*)pWidget,&pResTile->tRegion);

    arm_2d_container(pParentTile,tTarget , &newRegion)
    {
        pResTile->pchBuffer = (uint8_t *)pWidget->bgImgAddr;
#if USE_VIRTUAL_RESOURCE == 1
        ((arm_2d_vres_t*)pResTile)->pTarget=pWidget->bgImgAddr;
#endif
        pResTile->tInfo.tColourInfo.chScheme = ARM_2D_COLOUR;

        ldBaseImage(&tTarget,pResTile,pWidget->isWithBgMask,255);
        arm_2d_op_wait_async(NULL);

        do {


#if USE_VIRTUAL_RESOURCE == 0
            arm_2d_tile_t tempRes;
#else
            arm_2d_vres_t tempRes;
#endif
            tempRes=pWidget->resource;
            arm_2d_tile_t *pTempRes=(arm_2d_tile_t*)&tempRes;

            pTempRes->tRegion.tLocation.iX=0;
            pTempRes->tRegion.tLocation.iY=0;

            pTempRes->tRegion.tSize.iWidth=pWidget->pointerWidth;
            pTempRes->tRegion.tSize.iHeight=pWidget->pointerHeight;

            pTempRes->tInfo.bIsRoot=true;
            pTempRes->bHasEnforcedColour=true;

            pTempRes->pchBuffer = (uint8_t *)pWidget->pointerImgAddr;
#if USE_VIRTUAL_RESOURCE == 1
            ((arm_2d_vres_t*)pTempRes)->pTarget=pWidget->pointerImgAddr;
#endif
            pTempRes->tInfo.tColourInfo.chScheme = ARM_2D_COLOUR;

            arm_2d_location_t targetCentre =
            {
                .iX = pWidget->pointerOriginOffsetX,
                .iY = pWidget->pointerOriginOffsetY,
            };

            tTarget_canvas.tLocation.iX=pWidget->centreOffsetX;
            tTarget_canvas.tLocation.iY=pWidget->centreOffsetY;

            switch (pWidget->pointerImgType) {
            case withMask:
            {
#if USE_VIRTUAL_RESOURCE == 0
            arm_2d_tile_t maskTile;
#else
            arm_2d_vres_t maskTile;
#endif
                maskTile=tempRes;
                (*(arm_2d_tile_t*)(&maskTile)).tInfo.tColourInfo.chScheme = ARM_2D_COLOUR_8BIT;
#if LD_CFG_COLOR_DEPTH == 8
                (*(arm_2d_tile_t*)(&maskTile)).pchBuffer += (*(arm_2d_tile_t*)(&maskTile)).tRegion.tSize.iWidth * (*(arm_2d_tile_t*)(&maskTile)).tRegion.tSize.iHeight;
#endif
#if LD_CFG_COLOR_DEPTH == 16
                (*(arm_2d_tile_t*)(&maskTile)).pchBuffer += (*(arm_2d_tile_t*)(&maskTile)).tRegion.tSize.iWidth * (*(arm_2d_tile_t*)(&maskTile)).tRegion.tSize.iHeight * 2;
#endif
#if LD_CFG_COLOR_DEPTH == 32
                (*(arm_2d_tile_t*)(&maskTile)).pchBuffer += (*(arm_2d_tile_t*)(&maskTile)).tRegion.tSize.iWidth * (*(arm_2d_tile_t*)(&maskTile)).tRegion.tSize.iHeight * 4;
#endif
#if USE_VIRTUAL_RESOURCE == 1
                maskTile.pTarget=(uintptr_t)maskTile.tTile.pchBuffer;
#endif
                arm_2dp_tile_transform_with_src_mask_and_opacity(&pWidget->op,
                                                                 pTempRes,
                                                                 (arm_2d_tile_t*)&maskTile,
                                                                 &tTarget,
                                                                 &tTarget_canvas,
                                                                 targetCentre,
                                                                 ANGLE_2_RADIAN((pWidget->angle_x10)/10.0+ANGLE_OFFSET),
                                                                 1.0,
                                                                 255
                                                                 );
                break;
            }
            case onlyMask:
            {
                pTempRes->tInfo.tColourInfo.chScheme = ARM_2D_COLOUR_8BIT;

                arm_2dp_fill_colour_with_mask_opacity_and_transform((arm_2d_op_trans_opa_t *)&pWidget->op,
                                                                    pTempRes,
                                                                    &tTarget,
                                                                    &tTarget_canvas,
                                                                    targetCentre,
                                                                    ANGLE_2_RADIAN((pWidget->angle_x10)/10.0+ANGLE_OFFSET),//pWidget->helper.fAngle,
                                                                    1.0,//pWidget->helper.fScale,
                                                                    pWidget->keyingOrMaskColor,
                                                                    255);
                break;
            }
            case keying:
            {
                arm_2dp_tile_rotation_with_colour_keying((arm_2d_op_trans_t *)&pWidget->op,
                                                         pTempRes,
                                                         &tTarget,
                                                         &tTarget_canvas,
                                                         targetCentre,
                                                         ANGLE_2_RADIAN((pWidget->angle_x10)/10.0+ANGLE_OFFSET),
                                                         __RGB(255,255,255)
                                                         );
                //                arm_2dp_tile_transform_with_colour_keying((arm_2d_op_trans_t *)&pWidget->op,
                //                                                          pTempRes,
                //                                                          &tTarget,
                //                                                          &tTarget_canvas,
                //                                                          targetCentre,
                //                                                          ANGLE_2_RADIAN((pWidget->angle_x10)/10.0+ANGLE_OFFSET),
                //                                                          1.0,
                //                                                          __RGB(255,255,255)
                //                                                          );
                break;
            }
            default:
            {
                arm_2dp_tile_transform_only((arm_2d_op_trans_t *)&pWidget->op,
                                            pTempRes,
                                            &tTarget,
                                            &tTarget_canvas,
                                            targetCentre,
                                            ANGLE_2_RADIAN((pWidget->angle_x10)/10.0+ANGLE_OFFSET),
                                            1.0
                                            );
                break;
            }
            }





            arm_2d_op_wait_async(NULL);

//            arm_2d_helper_transform_update_dirty_regions(
//                        &pWidget->helper,
//                    &__centre_region,
//                    bIsNewFrame);


//            arm_2d_op_wait_async(&pWidget->op.use_as__arm_2d_op_core_t);
        } while(0);
    }
}

/**
 * @brief   设置指针图片
 * 
 * @param   pWidget              目标控件指针
 * @param   pointerImgAddr       指针图片地址
 * @param   pointerWidth         指针宽度
 * @param   pointerHeight        指针高度
 * @param   pointerOriginOffsetX 指针原点坐标x轴(相对本指针图片)
 * @param   pointerOriginOffsetY 指针原点坐标y轴(相对本指针图片)
 * @author  Ou Jianbo(59935554@qq.com)
 * @date    2023-11-13
 */
void ldGaugeSetPointerImage(ldGauge_t *pWidget,uint32_t pointerImgAddr,int16_t pointerWidth,int16_t pointerHeight,int16_t pointerOriginOffsetX,int16_t pointerOriginOffsetY)
{
    if (pWidget == NULL)
    {
        return;
    }
    pWidget->pointerImgAddr=pointerImgAddr;
    pWidget->pointerWidth=pointerWidth;
    pWidget->pointerHeight=pointerHeight;
    pWidget->pointerOriginOffsetX=pointerOriginOffsetX;
    pWidget->pointerOriginOffsetY=pointerOriginOffsetY;
}

/**
 * @brief   设置仪表盘的旋转中心点
 * 
 * @param   pWidget         目标控件指针
 * @param   centreOffsetX   中心点坐标x轴
 * @param   centreOffsetY   中心点坐标y轴
 * @author  Ou Jianbo(59935554@qq.com)
 * @date    2023-11-13
 */
void ldGaugeSetCenterOffset(ldGauge_t *pWidget, int16_t centreOffsetX, int16_t centreOffsetY)
{
    if (pWidget == NULL)
    {
        return;
    }
    pWidget->centreOffsetX=centreOffsetX;
    pWidget->centreOffsetY=centreOffsetY;
}

/**
 * @brief   仪表盘的指针显示角度
 * 
 * @param   pWidget         目标控件指针
 * @param   angle           旋转角度，小数点后1位有效
 * @author  Ou Jianbo(59935554@qq.com)
 * @date    2023-11-13
 */
void ldGaugeSetAngle(ldGauge_t *pWidget, float angle)
{
    if (pWidget == NULL)
    {
        return;
    }
    pWidget->angle_x10=(int16_t)(angle*10);
    pWidget->angle_x10%=3600;
}

/**
 * @brief   指针图片的素材类型
 * 
 * @param   pWidget           目标控件指针
 * @param   pointerImgType    nomal,withMask,onlyMask,keying
 * @param   keyingOrMaskColor 颜色
 * @author  Ou Jianbo(59935554@qq.com)
 * @date    2023-11-13
 */
void ldGaugeSetPointerImageType(ldGauge_t *pWidget,gaugePointerType_t pointerImgType,ldColor keyingOrMaskColor)
{
    if (pWidget == NULL)
    {
        return;
    }

    pWidget->pointerImgType=pointerImgType;
    pWidget->keyingOrMaskColor=keyingOrMaskColor;
}
#if defined(__clang__)
#pragma clang diagnostic pop
#endif
