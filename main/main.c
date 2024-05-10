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
#include "esp_heap_caps.h"

#include "lv_demos.h"
#include "lvgl.h"
#include "lvgl_interface.h"
#include "p_wifi_sta.h"
void lvgl_hander(void *arg);
void system_info(void *arg);

static const char *TAG = "main";

extern void menu_slider(void);

static int move = 0;

static lv_anim_timeline_t *animline;
static lv_timer_t *t1;
static lv_timer_t *t2;
static lv_timer_t *t3;
static lv_obj_t *lable1;
static lv_obj_t *lable2;
static lv_obj_t *lable3;
static lv_obj_t *lable4;
static lv_obj_t *obj1;
static lv_obj_t *obj2;
static lv_obj_t *obj3;
static lv_obj_t *obj4;
static lv_obj_t *obj5;
static lv_obj_t *obj6;
static lv_obj_t *obj7;
static lv_obj_t *obj8;
static lv_obj_t *obj11;
static lv_obj_t *obj22;
static lv_obj_t *obj33;
static lv_obj_t *obj44;
static lv_obj_t *obj55;
static lv_obj_t *obj66;
static lv_obj_t *obj77;
static lv_obj_t *obj88;
static lv_obj_t *tile;
static lv_obj_t *tile0;
static lv_obj_t *tile1;
static void timer_cb1(lv_timer_t *t);
static void timer_cb2(lv_timer_t *t);
static void timer_cb3(lv_timer_t *t);
static lv_obj_t *botton_left;
static lv_obj_t *botton_right;

static void botton_left_event_cb(lv_event_t *e);
static void botton_right_event_cb(lv_event_t *e);
static void event_cb(lv_event_t *e);
static void event_cb1(lv_event_t *e);
static void event_cb2(lv_event_t *e);
static void event_cb3(lv_event_t *e);
static void event_cb4(lv_event_t *e);
static void event_cb5(lv_event_t *e);
static void event_cb6(lv_event_t *e);
static void event_cb7(lv_event_t *e);
static void tile0_pressing_cb(lv_event_t *e);

void event_cb2(lv_event_t *e)
{
    // ballgame_start();
}

void menu_slider(void)
{
    lv_obj_set_style_bg_color(lv_scr_act(), lv_color_hex(0x000020), LV_PART_MAIN);
    lv_obj_clear_flag(lv_scr_act(), LV_OBJ_FLAG_SCROLLABLE);
    tile = lv_tileview_create(lv_scr_act());

    //	lv_obj_set_size(tile, 300, 300);
    //	lv_obj_align(tile, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_bg_color(tile, lv_color_hex(0x000000), LV_PART_MAIN);

    tile0 = lv_tileview_add_tile(tile, 0, 0, LV_DIR_VER);
    tile1 = lv_tileview_add_tile(tile, 0, 1, LV_DIR_VER);
    lv_obj_set_style_bg_color(tile0, lv_color_hex(0x000000), LV_PART_MAIN);
    lv_obj_set_style_bg_color(tile1, lv_color_hex(0x000000), LV_PART_MAIN);

    obj1 = lv_obj_create(tile0);
    lv_obj_set_style_radius(obj1, 20, LV_PART_MAIN);
    lv_obj_align(obj1, LV_ALIGN_CENTER, 0, -50);
    lv_obj_set_style_bg_color(obj1, lv_color_hex(0xff0020), LV_PART_MAIN);
    lv_obj_set_size(obj1, 80, 80);

    obj2 = lv_obj_create(tile0);
    lv_obj_set_style_radius(obj2, 20, LV_PART_MAIN);
    lv_obj_align(obj2, LV_ALIGN_CENTER, 150, -50);
    lv_obj_set_style_bg_color(obj2, lv_color_hex(0xff00ff), LV_PART_MAIN);
    lv_obj_set_size(obj2, 80, 80);

    obj3 = lv_obj_create(tile0);
    lv_obj_set_style_radius(obj3, 20, LV_PART_MAIN);
    lv_obj_align(obj3, LV_ALIGN_CENTER, 300, -50);
    lv_obj_set_style_bg_color(obj3, lv_color_hex(0x0000ff), LV_PART_MAIN);
    lv_obj_set_size(obj3, 80, 80);

    obj4 = lv_obj_create(tile0);
    lv_obj_set_style_radius(obj4, 20, LV_PART_MAIN);
    lv_obj_align(obj4, LV_ALIGN_CENTER, 450, -50);
    lv_obj_set_style_bg_color(obj4, lv_color_hex(0x00ffff), LV_PART_MAIN);
    lv_obj_set_size(obj4, 80, 80);

    obj5 = lv_obj_create(tile0);
    lv_obj_set_style_radius(obj5, 20, LV_PART_MAIN);
    lv_obj_align(obj5, LV_ALIGN_CENTER, 600, -50);
    lv_obj_set_style_bg_color(obj5, lv_color_hex(0x00ff00), LV_PART_MAIN);
    lv_obj_set_size(obj5, 80, 80);

    obj6 = lv_obj_create(tile0);
    lv_obj_set_style_radius(obj6, 20, LV_PART_MAIN);
    lv_obj_align(obj6, LV_ALIGN_CENTER, 750, -50);
    lv_obj_set_style_bg_color(obj6, lv_color_hex(0xffff00), LV_PART_MAIN);
    lv_obj_set_size(obj6, 80, 80);

    obj7 = lv_obj_create(tile0);
    lv_obj_set_style_radius(obj7, 20, LV_PART_MAIN);
    lv_obj_align(obj7, LV_ALIGN_CENTER, 950, -50);
    lv_obj_set_style_bg_color(obj7, lv_color_hex(0x000000), LV_PART_MAIN);

    lv_obj_set_style_border_width(obj7, 0, LV_STATE_DEFAULT);
    lv_obj_set_size(obj7, 80, 80);

    obj11 = lv_obj_create(tile1);
    lv_obj_set_style_radius(obj11, 20, LV_PART_MAIN);
    lv_obj_align(obj11, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_bg_color(obj11, lv_color_hex(0xff0020), LV_PART_MAIN);
    lv_obj_set_size(obj11, 80, 80);

    obj22 = lv_obj_create(tile1);
    lv_obj_set_style_radius(obj22, 20, LV_PART_MAIN);
    lv_obj_align(obj22, LV_ALIGN_CENTER, 150, 0);
    lv_obj_set_style_bg_color(obj22, lv_color_hex(0xff00ff), LV_PART_MAIN);
    lv_obj_set_size(obj22, 80, 80);

    obj33 = lv_obj_create(tile1);
    lv_obj_set_style_radius(obj33, 20, LV_PART_MAIN);
    lv_obj_align(obj33, LV_ALIGN_CENTER, 300, 0);
    lv_obj_set_style_bg_color(obj33, lv_color_hex(0x0000ff), LV_PART_MAIN);
    lv_obj_set_size(obj33, 80, 80);

    obj44 = lv_obj_create(tile1);
    lv_obj_set_style_radius(obj44, 20, LV_PART_MAIN);
    lv_obj_align(obj44, LV_ALIGN_CENTER, 450, 0);
    lv_obj_set_style_bg_color(obj44, lv_color_hex(0x00ffff), LV_PART_MAIN);
    lv_obj_set_size(obj44, 80, 80);

    obj55 = lv_obj_create(tile1);
    lv_obj_set_style_radius(obj55, 20, LV_PART_MAIN);
    lv_obj_align(obj55, LV_ALIGN_CENTER, 600, 0);
    lv_obj_set_style_bg_color(obj55, lv_color_hex(0x00ff00), LV_PART_MAIN);
    lv_obj_set_size(obj55, 80, 80);

    obj66 = lv_obj_create(tile1);
    lv_obj_set_style_radius(obj66, 20, LV_PART_MAIN);
    lv_obj_align(obj66, LV_ALIGN_CENTER, 750, 0);
    lv_obj_set_style_bg_color(obj66, lv_color_hex(0xffff00), LV_PART_MAIN);
    lv_obj_set_size(obj66, 80, 80);

    obj77 = lv_obj_create(tile1);
    lv_obj_set_style_radius(obj77, 20, LV_PART_MAIN);
    lv_obj_align(obj77, LV_ALIGN_CENTER, 900, 0);
    lv_obj_set_style_bg_color(obj77, lv_color_hex(0x8080ff), LV_PART_MAIN);
    lv_obj_set_size(obj77, 80, 80);

    obj88 = lv_obj_create(tile1);
    lv_obj_set_style_radius(obj88, 20, LV_PART_MAIN);
    lv_obj_align(obj88, LV_ALIGN_CENTER, 1050, 0);
    lv_obj_set_style_bg_color(obj88, lv_color_hex(0xff8000), LV_PART_MAIN);
    lv_obj_set_size(obj88, 80, 80);

    lv_obj_add_event_cb(tile0, event_cb, LV_EVENT_SCROLL_END, 0);

    t1 = lv_timer_create(timer_cb1, 20, 0);
}

static void event_cb(lv_event_t *e)
{

    move = lv_obj_get_scroll_x(tile0);

    if (move % 150 != 0)
    {
        if (move % 150 < 75)
        {
            lv_obj_scroll_to_x(tile0, move - (move % 150), LV_ANIM_ON);
        }
        if (move % 150 > 74)
        {
            lv_obj_scroll_to_x(tile0, move + 150 - (move % 150), LV_ANIM_ON);
        }
    }
}

// void tile0_pressing_cb(lv_event_t * e)
void timer_cb1(lv_timer_t *t)
{
    int move;

    move = lv_obj_get_scroll_x(tile0);
    if (move <= 150 && move >= 0)
    {
        lv_obj_set_width(obj1, 130 - move / 3);
        lv_obj_set_height(obj1, 130 - move / 3);
        lv_obj_set_y(obj1, -move / 3);

        lv_obj_set_width(obj2, 80 + move / 3);
        lv_obj_set_height(obj2, 80 + move / 3);
        lv_obj_set_y(obj2, move / 3 - 50);
    }

    if (move <= 300 && move >= 150)
    {
        lv_obj_set_width(obj2, 130 - (move - 150) / 3);
        lv_obj_set_height(obj2, 130 - (move - 150) / 3);
        lv_obj_set_y(obj2, -(move - 150) / 3);

        lv_obj_set_width(obj3, 80 + (move - 150) / 3);
        lv_obj_set_height(obj3, 80 + (move - 150) / 3);
        lv_obj_set_y(obj3, (move - 150) / 3 - 50);
    }

    if (move <= 450 && move >= 300)
    {
        lv_obj_set_width(obj3, 130 - (move - 300) / 3);
        lv_obj_set_height(obj3, 130 - (move - 300) / 3);
        lv_obj_set_y(obj3, -(move - 300) / 3);

        lv_obj_set_width(obj4, 80 + (move - 300) / 3);
        lv_obj_set_height(obj4, 80 + (move - 300) / 3);
        lv_obj_set_y(obj4, (move - 300) / 3 - 50);
    }

    if (move <= 600 && move >= 450)
    {
        lv_obj_set_width(obj4, 130 - (move - 450) / 3);
        lv_obj_set_height(obj4, 130 - (move - 450) / 3);
        lv_obj_set_y(obj4, -(move - 450) / 3);

        lv_obj_set_width(obj5, 80 + (move - 450) / 3);
        lv_obj_set_height(obj5, 80 + (move - 450) / 3);
        lv_obj_set_y(obj5, (move - 450) / 3 - 50);
    }

    if (move <= 750 && move >= 600)
    {
        lv_obj_set_width(obj5, 130 - (move - 600) / 3);
        lv_obj_set_height(obj5, 130 - (move - 600) / 3);
        lv_obj_set_y(obj5, -(move - 600) / 3);

        lv_obj_set_width(obj6, 80 + (move - 600) / 3);
        lv_obj_set_height(obj6, 80 + (move - 600) / 3);
        lv_obj_set_y(obj6, (move - 600) / 3 - 50);
    }

    if (move <= 900 && move >= 750)
    {
        lv_obj_set_width(obj6, 130 - (move - 750) / 3);
        lv_obj_set_height(obj6, 130 - (move - 750) / 3);
        lv_obj_set_y(obj6, -(move - 750) / 3);

        lv_obj_set_width(obj7, 80 + (move - 750) / 3);
        lv_obj_set_height(obj7, 80 + (move - 750) / 3);
        lv_obj_set_y(obj7, (move - 750) / 3 - 50);
    }

    if (move <= 1050 && move >= 900)
    {
        lv_obj_set_width(obj7, 130 - (move - 900) / 3);
        lv_obj_set_height(obj7, 130 - (move - 900) / 3);
        lv_obj_set_y(obj7, -(move - 900) / 3);

        lv_obj_set_width(obj8, 80 + (move - 900) / 3);
        lv_obj_set_height(obj8, 80 + (move - 900) / 3);
        lv_obj_set_y(obj8, (move - 900) / 3 - 50);
    }
}

void wifi_event_callback(void *event_handler_arg, esp_event_base_t event_base, int32_t event_id, void *event_data)
{
    static int s_retry_num = 0;

    if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_START)
    {
        esp_wifi_connect();
    }

    if (event_base == IP_EVENT && event_id == IP_EVENT_STA_GOT_IP)
    {
        ip_event_got_ip_t *event = (ip_event_got_ip_t *)event_data;

        s_retry_num = 0;
        ESP_LOGI("wifi", "Got IP: " IPSTR, IP2STR(&event->ip_info.ip));
        ESP_LOGI("wifi", "Got GW: " IPSTR, IP2STR(&event->ip_info.gw));
        ESP_LOGI("wifi", "Got NT: " IPSTR, IP2STR(&event->ip_info.netmask));

        xEventGroupSetBits(s_wifi_event_group, WIFI_CONNECTED_BIT);
    }

    if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_DISCONNECTED)
    {
        if (s_retry_num < 1000)
        {
            esp_wifi_connect();
            s_retry_num++;
            ESP_LOGE("wifi", "重新开始连接AP %d/%d", s_retry_num, 1000);
        }
        else
        {
            xEventGroupSetBits(s_wifi_event_group, WIFI_FAIL_BIT);
            ESP_LOGE("wifi", "连接失败");
        }
    }
}

void app_main(void)
{

    // wifi_sta_init(&wp, wifi_event_callback);

    lvgl_disp_init();
    lvgl_indev_init();
    lv_demo_benchmark();

    // menu_slider();

    xTaskCreatePinnedToCore(system_info, "system_info", 1024 * 4, NULL, 4, NULL, 1);
    xTaskCreatePinnedToCore(lvgl_hander, "lvgl_hander", 1024 * 4, NULL, 3, NULL, 1);
}

void system_info(void *arg)
{

    while (1)
    {
        printf("psram free: %d kb\r\n", heap_caps_get_free_size(MALLOC_CAP_SPIRAM) / 1024);
        printf("sram free: %d kb\r\n", heap_caps_get_free_size(MALLOC_CAP_INTERNAL) / 1024);
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

void lvgl_hander(void *arg)
{
    while (1)
    {
        vTaskDelay(pdMS_TO_TICKS(5));
        lv_timer_handler();
    }
}