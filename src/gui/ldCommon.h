#ifndef _LDCOMMON_H_
#define _LDCOMMON_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "stdbool.h"
#include "stdint.h"
#include "arm_extra_controls.h"
#include "xBtnAction.h"
#include "xConnect.h"
#include "xList.h"
#include "xLog.h"
#include "ldConfig.h"

#ifndef SET_BITS
#define SET_BITS(data,value)                    ((data)|=(value))
#endif
#ifndef CLR_BITS
#define CLR_BITS(data,value)                    ((data)&=~(value))
#endif
#ifndef SETBIT
#define SETBIT(data,move)                       ((data)|=(1<<(move)))
#endif
#ifndef CLRBIT
#define CLRBIT(data,move)                       ((data)&=~(1<<(move)))
#endif
#ifndef GETBIT
#define GETBIT(data,move)                       (((data)>>(move))&0x01)
#endif
#ifndef PUTBIT
#define PUTBIT(data,value,move)                 ((value)?SETBIT(data,move):CLRBIT(data,move))
#endif
#ifndef GET16H
#define GET16H(data)                            (((data)>>8)&0xFF)
#endif
#ifndef GET16L
#define GET16L(data)                            ((data)&0xFF)
#endif
#ifndef CONNECT16
#define CONNECT16(H,L)                          (((H)<<8)+(L))
#endif
#ifndef GET32H
#define GET32H(data)                          (((data)>>24)&0xFF)
#endif
#ifndef GET32MH
#define GET32MH(data)                           (((data)>>16)&0xFF)
#endif
#ifndef GET32ML
#define GET32ML(data)                           (((data)>>8)&0xFF)
#endif
#ifndef GET32L
#define GET32L(data)                            ((data)&0xFF)
#endif
#ifndef CONNECT32
#define CONNECT32(H,MH,ML,L)                    (((H)<<24)+((MH)<<16)+((ML)<<8)+(L))
#endif
#ifndef SWAP
#define SWAP(x,y)                               ((x) = (x)+(y),(y)=(x)-(y),(x)=(x)-(y))
#endif

#define ANGLE_2_RADIAN(angle)    ((float)(angle)*0.0174533f)

#define ldColor                                 COLOUR_INT

#define XMALLOC                                 ldMalloc
#define XFREE                                   ldFree

#define LD_MALLOC_WIDGET_INFO(widgetTypedef)    (widgetTypedef*)ldMalloc(sizeof(widgetTypedef))
#define LD_MALLOC_STRING(str)                   (uint8_t *)ldMalloc((strlen((const char *)str)+1)*sizeof(uint8_t))

#define LD_ALIGN_CENTER          0
#define LD_ALIGN_TOP             _BV(0)
#define LD_ALIGN_BOTTOM          _BV(1)
#define LD_ALIGN_LEFT            _BV(2)
#define LD_ALIGN_RIGHT           _BV(3)

#define LD_ALIGN_LEFT_AUTO       _BV(4) //GUI内部使用

#define LD_ADDR_NONE             0xffffffffu

#define LD_COLOR_LIGHT_PINK __RGB(255, 182, 193)             // 浅粉红
#define LD_COLOR_PINK __RGB(255, 192, 203)                   // 粉红
#define LD_COLOR_CRIMSON __RGB(220, 20, 60)                  // 深红/猩红
#define LD_COLOR_LAVENDER_BLUSH __RGB(255, 240, 245)         // 淡紫红
#define LD_COLOR_PALE_VIOLET_RED __RGB(219, 112, 147)        // 弱紫罗兰红
#define LD_COLOR_HOT_PINK __RGB(255, 105, 180)               // 热情的粉红
#define LD_COLOR_DEEP_PINK __RGB(255, 20, 147)               // 深粉红
#define LD_COLOR_MEDIUM_VIOLET_RED __RGB(199, 21, 133)       // 中紫罗兰红
#define LD_COLOR_ORCHID __RGB(218, 112, 214)                 // 暗紫色/兰花紫
#define LD_COLOR_THISTLE __RGB(216, 191, 216)                // 蓟色
#define LD_COLOR_PLUM __RGB(221, 160, 221)                   // 洋李色/李子紫
#define LD_COLOR_VIOLET __RGB(238, 130, 238)                 // 紫罗兰
#define LD_COLOR_MAGENTA __RGB(255, 0, 255)                  // 洋红/玫瑰红
#define LD_COLOR_FUCHSIA __RGB(255, 0, 255)                  // 紫红/灯笼海棠
#define LD_COLOR_DARK_MAGENTA __RGB(139, 0, 139)             // 深洋红
#define LD_COLOR_PURPLE __RGB(128, 0, 128)                   // 紫色
#define LD_COLOR_MEDIUM_ORCHID __RGB(186, 85, 211)           // 中兰花紫
#define LD_COLOR_DARK_VIOLET __RGB(148, 0, 211)              // 暗紫罗兰
#define LD_COLOR_DARK_ORCHID __RGB(153, 50, 204)             // 暗兰花紫
#define LD_COLOR_INDIGO __RGB(75, 0, 130)                    // 靛青/紫兰色
#define LD_COLOR_BLUE_VIOLET __RGB(138, 43, 226)             // 蓝紫罗兰
#define LD_COLOR_MEDIUM_PURPLE __RGB(147, 112, 219)          // 中紫色
#define LD_COLOR_MEDIUM_SLATE_BLUE __RGB(123, 104, 238)      // 中暗蓝色/中板岩蓝
#define LD_COLOR_SLATE_BLUE __RGB(106, 90, 205)              // 石蓝色/板岩蓝
#define LD_COLOR_DARK_SLATE_BLUE __RGB(72, 61, 139)          // 暗灰蓝色/暗板岩蓝
#define LD_COLOR_LAVENDER __RGB(230, 230, 250)               // 淡紫色/熏衣草淡紫
#define LD_COLOR_GHOST_WHITE __RGB(248, 248, 255)            // 幽灵白
#define LD_COLOR_BLUE __RGB(0, 0, 255)                       // 纯蓝
#define LD_COLOR_MEDIUM_BLUE __RGB(0, 0, 205)                // 中蓝色
#define LD_COLOR_MIDNIGHT_BLUE __RGB(25, 25, 112)            // 午夜蓝
#define LD_COLOR_DARK_BLUE __RGB(0, 0, 139)                  // 暗蓝色
#define LD_COLOR_NAVY __RGB(0, 0, 128)                       // 海军蓝
#define LD_COLOR_ROYAL_BLUE __RGB(65, 105, 225)              // 皇家蓝/宝蓝
#define LD_COLOR_CORNFLOWER_BLUE __RGB(100, 149, 237)        // 矢车菊蓝
#define LD_COLOR_LIGHT_STEEL_BLUE __RGB(176, 196, 222)       // 亮钢蓝
#define LD_COLOR_LIGHT_SLATE_GRAY __RGB(119, 136, 153)       // 亮蓝灰/亮石板灰
#define LD_COLOR_SLATE_GRAY __RGB(112, 128, 144)             // 灰石色/石板灰
#define LD_COLOR_DODGER_BLUE __RGB(30, 144, 255)             // 闪兰色/道奇蓝
#define LD_COLOR_ALICE_BLUE __RGB(240, 248, 255)             // 爱丽丝蓝
#define LD_COLOR_STEEL_BLUE __RGB(70, 130, 180)              // 钢蓝/铁青
#define LD_COLOR_LIGHT_SKY_BLUE __RGB(135, 206, 250)         // 亮天蓝色
#define LD_COLOR_SKY_BLUE __RGB(135, 206, 235)               // 天蓝色
#define LD_COLOR_DEEP_SKY_BLUE __RGB(0, 191, 255)            // 深天蓝
#define LD_COLOR_LIGHT_BLUE __RGB(173, 216, 230)             // 亮蓝
#define LD_COLOR_POWDER_BLUE __RGB(176, 224, 230)            // 粉蓝色/火药青
#define LD_COLOR_CADET_BLUE __RGB(95, 158, 160)              // 军兰色/军服蓝
#define LD_COLOR_AZURE __RGB(240, 255, 255)                  // 蔚蓝色
#define LD_COLOR_LIGHT_CYAN __RGB(224, 255, 255)             // 淡青色
#define LD_COLOR_PALE_TURQUOISE __RGB(175, 238, 238)         // 弱绿宝石
#define LD_COLOR_CYAN __RGB(0, 255, 255)                     // 青色
#define LD_COLOR_AQUA __RGB(0, 255, 255)                     // 浅绿色/水色
#define LD_COLOR_DARK_TURQUOISE __RGB(0, 206, 209)           // 暗绿宝石
#define LD_COLOR_DARK_SLATE_GRAY __RGB(47, 79, 79)           // 暗瓦灰色/暗石板灰
#define LD_COLOR_DARK_CYAN __RGB(0, 139, 139)                // 暗青色
#define LD_COLOR_TEAL __RGB(0, 128, 128)                     // 水鸭色
#define LD_COLOR_MEDIUM_TURQUOISE __RGB(72, 209, 204)        // 中绿宝石
#define LD_COLOR_LIGHT_SEA_GREEN __RGB(32, 178, 170)         // 浅海洋绿
#define LD_COLOR_TURQUOISE __RGB(64, 224, 208)               // 绿宝石
#define LD_COLOR_AQUAMARINE __RGB(127, 255, 212)             // 宝石碧绿
#define LD_COLOR_MEDIUM_AQUAMARINE __RGB(102, 205, 170)      // 中宝石碧绿
#define LD_COLOR_MEDIUM_SPRING_GREEN __RGB(0, 250, 154)      // 中春绿色
#define LD_COLOR_MINT_CREAM __RGB(245, 255, 250)             // 薄荷奶油
#define LD_COLOR_SPRING_GREEN __RGB(0, 255, 127)             // 春绿色
#define LD_COLOR_MEDIUM_SEA_GREEN __RGB(60, 179, 113)        // 中海洋绿
#define LD_COLOR_SEA_GREEN __RGB(46, 139, 87)                // 海洋绿
#define LD_COLOR_HONEYDEW __RGB(240, 255, 240)               // 蜜色/蜜瓜色
#define LD_COLOR_LIGHT_GREEN __RGB(144, 238, 144)            // 淡绿色
#define LD_COLOR_PALE_GREEN __RGB(152, 251, 152)             // 弱绿色
#define LD_COLOR_DARK_SEA_GREEN __RGB(143, 188, 143)         // 暗海洋绿
#define LD_COLOR_LIME_GREEN __RGB(50, 205, 50)               // 闪光深绿
#define LD_COLOR_LIME __RGB(0, 255, 0)                       // 闪光绿
#define LD_COLOR_FOREST_GREEN __RGB(34, 139, 34)             // 森林绿
#define LD_COLOR_GREEN __RGB(0, 128, 0)                      // 纯绿
#define LD_COLOR_DARK_GREEN __RGB(0, 100, 0)                 // 暗绿色
#define LD_COLOR_CHARTREUSE __RGB(127, 255, 0)               // 黄绿色/查特酒绿
#define LD_COLOR_LAWN_GREEN __RGB(124, 252, 0)               // 草绿色/草坪绿
#define LD_COLOR_GREEN_YELLOW __RGB(173, 255, 47)            // 绿黄色
#define LD_COLOR_DARK_OLIVE_GREEN __RGB(85, 107, 47)         // 暗橄榄绿
#define LD_COLOR_YELLOW_GREEN __RGB(154, 205, 50)            // 黄绿色
#define LD_COLOR_OLIVE_DRAB __RGB(107, 142, 35)              // 橄榄褐色
#define LD_COLOR_BEIGE __RGB(245, 245, 220)                  // 米色/灰棕色
#define LD_COLOR_LIGHT_GOLDENROD_YELLOW __RGB(250, 250, 210) // 亮菊黄
#define LD_COLOR_IVORY __RGB(255, 255, 240)                  // 象牙色
#define LD_COLOR_LIGHT_YELLOW __RGB(255, 255, 224)           // 浅黄色
#define LD_COLOR_YELLOW __RGB(255, 255, 0)                   // 纯黄
#define LD_COLOR_OLIVE __RGB(128, 128, 0)                    // 橄榄
#define LD_COLOR_DARK_KHAKI __RGB(189, 183, 107)             // 暗黄褐色/深卡叽布
#define LD_COLOR_LEMON_CHIFFON __RGB(255, 250, 205)          // 柠檬绸
#define LD_COLOR_PALE_GOLDENROD __RGB(238, 232, 170)         // 灰菊黄/苍麒麟色
#define LD_COLOR_KHAKI __RGB(240, 230, 140)                  // 黄褐色/卡叽布
#define LD_COLOR_GOLD __RGB(255, 215, 0)                     // 金色
#define LD_COLOR_CORNSILK __RGB(255, 248, 220)               // 玉米丝色
#define LD_COLOR_GOLDENROD __RGB(218, 165, 32)               // 金菊黄
#define LD_COLOR_DARK_GOLDENROD __RGB(184, 134, 11)          // 暗金菊黄
#define LD_COLOR_FLORAL_WHITE __RGB(255, 250, 240)           // 花的白色
#define LD_COLOR_OLD_LACE __RGB(253, 245, 230)               // 老花色/旧蕾丝
#define LD_COLOR_WHEAT __RGB(245, 222, 179)                  // 浅黄色/小麦色
#define LD_COLOR_MOCCASIN __RGB(255, 228, 181)               // 鹿皮色/鹿皮靴
#define LD_COLOR_ORANGE __RGB(255, 165, 0)                   // 橙色
#define LD_COLOR_PAPAYA_WHIP __RGB(255, 239, 213)            // 番木色/番木瓜
#define LD_COLOR_BLANCHED_ALMOND __RGB(255, 235, 205)        // 白杏色
#define LD_COLOR_NAVAJO_WHITE __RGB(255, 222, 173)           // 纳瓦白/土著白
#define LD_COLOR_ANTIQUE_WHITE __RGB(250, 235, 215)          // 古董白
#define LD_COLOR_TAN __RGB(210, 180, 140)                    // 茶色
#define LD_COLOR_BURLY_WOOD __RGB(222, 184, 135)             // 硬木色
#define LD_COLOR_BISQUE __RGB(255, 228, 196)                 // 陶坯黄
#define LD_COLOR_DARK_ORANGE __RGB(255, 140, 0)              // 深橙色
#define LD_COLOR_LINEN __RGB(250, 240, 230)                  // 亚麻布
#define LD_COLOR_PERU __RGB(205, 133, 63)                    // 秘鲁色
#define LD_COLOR_PEACH_PUFF __RGB(255, 218, 185)             // 桃肉色
#define LD_COLOR_SANDY_BROWN __RGB(244, 164, 96)             // 沙棕色
#define LD_COLOR_CHOCOLATE __RGB(210, 105, 30)               // 巧克力色
#define LD_COLOR_SADDLE_BROWN __RGB(139, 69, 19)             // 重褐色/马鞍棕色
#define LD_COLOR_SEASHELL __RGB(255, 245, 238)               // 海贝壳
#define LD_COLOR_SIENNA __RGB(160, 82, 45)                   // 黄土赭色
#define LD_COLOR_LIGHT_SALMON __RGB(255, 160, 122)           // 浅鲑鱼肉色
#define LD_COLOR_CORAL __RGB(255, 127, 80)                   // 珊瑚
#define LD_COLOR_ORANGE_RED __RGB(255, 69, 0)                // 橙红色
#define LD_COLOR_DARK_SALMON __RGB(233, 150, 122)            // 深鲜肉/鲑鱼色
#define LD_COLOR_TOMATO __RGB(255, 99, 71)                   // 番茄红
#define LD_COLOR_MISTY_ROSE __RGB(255, 228, 225)             // 浅玫瑰色/薄雾玫瑰
#define LD_COLOR_SALMON __RGB(250, 128, 114)                 // 鲜肉/鲑鱼色
#define LD_COLOR_SNOW __RGB(255, 250, 250)                   // 雪白色
#define LD_COLOR_LIGHT_CORAL __RGB(240, 128, 128)            // 淡珊瑚色
#define LD_COLOR_ROSY_BROWN __RGB(188, 143, 143)             // 玫瑰棕色
#define LD_COLOR_INDIAN_RED __RGB(205, 92, 92)               // 印度红
#define LD_COLOR_RED __RGB(255, 0, 0)                        // 纯红
#define LD_COLOR_BROWN __RGB(165, 42, 42)                    // 棕色
#define LD_COLOR_FIRE_BRICK __RGB(178, 34, 34)               // 火砖色/耐火砖
#define LD_COLOR_DARK_RED __RGB(139, 0, 0)                   // 深红色
#define LD_COLOR_MAROON __RGB(128, 0, 0)                     // 栗色
#define LD_COLOR_WHITE __RGB(255, 255, 255)                  // 纯白
#define LD_COLOR_WHITE_SMOKE __RGB(245, 245, 245)            // 白烟
#define LD_COLOR_GAINSBORO __RGB(220, 220, 220)              // 淡灰色
#define LD_COLOR_LIGHT_GREY __RGB(211, 211, 211)             // 浅灰色
#define LD_COLOR_SILVER __RGB(192, 192, 192)                 // 银灰色
#define LD_COLOR_DARK_GRAY __RGB(169, 169, 169)              // 深灰色
#define LD_COLOR_GRAY __RGB(128, 128, 128)                   // 灰色
#define LD_COLOR_DIM_GRAY __RGB(105, 105, 105)               // 暗淡灰
#define LD_COLOR_BLACK __RGB(0, 0, 0)                        // 纯黑

typedef enum{
    widgetTypeNone,
    widgetTypeBackground,
    widgetTypeWindow,
    widgetTypeButton,
    widgetTypeImage,
    widgetTypeText,
    widgetTypeLineEdit,
    widgetTypeGraph,
    widgetTypeCheckBox,
    widgetTypeSlider,
    widgetTypeProgressBar,
    widgetTypeGauge,
    widgetTypeQRCode,
    widgetTypeDateTime,
    widgetTypeIconSlider,
    widgetTypeComboBox,
    widgetTypeArc,
    widgetTypeRadialMenu,
    widgetTypeScrollSelecter,
    widgetTypeLabel,
    widgetTypeTable,
    widgetTypeKeyboard,
}ldWidgetType_t;

typedef enum{
    none,
    waitChange,
    waitUpdate
}ldDirtyRegionStateType_t;

#if USE_VIRTUAL_RESOURCE == 0
#define LD_COMMON_ATTRIBUTES  arm_2d_tile_t resource; \
                              ldWidgetType_t widgetType; \
                              void * parentWidget; \
                              xListNode *childList; \
                              uint16_t nameId; \
                              arm_2d_region_list_item_t dirtyRegionListItem; \
                              arm_2d_region_t dirtyRegionTemp; \
                              bool isHidden:1; \
                              bool isParentHidden:1; \
                              ldDirtyRegionStateType_t dirtyRegionState:2; \
                              bool isDirtyRegionAutoIgnore:1

typedef struct{
    uint8_t utf8[4];
    uint8_t advW[2];
    uint8_t offsetX[2];
    uint8_t offsetY[2];
    uint8_t width[2];
    uint8_t height[2];
    uint8_t addr[4];
}ldFontInfo_t;

typedef struct{
    uint8_t maskType;
    uint16_t count;
    uint16_t lineOffset;
    int16_t descender;
    uint16_t lineStrHeight;
    int16_t lineStrAscender;
    const ldFontInfo_t *pInfoList;
    const uint8_t *pFontSrc;
}ldFontDict_t;

typedef struct{
    arm_2d_tile_t fontTile;
    ldFontDict_t* pFontDict;//字典(目录)
    uint16_t strLen;
    ldColor charColor;
    uint8_t* pStr;
    uint8_t align:4;
}ldChar_t;
#else
#define LD_COMMON_ATTRIBUTES  arm_2d_vres_t resource; \
                              ldWidgetType_t widgetType; \
                              void * parentWidget; \
                              xListNode *childList; \
                              uint16_t nameId; \
                              arm_2d_region_list_item_t dirtyRegionListItem; \
                              arm_2d_region_t dirtyRegionTemp; \
                              bool isHidden:1; \
                              bool isParentHidden:1; \
                              ldDirtyRegionStateType_t dirtyRegionState:2; \
                              bool isDirtyRegionAutoIgnore:1

typedef struct{
    uint8_t utf8[4];
    uint8_t advW[2];
    uint8_t offsetX[2];
    uint8_t offsetY[2];
    uint8_t width[2];
    uint8_t height[2];
    uint8_t addr[4];
}ldFontInfo_t;

typedef struct{
    uint8_t maskType;
    uint32_t count;
    uint16_t lineOffset;
    int16_t descender;
    uint16_t lineStrHeight;
    int16_t lineStrAscender;
    const ldFontInfo_t *pInfoList;
    uint32_t pFontSrc;
}ldFontDict_t;

typedef struct{
    arm_2d_vres_t fontTile;
    ldFontDict_t* pFontDict;//字典(目录)
    uint16_t strLen;
    ldColor charColor;
    uint8_t* pStr;
    uint8_t align:4;
}ldChar_t;
#endif

typedef struct{
    LD_COMMON_ATTRIBUTES;
}ldCommon_t;

typedef struct{
    int16_t x;
    int16_t y;
}ldPoint_t;

typedef struct{
    int16_t width;
    int16_t height;
}ldSize_t;

typedef enum{
    typeString,
    typeInt,
    typeFloat
}ldEditType_t;

extern xListNode ldWidgetLink;

extern ldEditType_t gActiveEditType;

void *ldMalloc(uint32_t size);
void ldFree(void *p);
//void *ldRealloc(void *ptr,uint32_t newSize);


bool ldTimeOut(uint16_t ms, int64_t *pTimer,bool isReset);
void* ldBaseGetWidgetById(uint16_t nameId);
ldPoint_t ldBaseGetGlobalPos(ldCommon_t *pWidget);
xListNode* ldBaseGetWidgetInfoById(uint16_t nameId);
xListNode* ldBaseGetWidgetInfoByPos(int16_t x,int16_t y);



void ldBaseColor(arm_2d_tile_t* pTile,ldColor color,uint8_t opacity);
void ldBaseImage(arm_2d_tile_t* pTile,arm_2d_tile_t* pResTile,bool isWithMask,uint8_t opacity);
void ldBaseMaskImage(arm_2d_tile_t* pTile, arm_2d_tile_t* pResTile, ldColor textColor, uint8_t opacity);
void ldBaseImageScale(arm_2d_tile_t* pTile,arm_2d_tile_t* pResTile,bool isWithMask,float scale,arm_2d_op_trans_msk_opa_t *pOP,bool bIsNewFrame);

void ldBaseSetFont(ldChar_t **ppCharInfo, ldFontDict_t *pFontDictAddr);

void ldBaseTextDel(ldChar_t *pCharInfo);
ldChar_t * ldBaseCheckText(ldChar_t **ppCharInfo);

uint8_t ldBaseGetCharInfo(ldFontDict_t *ptFontDict,uint8_t *pCharUtf8,int16_t *pAdvWidth,int16_t *pOffsetX,int16_t *pOffsetY,int16_t *pWidth,int16_t *pHeight,uint32_t *pImgAddr);
void ldBaseShowText(arm_2d_tile_t target, arm_2d_region_t region, ldChar_t *pTextInfo, int16_t scrollOffset, uint8_t opacity);


void ldBaseSetText(ldChar_t **ppTextInfo, uint8_t *pStr);
void ldBaseSetTextColor(ldChar_t **ppTextInfo, ldColor charColor);
void ldBaseSetAlign(ldChar_t **ppTextInfo, uint8_t align);
arm_2d_size_t ldBaseGetStringSize(uint8_t* pStr,ldFontDict_t *pFontDict,int16_t *pRetBmpAscender, uint16_t frameWidth);

arm_2d_region_t ldBaseGetGlobalRegion(ldCommon_t *pWidget,arm_2d_region_t *pTargetRegion);

void ldBaseMove(ldCommon_t* pWidget,int16_t x,int16_t y);
void ldBaseSetHidden(ldCommon_t* pWidget,bool isHidden);
uint8_t ldBaseGetChScheme(uint8_t maskType);

void ldBaseCharacter(arm_2d_tile_t* pParentTile, arm_2d_region_t *pShowRegion, arm_2d_tile_t *pResTile, ldColor textColor, uint8_t opacity);
arm_2d_region_t ldBaseLineText(arm_2d_tile_t *pTile,arm_2d_tile_t *pResTileTemplate,uint8_t *pStr,ldFontDict_t* pFontDict,uint8_t align,ldColor textColor,int16_t scrollOffset,uint8_t opacity);

void ldBaseDrawLine(arm_2d_tile_t *pTile,int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint8_t lineSize, ldColor color,uint8_t opacityMax, uint8_t opacityMin);

void ldBaseAddDirtyRegion(arm_2d_region_list_item_t *pItemDirtyRegionList, arm_2d_region_list_item_t **ppRootDirtyRegionList);
void ldBaseDirtyRegionAutoUpdate(ldCommon_t* pWidget,arm_2d_region_t newRegion,bool isAutoIgnore);

void ldBaseBgMove(int16_t x,int16_t y);

arm_2d_region_t ldLayoutHorizontal(arm_2d_region_t *pWidgetRegion,arm_2d_region_t *pBufferRegion,int16_t width,int16_t height,int16_t leftSpace,int16_t rightSpace,int16_t topSpace,int16_t bottomSpace);
arm_2d_region_t ldLayoutVertical(arm_2d_region_t *pWidgetRegion,arm_2d_region_t *pBufferRegion,int16_t width,int16_t height,int16_t leftSpace,int16_t rightSpace,int16_t topSpace,int16_t bottomSpace);

#ifdef __cplusplus
}
#endif

#endif //_LDCOMMON_H_
