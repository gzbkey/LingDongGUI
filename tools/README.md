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

### 生成动画素材

#### 此处以gif格式讲解
    1. 登录网站：https://ezgif.com/gif-to-sprite
    2. 上传gif文件，此时可以看到frames值
    3. convert选项，选择to sprite
    4. custom grid中，columns设置适合的值(可填frames值)，行列相乘的值要等于frames值
    5. 点击convert，即可预览生成图片

#### 自己制作动画素材
    1. 动画拆分为多张图片，重新拼接为一张大图，例如九宫格图，从左上开始，从上到下，从左到右，行列相乘的值要等于总图片数。或者直接拼接排成一行的图片也可以。
    2. 储存格式为BMP即可。

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
