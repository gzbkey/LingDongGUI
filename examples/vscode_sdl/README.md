## 软件准备

### VSCode

https://code.visualstudio.com/Download

安装插件c/c++、Code Runner

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

## VSCode SDL Demo 的使用

### 安装 gcc

我使用的是MinGW-w64

https://www.mingw-w64.org/

然后在这网站，找到这里

https://github.com/niXman/mingw-builds-binaries/releases/

i686是32位

x86_64是64位

选择参考

686-13.1.0-release-win32-dwarf-ucrt-rt_v11-rev1.7z

x86_64-13.1.0-release-win32-seh-ucrt-rt_v11-rev1.7z

demo默认gcc安装位置为：

D:\mingw32

D:\mingw64

根据自己需求选择即可

### 修改配置

1. 通过vscode打开项目文件夹

2. 修改对应版本的compilerPath

    .vscode\c_cpp_properties.json

3. 修改对应版本的miDebuggerPath

    .vscode\launch.json

4. 修改对应版本的command

    .vscode\tasks.json

### 编译

1. 打开main.c

2. 点击左侧按键"运行和调试"，选择对应版本编译即可

3. 项目编译前，会自动复制SDL2的dll文件到对应的文件内，无需手动操作。
