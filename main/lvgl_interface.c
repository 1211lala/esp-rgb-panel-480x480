/******************************************************************
 * 参考: 移植触摸参考 https://blog.csdn.net/baidu_19348579/article/details/128732552
 *       PCLK时钟过快，导致屏幕上下偏移: https://blog.csdn.net/ami82/article/details/136257585?spm=1001.2014.3001.5502
 
 ********************************************************************/

#include <stdio.h>
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "esp_timer.h"
#include "esp_lcd_panel_ops.h"
#include "esp_lcd_panel_rgb.h"
#include "esp_lcd_panel_io.h"
#include "driver/gpio.h"
#include "esp_err.h"
#include "esp_log.h"
#include "esp_check.h"
#include "driver/i2c.h"

#include "demos/lv_demos.h"
#include "lvgl.h"
#include "st7701s.h"
#include "gt911.h"

/*****************************************************************显示设备***********************************************************************/
static const char *TAG = "rgb";

#define EXAMPLE_LCD_PIXEL_CLOCK_HZ (18 * 1000 * 1000)

#define EXAMPLE_LCD_BK_LIGHT_ON_LEVEL 1
#define EXAMPLE_LCD_BK_LIGHT_OFF_LEVEL !EXAMPLE_LCD_BK_LIGHT_ON_LEVEL
#define EXAMPLE_PIN_NUM_BK_LIGHT 38

#define EXAMPLE_PIN_NUM_DE 18
#define EXAMPLE_PIN_NUM_VSYNC 17
#define EXAMPLE_PIN_NUM_HSYNC 16
#define EXAMPLE_PIN_NUM_PCLK 21

#define EXAMPLE_PIN_NUM_DATA11 11 // R0
#define EXAMPLE_PIN_NUM_DATA12 12 // R1
#define EXAMPLE_PIN_NUM_DATA13 13 // R2
#define EXAMPLE_PIN_NUM_DATA14 14 // R3
#define EXAMPLE_PIN_NUM_DATA15 0  // R4

#define EXAMPLE_PIN_NUM_DATA5 8   // G0
#define EXAMPLE_PIN_NUM_DATA6 20  // G1
#define EXAMPLE_PIN_NUM_DATA7 3   // G2
#define EXAMPLE_PIN_NUM_DATA8 46  // G3
#define EXAMPLE_PIN_NUM_DATA9 9   // G4
#define EXAMPLE_PIN_NUM_DATA10 10 // G5

#define EXAMPLE_PIN_NUM_DATA0 4  // B0
#define EXAMPLE_PIN_NUM_DATA1 5  // B1
#define EXAMPLE_PIN_NUM_DATA2 6  // B2
#define EXAMPLE_PIN_NUM_DATA3 7  // B3
#define EXAMPLE_PIN_NUM_DATA4 15 // B4

#define LCD_SPI_CS 39

#define EXAMPLE_PIN_NUM_DISP_EN -1

// 设置屏幕的像素大小
#define EXAMPLE_LCD_H_RES 480
#define EXAMPLE_LCD_V_RES 480

#if CONFIG_EXAMPLE_DOUBLE_FB
#define EXAMPLE_LCD_NUM_FB 2
#else
#define EXAMPLE_LCD_NUM_FB 1
#endif

#define EXAMPLE_LVGL_TICK_PERIOD_MS 2

// 使用两个信号量来同步VSYNC事件和LVGL任务，以避免潜在的撕裂效应
#if CONFIG_EXAMPLE_AVOID_TEAR_EFFECT_WITH_SEM
SemaphoreHandle_t sem_vsync_end;
SemaphoreHandle_t sem_gui_ready;
#endif

static bool example_on_vsync_event(esp_lcd_panel_handle_t panel, const esp_lcd_rgb_panel_event_data_t *event_data, void *user_data)
{
    BaseType_t high_task_awoken = pdFALSE;
#if CONFIG_EXAMPLE_AVOID_TEAR_EFFECT_WITH_SEM
    if (xSemaphoreTakeFromISR(sem_gui_ready, &high_task_awoken) == pdTRUE)
    {
        xSemaphoreGiveFromISR(sem_vsync_end, &high_task_awoken);
    }
#endif
    return high_task_awoken == pdTRUE;
}

static void example_lvgl_flush_cb(lv_disp_drv_t *drv, const lv_area_t *area, lv_color_t *color_map)
{
    esp_lcd_panel_handle_t panel_handle = (esp_lcd_panel_handle_t)drv->user_data;
    int offsetx1 = area->x1;
    int offsetx2 = area->x2;
    int offsety1 = area->y1;
    int offsety2 = area->y2;
#if CONFIG_EXAMPLE_AVOID_TEAR_EFFECT_WITH_SEM
    xSemaphoreGive(sem_gui_ready);
    xSemaphoreTake(sem_vsync_end, portMAX_DELAY);
#endif
    // 将绘制缓冲区传递给驱动程序
    esp_lcd_panel_draw_bitmap(panel_handle, offsetx1, offsety1, offsetx2 + 1, offsety2 + 1, color_map);
    // 通知lvgl填充完成
    lv_disp_flush_ready(drv);
}

void example_increase_lvgl_tick(void *arg)
{
    lv_tick_inc(EXAMPLE_LVGL_TICK_PERIOD_MS);
}

void lvgl_disp_init(void)
{

    spi_9bit_init();
    st7701s_init(1);

    static lv_disp_drv_t disp_drv;
    static lv_disp_draw_buf_t disp_buf;
#if CONFIG_EXAMPLE_AVOID_TEAR_EFFECT_WITH_SEM
    ESP_LOGI(TAG, "Create semaphores");
    sem_vsync_end = xSemaphoreCreateBinary();
    assert(sem_vsync_end);
    sem_gui_ready = xSemaphoreCreateBinary();
    assert(sem_gui_ready);
#endif
    ESP_LOGI(TAG, "初始化背光");
    gpio_config_t bk_gpio_config = {};
    bk_gpio_config.mode = GPIO_MODE_OUTPUT,
    bk_gpio_config.pin_bit_mask = 1ULL << EXAMPLE_PIN_NUM_BK_LIGHT;
    ESP_ERROR_CHECK(gpio_config(&bk_gpio_config));

    ESP_LOGI(TAG, "初始化LCD驱动");
    esp_lcd_panel_handle_t panel_handle = NULL;
    esp_lcd_rgb_panel_config_t panel_config = {
        .data_width = 16,
        .psram_trans_align = 64,
        .num_fbs = EXAMPLE_LCD_NUM_FB,
#if CONFIG_EXAMPLE_USE_BOUNCE_BUFFER
        .bounce_buffer_size_px = 10 * EXAMPLE_LCD_H_RES,
#endif
        .clk_src = LCD_CLK_SRC_DEFAULT,
        .disp_gpio_num = EXAMPLE_PIN_NUM_DISP_EN,
        .pclk_gpio_num = EXAMPLE_PIN_NUM_PCLK,
        .vsync_gpio_num = EXAMPLE_PIN_NUM_VSYNC,
        .hsync_gpio_num = EXAMPLE_PIN_NUM_HSYNC,
        .de_gpio_num = EXAMPLE_PIN_NUM_DE,
        .data_gpio_nums = {
            EXAMPLE_PIN_NUM_DATA0,
            EXAMPLE_PIN_NUM_DATA1,
            EXAMPLE_PIN_NUM_DATA2,
            EXAMPLE_PIN_NUM_DATA3,
            EXAMPLE_PIN_NUM_DATA4,
            EXAMPLE_PIN_NUM_DATA5,
            EXAMPLE_PIN_NUM_DATA6,
            EXAMPLE_PIN_NUM_DATA7,
            EXAMPLE_PIN_NUM_DATA8,
            EXAMPLE_PIN_NUM_DATA9,
            EXAMPLE_PIN_NUM_DATA10,
            EXAMPLE_PIN_NUM_DATA11,
            EXAMPLE_PIN_NUM_DATA12,
            EXAMPLE_PIN_NUM_DATA13,
            EXAMPLE_PIN_NUM_DATA14,
            EXAMPLE_PIN_NUM_DATA15,
        },
        .timings = {
            .pclk_hz = EXAMPLE_LCD_PIXEL_CLOCK_HZ,
            .h_res = EXAMPLE_LCD_H_RES,
            .v_res = EXAMPLE_LCD_V_RES,
            .hsync_back_porch = 50,
            .hsync_front_porch = 10,
            .hsync_pulse_width = 8,
            .vsync_back_porch = 20,
            .vsync_front_porch = 10,
            .vsync_pulse_width = 8,
            // .hsync_back_porch = 40,
            // .hsync_front_porch = 20,
            // .hsync_pulse_width = 1,
            // .vsync_back_porch = 8,
            // .vsync_front_porch = 4,
            // .vsync_pulse_width = 1,
            // .flags.pclk_active_neg = true,
        },
        /*缓存被分配在spram中*/
        .flags.fb_in_psram = true,
    };
    ESP_ERROR_CHECK(esp_lcd_new_rgb_panel(&panel_config, &panel_handle));

    ESP_LOGI(TAG, "注册VSYNC回调事件函数");
    esp_lcd_rgb_panel_event_callbacks_t cbs = {
        .on_vsync = example_on_vsync_event,
    };
    ESP_ERROR_CHECK(esp_lcd_rgb_panel_register_event_callbacks(panel_handle, &cbs, &disp_drv));

    ESP_LOGI(TAG, "初始化LCD面板");
    ESP_ERROR_CHECK(esp_lcd_panel_reset(panel_handle));
    ESP_ERROR_CHECK(esp_lcd_panel_init(panel_handle));

    ESP_LOGI(TAG, "打开背光");
    gpio_set_level(EXAMPLE_PIN_NUM_BK_LIGHT, EXAMPLE_LCD_BK_LIGHT_ON_LEVEL);

    ESP_LOGI(TAG, "初始化lvgl库");
    lv_init();
    void *buf1 = NULL;
    void *buf2 = NULL;

#if CONFIG_EXAMPLE_DOUBLE_FB
    ESP_LOGI(TAG, "Use frame buffers as LVGL draw buffers");
    ESP_ERROR_CHECK(esp_lcd_rgb_panel_get_frame_buffer(panel_handle, 2, &buf1, &buf2));
#else
    ESP_LOGI(TAG, "从spram中分配内存");
    buf1 = heap_caps_malloc(EXAMPLE_LCD_H_RES * 480 * sizeof(lv_color_t), MALLOC_CAP_SPIRAM);
    assert(buf1);
    buf2 = heap_caps_malloc(EXAMPLE_LCD_H_RES * 480 * sizeof(lv_color_t), MALLOC_CAP_SPIRAM);
    assert(buf2);
    lv_disp_draw_buf_init(&disp_buf, buf1, buf2, EXAMPLE_LCD_H_RES * 480);
#endif
    ESP_LOGI(TAG, "注册lvgl_disp显示驱动");

    lv_disp_drv_init(&disp_drv);
    disp_drv.hor_res = EXAMPLE_LCD_H_RES;
    disp_drv.ver_res = EXAMPLE_LCD_V_RES;
    disp_drv.flush_cb = example_lvgl_flush_cb;
    disp_drv.draw_buf = &disp_buf;
    disp_drv.user_data = panel_handle;
#if CONFIG_EXAMPLE_DOUBLE_FB
    disp_drv.full_refresh = true; // 用于保证两个帧缓冲区之间的同步
#endif
    lv_disp_drv_register(&disp_drv);
    // lv_disp_t *disp = lv_disp_drv_register(&disp_drv);
    ESP_LOGI(TAG, "注册lvgl_tick时基回调函数");
    const esp_timer_create_args_t lvgl_tick_timer_args = {
        .callback = &example_increase_lvgl_tick,
        .name = "lvgl_tick"};
    esp_timer_handle_t lvgl_tick_timer = NULL;
    ESP_ERROR_CHECK(esp_timer_create(&lvgl_tick_timer_args, &lvgl_tick_timer));
    ESP_ERROR_CHECK(esp_timer_start_periodic(lvgl_tick_timer, EXAMPLE_LVGL_TICK_PERIOD_MS * 1000));
    ESP_LOGI(TAG, "lvlg显示设备初始化完成");
}

/************************************************************************触摸设备******************************************************************************/

void touchpad_read(lv_indev_drv_t *indev_drv, lv_indev_data_t *data)
{
    static lv_coord_t last_x = 0;
    static lv_coord_t last_y = 0;

    gt911_scan(0);
    if (tp.status & TP_PRES_DOWN)
    {
        last_x = tp.x[0];
        last_y = tp.y[0];
        data->state = LV_INDEV_STATE_PR;
    }
    else
    {
        data->state = LV_INDEV_STATE_REL;
    }
    data->point.x = last_x;
    data->point.y = last_y;
}

void lvgl_indev_init(void)
{
    gt911_init(100000);
    static lv_indev_drv_t indev_drv;

    lv_indev_drv_init(&indev_drv);
    indev_drv.type = LV_INDEV_TYPE_POINTER;
    indev_drv.read_cb = touchpad_read;
    lv_indev_drv_register(&indev_drv);
}
