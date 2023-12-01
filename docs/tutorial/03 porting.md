## 移植

本文档以单片机为标准，说明移植过程

## Keil

安装好MDK-ARM，这里使用的版本是5.38。建议使用最新版本

### 移植arm-2d准备
 * 安装arm-2d的pack，最新版本[下载](https://github.com/ARM-software/Arm-2D/releases)
 * 安装perf_counter的pack，最新版本[下载](https://github.com/GorgonMeducer/perf_counter/releases)
 * 准备带屏幕的开发板，可以正常显示图片的keil项目(lcd_project)
 * 屏幕接口
    ```c 
    void Disp0_DrawBitmap (uint32_t x, 
                           uint32_t y, 
                           uint32_t width, 
                           uint32_t height, 
                           const uint8_t *bitmap)
    ```
 * 下载ldgui源码
🏠️主仓库: https://gitee.com/gzbkey/LingDongGUI
🏠️镜像仓库: https://github.com/gzbkey/LingDongGUI

|ℹ️ 关于github下载慢的问题|
|:----|
|推荐使用Watt Toolkit加速|

### 先让arm-2d跑起来

1. 在lcd_project中加入arm-2d、perf_counter、DSP、CMSIS，Project -> Manage -> Run-Time Environment

    ![keilPackSelect](../tutorial/images/03/arm2d%20Manage%20Run-Time%20Environment.png)

2. 选择ac6编译器，并且选择gnu11

    ![ac6Setting](../tutorial/images/03/ac6%20setting.png)

3. 如果使用ac5编译器，则需要选择c99和gnu支持，但是不建议使用ac5

    ![ac5Setting](../tutorial/images/03/ac5%20setting.png)

4. 确保keil的CMSIS版本不得低于5.7.0，查看方式，Project -> Manage -> Select Software Packs

    ![cmsisVersion](../tutorial/images/03/cmsis%20version.png)

5. 树目录中的Acceleration，找到arm_2d_disp_adapter_0.h。编辑器的左下角选择 Configuration Wizard，进入图形配置界面，根据实际情况配置
颜色位数（Screen Colour Depth）
横向分辨率（Width of the screen）
纵向分辨率（Height of the Screen）
部分刷新缓冲块的宽度（Width of the PFB Block），配置为width/2
部分刷新缓冲块的高度（Height of the PFB Block），配置为8

1. 树目录中的Acceleration，找到arm_2d_disp_adapter_0.h。编辑器的左下角选择 Configuration Wizard，进入图形配置界面，配置Extra下的colour depth、width和height

2. main文件加入如下代码
    ```c 
    #include "arm_2d.h"
    #include "arm_2d_disp_adapters.h"

    int main(void) 
    {
        system_init();     // 包括 LCD 在内的系统初始化
        ...
        arm_irq_safe
        {
            arm_2d_init(); // 初始化 arm-2d
        }

        disp_adapter0_init();
        while(1)
        {
            disp_adapter0_task();
        }
    }
    ```

3. 运行效果

   ![arm2d-demo](../tutorial/images/03/arm2d%20demo.gif)

### 加入ldgui

1. 
