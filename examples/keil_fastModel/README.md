# 使用前准备
0、重要信息
MDK是Professional License，可以使用CM4模拟

1、安装好keil

2、安装对应模拟芯片的pack

V2M-MPS2_CMx_BSP

V2M_MPS3_SSE_300_BSP

下载链接

https://www.keil.com/dd2/pack/

3、安装perf_counter

https://github.com/GorgonMeducer/perf_counter/tree/main/cmsis-pack

4、安装Arm-2D

https://github.com/ARM-software/Arm-2D/releases

5、调试器配置

keil中配置

Option for Target - Debug - Models Cortex-M Debugger - Settings

keil老版本的模拟器叫FVP

keil新版本的模拟器叫VHT

配置选项参考如下

.\FVP\MPS2_Cortex-M\FVP_MPS2_Cortex-M4_MDK.exe

.\VHT\VHT_MPS2_Cortex-M4.exe

.\FVP\Corstone_SSE-300\models\Win64_VC2017\FVP_Corstone_SSE-300_Ethos-U55.exe

.\VHT\VHT_Corstone_SSE-300_Ethos-U55.exe