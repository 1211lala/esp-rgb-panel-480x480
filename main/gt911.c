#include "gt911.h"
#include "string.h"

static const char *TAG = "GT911";

uint8_t GT911_ADDR = 0x5D;

struct TP tp = {
    .freq = 400000,
    .i2c = I2C_NUM_0,
    .sdaPin = 19,
    .sclPin = 45,
    .intPin = 40,
    .resetPin = -1,
    .height = 480,
    .width = 480,
    .status = 0,
    .touchtype = 0,
    .maxTouch = MAX_TOUCH_NUM,
};

/******************************************************************************
 * 函数描述: GT911初始化程序，初始化使用到的IIC
 * 参  数1: IIC总线频率
 *******************************************************************************/
void gt911_init(uint32_t freq)
{
    i2c_config_t config = {};
    config.mode = I2C_MODE_MASTER;
    config.sda_io_num = tp.sdaPin;
    config.sda_pullup_en = GPIO_PULLUP_ENABLE;
    config.scl_io_num = tp.sclPin;
    config.scl_pullup_en = GPIO_PULLUP_ENABLE;
    config.master.clk_speed = tp.freq;
    i2c_param_config(tp.i2c, &config);
    i2c_driver_install(tp.i2c, config.mode, 0, 0, 0);
    /* 判断地址是否正确 */
    uint8_t temp[5];
    gt911_read_reg(0X8140, temp, 4);
    temp[4] = '\0';
    ESP_LOGE(TAG, "TP Chip is GT%s\r\n", temp);
    if (strcmp((char *)temp, "911"))
    {
        vTaskDelay(20);
        GT911_ADDR = 0x14;
        gt911_read_reg(0X8140, temp, 4);
        temp[4] = '\0';
        ESP_LOGE(TAG, "TP Chip is GT%s\r\n", temp);
    }

    temp[0] = 0X02;
    gt911_write_reg(GT9XXX_CTRL_REG, &temp[0], 1);
    vTaskDelay(200);
    temp[0] = 0X00;
    gt911_write_reg(GT9XXX_CTRL_REG, &temp[0], 1);
}

void gt911_write_reg(uint16_t reg, uint8_t *data, uint8_t len)
{
    uint8_t *send = (uint8_t *)malloc(len + 2);
    uint8_t reg_h = reg >> 8;
    uint8_t reg_l = reg & 0xFF;
    memcpy(send, &reg_h, 1);
    memcpy(send + 1, &reg_l, 1);
    memcpy(send + 2, data, len);
    i2c_master_write_to_device(I2C_NUM_0, GT911_ADDR, send, len + 2, 1000);

    free(send);
}

void gt911_read_reg(uint16_t reg, uint8_t *data, uint8_t len)
{
    uint8_t reg_h = (reg >> 8);
    uint8_t reg_l = (uint8_t)reg & 0xff;
    uint8_t send[2] = {reg_h, reg_l};

    i2c_master_write_read_device(I2C_NUM_0, GT911_ADDR, send, 2, data, len, 1000);
}

/* GT9XXX 10个触摸点(最多) 对应的寄存器表 */
const uint16_t GT9XXX_TPX_TBL[10] =
    {
        GT9XXX_TP1_REG,
        GT9XXX_TP2_REG,
        GT9XXX_TP3_REG,
        GT9XXX_TP4_REG,
        GT9XXX_TP5_REG,
        GT9XXX_TP6_REG,
        GT9XXX_TP7_REG,
        GT9XXX_TP8_REG,
        GT9XXX_TP9_REG,
        GT9XXX_TP10_REG,
};

/**
 * @brief       扫描触摸屏(采用查询方式)
 * @retval      当前触屏状态
 *   @arg       0, 触屏无触摸;
 *   @arg       1, 触屏有触摸;
 */
uint8_t gt911_scan(uint8_t mode)
{
    uint8_t buf[4];
    uint8_t i = 0;
    uint8_t res = 0;
    uint16_t temp;
    uint16_t tempsta;
    static uint8_t t = 0; /* 控制查询间隔,从而降低CPU占用率 */
    t++;

    // if ((t % 10) == 0 || t < 10)    /* 空闲时,每进入10次CTP_Scan函数才检测1次,从而节省CPU使用率 */
    // {
    gt911_read_reg(GT9XXX_GSTID_REG, &mode, 1); /* 读取触摸点的状态 */

    if ((mode & 0X80) && ((mode & 0XF) <= tp.maxTouch))
    {
        i = 0;
        gt911_write_reg(GT9XXX_GSTID_REG, &i, 1); /* 清标志 */
    }

    if ((mode & 0XF) && ((mode & 0XF) <= tp.maxTouch))
    {
        temp = 0XFFFF << (mode & 0XF); /* 将点的个数转换为1的位数,匹配tp.status定义 */
        tempsta = tp.status;           /* 保存当前的tp.status值 */
        tp.status = (~temp) | TP_PRES_DOWN | TP_CATH_PRES;
        tp.x[tp.maxTouch - 1] = tp.x[0]; /* 保存触点0的数据,保存在最后一个上 */
        tp.y[tp.maxTouch - 1] = tp.y[0];

        for (i = 0; i < tp.maxTouch; i++)
        {
            if (tp.status & (1 << i)) /* 触摸有效? */
            {
                gt911_read_reg(GT9XXX_TPX_TBL[i], buf, 4); /* 读取XY坐标值 */

                if (tp.touchtype & 0X01) /* 横屏 */
                {
                    tp.y[i] = ((uint16_t)buf[1] << 8) + buf[0];
                    tp.x[i] = tp.width - (((uint16_t)buf[3] << 8) + buf[2]);
                }
                else
                {
                    tp.x[i] = ((uint16_t)buf[1] << 8) + buf[0];
                    tp.y[i] = ((uint16_t)buf[3] << 8) + buf[2];
                }
            }
        }

        res = 1;

        if (tp.x[0] > tp.width || tp.y[0] > tp.height) /* 非法数据(坐标超出了) */
        {
            if ((mode & 0XF) > 1) /* 有其他点有数据,则复第二个触点的数据到第一个触点. */
            {
                tp.x[0] = tp.x[1];
                tp.y[0] = tp.y[1];
                t = 0; /* 触发一次,则会最少连续监测10次,从而提高命中率 */
            }
            else /* 非法数据,则忽略此次数据(还原原来的) */
            {
                tp.x[0] = tp.x[tp.maxTouch - 1];
                tp.y[0] = tp.y[tp.maxTouch - 1];
                mode = 0X80;
                tp.status = tempsta; /* 恢复tp.status */
            }
        }
        else
        {
            t = 0; /* 触发一次,则会最少连续监测10次,从而提高命中率 */
        }
    }
    // }

    if ((mode & 0X8F) == 0X80) /* 无触摸点按下 */
    {
        if (tp.status & TP_PRES_DOWN) /* 之前是被按下的 */
        {
            tp.status &= ~TP_PRES_DOWN; /* 标记按键松开 */
        }
        else /* 之前就没有被按下 */
        {
            tp.x[0] = 0xffff;
            tp.y[0] = 0xffff;
            tp.status &= 0XE000; /* 清除点有效标记 */
        }
    }

    if (t > 240)
        t = 10; /* 重新从10开始计数 */

    return res;
}

void gt911_get_pos(uint8_t index, uint16_t *x, uint16_t *y)
{
    *x = tp.x[index];
    *y = tp.y[index];
}
