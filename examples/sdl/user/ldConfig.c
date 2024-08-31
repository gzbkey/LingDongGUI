#include "ldConfig.h"
#include "arm_2d_types.h"
#include "ldMem.h"
#include "Virtual_TFT_Port.h"
#include <time.h>
#include "virtualNor.h"

/**
 * @brief   获取触摸坐标
 * 
 * @param   x 返回的x坐标
 * @param   y 返回的y坐标
 * @return  true 有效触摸
 * @return  false 无效触摸
 */
bool ldCfgTouchGetPoint(int16_t *x,int16_t *y)
{
    bool touchState=false;
    int16_t rx;
    int16_t ry;
    
    //添加触摸函数
    touchState=vtMouseGetPoint(&rx,&ry);

    if((touchState!=0)&&(((rx!=-1)&&(ry!=-1))||((rx!=0)&&(ry!=0))))
    {
        if(rx<0)
        {
            rx=0;
        }
        if(ry<0)
        {
            ry=0;
        }
        if(rx>LD_CFG_SCEEN_WIDTH)
        {
            rx=LD_CFG_SCEEN_WIDTH;
        }
        if(ry>LD_CFG_SCEEN_HEIGHT)
        {
            ry=LD_CFG_SCEEN_HEIGHT;
        }
        *x=rx;
        *y=ry;
        touchState=true;
    }
    else
    {
        touchState=false;
        *x=-1;
        *y=-1;
    }
    return touchState;
}

#if USE_VIRTUAL_RESOURCE == 1
void __disp_adapter0_vres_read_memory( intptr_t pObj,
                                       void *pBuffer,
                                       uintptr_t pAddress,
                                       size_t nSizeInByte)
{
    (void)pObj;
    norRead(pBuffer,pAddress,nSizeInByte);
}

uintptr_t __disp_adapter0_vres_get_asset_address(
    uintptr_t pObj,
    arm_2d_vres_t *ptVRES)
{
    (void)ptVRES;
    return pObj;
}
#endif

void Disp0_DrawBitmap (uint32_t x,uint32_t y,uint32_t width,uint32_t height,const uint8_t *bitmap) 
{
    //对接屏幕驱动的彩色填充函数
    //参考1
    //函数原型 void lcd_colorFill(uint16_t x0,uint16_t y0,uint16_t x1,uint16_t y1,uint16_t *color)
    //填写 lcd_colorFill(x,y,x+width-1,y+height-1,(uint16_t *)bitmap);
    //参考2
    //函数原型 void lcd_colorFill(uint16_t x,uint16_t y,uint16_t width,uint16_t height,uint16_t *color)
    //填写 lcd_colorFill(x,y,width,height,(uint16_t *)bitmap);
    vtFillMultipleColors(x, y, x + width - 1, y + height - 1, (color_typedef *)bitmap);
}

#if 0
void __disp_adapter0_request_async_flushing(
        void *pTarget,
        bool bIsNewFrame,
        int16_t iX,
        int16_t iY,
        int16_t iWidth,
        int16_t iHeight,
        const COLOUR_INT *pBuffer)
{

    VT_Fill_Multiple_Colors(iX, iY,iX+iWidth-1,iY+iHeight-1,(color_typedef*) pBuffer);
    s_bRequestAsyncFlush = true;
}
#endif

#if defined(_POSIX_VERSION) || defined(CLOCK_MONOTONIC) || defined(__APPLE__)
int64_t arm_2d_helper_get_system_timestamp(void)
{
    struct timespec timestamp;
    clock_gettime(CLOCK_MONOTONIC, &timestamp);

    return 1000000ul * timestamp.tv_sec + timestamp.tv_nsec / 1000ul;
}

uint32_t arm_2d_helper_get_reference_clock_frequency(void)
{
    return 1000000ul;
}
#else
#if defined (__riscv)
int64_t arm_2d_helper_get_system_timestamp(void)
{
    return (int64_t)clock();
}

uint32_t arm_2d_helper_get_reference_clock_frequency(void)
{
    return CLOCKS_PER_SEC;
}
#endif
#endif

#if ( !__x86_64__ && !__i386__ && !__APPLE__ )

void *__arm_2d_allocate_scratch_memory( uint32_t wSize, 
                                        uint_fast8_t nAlign,
                                        arm_2d_mem_type_t tType)
{
    ARM_2D_UNUSED(nAlign);
    ARM_2D_UNUSED(tType);

    /* ensure nAlign is 2^n */
    assert((((~nAlign) + 1) & nAlign) == nAlign);

    void *pBuff = ldCalloc(1,wSize);
    assert(0 == ((uintptr_t)pBuff & (nAlign - 1)));
    
    return pBuff;
}

void __arm_2d_free_scratch_memory( arm_2d_mem_type_t tType,
                                   void *pBuff)
{
    ARM_2D_UNUSED(tType);

    ldFree(pBuff);
}

#if __DISP0_CFG_VIRTUAL_RESOURCE_HELPER__

void * __disp_adapter0_aligned_malloc(size_t nSize, size_t nAlign)
{
    ARM_2D_UNUSED(nAlign);

    /* ensure nAlign is 2^n */
    assert((((~nAlign) + 1) & nAlign) == nAlign);

    void * pMem = ldCalloc(1,nSize);
    assert( 0 == ((uintptr_t)pMem & (nAlign - 1)));
    return pMem;
}

void __disp_adapter0_free(void *pMem)
{
    if (NULL != pMem) {
        ldFree(pMem);
    }
}

#endif

#endif
