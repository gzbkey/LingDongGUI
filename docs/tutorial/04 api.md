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
复选框 + 单选功能，支持自定义图片和文字显示
### 函数列表
* ldCheckBox_t *ldCheckBoxInit(uint16_t nameId, uint16_t parentNameId, int16_t x, int16_t y, int16_t width, int16_t height);
* void ldCheckBoxLoop(ldCheckBox_t *pWidget,const arm_2d_tile_t *pParentTile,bool bIsNewFrame);
* void ldCheckBoxSetColor(ldCheckBox_t* pWidget,ldColor bgColor,ldColor fgColor);
* void ldCheckBoxSetImage(ldCheckBox_t* pWidget,uint16_t boxWidth,uint32_t uncheckedImgAddr,bool isUncheckedMask,uint32_t checkedImgAddr,bool isCheckedMask);
* void ldCheckBoxSetFont(ldCheckBox_t *pWidget,ldFontDict_t *pFontDict);
* void ldCheckBoxSetText(ldCheckBox_t* pWidget,uint8_t *pStr);
* void ldCheckBoxSetRadioButtonGroup(ldCheckBox_t* pWidget,uint8_t num);
* void ldCheckBoxSetCorner(ldCheckBox_t* pWidget,bool isCorner);
### 信号列表
### 函数说明
#### ldCheckBoxInit
##### 原型
```c
ldCheckBox_t *ldCheckBoxInit(uint16_t nameId, uint16_t parentNameId, int16_t x, int16_t y, int16_t width, int16_t height);
```
##### 说明
    check               box初始化函数

##### 参数
    nameId              新控件id
    parentNameId        父控件id
    x                   相对坐标x轴
    y                   相对坐标y轴
    width               控件宽度
    height              控件高度

##### 返回
    ldCheckBox_t*       新控件指针
<br>

#### ldCheckBoxLoop
##### 原型
```c
void ldCheckBoxLoop(ldCheckBox_t *pWidget,const arm_2d_tile_t *pParentTile,bool bIsNewFrame);
```
##### 说明
    check               box循环处理函数

##### 参数
    pWidget             目标控件指针
    pParentTile         父控件tile对象
    bIsNewFrame         新的一帧开始标志

<br>

#### ldCheckBoxSetColor
##### 原型
```c
void ldCheckBoxSetColor(ldCheckBox_t* pWidget,ldColor bgColor,ldColor fgColor);
```
##### 说明
    设定颜色

##### 参数
    pWidget             目标控件指针
    bgColor             背景颜色
    fgColor             前景颜色

<br>

#### ldCheckBoxSetImage
##### 原型
```c
void ldCheckBoxSetImage(ldCheckBox_t* pWidget,uint16_t boxWidth,uint32_t uncheckedImgAddr,bool isUncheckedMask,uint32_t checkedImgAddr,bool isCheckedMask);
```
##### 说明
    设定图片，只能设定方型图片

##### 参数
    pWidget             目标控件指针
    boxWidth            图片宽度
    uncheckedImgAddr    未选中的显示图片
    isUncheckedMask     未选中图片是否带透明度
    checkedImgAddr      选中的显示图片
    isCheckedMask       选中图片是否带透明度

<br>

#### ldCheckBoxSetFont
##### 原型
```c
void ldCheckBoxSetFont(ldCheckBox_t *pWidget,ldFontDict_t *pFontDict);
```
##### 说明
    设置字体

##### 参数
    pWidget             目标控件指针
    pFontDict           字体

<br>

#### ldCheckBoxSetText
##### 原型
```c
void ldCheckBoxSetText(ldCheckBox_t* pWidget,uint8_t *pStr);
```
##### 说明
    设置显示文字

##### 参数
    pWidget             目标控件指针
    pStr                需要显示的字符串

<br>

#### ldCheckBoxSetRadioButtonGroup
##### 原型
```c
void ldCheckBoxSetRadioButtonGroup(ldCheckBox_t* pWidget,uint8_t num);
```
##### 说明
    单选功能设定为同一组
    实现同一组的radio button自动单选

##### 参数
    pWidget             目标控件指针
    num                 组号 0-255

<br>

#### ldCheckBoxSetCorner
##### 原型
```c
void ldCheckBoxSetCorner(ldCheckBox_t* pWidget,bool isCorner);
```
##### 说明
    实现圆角显示效果

##### 参数
    pWidget             目标控件指针
    isCorner            true=圆角 false=方角

<br>

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
# gauge
### 简述
仪表盘控件
### 函数列表
* ldGauge_t *ldGaugeInit(uint16_t nameId, uint16_t parentNameId, int16_t x, int16_t y, int16_t width, int16_t height,uint32_t bgImgAddr,bool isBgMask);
* void ldGaugeLoop(ldGauge_t *pWidget,const arm_2d_tile_t *pParentTile,bool bIsNewFrame);
* void ldGaugeSetPointerImage(ldGauge_t *pWidget,uint32_t pointerImgAddr,int16_t pointerWidth,int16_t pointerHeight,int16_t pointerOriginOffsetX,int16_t pointerOriginOffsetY);
* void ldGaugeSetCenterOffset(ldGauge_t *pWidget, int16_t centreOffsetX, int16_t centreOffsetY);
* void ldGaugeSetAngle(ldGauge_t *pWidget, float angle);
* void ldGaugeSetPointerImageType(ldGauge_t *pWidget,gaugePointerType_t pointerImgType,ldColor keyingOrMaskColor);
### 信号列表
### 函数说明
#### ldGaugeInit
##### 原型
```c
ldGauge_t *ldGaugeInit(uint16_t nameId, uint16_t parentNameId, int16_t x, int16_t y, int16_t width, int16_t height,uint32_t bgImgAddr,bool isBgMask);
```
##### 说明
    gauge初始化函数

##### 参数
    nameId              新控件id
    parentNameId        父控件id
    x                   相对坐标x轴
    y                   相对坐标y轴
    width               控件宽度
    height              控件高度

##### 返回
    ldGauge_t*          新控件指针
<br>

#### ldGaugeLoop
##### 原型
```c
void ldGaugeLoop(ldGauge_t *pWidget,const arm_2d_tile_t *pParentTile,bool bIsNewFrame);
```
##### 说明
    gauge循环处理函数

##### 参数
    pWidget             目标控件指针
    pParentTile         父控件tile对象
    bIsNewFrame         新的一帧开始标志

<br>

#### ldGaugeSetPointerImage
##### 原型
```c
void ldGaugeSetPointerImage(ldGauge_t *pWidget,uint32_t pointerImgAddr,int16_t pointerWidth,int16_t pointerHeight,int16_t pointerOriginOffsetX,int16_t pointerOriginOffsetY);
```
##### 说明
    设置指针图片

##### 参数
    pWidget             目标控件指针
    pointerImgAddr      指针图片地址
    pointerWidth        指针宽度
    pointerHeight       指针高度
    pointerOriginOffsetX指针原点坐标x轴(相对本指针图片)
    pointerOriginOffsetY指针原点坐标y轴(相对本指针图片)

<br>

#### ldGaugeSetCenterOffset
##### 原型
```c
void ldGaugeSetCenterOffset(ldGauge_t *pWidget, int16_t centreOffsetX, int16_t centreOffsetY);
```
##### 说明
    设置仪表盘的旋转中心点

##### 参数
    pWidget             目标控件指针
    centreOffsetX       中心点坐标x轴
    centreOffsetY       中心点坐标y轴

<br>

#### ldGaugeSetAngle
##### 原型
```c
void ldGaugeSetAngle(ldGauge_t *pWidget, float angle);
```
##### 说明
    仪表盘的指针显示角度

##### 参数
    pWidget             目标控件指针
    angle               旋转角度，小数点后1位有效

<br>

#### ldGaugeSetPointerImageType
##### 原型
```c
void ldGaugeSetPointerImageType(ldGauge_t *pWidget,gaugePointerType_t pointerImgType,ldColor keyingOrMaskColor);
```
##### 说明
    指针图片的素材类型

##### 参数
    pWidget             目标控件指针
    pointerImgType      nomal,withMask,onlyMask,keying
    keyingOrMaskColor   颜色

<br>

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
# QRCode
### 简述
qr code widget
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
# Table
### 简述
表格控件
### 函数列表
* uint8_t ldTableCurrentColumn(ldTable_t *pWidget);
* uint8_t ldTableCurrentRow(ldTable_t *pWidget);
* ldTableItem_t *ldTableCurrentItem(ldTable_t *pWidget);
* ldTableItem_t *ldTableItem(ldTable_t *pWidget,uint8_t row, uint8_t column);
* ldTableItem_t *ldTableItemAt(ldTable_t *pWidget,int16_t x,int16_t y);
* ldTable_t *ldTableInit(uint16_t nameId, uint16_t parentNameId, int16_t x, int16_t y, int16_t width, int16_t height, uint8_t rowCount, uint8_t columnCount, uint8_t itemSpace);
* void ldTableLoop(ldTable_t *pWidget,const arm_2d_tile_t *pParentTile,bool bIsNewFrame);
* void ldTableSetItemWidth(ldTable_t *pWidget,uint8_t column,int16_t width);
* void ldTableSetItemHeight(ldTable_t *pWidget,uint8_t row,int16_t height);
* void ldTableSetItemText(ldTable_t *pWidget,uint8_t row,uint8_t column,uint8_t *pText,ldFontDict_t* pFontDict);
* void ldTableSetItemStaticText(ldTable_t *pWidget,uint8_t row,uint8_t column,uint8_t *pText,ldFontDict_t* pFontDict);
* void ldTableSetItemColor(ldTable_t *pWidget,uint8_t row,uint8_t column,ldColor textColor,ldColor bgColor);
* void ldTableSetBgColor(ldTable_t *pWidget,ldColor bgColor);
* void ldTableSetItemAlign(ldTable_t *pWidget,uint8_t row,uint8_t column,uint8_t align);
* void ldTableSetItemImage(ldTable_t *pWidget,uint8_t row,uint8_t column,int16_t x,int16_t y,int16_t width,int16_t height,uint32_t imgAddr);
* void ldTableSetItemButton(ldTable_t *pWidget,uint8_t row,uint8_t column,int16_t x,int16_t y,int16_t width,int16_t height,uint32_t releaseImgAddr,uint32_t pressImgAddr,bool isCheckable);
### 信号列表
### 函数说明
#### ldTableCurrentColumn
##### 原型
```c
uint8_t ldTableCurrentColumn(ldTable_t *pWidget);
```
##### 说明
    获取选中列的编号

##### 参数
    pWidget             目标控件指针

##### 返回
    uint8_t             列号
<br>

#### ldTableCurrentRow
##### 原型
```c
uint8_t ldTableCurrentRow(ldTable_t *pWidget);
```
##### 说明
    获取选中行的编号

##### 参数
    pWidget             目标控件指针

##### 返回
    uint8_t             行号
<br>

#### ldTableCurrentItem
##### 原型
```c
ldTableItem_t *ldTableCurrentItem(ldTable_t *pWidget);
```
##### 说明
    获取选中行的项目

##### 参数
    pWidget             目标控件指针

##### 返回
    ldTableItem_t*      返回目标item指针
<br>

#### ldTableItem
##### 原型
```c
ldTableItem_t *ldTableItem(ldTable_t *pWidget,uint8_t row, uint8_t column);
```
##### 说明
    获取指定的项目

##### 参数
    pWidget             目标控件指针
    row                 行号
    column              列号

##### 返回
    ldTableItem_t*      返回目标item指针
<br>

#### ldTableItemAt
##### 原型
```c
ldTableItem_t *ldTableItemAt(ldTable_t *pWidget,int16_t x,int16_t y);
```
##### 说明
    获取指定坐标的项目

##### 参数
    pWidget             目标控件指针
    x                   全局坐标x轴
    y                   全局坐标y轴

##### 返回
    ldTableItem_t*      返回目标item指针
<br>

#### ldTableInit
##### 原型
```c
ldTable_t *ldTableInit(uint16_t nameId, uint16_t parentNameId, int16_t x, int16_t y, int16_t width, int16_t height, uint8_t rowCount, uint8_t columnCount, uint8_t itemSpace);
```
##### 说明
    表格初始化

##### 参数
    nameId              新控件id
    parentNameId        父控件id
    x                   相对坐标x轴
    y                   相对坐标y轴
    width               控件宽度
    height              控件高度
    rowCount            行数
    columnCount         列数
    itemSpace           间隔

##### 返回
    ldTable_t*          新控件指针
<br>

#### ldTableLoop
##### 原型
```c
void ldTableLoop(ldTable_t *pWidget,const arm_2d_tile_t *pParentTile,bool bIsNewFrame);
```
##### 说明
    表格显示处理

##### 参数
    pWidget             目标控件指针
    pParentTile         父控件tile对象
    bIsNewFrame         新的一帧开始标志

<br>

#### ldTableSetItemWidth
##### 原型
```c
void ldTableSetItemWidth(ldTable_t *pWidget,uint8_t column,int16_t width);
```
##### 说明
    设置指定列的宽度

##### 参数
    pWidget             目标控件指针
    column              列
    width               宽度

<br>

#### ldTableSetItemHeight
##### 原型
```c
void ldTableSetItemHeight(ldTable_t *pWidget,uint8_t row,int16_t height);
```
##### 说明
    设置指定行的高度

##### 参数
    pWidget             目标控件指针
    row                 行
    height              高度

<br>

#### ldTableSetItemText
##### 原型
```c
void ldTableSetItemText(ldTable_t *pWidget,uint8_t row,uint8_t column,uint8_t *pText,ldFontDict_t* pFontDict);
```
##### 说明
    设置项目的文本

##### 参数
    pWidget             目标控件指针
    row                 行
    column              列
    pText               文本内容
    pFontDict           字体

<br>

#### ldTableSetItemStaticText
##### 原型
```c
void ldTableSetItemStaticText(ldTable_t *pWidget,uint8_t row,uint8_t column,uint8_t *pText,ldFontDict_t* pFontDict);
```
##### 说明
    设置项目的静态文本，不可变更的文本内容，不消耗内存

##### 参数
    pWidget             目标控件指针
    row                 行
    column              列
    pText               文本内容
    pFontDict           字体

<br>

#### ldTableSetItemColor
##### 原型
```c
void ldTableSetItemColor(ldTable_t *pWidget,uint8_t row,uint8_t column,ldColor textColor,ldColor bgColor);
```
##### 说明
    设置项目颜色

##### 参数
    pWidget             目标控件指针
    row                 行
    column              列
    textColor           文字颜色
    bgColor             文字背景颜色

<br>

#### ldTableSetBgColor
##### 原型
```c
void ldTableSetBgColor(ldTable_t *pWidget,ldColor bgColor);
```
##### 说明
    设置表格底色

##### 参数
    pWidget             目标控件指针
    bgColor             底色

<br>

#### ldTableSetItemAlign
##### 原型
```c
void ldTableSetItemAlign(ldTable_t *pWidget,uint8_t row,uint8_t column,uint8_t align);
```
##### 说明
    设置指定项目文本对齐方式

##### 参数
    pWidget             目标控件指针
    row                 行号
    column              列号
    align               对齐方式

<br>

#### ldTableSetItemImage
##### 原型
```c
void ldTableSetItemImage(ldTable_t *pWidget,uint8_t row,uint8_t column,int16_t x,int16_t y,int16_t width,int16_t height,uint32_t imgAddr);
```
##### 说明
    设置项目图片

##### 参数
    pWidget             目标控件指针
    row                 行号
    column              列号
    x                   图片在项目中的x轴坐标
    y                   图片在项目中的y轴坐标
    width               图片宽度
    height              图片高度
    imgAddr             图片地址

<br>

#### ldTableSetItemButton
##### 原型
```c
void ldTableSetItemButton(ldTable_t *pWidget,uint8_t row,uint8_t column,int16_t x,int16_t y,int16_t width,int16_t height,uint32_t releaseImgAddr,uint32_t pressImgAddr,bool isCheckable);
```
##### 说明
    设置项目按键图片

##### 参数
    pWidget             目标控件指针
    row                 行号
    column              列号
    x                   图片在项目中的x轴坐标
    y                   图片在项目中的y轴坐标
    width               图片宽度
    height              图片高度
    releaseImgAddr      释放的图片地址
    pressImgAddr        按下的图片地址
    isCheckable         是否为开关型按键

<br>

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
