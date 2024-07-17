# 软件说明

基于arm-2d的工具脚本，进一步简化操作

## 图片生成脚本

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
