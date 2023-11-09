# Button
### 简述
button widget
### 函数列表
* ldButton_t* ldButtonInit(uint16_t nameId, uint16_t parentNameId, int16_t x,int16_t y,int16_t width,int16_t height);
* void ldButtonSetText(ldButton_t* pWidget,uint8_t *pStr);
* void ldButtonSetColor(ldButton_t* pWidget,ldColor releaseColor,ldColor pressColor);
* void ldButtonSetImage(ldButton_t* pWidget,uint32_t releaseImgAddr,bool isReleaseMask,uint32_t pressImgAddr,bool isPressMask);
* void ldButtonSetSelectImage(ldButton_t* pWidget,uint32_t selectMaskAddr,ldColor selectColor);
* void ldButtonSetTextColor(ldButton_t* pWidget,ldColor charColor);
* void ldButtonSetTransparent(ldButton_t* pWidget,bool isTransparent);
* void ldButtonSetRoundCorner(ldButton_t* pWidget,bool isCorner);
* void ldButtonSetSelect(ldButton_t* pWidget,bool isSelected);
* void ldButtonLoop(ldButton_t *pWidget,const arm_2d_tile_t *pParentTile,bool bIsNewFrame);
* void ldButtonSetAlign(ldButton_t *pWidget,uint8_t align);
* void ldButtonSetFont(ldButton_t *pWidget,ldFontDict_t *pFontDict);
### 信号列表
* SIGNAL_PRESS
* SIGNAL_RELEASE
### 函数说明
#### ldButtonInit
##### 原型
```c
ldButton_t* ldButtonInit(uint16_t nameId, uint16_t parentNameId, int16_t x,int16_t y,int16_t width,int16_t height);
```
##### 说明
    button初始化函数

##### 参数
    nameId              新控件id
    parentNameId        父控件id
    x                   相对坐标x轴
    y                   相对坐标y轴
    width               控件宽度
    height              控件高度

##### 返回
    ldButton_t*         新控件指针
<br>

#### ldButtonSetText
##### 原型
```c
void ldButtonSetText(ldButton_t* pWidget,uint8_t *pStr);
```
##### 说明
    设置按键显示文本

##### 参数
    pWidget             目标控件指针
    pStr                字符串指针

<br>

#### ldButtonSetColor
##### 原型
```c
void ldButtonSetColor(ldButton_t* pWidget,ldColor releaseColor,ldColor pressColor);
```
##### 说明
    设置按键颜色，设置该函数后，图片设置无效

##### 参数
    pWidget             目标控件指针
    releaseColor        松开显示的颜色
    pressColor          按下显示的颜色

<br>

#### ldButtonSetImage
##### 原型
```c
void ldButtonSetImage(ldButton_t* pWidget,uint32_t releaseImgAddr,bool isReleaseMask,uint32_t pressImgAddr,bool isPressMask);
```
##### 说明
    设置按键图片，设置该函数后，颜色设置无效

##### 参数
    pWidget             目标控件指针
    releaseImgAddr      松开显示的图片
    isReleaseMask       松开显示的图片是否带透明度(蒙版)
    pressImgAddr        按下显示的图片
    isPressMask         按下显示的图片是否带透明度(蒙版)

<br>

#### ldButtonSetSelectImage
##### 原型
```c
void ldButtonSetSelectImage(ldButton_t* pWidget,uint32_t selectMaskAddr,ldColor selectColor);
```
##### 说明
    选中按键的选中框(图片)

##### 参数
    pWidget             目标控件指针
    selectMaskAddr      选择效果显示的图片(蒙版)
    selectColor         显示的颜色

<br>

#### ldButtonSetTextColor
##### 原型
```c
void ldButtonSetTextColor(ldButton_t* pWidget,ldColor charColor);
```
##### 说明
    设置文字颜色

##### 参数
    pWidget             目标控件指针
    charColor           文字颜色

<br>

#### ldButtonSetTransparent
##### 原型
```c
void ldButtonSetTransparent(ldButton_t* pWidget,bool isTransparent);
```
##### 说明
    按键设置为透明，则不显示，但按下有按键效果
    例如触摸某图片的左右两侧后，会切换图片，
    则可以使用功能两个透明按键放置图片顶层的两侧，
    即可实现该功能

##### 参数
    pWidget             目标控件指针
    isTransparent       true=透明 false=不透明

<br>

#### ldButtonSetRoundCorner
##### 原型
```c
void ldButtonSetRoundCorner(ldButton_t* pWidget,bool isCorner);
```
##### 说明
    按键实现圆角显示效果

##### 参数
    pWidget             目标控件指针
    isCorner            true=圆角 false=方角

<br>

#### ldButtonSetSelect
##### 原型
```c
void ldButtonSetSelect(ldButton_t* pWidget,bool isSelected);
```
##### 说明
    选择按键，触发选中的显示效果

##### 参数
    pWidget             目标控件指针
    isSelected          true=选中 false=不选中

<br>

#### ldButtonLoop
##### 原型
```c
void ldButtonLoop(ldButton_t *pWidget,const arm_2d_tile_t *pParentTile,bool bIsNewFrame);
```
##### 说明
    按键循环处理函数

##### 参数
    pWidget             目标控件指针
    pParentTile         父控件tile对象
    bIsNewFrame         新的一帧开始标志

<br>

#### ldButtonSetAlign
##### 原型
```c
void ldButtonSetAlign(ldButton_t *pWidget,uint8_t align);
```
##### 说明
    文本的对齐方式

##### 参数
    pWidget             目标控件指针
    align               LD_ALIGN_CENTER
                        LD_ALIGN_TOP
                        LD_ALIGN_BOTTOM
                        LD_ALIGN_LEFT
                        LD_ALIGN_RIGHT

<br>

#### ldButtonSetFont
##### 原型
```c
void ldButtonSetFont(ldButton_t *pWidget,ldFontDict_t *pFontDict);
```
##### 说明
    设置字体

##### 参数
    pWidget             目标控件指针
    pFontDict           字体指针

<br>

---
# CheckBox
### 简述
check box widget
### 函数列表
### 信号列表
### 函数说明
---
# Common
### 简述
通用函数文件
### 函数列表
### 信号列表
### 函数说明
---
# DateTime
### 简述
date time widget
### 函数列表
### 信号列表
### 函数说明
---
# IconSlider
### 简述
icon slider widget
### 函数列表
### 信号列表
* SIGNAL_CLICKED_ITEM
### 函数说明
---
# Image
### 简述
image widget
### 函数列表
### 信号列表
### 函数说明
---
# Label
### 简述
label widget
### 函数列表
### 信号列表
### 函数说明
---
# ProgressBar
### 简述
progress bar widget
### 函数列表
### 信号列表
### 函数说明
---
# RadialMenu
### 简述
radial menu widget
### 函数列表
### 信号列表
### 函数说明
---
# ScrollSelecter
### 简述
scroll selecter widget
### 函数列表
### 信号列表
### 函数说明
---
# Text
### 简述
text widget
### 函数列表
### 信号列表
### 函数说明
---
# Window
### 简述
window widget 窗体控件可包含其他的控件，实现复合型控件或者控件组合模块
### 函数列表
* ldWindow_t* ldWindowInit(uint16_t nameId, uint16_t parentNameId, int16_t x,int16_t y,int16_t width,int16_t height);
* void ldWindowSetTransparent(ldWindow_t *pWidget,bool isTransparent);
### 信号列表
### 函数说明
#### ldWindowInit
##### 原型
```c
ldWindow_t* ldWindowInit(uint16_t nameId, uint16_t parentNameId, int16_t x,int16_t y,int16_t width,int16_t height);
```
##### 说明
    window初始化函数

##### 参数
    nameId              控件id
    parentNameId        父控件id
    x                   相对坐标x轴
    y                   相对坐标y轴
    width               控件宽度
    height              控件高度

##### 返回
    ldWindow_t*         新控件指针
<br>

#### ldWindowSetTransparent
##### 原型
```c
void ldWindowSetTransparent(ldWindow_t *pWidget,bool isTransparent);
```
##### 说明
    window控件设置透明

##### 参数
    pWidget             window控件指针
    isTransparent       true=透明 false=不透明

<br>

---
