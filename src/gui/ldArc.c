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
 * @file    ldArc.c
 * @author  Ou Jianbo(59935554@qq.com)
 * @brief   圆环控件
 * @version 0.1
 * @date    2023-12-30
 */

#include "ldArc.h"
#include "ldGui.h"
#include "ldWindow.h"

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

void ldArcDel(ldArc_t *pWidget);
void ldArcFrameUpdate(ldArc_t* pWidget);
void ldArcLoop(ldArc_t *pWidget,const arm_2d_tile_t *pParentTile,bool bIsNewFrame);
const ldGuiCommonFunc_t ldArcCommonFunc={
    ldArcDel,
    ldArcLoop,
    ldArcFrameUpdate,
};

static bool _arcDel(xListNode *pEachInfo, void *pTarget)
{
    if (pEachInfo->info == pTarget)
    {
        //del user object

        ldFree(((ldArc_t *)pTarget));
        xListInfoDel(pEachInfo);
    }
    return false;
}

void ldArcDel(ldArc_t *pWidget)
{
    xListNode *listInfo;

    if (pWidget == NULL)
    {
        return;
    }

    if(pWidget->widgetType!=widgetTypeArc)
    {
        return;
    }

    LOG_INFO("[arc] del,id:%d\n",pWidget->nameId);

    xDeleteConnect(pWidget->nameId);

    listInfo = ldBaseGetWidgetInfoById(((ldCommon_t *)pWidget->parentWidget)->nameId);
    listInfo = ((ldCommon_t *)listInfo->info)->childList;

    if (listInfo != NULL)
    {
        xListInfoPrevTraverse(listInfo, pWidget, _arcDel);
    }
}

void ldArcFrameUpdate(ldArc_t* pWidget)
{
    if(pWidget->dirtyRegionState==waitChange)
    {
        pWidget->dirtyRegionTemp=((arm_2d_tile_t*)&(pWidget->resource))->tRegion;
    }
    ldBaseDirtyRegionAutoUpdate((ldCommon_t*)pWidget,((arm_2d_tile_t*)&(pWidget->resource))->tRegion,pWidget->isDirtyRegionAutoIgnore);
}

/**
 * @brief   圆环控件的初始化函数
 *          圆环素材尺寸建议为单数的像素点，
 *          例如101x101的圆环，裁剪51x51的左上角图片作为素材
 * 
 * @param   nameId          新控件id
 * @param   parentNameId    父控件id
 * @param   x               相对坐标x轴
 * @param   y               相对坐标y轴
 * @param   width           控件宽度
 * @param   height          控件高度
 * @param   srcQuarterAddr  圆环素材左上角(四分之一),mask A8
 * @param   maskQuarterAddr 圆环素材左上角(四分之一),mask A8，遮挡区域比src的稍大
 * @param   parentColor     背景颜色
 * @return  ldArc_t*        新控件指针
 * @author  Ou Jianbo(59935554@qq.com)
 * @date    2023-12-30
 */
ldArc_t *ldArcInit(uint16_t nameId, uint16_t parentNameId, int16_t x, int16_t y, int16_t width, int16_t height,uint32_t srcQuarterAddr,uint32_t maskQuarterAddr,ldColor parentColor)
{
    ldArc_t *pNewWidget = NULL;
    xListNode *parentInfo;
    xListNode *parentList = NULL;
    arm_2d_tile_t *tResTile;

    parentInfo = ldBaseGetWidgetInfoById(parentNameId);
    pNewWidget = LD_CALLOC_WIDGET_INFO(ldArc_t);
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
        pNewWidget->widgetType = widgetTypeArc;
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
        pNewWidget->dirtyRegionListItem.ptNext = NULL;
        pNewWidget->dirtyRegionListItem.tRegion = ldBaseGetGlobalRegion((ldCommon_t *)pNewWidget,&((arm_2d_tile_t*)&pNewWidget->resource)->tRegion);
        pNewWidget->dirtyRegionListItem.bIgnore = false;
        pNewWidget->dirtyRegionListItem.bUpdated = true;
        pNewWidget->dirtyRegionState=waitChange;
        pNewWidget->dirtyRegionTemp=tResTile->tRegion;
        pNewWidget->isDirtyRegionAutoIgnore=false;

        pNewWidget->bgColor=LD_COLOR_LIGHT_GREY;
        pNewWidget->fgColor=LD_COLOR_LIGHT_BLUE;
        pNewWidget->parentColor=parentColor;
        pNewWidget->srcAddr=srcQuarterAddr;
        pNewWidget->maskAddr=maskQuarterAddr;

        pNewWidget->startAngle_x10[0]=0;
        pNewWidget->endAngle_x10[0]=3600;
        pNewWidget->startAngle_x10[1]=0;
        pNewWidget->endAngle_x10[1]=1800;
        pNewWidget->rotationAngle_x10=0;
        pNewWidget->pFunc=&ldArcCommonFunc;

        LOG_INFO("[arc] init,id:%d\n",nameId);
    }
    else
    {
        ldFree(pNewWidget);

        LOG_ERROR("[arc] init failed,id:%d\n",nameId);
    }

    return pNewWidget;
}

arm_2d_location_t _ldArcGetStartEndAreaPos(uint8_t quarterNum,arm_2d_size_t widgetFullSize)
{
    arm_2d_location_t retPos={0};
    switch (quarterNum)
    {
    case 7:
    case 3:
    {
        retPos.iX=0;
        retPos.iY=0;
        break;
    }
    case 8:
    case 4:
    case 0:
    {
        retPos.iX=widgetFullSize.iWidth>>1;
        retPos.iY=0;
        break;
    }
    case 5:
    case 1:
    {
        retPos.iX=widgetFullSize.iWidth>>1;
        retPos.iY=widgetFullSize.iHeight>>1;
        break;
    }
    case 6:
    case 2:
    {
        retPos.iX=0;
        retPos.iY=widgetFullSize.iHeight>>1;
        break;
    }
    }
    return retPos;
}

void ldArcLoop(ldArc_t *pWidget,const arm_2d_tile_t *pParentTile,bool bIsNewFrame)
{
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

    if((pWidget->isParentHidden)||(pWidget->isHidden))
    {
        return;
    }


    //         270                   0
    //          |                    |
    //        2 | 1                3 | 0
    //    180-------0    ==>>  270-------90
    //        3 | 4                2 | 1
    //          |                    |
    //         90                   180

    arm_2d_location_t center;
    arm_2d_region_t showRegion;
    float fStartAngle,fEndAngle;
    arm_2d_location_t maskCenter;
    uint8_t startQuarter,endQuarter,startQuarter0,endQuarter0;
    ldColor arcColor;

    arm_2d_region_t newRegion=ldBaseGetGlobalRegion((ldCommon_t*)pWidget,&pResTile->tRegion);

    arm_2d_container(pParentTile,tTarget , &newRegion)
    {
        //                static arm_2d_op_fill_cl_msk_opa_trans_t s_tMaskRotateCB0 = {0};
        //                static arm_2d_op_fill_cl_msk_opa_trans_t s_tMaskRotateCB90 = {0};
        //                static arm_2d_op_fill_cl_msk_opa_trans_t s_tMaskRotateCB180 = {0};
        //                static arm_2d_op_fill_cl_msk_opa_trans_t s_tMaskRotateCB270 = {0};

        ((arm_2d_tile_t*)&tempRes)->pchBuffer = (uint8_t *)pWidget->srcAddr;
#if USE_VIRTUAL_RESOURCE == 1
        ((arm_2d_vres_t*)(&tempRes))->pTarget=pWidget->srcAddr;
#endif
        ((arm_2d_tile_t*)&tempRes)->tInfo.tColourInfo.chScheme=ARM_2D_COLOUR_MASK_A8;

        ((arm_2d_tile_t*)&tempRes)->tRegion.tSize.iWidth=(((arm_2d_tile_t*)&tempRes)->tRegion.tSize.iWidth>>1)+1;
        ((arm_2d_tile_t*)&tempRes)->tRegion.tSize.iHeight=(((arm_2d_tile_t*)&tempRes)->tRegion.tSize.iHeight>>1)+1;

        center.iX=((arm_2d_tile_t*)&tempRes)->tRegion.tSize.iWidth-1;
        center.iY=((arm_2d_tile_t*)&tempRes)->tRegion.tSize.iHeight-1;

        maskCenter.iX=tTarget_canvas.tSize.iWidth>>1;
        maskCenter.iY=tTarget_canvas.tSize.iHeight>>1;

        for(uint8_t i=0;i<2;i++)
        {
            if((i==1)&&(pWidget->endAngle_x10[i]==0))
            {
                continue;
            }
            if(i==0)//bgArc
            {
                arcColor=pWidget->bgColor;
            }
            else//fgArc
            {
                arcColor=pWidget->fgColor;
            }
            fStartAngle=(float)(pWidget->startAngle_x10[i]+pWidget->rotationAngle_x10)/10.0;
            fEndAngle=(float)(pWidget->endAngle_x10[i]+pWidget->rotationAngle_x10)/10.0;
            startQuarter = fStartAngle / 90;
            endQuarter = fEndAngle / 90;
            startQuarter0=startQuarter;
            endQuarter0=endQuarter;

            showRegion.tSize.iWidth=(tTarget_canvas.tSize.iWidth>>1)+1;
            showRegion.tSize.iHeight=(tTarget_canvas.tSize.iHeight>>1)+1;

            if((startQuarter%4)==(endQuarter%4))//同一象限
            {
                showRegion.tLocation=_ldArcGetStartEndAreaPos (endQuarter,tTarget_canvas.tSize);
                if((fEndAngle-fStartAngle)>90)// 大于270度圆弧
                {
                    if(fEndAngle>=360.0)
                    {
                        fEndAngle-=360.0;
                    }
                    arm_2d_fill_colour_with_mask_opacity_and_transform((arm_2d_tile_t*)&tempRes,
                                                                       &tTarget,
                                                                       &showRegion,
                                                                       center,
                                                                       ARM_2D_ANGLE(fEndAngle),
                                                                       1.0f,
                                                                       arcColor,
                                                                       255,
                                                                       &maskCenter
                                                                       );

                    fStartAngle+=90;
                    if(fStartAngle>=360.0)
                    {
                        fStartAngle-=360.0;
                    }
                    arm_2d_fill_colour_with_mask_opacity_and_transform((arm_2d_tile_t*)&tempRes,
                                                                       &tTarget,
                                                                       &showRegion,
                                                                       center,
                                                                       ARM_2D_ANGLE(fStartAngle),
                                                                       1.0f,
                                                                       arcColor,
                                                                       255,
                                                                       &maskCenter
                                                                       );
                }
                else// 小于90度圆弧
                {
                    if(fEndAngle>=360.0)
                    {
                        fEndAngle-=360.0;
                    }
//                    if((fEndAngle!=360)&&(fEndAngle!=90)&&(fEndAngle!=180)&&(fEndAngle!=270))
                    {
                        arm_2d_fill_colour_with_mask_opacity_and_transform((arm_2d_tile_t*)&tempRes,
                                                                           &tTarget,
                                                                           &showRegion,
                                                                           center,
                                                                           ARM_2D_ANGLE(fEndAngle),
                                                                           1.0f,
                                                                           arcColor,
                                                                           255,
                                                                           &maskCenter
                                                                           );
                    }

                    if(fStartAngle>=360.0)
                    {
                        fStartAngle-=360.0;
                    }
                    if((fStartAngle==0)||(fStartAngle==90)||(fStartAngle==180)||(fStartAngle==270))
                    {
                        continue;
                    }
                    ((arm_2d_tile_t*)&tempRes)->pchBuffer = (uint8_t *)pWidget->maskAddr;
#if USE_VIRTUAL_RESOURCE == 1
                    ((arm_2d_vres_t*)(&tempRes))->pTarget=pWidget->maskAddr;
#endif
                    arm_2d_fill_colour_with_mask_opacity_and_transform((arm_2d_tile_t*)&tempRes,
                                                                       &tTarget,
                                                                       &showRegion,
                                                                       center,
                                                                       ARM_2D_ANGLE(fStartAngle),
                                                                       1.0f,
                                                                       pWidget->parentColor,
                                                                       255,
                                                                       &maskCenter
                                                                       );
                    ((arm_2d_tile_t*)&tempRes)->pchBuffer = (uint8_t *)pWidget->srcAddr;
#if USE_VIRTUAL_RESOURCE == 1
                    ((arm_2d_vres_t*)(&tempRes))->pTarget=pWidget->srcAddr;
#endif
                }
            }
            else
            {
                showRegion.tLocation=_ldArcGetStartEndAreaPos (endQuarter,tTarget_canvas.tSize);
                if(fEndAngle>=360.0)
                {
                    fEndAngle-=360.0;
                }
                arm_2d_fill_colour_with_mask_opacity_and_transform((arm_2d_tile_t*)&tempRes,
                                                                   &tTarget,
                                                                   &showRegion,
                                                                   center,
                                                                   ARM_2D_ANGLE(fEndAngle),
                                                                   1.0f,
                                                                   arcColor,
                                                                   255,
                                                                   &maskCenter
                                                                   );

                showRegion.tLocation=_ldArcGetStartEndAreaPos (startQuarter,tTarget_canvas.tSize);
                fStartAngle+=90;
                if(fStartAngle>=360.0)
                {
                    fStartAngle-=360.0;
                }
                arm_2d_fill_colour_with_mask_opacity_and_transform((arm_2d_tile_t*)&tempRes,
                                                                   &tTarget,
                                                                   &showRegion,
                                                                   center,
                                                                   ARM_2D_ANGLE(fStartAngle),
                                                                   1.0f,
                                                                   arcColor,
                                                                   255,
                                                                   &maskCenter
                                                                   );
            }
            arm_2d_op_wait_async(NULL);

            startQuarter0++;
            while(startQuarter0<endQuarter0)
            {
                startQuarter0++;
                arm_2d_fill_colour_with_mask_opacity_and_transform((arm_2d_tile_t*)&tempRes,
                                                                   &tTarget,
                                                                   NULL,
                                                                   center,
                                                                   ARM_2D_ANGLE((startQuarter0)*90.0),
                                                                   1.0f,
                                                                   arcColor,
                                                                   255
                                                                   );
            }
            arm_2d_op_wait_async(NULL);
        }
    }
}

/**
 * @brief   设定底层圆环角度范围
 * 
 * @param   pWidget         目标控件指针
 * @param   bgStart         底层圆环起始角度
 * @param   bgEnd           底层圆环结束角度
 * @author  Ou Jianbo(59935554@qq.com)
 * @date    2023-12-30
 */
void ldArcSetBgAngle(ldArc_t *pWidget,float bgStart,float bgEnd)
{
    if(pWidget==NULL)
    {
        return;
    }
    pWidget->dirtyRegionState=waitChange;
    pWidget->startAngle_x10[0]=bgStart*10;
    pWidget->endAngle_x10[0]=bgEnd*10;
    pWidget->startAngle_x10[1]=pWidget->startAngle_x10[0];
}

/**
 * @brief   设定顶层圆环角度范围
 * 
 * @param   pWidget         目标控件指针
 * @param   fgEnd           顶层圆环结束角度(1-359)
 *                          必须在底层圆环角度范围内
 *                          设定为0则屏蔽顶层圆环
 * @author  Ou Jianbo(59935554@qq.com)
 * @date    2023-12-30
 */
void ldArcSetFgAngle(ldArc_t *pWidget,float fgEnd)
{
    if(pWidget==NULL)
    {
        return;
    }
    pWidget->dirtyRegionState=waitChange;
    pWidget->startAngle_x10[1]=pWidget->startAngle_x10[0];
    pWidget->endAngle_x10[1]=fgEnd*10;
}

/**
 * @brief   设定圆环整体旋转角度
 * 
 * @param   pWidget         目标控件指针
 * @param   rotationAngle   旋转角度，0-359
 * @author  Ou Jianbo(59935554@qq.com)
 * @date    2023-12-30
 */
void ldArcSetRotationAngle(ldArc_t *pWidget,float rotationAngle)
{
    if(pWidget==NULL)
    {
        return;
    }
    pWidget->dirtyRegionState=waitChange;
    pWidget->rotationAngle_x10=rotationAngle*10;
}

/**
 * @brief   设定圆环颜色
 * 
 * @param   pWidget         目标控件指针
 * @param   bgColor         底层颜色
 * @param   fgColor         顶层颜色
 * @author  Ou Jianbo(59935554@qq.com)
 * @date    2023-12-30
 */
void ldArcSetColor(ldArc_t *pWidget,ldColor bgColor,ldColor fgColor)
{
    if(pWidget==NULL)
    {
        return;
    }
    pWidget->dirtyRegionState=waitChange;
    pWidget->bgColor=bgColor;
    pWidget->fgColor=fgColor;
}

//void ldArcSetDirection(ldArc_t *pWidget,bool isClockwise)
//{
//    if(pWidget==NULL)
//    {
//        return;
//    }
//    pWidget->isClockwise=isClockwise;
//}

#if defined(__clang__)
#pragma clang diagnostic pop
#endif
