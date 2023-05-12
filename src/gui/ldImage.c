#include "ldImage.h"
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

static bool _imageDel(xListInfo* pEachInfo,void* pTarget)
{
    if(pEachInfo->info==pTarget)
                {
                    xListDelete((xListNode *)pEachInfo);
                    ldFree(pEachInfo);
                    
                    ldFree(((ldImage *)pTarget)->imgList);
                    ldFree(((ldImage *)pTarget));
                }
    return false;
}

void pImageDel(ldImage *widget)
{
    xListInfo *listInfo;
    
    listInfo=ldGetWidgetInfoById(widget->nameId);
    
    if(listInfo!=NULL)
    {
        if((ldCommon*)widget->parentWidget==NULL)
        {
            //自身是根控件
            xListInfoPrevTraverse(&ldWidgetLink,widget,_imageDel);
        }
        else
        {
            xListInfoPrevTraverse(&listInfo->parentNode,widget,_imageDel);
        }
    }
}

ldImage* ldImageInit(uint16_t nameId, uint16_t parentNameId, int16_t x,int16_t y,int16_t width,int16_t height,ldColor bgColor,uint32_t imageAddr,uint16_t maxImageNum,bool isPng,bool isJpg,bool isHidden)
{
    ldImage * pNewWidget = NULL;
    xListInfo *parentInfo;
    xListNode* parent_link;
    uint32_t *pImgList;
    uint32_t jpegAddr,jpegSize;
        
    parentInfo=ldGetWidgetInfoById(parentNameId);
    
    if(parentInfo)
    {
        parent_link=((ldCommon*)parentInfo->info)->childList;
    }
    else
    {
        parent_link=&ldWidgetLink;
    }
    
        pNewWidget=LD_MALLOC_WIDGET_INFO(ldImage);
        if(maxImageNum>1)
        {
            pImgList=(uint32_t*)ldMalloc(maxImageNum*(sizeof(uint32_t)));
        }
        if(((pNewWidget!=NULL)&&(maxImageNum<=1))||((pNewWidget!=NULL)&&(pImgList!=NULL)&&(maxImageNum>1)))
        {
            pNewWidget->nameId=nameId;
            pNewWidget->childList=NULL;

            pNewWidget->widgetType=widgetTypeImage;
            pNewWidget->geometry.x=x;
            pNewWidget->geometry.y=y;
            pNewWidget->geometry.width=width;
            pNewWidget->geometry.height=height;

            xListInfoAdd(parent_link,pNewWidget);
            if(parent_link==&ldWidgetLink)//自身为bg
            {
                pNewWidget->parentWidget=NULL;
                pNewWidget->parentType=widgetTypeNone;
            }
            else
            {
                pNewWidget->parentType=((ldCommon*)(parentInfo->info))->widgetType;
                pNewWidget->parentWidget=parentInfo->info;
            }
            

//            pNewWidget->deleteFunc=nImageDelete;
//            pNewWidget->actionFunc=llImageAction;
//            pNewWidget->refreshFunc=nImageRefresh;

            pNewWidget->RES_TILE(resource).tRegion.tSize.iWidth=width;
            pNewWidget->RES_TILE(resource).tRegion.tSize.iHeight=height;
            pNewWidget->RES_TILE(resource).tInfo.bIsRoot=true;
            pNewWidget->RES_TILE(resource).tInfo.bHasEnforcedColour=true;
            pNewWidget->RES_TILE(resource).tInfo.tColourInfo.chScheme=ARM_2D_COLOUR;
#if __DISP0_CFG_VIRTUAL_RESOURCE_HELPER__
            pNewWidget->RES_TILE(resource).tInfo.bVirtualResource=true;
            pNewWidget->resource.Load=&__disp_adapter0_vres_asset_loader;
            pNewWidget->resource.Depose=&__disp_adapter0_vres_buffer_deposer;
#endif
//            if(isJpg)
//            {
//#if __DISP0_CFG_VIRTUAL_RESOURCE_HELPER__
//                
//                
//                
//#else
//                
//                
//                JPEG_initStruct.DoneIEn = 0;
//    JPEG_initStruct.ErrorIEn = 0;
//    JPEG_Init(JPEG, &JPEG_initStruct);
//    jpeg_outset.format = JPEG_OUT_RGB565;
//    jpeg_outset.dither = 0;
//    jpeg_outset.RGBAddr = JPEG_BUF_2;
//                
//                
//                jpegSize=gGetSdramU32(imageAddr);
//                //jpegSize=*(uint32_t*)imageAddr;
//            jpegAddr=imageAddr+4;
//                
//                memcpy((uint8_t*)JPEG_BUF, (uint8_t*)jpegAddr, jpegSize);
//        jfif_parse((uint8_t*)JPEG_BUF, jpegSize, &jfif_info);

//                
//                
//                JPEG_Decode(JPEG, &jfif_info, &jpeg_outset);
//        while(JPEG_DecodeBusy(JPEG)) __NOP();
//        

//pNewWidget->RES_IMG_ADDR(resource)=(uint8_t*)JPEG_BUF_2;
//                
//#endif
//            }
//            else
            {
                pNewWidget->RES_IMG_ADDR(resource)=(uint8_t*)imageAddr;
            }

            pNewWidget->isJpg=isJpg;
            pNewWidget->bgColor=bgColor;
            pNewWidget->imgMax=maxImageNum;
            pNewWidget->imgCount=0;
            pNewWidget->imgNow=0;
            pNewWidget->speedMs=0;
            pNewWidget->isPng=isPng;
            pNewWidget->isHidden=isHidden;
            
            pNewWidget->textAlpha=255;
            
            if(maxImageNum>1)
            {
                pNewWidget->imgList=pImgList;
                pNewWidget->imgList[0]=imageAddr;
                pNewWidget->imgCount=1;
            }

        }
        else
        {
            ldFree(pNewWidget);
            ldFree(pImgList);
        }
    
    

    return pNewWidget;
}

void ldImageLoop(ldImage *info,const arm_2d_tile_t *ptParent,bool bIsNewFrame)
{
    uint32_t jpegAddr,jpegSize;
//    jpeg_outset_t jpeg_outset;
//    JPEG_InitStructure JPEG_initStruct;
//    jfif_info_t jfif_info;
    
    assert(NULL != ptParent);

    arm_2d_tile_t tTarget;

    if(info==NULL)
    {
        return;
    }
    
    if (NULL != arm_2d_tile_generate_child( ptParent,
                                            &info->RES_TILE(resource).tRegion,
                                            &tTarget,
                                            false))
    {
        tTarget.tRegion.tLocation.iX=info->geometry.x;
        tTarget.tRegion.tLocation.iY=info->geometry.y;
    }
    else
    {
        return;
    }
    
    if(info->imgMax>0)
    {
    if(info->isInitEnd==false)
    {
        info->isInitEnd=true;
        info->_lastTimeLog = arm_2d_helper_get_system_timestamp();
        info->_timeUnit=(uint32_t)arm_2d_helper_convert_ms_to_ticks(info->speedMs);
    }
    
    if(info->imgCount>1)
    {
        if (bIsNewFrame)
                {
                    if(info->speedMs>0)
                    {
                    int64_t lClocks = arm_2d_helper_get_system_timestamp();
                    int32_t nElapsed = (int32_t)((lClocks - info->_lastTimeLog));

                    if (nElapsed >= info->_timeUnit)
                    {
                        info->_lastTimeLog = lClocks;
                        
                        //切换下一张图
                        info->imgNow++;
                        if(info->imgNow>=info->imgCount)
                        {
                            info->imgNow=0;
                        }
                        
//                        if(info->isJpg)
//                        {
//                            JPEG_initStruct.DoneIEn = 0;
//    JPEG_initStruct.ErrorIEn = 0;
//    JPEG_Init(JPEG, &JPEG_initStruct);
//    jpeg_outset.format = JPEG_OUT_RGB565;
//    jpeg_outset.dither = 0;
//    jpeg_outset.RGBAddr = JPEG_BUF_2;
//                
//                            jpegSize=gGetSdramU32(info->imgList[info->imgNow]);
//                //jpegSize=*(uint32_t*)info->imgList[info->imgNow];
//            jpegAddr=info->imgList[info->imgNow]+4;
//                
//                memcpy((uint8_t*)JPEG_BUF, (uint8_t*)jpegAddr, jpegSize);
//        jfif_parse((uint8_t*)JPEG_BUF, jpegSize, &jfif_info);

//                
//                
//                JPEG_Decode(JPEG, &jfif_info, &jpeg_outset);
//        while(JPEG_DecodeBusy(JPEG)) __NOP();
//                        }
//                        else
//                        {
                       info->RES_IMG_ADDR(resource)=(uint8_t*) info->imgList[info->imgNow];
//                        }
                    }
                    }
                    else
                    {
                        //切换下一张图
                        info->imgNow++;
                        if(info->imgNow>=info->imgCount)
                        {
                            info->imgNow=0;
                        }
//                        if(info->isJpg)
//                        {
//                            JPEG_initStruct.DoneIEn = 0;
//    JPEG_initStruct.ErrorIEn = 0;
//    JPEG_Init(JPEG, &JPEG_initStruct);
//    jpeg_outset.format = JPEG_OUT_RGB565;
//    jpeg_outset.dither = 0;
//    jpeg_outset.RGBAddr = JPEG_BUF_2;
//                
//                            jpegSize=gGetSdramU32(info->imgList[info->imgNow]);
//                //jpegSize=*(uint32_t*)info->imgList[info->imgNow];
//            jpegAddr=info->imgList[info->imgNow]+4;
//                
//                memcpy((uint8_t*)JPEG_BUF, (uint8_t*)jpegAddr, jpegSize);
//        jfif_parse((uint8_t*)JPEG_BUF, jpegSize, &jfif_info);

//                
//                
//                JPEG_Decode(JPEG, &jfif_info, &jpeg_outset);
//        while(JPEG_DecodeBusy(JPEG)) __NOP();
//                        }
//                        else
//                        {
                       info->RES_IMG_ADDR(resource)=(uint8_t*) info->imgList[info->imgNow];
//                            }
                    }
                }
    }
    

        if(info->isPng)
        {
#if __DISP0_CFG_VIRTUAL_RESOURCE_HELPER__
            arm_2d_vres_t srcTile,maskTile;
    
            srcTile=info->resource;
            srcTile.tTile.tInfo.tColourInfo.chScheme=ARM_2D_COLOUR;
            srcTile.pTarget+=srcTile.tTile.tRegion.tSize.iWidth*srcTile.tTile.tRegion.tSize.iHeight;
    
            maskTile=info->resource;
            maskTile.tTile.tInfo.tColourInfo.chScheme=ARM_2D_COLOUR_8BIT;
#else
            arm_2d_tile_t srcTile,maskTile;
    
            srcTile=info->resource;
            srcTile.tInfo.tColourInfo.chScheme=ARM_2D_COLOUR;
            srcTile.pchBuffer+=srcTile.tRegion.tSize.iWidth*srcTile.tRegion.tSize.iHeight;
    
            maskTile=info->resource;
            maskTile.tInfo.tColourInfo.chScheme=ARM_2D_COLOUR_8BIT;
#endif
            arm_2d_tile_copy_with_src_mask_only( (arm_2d_tile_t*)&srcTile,
                                        (arm_2d_tile_t*)&maskTile,
                                        &tTarget,
                                        NULL);
        }
        else
        {
        arm_2d_region_t tDrawRegion = 
    {
        .tLocation = 
        {
            .iX = 0, 
            .iY = 0,
        },
        .tSize = info->RES_TILE(resource).tRegion.tSize,
    };
    
        switch(info->RES_TILE(resource).tInfo.tColourInfo.chScheme)
        {
            case ARM_2D_COLOUR_1BIT:
            {
                arm_2d_draw_pattern((arm_2d_tile_t*)&info->resource,
                                    &tTarget,
                                    &tDrawRegion,
                                    ARM_2D_DRW_PATN_MODE_COPY,
                                    info->specialColor,
                                    GLCD_COLOR_BLACK);
            
                break;
            }
            case ARM_2D_COLOUR_8BIT:
            {
                arm_2d_fill_colour_with_mask_and_opacity(&tTarget,
                                            &tDrawRegion,
                                            (arm_2d_tile_t*)&info->resource,
                                            (__arm_2d_color_t) {info->specialColor},
                                            info->textAlpha);
                break;
            }
            case ARM_2D_COLOUR_RGB565:
            {
                arm_2d_tile_copy_only((arm_2d_tile_t*)&info->resource,
                                  &tTarget,
                                  NULL);
                break;
            }
        
            default:
                break;
        }
    }   

    
    }
    else
    {
        arm_2d_fill_colour(&tTarget, NULL, info->bgColor);
    }
    arm_2d_op_wait_async(NULL);
}

void pImageSetAddr(ldImage *info,uint32_t addr)
{
    if(info==NULL)
    {
        return;
    }
    
    info->RES_IMG_ADDR(resource)=(uint8_t*)addr;
}

void nImageSetAddr(uint16_t nameId,uint32_t addr)
{
    xListInfo* pListInfo;
    pListInfo=ldGetWidgetInfoById(nameId);
    pImageSetAddr(pListInfo->info,addr);
}

bool pImageAddList(ldImage *info,uint32_t addr,uint16_t speedMs)
{
    if(info==NULL)
    {
        return false;
    }
    
    if(info->imgCount<info->imgMax)
    {
        info->speedMs=speedMs;
        info->imgList[info->imgCount]=addr;
        info->imgCount++;
        return true;
    }
    return false;
}

bool nImageAddList(uint16_t nameId,uint32_t addr,uint16_t speedMs)
{
    xListInfo* pListInfo;
    pListInfo=ldGetWidgetInfoById(nameId);
    return pImageAddList(pListInfo->info,addr,speedMs);
}

void pImageSetColorType(ldImage *info,uint8_t colorType,ldColor writeColor)
{
    if(info==NULL)
    {
        return;
    }
    
    info->RES_TILE(resource).tColourInfo.chScheme=colorType;
    info->specialColor=writeColor;
}

void nImageSetColorType(uint16_t nameId,uint8_t colorType,ldColor writeColor)
{
    xListInfo* pListInfo;
    pListInfo=ldGetWidgetInfoById(nameId);
    pImageSetColorType(pListInfo->info,colorType,writeColor);
}



#if defined(__clang__)
#   pragma clang diagnostic pop
#endif

