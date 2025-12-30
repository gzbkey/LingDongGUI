#include "homeLogic.h"
#include "home.h"
#include "ldGui.h"
#include "serial_win.h"
#include "xString.h"
#include <ctype.h>

void home_Timer_0_event(ld_scene_t* ptScene)
{
    ldText_t *ptWidget=ldBaseGetWidgetById(ID_TEXT_SHOW);
}


bool home_action_Button_open_port_press(ld_scene_t *ptScene,ldMsg_t msg)
{
    ldButton_t *btn=msg.ptSender;
    if(btn->isPressed==false)
    {
        ldImage_t *img=ldBaseGetWidgetById(ID_IMAGE_OPEN);
        ldImageSetImage(img,IMAGE_LIGHT_GREEN_SMALL_PNG,IMAGE_LIGHT_GREEN_SMALL_PNG_MASK);
        ldButtonSetText(btn,"Close");

        ldComboBox_t *comb=ldBaseGetWidgetById(ID_COMBOBOX_PORTNAME);
        uint8_t selectNum=ldComboBoxGetSelectItem(comb);
        uint8_t *portNameStr=ldComboBoxGetText(comb,selectNum);


        comb=ldBaseGetWidgetById(ID_COMBOBOX_BAUDRATE);
        selectNum=ldComboBoxGetSelectItem(comb);
        uint8_t *pStr=ldComboBoxGetText(comb,selectNum);
        int baudrate=atoi((char*)pStr);
        LOG_DEBUG("baudrate:%s",pStr);

        comb=ldBaseGetWidgetById(ID_COMBOBOX_DATABITS);
        selectNum=ldComboBoxGetSelectItem(comb);
        pStr=ldComboBoxGetText(comb,selectNum);
        LOG_DEBUG("data bits:%s",pStr);

        ldCheckBox_t *checkb= ldCheckBoxGetRadioSelected(0);
        pStr=ldCheckBoxGetText(checkb);
        LOG_DEBUG("stop bits:%s",pStr);

        checkb= ldCheckBoxGetRadioSelected(1);
        pStr=ldCheckBoxGetText(checkb);
        LOG_DEBUG("parity:%s",pStr);

        //打开串口
#if defined (_WIN32)
        com_open(portNameStr, baudrate);
#endif
    }
   return false;
}

bool home_action_Button_open_port_release(ld_scene_t *ptScene,ldMsg_t msg)
{
    ldButton_t *btn=msg.ptSender;
    if(btn->isPressed==false)
    {
        ldImage_t *img=ldBaseGetWidgetById(ID_IMAGE_OPEN);
        ldImageSetImage(img,IMAGE_LIGHT_RED_SMALL_PNG,IMAGE_LIGHT_RED_SMALL_PNG_MASK);
        ldButtonSetText(btn,"Open");

        // 关闭串口
#if defined (_WIN32)
        com_close();
#endif
    }
   return false;
}

bool home_action_Button_setting_release(ld_scene_t *ptScene,ldMsg_t msg)
{
    ldWindow_t *win=ldBaseGetWidgetById(ID_WINDOW_SETTING);
    if(ldBaseIsHidden((ldBase_t*)win))
    {
        ldBaseSetHidden((ldBase_t*)win,false);
    }
    return false;
}

bool home_action_Button_close_release(ld_scene_t *ptScene,ldMsg_t msg)
{
    ldWindow_t *win=ldBaseGetWidgetById(ID_WINDOW_SETTING);

    ldBaseSetHidden((ldBase_t*)win,true);

    return false;
}

bool home_action_Button_send_release(ld_scene_t *ptScene,ldMsg_t msg)
{
    ldLineEdit_t *le=ldBaseGetWidgetById(ID_LINEEDIT_SEND);
    uint8_t *text=ldLineEditGetText(le);

    ldCheckBox_t *hexCb=ldBaseGetWidgetById(ID_CHECKBOX_HEXSEND);
    bool isHexSend=ldCheckBoxIsChecked(hexCb);

    if(isHexSend)
    {
        while (*text) {
            unsigned char high, low;

            while (*text == ' ') ++text;
            if (!*text) break;

            high = toupper(*text++);
            if (!isxdigit(high)) return -1;
            high = (high >= 'A') ? (high - 'A' + 10) : (high - '0');

            low = toupper(*text++);
            if (!isxdigit(low)) return -1;
            low = (low >= 'A') ? (low - 'A' + 10) : (low - '0');

            uint8_t dat=(high << 4) | low;
#if defined (_WIN32)
            com_send(&dat,1);
#endif
            ++text;
        }
    }
    else
    {
#if defined (_WIN32)
    com_send(text,strlen(text));
#endif
    }
    return false;
}

ldText_t *textShow;


#define RING_SIZE 4096
static uint8_t textBuf[RING_SIZE] = "12345678\n123\n123\n123\n123\n123\n123\n123\n5555\n123\n123\n123\n123\n123\n123\n9999\n123\n123\n123\n123\n123\n123\n123\n123\n123\n77777\n123\n123\n123\n123\n123\n123\n123\n123\n123";
static uint16_t len = 12;   /* 当前有效字符数 */

/* 追加 1 字节，并滑动窗口保持最新 len 字符 */
void ring_put(uint8_t ch)
{
    if (len < RING_SIZE - 1) {          /* 没满，直接追加 */
        textBuf[len++] = ch;
    } else {                            /* 满了，整体前移 1 字节 */
        memmove(textBuf, textBuf + 1, len - 1);
        textBuf[len - 1] = ch;              /* 新字符放最后 */
    }
    textBuf[len] = 0;                       /* 实时 '\0' 结尾 */
}

void homeLogicInit(ld_scene_t* ptScene)
{
    ldBase_t *win=ldBaseGetWidgetById(ID_WINDOW_SETTING);

    ldBaseSetHidden(win,true);

#if defined (_WIN32)
    ldComboBox_t *cb=ldBaseGetWidgetById(ID_COMBOBOX_PORTNAME);
    int n = com_count();
    ldComboBoxSetItemMax(cb,n);
    for (int i = 0; i < n; ++i) {
        char name[8];
        com_name(i, name);
        ldComboBoxAddItem(cb,name);
    }
#endif

    textShow=ldBaseGetWidgetById(ID_TEXT_SHOW);
    ldTextSetStaticText(textShow,textBuf);
}

void homeLogicLoop(ld_scene_t* ptScene)
{
    char rx[64+1];

    int num = com_recv(rx, sizeof(rx) - 1);

    if(num>0)
    {
    for(int i=0;i<num;i++)
    {
        ring_put(rx[i]);
        printf("%02X ",rx[i]);
    }
    textShow->use_as__ldBase_t.isDirtyRegionUpdate = true;
    text_box_c_str_reader_init( &textShow->tStringReader,
                                (char*)textShow->pStr,
                                strlen((char*)textShow->pStr));

    if(num)
    {
        LOG_DEBUG("recv num:%d",num);
        for(int i=0;i<100;i++)
        {
            printf("%02X ",textBuf[i]);
        }
    }
    }
//        if (num > 0)
//        {
//            rx[num] = 0;


//            LOG_DEBUG("recv %d bytes: %s", num, rx);
//        }



}

void homeLogicQuit(ld_scene_t* ptScene)
{





}
