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
