#ifndef _GT911_H_
#define _GT911_H_

#include "driver/i2c.h"
#include "esp_log.h"

// #define GT911_ADDR       0x5D        /* 写命令 */

/* GT9XXX 部分寄存器定义  */
#define GT9XXX_CTRL_REG     0X8040      /* GT9XXX控制寄存器 */
#define GT9XXX_CFGS_REG     0X8047      /* GT9XXX配置起始地址寄存器 */
#define GT9XXX_CHECK_REG    0X80FF      /* GT9XXX校验和寄存器 */
#define GT9XXX_PID_REG      0X8140      /* GT9XXX产品ID寄存器 */

#define GT9XXX_GSTID_REG    0X814E      /* GT9XXX当前检测到的触摸情况 */
#define GT9XXX_TP1_REG      0X8150      /* 第一个触摸点数据地址 */
#define GT9XXX_TP2_REG      0X8158      /* 第二个触摸点数据地址 */
#define GT9XXX_TP3_REG      0X8160      /* 第三个触摸点数据地址 */
#define GT9XXX_TP4_REG      0X8168      /* 第四个触摸点数据地址 */
#define GT9XXX_TP5_REG      0X8170      /* 第五个触摸点数据地址 */
#define GT9XXX_TP6_REG      0X8178      /* 第六个触摸点数据地址 */
#define GT9XXX_TP7_REG      0X8180      /* 第七个触摸点数据地址 */
#define GT9XXX_TP8_REG      0X8188      /* 第八个触摸点数据地址 */
#define GT9XXX_TP9_REG      0X8190      /* 第九个触摸点数据地址 */
#define GT9XXX_TP10_REG     0X8198      /* 第十个触摸点数据地址 */

#define TP_PRES_DOWN        0x8000      /* 触屏被按下 */
#define TP_CATH_PRES        0x4000      /* 有按键按下了 */

#define MAX_TOUCH_NUM 5 

struct TP
{
    char * chipId;
    i2c_port_t i2c;
    uint32_t freq;
    int8_t sdaPin;
    int8_t sclPin;
    int8_t intPin;
    int8_t resetPin;
    uint16_t height;
    uint16_t width;
    uint8_t maxTouch;
    uint16_t x[MAX_TOUCH_NUM];
    uint16_t y[MAX_TOUCH_NUM];
    
    uint16_t status;            /* 触摸状态
                                 * b15:按下1/松开0;
                                 * b14:0,没有按键按下;1,有按键按下.
                                 * b13~b10:保留
                                 * b9~b0:电容触摸屏按下的点数(0,表示未按下,1表示按下)
                                 */


    uint8_t touchtype;         /* 当触摸屏的左右上下完全颠倒时需要用到.
                                * b0:0, 竖屏(适合左右为X坐标,上下为Y坐标的TP)
                                *    1, 横屏(适合左右为Y坐标,上下为X坐标的TP)
                                * b1~6: 保留.
                                * b7:0, 电阻屏
                                *    1, 电容屏
                                */
};

extern struct TP tp;
void gt911_init(uint32_t freq);
void gt911_write_reg(uint16_t reg, uint8_t *data, uint8_t len);
void gt911_read_reg(uint16_t reg, uint8_t *data, uint8_t len);
uint8_t gt911_scan(uint8_t mode);
void gt911_get_pos(uint8_t index, uint16_t *x, uint16_t *y);





#endif