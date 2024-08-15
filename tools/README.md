# 软件说明

基于arm-2d的工具脚本，进一步简化操作

## 图片数据生成脚本

### 依赖

```sh
pip install Pillow
pip install numpy
```

### 如何使用

#### 图片和脚本同一目录下
    python ./auto_img2c.py
#### 图片在特定文件夹中(如./img)
    python ./auto_img2c.py ./img

## 动态图片转PNG脚本

### 依赖

```sh
pip install Pillow
```

### 说明
    1. 脚本gif2png.py实际上可以处理任意动态图片，并且自动识别，输出png或者bmp文件。
    2. 运行后，会提示输入跳帧参数，不需要跳帧，则直接回车。
    3. 跳帧参数输入最小值是2

### 实例说明
原图如下

![原图](../docs/tools/demo.gif)

---
无跳帧效果

![无跳帧](../docs/tools/demo_frames_interval_0_out.png)

---
参数为2原理

![参数为2原理](../docs/tools/demo_frames_interval_2.png)

参数为3原理

![参数为3原理](../docs/tools/demo_frames_interval_3.png)

---
参数为2实际输出效果

![参数为2](../docs/tools/demo_frames_interval_2_out.png)

参数为3实际输出效果

![参数为3](../docs/tools/demo_frames_interval_3_out.png)

---

### 如何使用

#### 图片和脚本同一目录下
    python ./gif2png.py
#### 图片在特定文件夹中(如./img)
    python ./gif2png.py ./img

#### 自己制作动画素材
    1. 动画拆分为多张图片，重新拼接为一张大图，例如九宫格图，从左上开始，从上到下，从左到右，行列相乘的值要等于总图片数。或者直接拼接排成一行的图片也可以。
    2. 储存格式为PNG格式即可，如果没有透明度要求，可以储存为BMP格式。

## 字体点阵生成脚本

### 依赖

```sh
pip install freetype-py
pip install numpy
pip install pyyaml
```

### 如何使用

#### 输出文件和脚本同一目录下
1. 自动生成yml文件
    python ./auto_ttf2c.py

2. 修改yml文件

3. python ./auto_ttf2c.py
    
#### 输出文件指定文件夹(如./font)
1. 自动生成yml文件
    python ./auto_ttf2c.py ./font

2. 修改yml文件

3. python ./auto_ttf2c.py ./font
