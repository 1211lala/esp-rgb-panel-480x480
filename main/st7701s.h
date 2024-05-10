#ifndef _ST7701S_H
#define _ST7701S_H

#include <stdio.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"
#include "freertos/task.h"
#include "esp_log.h"

#include "driver/spi_master.h"


void spi_9bit_init(void);
void st7701s_init(unsigned char type);
void spi_send_data(const uint8_t *data, int16_t len);
void lcd_write_1R1D(const uint8_t cmd, const uint8_t data);


#endif 
