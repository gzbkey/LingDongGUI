## 如何添加自定义控件

1、运行python脚本，[src/gui/createWidget.py](../../src/gui/createWidget.py)，输入控件名称后，自动创建控件文件和自动修改代码，例如输入checkBox，则新建ldCheckBox.c、ldCheckBox.h

2、添加刚才新建的.c .h文件到项目中(新文件在./src/gui中)

3、修改新控件的.c .h文件

---
自动处理：

修改ldCommon.h中的控件类型,ldWidgetType

ldGui.c中添加新控件的头文件

修改ldGui.c中的函数，ldGuiDelWidget，_widgetLoop

---

## 功能库

#### btnAction 按键处理
这是一个独立的多功能按键库，可以对GUI按键和实体按键进行行为检测。ldgui内部已经调用该库，无需用户操作。

##### 信号
该库可以检测以下信号/数据

1. BTN_PRESS

    按下
2. BTN_HOLD_DOWN

    按住不放
3. BTN_RELEASE

    释放
4. BTN_DOUBLE_CLICK

    双击
5. BTN_REPEAT_COUNT

    获取连击次数
6. BTN_HOLD_TIME

    按住不放的时间
7. BTN_LONG_START

    长按触发一次
8. BTN_LONG_SHOOT

    长按连续触发

##### 函数列表
* void xBtnInit(size_t addrOrNum,uint16_t nameId,bool (*getBtnStateFunc)(size_t));
* void xBtnConfig(uint8_t debounceMs,uint16_t longPressMs,uint16_t longShootMs,uint16_t clickTimeOutMs);
* void xBtnTick(uint8_t cycleMs);
* uint16_t xBtnGetState(uint16_t nameIdOrNum,uint8_t state);
* void xBtnClean(void);

##### 函数说明
<table>
    <tr>
        <td>函数</td>
        <td colspan="2">
            <pre><code class="language-c">void xBtnInit(size_t addrOrNum,uint16_t nameId,bool (*getBtnStateFunc)(size_t));</code></pre>
        </td>
    </tr>
    <tr>
        <td>说明</td>
        <td colspan="2">button初始化函数</td>
    </tr>
    <tr>
        <td rowspan="3">参数</td>
        <td>addrOrNum</td>
        <td>按键自定义序号</td>
    </tr>
    <tr>
        <td>nameId</td>
        <td>外部按键填0</td>
    </tr>
    <tr>
        <td>getBtnStateFunc</td>
        <td>获取按键状态的函数指针</td>
    </tr>
</table>

<table>
    <tr>
        <td>ℹ️ Note</td>
    </tr>
    <tr>
        <td>实体按键，可以使用宏定义X_BTN_KEY_INIT(keyNum,func)进行初始化按键。</td>
    </tr>
    <tr>
        <td>
        getBtnStateFunc参考代码如下
        <pre><code class="language-c">
bool vtGetKeyState(size_t value)
{
    switch (value)
    {
    case KEY_NUM_UP:
    {
        return keyUpState;
        break;
    }
    case KEY_NUM_DOWN:
    {
        return keyDownState;
        break;
    }
    default:
        break;
    }
    return 0;
}
        </code></pre>
        </td>
    </tr>
</table>

<table>
    <tr>
        <td>函数</td>
        <td colspan="2">
            <pre><code class="language-c">void xBtnConfig(uint8_t debounceMs,uint16_t longPressMs,uint16_t longShootMs,uint16_t clickTimeOutMs);</code></pre>
        </td>
    </tr>
    <tr>
        <td>说明</td>
        <td colspan="2">按键参数配置，如果不配置则使用预设参数</td>
    </tr>
    <tr>
        <td rowspan="4">参数</td>
        <td>debounceMs</td>
        <td>按键滤波时间</td>
    </tr>
    <tr>
        <td>longPressMs</td>
        <td>长按触发时间</td>
    </tr>
    <tr>
        <td>longShootMs</td>
        <td>长按连续触发间隔</td>
    </tr>
    <tr>
        <td>clickTimeOutMs</td>
        <td>连击超时时间</td>
    </tr>
</table>


<table>
    <tr>
        <td>函数</td>
        <td colspan="2">
            <pre><code class="language-c">void xBtnTick(uint8_t cycleMs);</code></pre>
        </td>
    </tr>
    <tr>
        <td>说明</td>
        <td colspan="2">指定时间间隔内定时执行的函数，ldgui内部已经集成</td>
    </tr>
    <tr>
        <td>参数</td>
        <td>cycleMs</td>
        <td>定时周期</td>
    </tr>
</table>

<table>
    <tr>
        <td>函数</td>
        <td colspan="2">
            <pre><code class="language-c">uint16_t xBtnGetState(uint16_t nameIdOrNum,uint8_t state);</code></pre>
        </td>
    </tr>
    <tr>
        <td>说明</td>
        <td colspan="2">检测目标按键是否符合对应状态</td>
    </tr>
    <tr>
        <td rowspan="2">参数</td>
        <td>nameIdOrNum</td>
        <td>ldgui内部按键填写nameId，外部实体按键填写自定义的按键编号</td>
    </tr>
    <tr>
        <td>state</td>
        <td>期望状态类型</td>
    </tr>
    <tr>
        <td>返回</td>
        <td colspan="2">状态值或需要的数据</td>
    </tr>
</table>

<table>
    <tr>
        <td>函数</td>
        <td colspan="2">
            <pre><code class="language-c">void xBtnClean(void);</code></pre>
        </td>
    </tr>
    <tr>
        <td>说明</td>
        <td colspan="2">清除按键检测的过程数据，一般切换页面的时候使用</td>
    </tr>
</table>

##### 使用举例
```c
//自定义实体按键序号
#define KEY_NUM_UP   0

//根据按键序号，获取按键状态
bool vtGetKeyState(size_t value)
{
    switch (value)
    {
    case KEY_NUM_UP:
    {
        return keyUp;
    }
    default:
        break;
    }
    return 0;
}

//初始化按键
X_BTN_KEY_INIT(KEY_NUM_UP,vtGetKeyState);

//循环处理函数
void loopFunc(void)
{
    //判断按键按下
    if(xBtnGetState(KEY_NUM_UP,BTN_PRESS))
    {
        //按键按下的用户处理代码
    }
}
```

#### queue 队列
这是一个简单的队列软件库

##### 函数列表
* xQueue_t* xQueueCreate(uint32_t length, uint32_t itemSize);
* bool xQueueEnqueue(xQueue_t *queue ,void * pInItem,uint32_t itemSize);
* bool xQueueDequeue(xQueue_t *queue ,void * pOutItem,uint32_t itemSize);
* uint32_t xQueueGetLength(xQueue_t *queue);
* void xQueueClear(xQueue_t* queue);

##### 使用举例
```c
typedef struct{
    uint16_t senderId;
    uint8_t signalType;
    size_t value;
}emitInfo_t;

xQueue_t *emitQueue=NULL;
emitInfo_t emitInfo;

//创建队列
emitQueue=xQueueCreate(EMIT_QUEUE_SIZE,sizeof (emitInfo_t));

//写入数据，入队
emitInfo.senderId=0;
emitInfo.signalType=1;
emitInfo.value=10;
xQueueEnqueue(emitQueue,&emitInfo,sizeof (emitInfo_t));

//判断数据数量
if(xQueueGetLength(emitQueue)>0)
{
    //读取数据，出队
    if(xQueueDequeue(emitQueue,&emitInfo,sizeof (emitInfo)))
}
```

#### log 打印日志
* 支持普通打印和彩色打印
* 支持信息等级分类

##### 配置
* USE_LOG_COLOR

    =0 普通打印

    =1 彩色打印(需要控制台支持，推荐MobaXterm)
* SET_LOG_LEVEL

    =LOG_LEVEL_NONE

    =LOG_LEVEL_ERROR

    =LOG_LEVEL_WARNING

    =LOG_LEVEL_INFO

    =LOG_LEVEL_DEBUG
* LOG_PRINT

    默认为printf，可自定义串口硬件

##### 函数接口
* LOG_ERROR(fmt, ...)
* LOG_WARNING(fmt, ...)
* LOG_INFO(fmt, ...)
* LOG_DEBUG(fmt, ...)

##### 使用举例
```c
LOG_DEBUG("click widget id:%d\n",pWidget->nameId);
```

#### string 字符串处理
对字符串进行简单的转换处理

#### 函数接口
* int32_t xStrToI32(uint8_t * str);
* uint8_t* xI32ToStr(int32_t inInt32,uint8_t *buf);
* float xStrToFloat(uint8_t * str);
* uint8_t* xFloatToStr(float inFloat,uint8_t afterDecimalPointNum,uint8_t *buf);

##### 使用举例
```c
int32_t value;
value = xStrToI32("123");
// value = 123

uint8_t text[16];
float fValue=3.1415926;
text = xFloatToStr(fValue,2,text);
// text = "3.14"
```

#### timer 软件定时器
基于arm2d的基础上衍生的软件定时器

#### 函数接口
* bool ldTimeOut(uint16_t ms, int64_t *pTimer,bool isReset)
##### 使用举例
```c
//计数变量
int64_t sysTimer=0;

//循环处理函数
void loopFunc(void)
{
    if(ldTimeOut(10,&sysTimer,true))
    {
        //10ms处理一次的用户代码
    }
}

```
