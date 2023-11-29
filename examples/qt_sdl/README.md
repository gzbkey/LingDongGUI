## 软件准备

### Qt

当前项目使用Qt 5.14.2(32bit 64bit)编译通过

https://download.qt.io/archive/qt/5.14/5.14.2/qt-opensource-windows-x86-5.14.2.exe

此版本为官方离线安装版的最后一个版本。安装该软件时，***电脑断网***则无需登录即可完成安装。

### git

安装git，用于下载源码。(也可以手动下载源码，但不推荐)

https://git-scm.com/download/win

## 下载源码

### 方法1

##### 步骤1
//gitee

git clone https://gitee.com/gzbkey/LingDongGUI.git

//github 

git clone https://github.com/gzbkey/LingDongGUI.git

##### 步骤2

git submodule init

git submodule update

### 方法2

//gitee

git clone --recursive https://gitee.com/gzbkey/LingDongGUI.git

//github

git clone --recursive https://github.com/gzbkey/LingDongGUI.git

## Qt SDL Demo的使用

1、选择mingw编译器。

2、双击Virtual_TFT.pro，启动Qt。

3、项目打开后，会自动复制SDL2的dll文件到对应的文件内，无需手动操作。

4、直接点击左下角三角形，进行编译、运行。
