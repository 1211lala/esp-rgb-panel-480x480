#include "st7701s.h"

#define delay(ms) vTaskDelay(ms / portTICK_PERIOD_MS)

static spi_device_handle_t spi;

void spi_9bit_init(void)
{
    spi_bus_config_t spibuscfg = {};
    spibuscfg.sclk_io_num = 48;
    spibuscfg.mosi_io_num = 47;
    spibuscfg.miso_io_num = -1;
    spibuscfg.quadhd_io_num = -1;
    spibuscfg.quadwp_io_num = -1;
    spibuscfg.flags = SPICOMMON_BUSFLAG_DUAL;
    spibuscfg.max_transfer_sz = 4096;
    ESP_ERROR_CHECK(spi_bus_initialize(SPI3_HOST, &spibuscfg, SPI_DMA_CH_AUTO));

    spi_device_interface_config_t devcfg = {};
    devcfg.clock_source = SPI_CLK_SRC_DEFAULT;
    devcfg.clock_speed_hz = 8 * 1000 * 1000;
    devcfg.command_bits = 1;
    devcfg.mode = 0;
    devcfg.spics_io_num = 39;
    devcfg.queue_size = 1;
    devcfg.flags = SPI_DEVICE_3WIRE | SPI_DEVICE_HALFDUPLEX;
    ESP_ERROR_CHECK(spi_bus_add_device(SPI3_HOST, &devcfg, &spi));
}


void spi_writecmd(uint8_t cmd)
{
    int ret = 0;
    spi_transaction_t spi_tran = {};
   
    memset(&spi_tran, 0, sizeof(spi_tran));
    spi_tran.cmd = 0;
    spi_tran.length = 8;
    spi_tran.tx_buffer = &cmd;
    ret = spi_device_polling_transmit(spi, &spi_tran);
    assert(ret == ESP_OK);
}

void spi_writedata(uint8_t data)
{
    int ret = 0;
    spi_transaction_t spi_tran = {};
   
    memset(&spi_tran, 0, sizeof(spi_tran));
    spi_tran.cmd = 1;
    spi_tran.length = 8;
    spi_tran.tx_buffer = &data;
    ret = spi_device_polling_transmit(spi, &spi_tran);
    assert(ret == ESP_OK);
}


void st7701s_init(unsigned char type)
{
    if (type == 1)
    {
        spi_writecmd(0x11);
        vTaskDelay(10);

        //--------------------------Bank0 Setting-----------------------------------//
        //----------------------Display Control setting-----------------------------//
        //--------------------------Bank1 Setting-----------------------------------//
        spi_writecmd(0xFF);
        spi_writedata(0x77);
        spi_writedata(0x01);
        spi_writedata(0x00);
        spi_writedata(0x00);
        spi_writedata(0x10);

        spi_writecmd(0xC0);
        spi_writedata(0x3B);
        spi_writedata(0x00);

        spi_writecmd(0xC1);
        spi_writedata(0x0D);
        spi_writedata(0x02);

        spi_writecmd(0xC2);
        spi_writedata(0x31);
        spi_writedata(0x05);
        /* 自己添加*/
        spi_writecmd(0xCD);
        spi_writedata(0x08);
        //--------------------------Gamma Cluster Setting--------------------------//
        spi_writecmd(0xB0);
        spi_writedata(0x00);
        spi_writedata(0x11);
        spi_writedata(0x18);
        spi_writedata(0x0E);
        spi_writedata(0x11);
        spi_writedata(0x06);
        spi_writedata(0x07);
        spi_writedata(0x08);
        spi_writedata(0x07);
        spi_writedata(0x22);
        spi_writedata(0x04);
        spi_writedata(0x12);
        spi_writedata(0x0F);
        spi_writedata(0xAA);
        spi_writedata(0x31);
        spi_writedata(0x18);

        spi_writecmd(0xB1);
        spi_writedata(0x00);
        spi_writedata(0x11);
        spi_writedata(0x19);
        spi_writedata(0x0E);
        spi_writedata(0x12);
        spi_writedata(0x07);
        spi_writedata(0x08);
        spi_writedata(0x08);
        spi_writedata(0x08);
        spi_writedata(0x22);
        spi_writedata(0x04);
        spi_writedata(0x11);
        spi_writedata(0x11);
        spi_writedata(0xA9);
        spi_writedata(0x32);
        spi_writedata(0x18);

        //-------------------------Bank1 Setting---------------------------------//
        spi_writecmd(0xFF);
        spi_writedata(0x77);
        spi_writedata(0x01);
        spi_writedata(0x00);
        spi_writedata(0x00);
        spi_writedata(0x11);

        spi_writecmd(0xb0);
        spi_writedata(0x60);
        //--------------------------Vcom Setting--------------------------------//
        spi_writecmd(0xb1);
        spi_writedata(0x32);
        //-----------------------End Vcom Setting------------------------------//
        spi_writecmd(0xb2);
        spi_writedata(0x07);

        spi_writecmd(0xb3);
        spi_writedata(0x80);

        spi_writecmd(0xb5);
        spi_writedata(0x49);

        spi_writecmd(0xb7);
        spi_writedata(0x85);

        spi_writecmd(0xb8);
        spi_writedata(0x21);

        spi_writecmd(0xC1);
        spi_writedata(0x78);

        spi_writecmd(0xC2);
        spi_writedata(0x78);

        // spi_writecmd(0xD0);
        // SPI_WriteData(0x88);
        //-----------End Power Control Registers Initial ------------------//
        // vTaskDelayNms(100);
        //----------------GIP Setting--------------------------------------//
        spi_writecmd(0xE0);
        spi_writedata(0x00);
        spi_writedata(0x1B);
        spi_writedata(0x02);

        spi_writecmd(0xE1);
        spi_writedata(0x08);
        spi_writedata(0xA0);
        spi_writedata(0x00);
        spi_writedata(0x00);
        spi_writedata(0x07);
        spi_writedata(0xA0);
        spi_writedata(0x00);
        spi_writedata(0x00);
        spi_writedata(0x00);
        spi_writedata(0x44);
        spi_writedata(0x44);

        spi_writecmd(0xE2);
        spi_writedata(0x11);
        spi_writedata(0x11);
        spi_writedata(0x44);
        spi_writedata(0x44);
        spi_writedata(0xED);
        spi_writedata(0xA0);
        spi_writedata(0x00);
        spi_writedata(0x00);
        spi_writedata(0xEC);
        spi_writedata(0xA0);
        spi_writedata(0x00);
        spi_writedata(0x00);

        spi_writecmd(0xE3);
        spi_writedata(0x00);
        spi_writedata(0x00);
        spi_writedata(0x11);
        spi_writedata(0x11);

        spi_writecmd(0xE4);
        spi_writedata(0x44);
        spi_writedata(0x44);

        spi_writecmd(0xE5);
        spi_writedata(0x0A);
        spi_writedata(0xE9);
        spi_writedata(0xD8);
        spi_writedata(0xA0);
        spi_writedata(0x0C);
        spi_writedata(0xEB);
        spi_writedata(0xD8);
        spi_writedata(0xA0);
        spi_writedata(0x0E);
        spi_writedata(0xED);
        spi_writedata(0xD8);
        spi_writedata(0xA0);
        spi_writedata(0x10);
        spi_writedata(0xEF);
        spi_writedata(0xD8);
        spi_writedata(0xA0);

        spi_writecmd(0xE6);
        spi_writedata(0x00);
        spi_writedata(0x00);
        spi_writedata(0x11);
        spi_writedata(0x11);

        spi_writecmd(0xE7);
        spi_writedata(0x44);
        spi_writedata(0x44);

        spi_writecmd(0xE8);
        spi_writedata(0x09);
        spi_writedata(0xE8);
        spi_writedata(0xD8);
        spi_writedata(0xA0);
        spi_writedata(0x0B);
        spi_writedata(0xEA);
        spi_writedata(0xD8);
        spi_writedata(0xA0);
        spi_writedata(0x0D);
        spi_writedata(0xEC);
        spi_writedata(0xD8);
        spi_writedata(0xA0);
        spi_writedata(0x0F);
        spi_writedata(0xEE);
        spi_writedata(0xD8);
        spi_writedata(0xA0);

        spi_writecmd(0xEB);
        spi_writedata(0x02);
        spi_writedata(0x00);
        spi_writedata(0xE4);
        spi_writedata(0xE4);
        spi_writedata(0x88);
        spi_writedata(0x00);
        spi_writedata(0x40);

        spi_writecmd(0xEC);
        spi_writedata(0x3c);
        spi_writedata(0x00);

        spi_writecmd(0xED); // ok
        spi_writedata(0xAB);
        spi_writedata(0x89);
        spi_writedata(0x76);
        spi_writedata(0x54);
        spi_writedata(0x02);
        spi_writedata(0xFF);
        spi_writedata(0xFF);
        spi_writedata(0xFF);
        spi_writedata(0xFF);
        spi_writedata(0xFF);
        spi_writedata(0xFF);
        spi_writedata(0x20);
        spi_writedata(0x45);
        spi_writedata(0x67);
        spi_writedata(0x98);
        spi_writedata(0xBA);

        // spi_writecmd(0x35);
        // SPI_WriteData(0x00); // Sleep-Out

        // spi_writecmd(0x36);
        // SPI_WriteData(0x00); // BGR
        // // spi_writecmd(0x36); SPI_WriteData(0x08); //RGB

        // // spi_writecmd(0x3A); SPI_WriteData(0x77); //24bit
        // // spi_writecmd(0x20);//display inversion off
        // spi_writecmd(0x3A);
        // SPI_WriteData(0x55); // 16bit
        // // spi_writecmd(0x3A); SPI_WriteData(0x66);//18bit
        // // spi_writecmd(0x22);//All pixel off

        // // SPI_WriteCmd(0x21);    //Normal Black
        // // spi_writecmd(0x11);//

        //--------------------------Bank3 Setting--------------------------------//
        spi_writecmd(0xFF);
        spi_writedata(0x77);
        spi_writedata(0x01);
        spi_writedata(0x00);
        spi_writedata(0x00);
        spi_writedata(0x13);

        spi_writecmd(0xE5);
        spi_writedata(0xE4);

        spi_writecmd(0xFF);
        spi_writedata(0x77);
        spi_writedata(0x01);
        spi_writedata(0x00);
        spi_writedata(0x00);
        spi_writedata(0x00);

        spi_writecmd(0x21);
        spi_writecmd(0x3a);
        spi_writedata(0x60);

        spi_writecmd(0x11);

        delay(10);
        spi_writecmd(0x29);
    }
    else if (type == 2)
    {
        // 从Arduino GFX库中移植

        spi_writecmd(0x11); // reset command

        spi_writecmd(0xFF);
        spi_writedata(0x77);
        spi_writedata(0x01);
        spi_writedata(0x00);
        spi_writedata(0x00);
        spi_writedata(0x10);

        spi_writecmd(0xC0);
        spi_writedata(0x3B);
        spi_writedata(0x00);

        spi_writecmd(0xC1);
        spi_writedata(0x0D);
        spi_writedata(0x02);

        spi_writecmd(0xC2);
        spi_writedata(0x31);
        spi_writedata(0x05);

        spi_writecmd(0xCD);
        spi_writedata(0x08);

        spi_writecmd(0xB0);
        spi_writedata(0x00);
        spi_writedata(0x11);
        spi_writedata(0x18);
        spi_writedata(0x0E);
        spi_writedata(0x11);
        spi_writedata(0x06);
        spi_writedata(0x07);
        spi_writedata(0x08);
        spi_writedata(0x07);
        spi_writedata(0x22);
        spi_writedata(0x04);
        spi_writedata(0x12);
        spi_writedata(0x0F);
        spi_writedata(0xAA);
        spi_writedata(0x31);
        spi_writedata(0x18);

        spi_writecmd(0xB1);
        spi_writedata(0x00);
        spi_writedata(0x11);
        spi_writedata(0x19);
        spi_writedata(0x0E);
        spi_writedata(0x12);
        spi_writedata(0x07);
        spi_writedata(0x08);
        spi_writedata(0x08);
        spi_writedata(0x08);
        spi_writedata(0x22);
        spi_writedata(0x04);
        spi_writedata(0x11);
        spi_writedata(0x11);
        spi_writedata(0xA9);
        spi_writedata(0x32);
        spi_writedata(0x18);

        spi_writecmd(0xFF);
        spi_writedata(0x77);
        spi_writedata(0x01);
        spi_writedata(0x00);
        spi_writedata(0x00);
        spi_writedata(0x11);

        spi_writecmd(0xB0);
        spi_writedata(0x60);

        spi_writecmd(0xB1);
        spi_writedata(0x32);

        spi_writecmd(0xB2);
        spi_writedata(0x07);

        spi_writecmd(0xB3);
        spi_writedata(0x80);

        spi_writecmd(0xB5);
        spi_writedata(0x49);

        spi_writecmd(0xB7);
        spi_writedata(0x85);

        spi_writecmd(0xB8);
        spi_writedata(0x21);

        spi_writecmd(0xC1);
        spi_writedata(0x78);

        spi_writecmd(0xC2);
        spi_writedata(0x78);

        spi_writecmd(0xE0);
        spi_writedata(0x00);
        spi_writedata(0x1B);
        spi_writedata(0x02);

        spi_writecmd(0xE1);
        spi_writedata(0x08);
        spi_writedata(0xA0);
        spi_writedata(0x00);
        spi_writedata(0x00);
        spi_writedata(0x07);
        spi_writedata(0xA0);
        spi_writedata(0x00);
        spi_writedata(0x00);
        spi_writedata(0x00);
        spi_writedata(0x44);
        spi_writedata(0x44);

        spi_writecmd(0xE2);
        spi_writedata(0x11);
        spi_writedata(0x11);
        spi_writedata(0x44);
        spi_writedata(0x44);
        spi_writedata(0xED);
        spi_writedata(0xA0);
        spi_writedata(0x00);
        spi_writedata(0x00);
        spi_writedata(0xEC);
        spi_writedata(0xA0);
        spi_writedata(0x00);
        spi_writedata(0x00);

        spi_writecmd(0xE3);
        spi_writedata(0x00);
        spi_writedata(0x00);
        spi_writedata(0x11);
        spi_writedata(0x11);

        spi_writecmd(0xE4);
        spi_writedata(0x44);
        spi_writedata(0x44);

        spi_writecmd(0xE5);
        spi_writedata(0x0A);
        spi_writedata(0xE9);
        spi_writedata(0xD8);
        spi_writedata(0xA0);
        spi_writedata(0x0C);
        spi_writedata(0xEB);
        spi_writedata(0xD8);
        spi_writedata(0xA0);
        spi_writedata(0x0E);
        spi_writedata(0xED);
        spi_writedata(0xD8);
        spi_writedata(0xA0);
        spi_writedata(0x10);
        spi_writedata(0xEF);
        spi_writedata(0xD8);
        spi_writedata(0xA0);

        spi_writecmd(0xE6);
        spi_writedata(0x00);
        spi_writedata(0x00);
        spi_writedata(0x11);
        spi_writedata(0x11);

        spi_writecmd(0xE7);
        spi_writedata(0x44);
        spi_writedata(0x44);

        spi_writecmd(0xE8);
        spi_writedata(0x09);
        spi_writedata(0xE8);
        spi_writedata(0xD8);
        spi_writedata(0xA0);
        spi_writedata(0x0B);
        spi_writedata(0xEA);
        spi_writedata(0xD8);
        spi_writedata(0xA0);
        spi_writedata(0x0D);
        spi_writedata(0xEC);
        spi_writedata(0xD8);
        spi_writedata(0xA0);
        spi_writedata(0x0F);
        spi_writedata(0xEE);
        spi_writedata(0xD8);
        spi_writedata(0xA0);

        spi_writecmd(0xEB);
        spi_writedata(0x02);
        spi_writedata(0x00);
        spi_writedata(0xE4);
        spi_writedata(0xE4);
        spi_writedata(0x88);
        spi_writedata(0x00);
        spi_writedata(0x40);

        spi_writecmd(0xEC);
        spi_writedata(0x3C);
        spi_writedata(0x00);

        spi_writecmd(0xED);
        spi_writedata(0xAB);
        spi_writedata(0x89);
        spi_writedata(0x76);
        spi_writedata(0x54);
        spi_writedata(0x02);
        spi_writedata(0xFF);
        spi_writedata(0xFF);
        spi_writedata(0xFF);
        spi_writedata(0xFF);
        spi_writedata(0xFF);
        spi_writedata(0xFF);
        spi_writedata(0x20);
        spi_writedata(0x45);
        spi_writedata(0x67);
        spi_writedata(0x98);
        spi_writedata(0xBA);

        spi_writecmd(0xFF);
        spi_writedata(0x77);
        spi_writedata(0x01);
        spi_writedata(0x00);
        spi_writedata(0x00);
        spi_writedata(0x13);

        spi_writecmd(0xE5);
        spi_writedata(0xE4);

        spi_writecmd(0xFF);
        spi_writedata(0x77);
        spi_writedata(0x01);
        spi_writedata(0x00);
        spi_writedata(0x00);
        spi_writedata(0x00);

        spi_writecmd(0x3A);
        spi_writedata(0x60);

        spi_writecmd(0x11);

        delay(120);

        spi_writecmd(0x29);

        // after batchOperation
        spi_writecmd(0x21);
        spi_writecmd(0xFF);
        spi_writedata(0x77);
        spi_writedata(0x01);
        spi_writedata(0x00);
        spi_writedata(0x00);
        spi_writedata(0x10);
        spi_writecmd(0xC7);
        spi_writedata(0x00);
        spi_writecmd(0xFF);
        spi_writedata(0x77);
        spi_writedata(0x01);
        spi_writedata(0x00);
        spi_writedata(0x00);
        spi_writedata(0x00);
        spi_writecmd(0x36);
        spi_writedata(0x00); // BGR
        // SPI_WriteData(0x08); //RGB
    }
    else if (type == 3)
    {
        // 网络查询复制, https://www.bilibili.com/video/BV1hB4y197YN/

        spi_writecmd(0xFF);
        spi_writedata(0x77);
        spi_writedata(0x01);
        spi_writedata(0x00);
        spi_writedata(0x00);
        spi_writedata(0x13);
        spi_writecmd(0xEF);
        spi_writedata(0x08);
        spi_writecmd(0xFF);
        spi_writedata(0x77);
        spi_writedata(0x01);
        spi_writedata(0x00);
        spi_writedata(0x00);
        spi_writedata(0x10);
        spi_writecmd(0xC0);
        spi_writedata(0x3B);
        spi_writedata(0x00);
        spi_writecmd(0xC1);
        spi_writedata(0x0D);
        spi_writedata(0x02);
        spi_writecmd(0xC2);
        spi_writedata(0x21);
        spi_writedata(0x08);

        spi_writecmd(0xCD);
        spi_writedata(0x08); // 18-bit/pixel: MDT=0:D[21:16]=R,D[13:8]=G,D[5:0]=B(CDH=00) ;
                             //               MDT=1:D[17:12]=R,D[11:6]=G,D[5:0]=B(CDH=08) ;

        spi_writecmd(0xB0);
        spi_writedata(0x00);
        spi_writedata(0x11);
        spi_writedata(0x18);
        spi_writedata(0x0E);
        spi_writedata(0x11);
        spi_writedata(0x06);
        spi_writedata(0x07);
        spi_writedata(0x08);
        spi_writedata(0x07);
        spi_writedata(0x22);
        spi_writedata(0x04);
        spi_writedata(0x12);
        spi_writedata(0x0F);
        spi_writedata(0xAA);
        spi_writedata(0x31);
        spi_writedata(0x18);
        spi_writecmd(0xB1);
        spi_writedata(0x00);
        spi_writedata(0x11);
        spi_writedata(0x19);
        spi_writedata(0x0E);
        spi_writedata(0x12);
        spi_writedata(0x07);
        spi_writedata(0x08);
        spi_writedata(0x08);
        spi_writedata(0x08);
        spi_writedata(0x22);
        spi_writedata(0x04);
        spi_writedata(0x11);
        spi_writedata(0x11);
        spi_writedata(0xA9);
        spi_writedata(0x32);
        spi_writedata(0x18);
        spi_writecmd(0xFF);
        spi_writedata(0x77);
        spi_writedata(0x01);
        spi_writedata(0x00);
        spi_writedata(0x00);
        spi_writedata(0x11);
        spi_writecmd(0xB0);
        spi_writedata(0x60);
        spi_writecmd(0xB1);
        spi_writedata(0x30);
        spi_writecmd(0xB2);
        spi_writedata(0x87);
        spi_writecmd(0xB3);
        spi_writedata(0x80);
        spi_writecmd(0xB5);
        spi_writedata(0x49);
        spi_writecmd(0xB7);
        spi_writedata(0x85);
        spi_writecmd(0xB8);
        spi_writedata(0x21);
        spi_writecmd(0xC1);
        spi_writedata(0x78);
        spi_writecmd(0xC2);
        spi_writedata(0x78);
        delay(2);
        spi_writecmd(0xE0);
        spi_writedata(0x00);
        spi_writedata(0x1B);
        spi_writedata(0x02);
        spi_writecmd(0xE1);
        spi_writedata(0x08);
        spi_writedata(0xA0);
        spi_writedata(0x00);
        spi_writedata(0x00);
        spi_writedata(0x07);
        spi_writedata(0xA0);
        spi_writedata(0x00);
        spi_writedata(0x00);
        spi_writedata(0x00);
        spi_writedata(0x44);
        spi_writedata(0x44);
        spi_writecmd(0xE2);
        spi_writedata(0x11);
        spi_writedata(0x11);
        spi_writedata(0x44);
        spi_writedata(0x44);
        spi_writedata(0xED);
        spi_writedata(0xA0);
        spi_writedata(0x00);
        spi_writedata(0x00);
        spi_writedata(0xEC);
        spi_writedata(0xA0);
        spi_writedata(0x00);
        spi_writedata(0x00);
        spi_writecmd(0xE3);
        spi_writedata(0x00);
        spi_writedata(0x00);
        spi_writedata(0x11);
        spi_writedata(0x11);
        spi_writecmd(0xE4);
        spi_writedata(0x44);
        spi_writedata(0x44);
        spi_writecmd(0xE5);
        spi_writedata(0x0A);
        spi_writedata(0xE9);
        spi_writedata(0xD8);
        spi_writedata(0xA0);
        spi_writedata(0x0C);
        spi_writedata(0xEB);
        spi_writedata(0xD8);
        spi_writedata(0xA0);
        spi_writedata(0x0E);
        spi_writedata(0xED);
        spi_writedata(0xD8);
        spi_writedata(0xA0);
        spi_writedata(0x10);
        spi_writedata(0xEF);
        spi_writedata(0xD8);
        spi_writedata(0xA0);
        spi_writecmd(0xE6);
        spi_writedata(0x00);
        spi_writedata(0x00);
        spi_writedata(0x11);
        spi_writedata(0x11);
        spi_writecmd(0xE7);
        spi_writedata(0x44);
        spi_writedata(0x44);
        spi_writecmd(0xE8);
        spi_writedata(0x09);
        spi_writedata(0xE8);
        spi_writedata(0xD8);
        spi_writedata(0xA0);
        spi_writedata(0x0B);
        spi_writedata(0xEA);
        spi_writedata(0xD8);
        spi_writedata(0xA0);
        spi_writedata(0x0D);
        spi_writedata(0xEC);
        spi_writedata(0xD8);
        spi_writedata(0xA0);
        spi_writedata(0x0F);
        spi_writedata(0xEE);
        spi_writedata(0xD8);
        spi_writedata(0xA0);
        spi_writecmd(0xEB);
        spi_writedata(0x02);
        spi_writedata(0x00);
        spi_writedata(0xE4);
        spi_writedata(0xE4);
        spi_writedata(0x88);
        spi_writedata(0x00);
        spi_writedata(0x40);
        spi_writecmd(0xEC);
        spi_writedata(0x3C);
        spi_writedata(0x00);
        spi_writecmd(0xED);
        spi_writedata(0xAB);
        spi_writedata(0x89);
        spi_writedata(0x76);
        spi_writedata(0x54);
        spi_writedata(0x02);
        spi_writedata(0xFF);
        spi_writedata(0xFF);
        spi_writedata(0xFF);
        spi_writedata(0xFF);
        spi_writedata(0xFF);
        spi_writedata(0xFF);
        spi_writedata(0x20);
        spi_writedata(0x45);
        spi_writedata(0x67);
        spi_writedata(0x98);
        spi_writedata(0xBA);
        spi_writecmd(0xEF);
        spi_writedata(0x10);
        spi_writedata(0x0D);
        spi_writedata(0x04);
        spi_writedata(0x08);
        spi_writedata(0x3F);
        spi_writedata(0x1F);
        spi_writecmd(0xFF);
        spi_writedata(0x77);
        spi_writedata(0x01);
        spi_writedata(0x00);
        spi_writedata(0x00);
        spi_writedata(0x00);

        spi_writecmd(0x3A);
        spi_writedata(0x50); // 55/50=16bit(RGB565);66=18bit(RGB666);77或默认不写3AH是=24bit(RGB888)

        spi_writecmd(0xFF);
        spi_writedata(0x77);
        spi_writedata(0x01);
        spi_writedata(0x00);
        spi_writedata(0x00);
        spi_writedata(0x13);
        spi_writecmd(0xE8);
        spi_writedata(0x00);
        spi_writedata(0x0E);
        spi_writecmd(0xFF);
        spi_writedata(0x77);
        spi_writedata(0x01);
        spi_writedata(0x00);
        spi_writedata(0x00);
        spi_writedata(0x00);
        spi_writecmd(0x11);
        delay(12);
        spi_writecmd(0xFF);
        spi_writedata(0x77);
        spi_writedata(0x01);
        spi_writedata(0x00);
        spi_writedata(0x00);
        spi_writedata(0x13);
        spi_writecmd(0xE8);
        spi_writedata(0x00);
        spi_writedata(0x0C);
        delay(1);
        spi_writecmd(0xE8);
        spi_writedata(0x00);
        spi_writedata(0x00);
        spi_writecmd(0xFF);
        spi_writedata(0x77);
        spi_writedata(0x01);
        spi_writedata(0x00);
        spi_writedata(0x00);
        spi_writedata(0x00);

        spi_writecmd(0x36);
        spi_writedata(0x00);

        spi_writecmd(0x20);

        spi_writecmd(0x29);
        delay(2);
    }
    else if (type == 4)
    {
        // 网络查询复制, https://www.bilibili.com/video/BV1hB4y197YN/

        spi_writecmd(0x11);
        delay(1);

        spi_writecmd(0xFF);
        spi_writedata(0x77);
        spi_writedata(0x01);
        spi_writedata(0x00);
        spi_writedata(0x00);
        spi_writedata(0x10);

        spi_writecmd(0xC0);
        spi_writedata(0x3B);
        spi_writedata(0x00);

        spi_writecmd(0xC1);
        spi_writedata(0x0D);
        spi_writedata(0x02);

        spi_writecmd(0xC2);
        spi_writedata(0x31);
        spi_writedata(0x05);

        spi_writecmd(0xB0);
        spi_writedata(0x00);
        spi_writedata(0x11);
        spi_writedata(0x18);
        spi_writedata(0x0E);
        spi_writedata(0x11);
        spi_writedata(0x06);
        spi_writedata(0x07);
        spi_writedata(0x08);
        spi_writedata(0x07);
        spi_writedata(0x22);
        spi_writedata(0x04);
        spi_writedata(0x12);
        spi_writedata(0x0F);
        spi_writedata(0xAA);
        spi_writedata(0x31);
        spi_writedata(0x18);

        spi_writecmd(0xB1);
        spi_writedata(0x00);
        spi_writedata(0x11);
        spi_writedata(0x19);
        spi_writedata(0x0E);
        spi_writedata(0x12);
        spi_writedata(0x07);
        spi_writedata(0x08);
        spi_writedata(0x08);
        spi_writedata(0x08);
        spi_writedata(0x22);
        spi_writedata(0x04);
        spi_writedata(0x11);
        spi_writedata(0x11);
        spi_writedata(0xA9);
        spi_writedata(0x32);
        spi_writedata(0x18);

        spi_writecmd(0xFF);
        spi_writedata(0x77);
        spi_writedata(0x01);
        spi_writedata(0x00);
        spi_writedata(0x00);
        spi_writedata(0x11);

        spi_writecmd(0xB0);
        spi_writedata(0x60);

        spi_writecmd(0xB1);
        spi_writedata(0x30);

        spi_writecmd(0xB2);
        spi_writedata(0x87);

        spi_writecmd(0xB3);
        spi_writedata(0x80);

        spi_writecmd(0xB5);
        spi_writedata(0x49);

        spi_writecmd(0xB7);
        spi_writedata(0x85);

        spi_writecmd(0xB8);
        spi_writedata(0x21);

        spi_writecmd(0xC1);
        spi_writedata(0x78);

        spi_writecmd(0xC2);
        spi_writedata(0x78);
        delay(2);
        spi_writecmd(0xE0);
        spi_writedata(0x00);
        spi_writedata(0x1B);
        spi_writedata(0x02);

        spi_writecmd(0xE1);
        spi_writedata(0x08);
        spi_writedata(0xA0);
        spi_writedata(0x00);
        spi_writedata(0x00);
        spi_writedata(0x07);
        spi_writedata(0xA0);
        spi_writedata(0x00);
        spi_writedata(0x00);
        spi_writedata(0x00);
        spi_writedata(0x44);
        spi_writedata(0x44);

        spi_writecmd(0xE2);
        spi_writedata(0x11);
        spi_writedata(0x11);
        spi_writedata(0x44);
        spi_writedata(0x44);
        spi_writedata(0xED);
        spi_writedata(0xA0);
        spi_writedata(0x00);
        spi_writedata(0x00);
        spi_writedata(0xEC);
        spi_writedata(0xA0);
        spi_writedata(0x00);
        spi_writedata(0x00);

        spi_writecmd(0xE3);
        spi_writedata(0x00);
        spi_writedata(0x00);
        spi_writedata(0x11);
        spi_writedata(0x11);

        spi_writecmd(0xE4);
        spi_writedata(0x44);
        spi_writedata(0x44);

        spi_writecmd(0xE5);
        spi_writedata(0x0A);
        spi_writedata(0xE9);
        spi_writedata(0xD8);
        spi_writedata(0xA0);
        spi_writedata(0x0C);
        spi_writedata(0xEB);
        spi_writedata(0xD8);
        spi_writedata(0xA0);
        spi_writedata(0x0E);
        spi_writedata(0xED);
        spi_writedata(0xD8);
        spi_writedata(0xA0);
        spi_writedata(0x10);
        spi_writedata(0xEF);
        spi_writedata(0xD8);
        spi_writedata(0xA0);

        spi_writecmd(0xE6);
        spi_writedata(0x00);
        spi_writedata(0x00);
        spi_writedata(0x11);
        spi_writedata(0x11);

        spi_writecmd(0xE7);
        spi_writedata(0x44);
        spi_writedata(0x44);

        spi_writecmd(0xE8);
        spi_writedata(0x09);
        spi_writedata(0xE8);
        spi_writedata(0xD8);
        spi_writedata(0xA0);
        spi_writedata(0x0B);
        spi_writedata(0xEA);
        spi_writedata(0xD8);
        spi_writedata(0xA0);
        spi_writedata(0x0D);
        spi_writedata(0xEC);
        spi_writedata(0xD8);
        spi_writedata(0xA0);
        spi_writedata(0x0F);
        spi_writedata(0xEE);
        spi_writedata(0xD8);
        spi_writedata(0xA0);

        spi_writecmd(0xEB);
        spi_writedata(0x02);
        spi_writedata(0x00);
        spi_writedata(0xE4);
        spi_writedata(0xE4);
        spi_writedata(0x88);
        spi_writedata(0x00);
        spi_writedata(0x40);

        spi_writecmd(0xEC);
        spi_writedata(0x3C);
        spi_writedata(0x00);

        spi_writecmd(0xED);
        spi_writedata(0xAB);
        spi_writedata(0x89);
        spi_writedata(0x76);
        spi_writedata(0x54);
        spi_writedata(0x02);
        spi_writedata(0xFF);
        spi_writedata(0xFF);
        spi_writedata(0xFF);
        spi_writedata(0xFF);
        spi_writedata(0xFF);
        spi_writedata(0xFF);
        spi_writedata(0x20);
        spi_writedata(0x45);
        spi_writedata(0x67);
        spi_writedata(0x98);
        spi_writedata(0xBA);

        spi_writecmd(0xFF);
        spi_writedata(0x77);
        spi_writedata(0x01);
        spi_writedata(0x00);
        spi_writedata(0x00);
        spi_writedata(0x10);

        // spi_writecmd(0xCD);
        // SPI_WriteData(0x0a);

        spi_writecmd(0x36);
        spi_writedata(0x00); // RGB

        spi_writecmd(0xE5);
        spi_writedata(0xE4);

        // spi_writecmd(0x3A); SPI_WriteData(0x66); //24bit
        // spi_writecmd(0x20);//display inversion off
        spi_writecmd(0x3A);
        spi_writedata(0x55); // 16bit
        // spi_writecmd(0x3A); SPI_WriteData(0x66);//18bit``````````````````````````````````````````````````````````````````````
        // spi_writecmd(0x22);//All pixel off

        spi_writecmd(0x2a);
        spi_writedata(0x00);
        spi_writedata(0x00);
        spi_writedata(0x01);
        spi_writedata(0xdf);

        spi_writecmd(0x2b);
        spi_writedata(0x00);
        spi_writedata(0x00);
        spi_writedata(0x03);
        spi_writedata(0x55);

        spi_writecmd(0x29);
    }
    else if (type == 5)
    {
        // 2.1寸圆屏厂商提供驱动 - 华迪创显
        delay(120);

        spi_writecmd(0xFF);
        spi_writedata(0x77);
        spi_writedata(0x01);
        spi_writedata(0x00);
        spi_writedata(0x00);
        spi_writedata(0x10);

        spi_writecmd(0xC0);
        spi_writedata(0x3B); // Scan line
        spi_writedata(0x00);

        spi_writecmd(0xC1);
        spi_writedata(0x0B); // VBP
        spi_writedata(0x02);

        spi_writecmd(0xC2);
        spi_writedata(0x07);
        spi_writedata(0x02);

        spi_writecmd(0xCC);
        spi_writedata(0x10);

        // spi_writecmd(0xCD);//RGB format
        // SPI_WriteData(0x08);        //用565时屏蔽    666打开

        spi_writecmd(0xB0); // IPS
        spi_writedata(0x00); // 255
        spi_writedata(0x11); // 251
        spi_writedata(0x16); // 247  down
        spi_writedata(0x0e); // 239
        spi_writedata(0x11); // 231
        spi_writedata(0x06); // 203
        spi_writedata(0x05); // 175
        spi_writedata(0x09); // 147
        spi_writedata(0x08); // 108
        spi_writedata(0x21); // 80
        spi_writedata(0x06); // 52
        spi_writedata(0x13); // 24
        spi_writedata(0x10); // 16
        spi_writedata(0x29); // 8    down
        spi_writedata(0x31); // 4
        spi_writedata(0x18); // 0

        spi_writecmd(0xB1); //  IPS
        spi_writedata(0x00); //  255
        spi_writedata(0x11); //  251
        spi_writedata(0x16); //  247   down
        spi_writedata(0x0e); //  239
        spi_writedata(0x11); //  231
        spi_writedata(0x07); //  203
        spi_writedata(0x05); //  175
        spi_writedata(0x09); //  147
        spi_writedata(0x09); //  108
        spi_writedata(0x21); //  80
        spi_writedata(0x05); //  52
        spi_writedata(0x13); //  24
        spi_writedata(0x11); //  16
        spi_writedata(0x2a); //  8  down
        spi_writedata(0x31); //  4
        spi_writedata(0x18); //  0

        spi_writecmd(0xFF);
        spi_writedata(0x77);
        spi_writedata(0x01);
        spi_writedata(0x00);
        spi_writedata(0x00);
        spi_writedata(0x11);

        spi_writecmd(0xB0); // VOP  3.5375+ *x 0.0125
        spi_writedata(0x6d); // 5D

        spi_writecmd(0xB1); // VCOM amplitude setting
        spi_writedata(0x37); //

        spi_writecmd(0xB2); // VGH Voltage setting
        spi_writedata(0x81); // 12V

        spi_writecmd(0xB3);
        spi_writedata(0x80);

        spi_writecmd(0xB5); // VGL Voltage setting
        spi_writedata(0x43); //-8.3V

        spi_writecmd(0xB7);
        spi_writedata(0x85);

        spi_writecmd(0xB8);
        spi_writedata(0x20);

        spi_writecmd(0xC1);
        spi_writedata(0x78);

        spi_writecmd(0xC2);
        spi_writedata(0x78);

        spi_writecmd(0xD0);
        spi_writedata(0x88);

        spi_writecmd(0xE0);
        spi_writedata(0x00);
        spi_writedata(0x00);
        spi_writedata(0x02);

        spi_writecmd(0xE1);
        spi_writedata(0x03);
        spi_writedata(0xA0);
        spi_writedata(0x00);
        spi_writedata(0x00);
        spi_writedata(0x04);
        spi_writedata(0xA0);
        spi_writedata(0x00);
        spi_writedata(0x00);
        spi_writedata(0x00);
        spi_writedata(0x20);
        spi_writedata(0x20);

        spi_writecmd(0xE2);
        spi_writedata(0x00);
        spi_writedata(0x00);
        spi_writedata(0x00);
        spi_writedata(0x00);
        spi_writedata(0x00);
        spi_writedata(0x00);
        spi_writedata(0x00);
        spi_writedata(0x00);
        spi_writedata(0x00);
        spi_writedata(0x00);
        spi_writedata(0x00);
        spi_writedata(0x00);
        spi_writedata(0x00);

        spi_writecmd(0xE3);
        spi_writedata(0x00);
        spi_writedata(0x00);
        spi_writedata(0x11);
        spi_writedata(0x00);

        spi_writecmd(0xE4);
        spi_writedata(0x22);
        spi_writedata(0x00);

        spi_writecmd(0xE5);
        spi_writedata(0x05);
        spi_writedata(0xEC);
        spi_writedata(0xA0);
        spi_writedata(0xA0);
        spi_writedata(0x07);
        spi_writedata(0xEE);
        spi_writedata(0xA0);
        spi_writedata(0xA0);
        spi_writedata(0x00);
        spi_writedata(0x00);
        spi_writedata(0x00);
        spi_writedata(0x00);
        spi_writedata(0x00);
        spi_writedata(0x00);
        spi_writedata(0x00);
        spi_writedata(0x00);

        spi_writecmd(0xE6);
        spi_writedata(0x00);
        spi_writedata(0x00);
        spi_writedata(0x11);
        spi_writedata(0x00);

        spi_writecmd(0xE7);
        spi_writedata(0x22);
        spi_writedata(0x00);

        spi_writecmd(0xE8);
        spi_writedata(0x06);
        spi_writedata(0xED);
        spi_writedata(0xA0);
        spi_writedata(0xA0);
        spi_writedata(0x08);
        spi_writedata(0xEF);
        spi_writedata(0xA0);
        spi_writedata(0xA0);
        spi_writedata(0x00);
        spi_writedata(0x00);
        spi_writedata(0x00);
        spi_writedata(0x00);
        spi_writedata(0x00);
        spi_writedata(0x00);
        spi_writedata(0x00);
        spi_writedata(0x00);

        spi_writecmd(0xEB);
        spi_writedata(0x00);
        spi_writedata(0x00);
        spi_writedata(0x40);
        spi_writedata(0x40);
        spi_writedata(0x00);
        spi_writedata(0x00);
        spi_writedata(0x00);

        spi_writecmd(0xED);
        spi_writedata(0xFF);
        spi_writedata(0xFF);
        spi_writedata(0xFF);
        spi_writedata(0xBA);
        spi_writedata(0x0A);
        spi_writedata(0xBF);
        spi_writedata(0x45);
        spi_writedata(0xFF);
        spi_writedata(0xFF);
        spi_writedata(0x54);
        spi_writedata(0xFB);
        spi_writedata(0xA0);
        spi_writedata(0xAB);
        spi_writedata(0xFF);
        spi_writedata(0xFF);
        spi_writedata(0xFF);

        spi_writecmd(0xEF);
        spi_writedata(0x10);
        spi_writedata(0x0D);
        spi_writedata(0x04);
        spi_writedata(0x08);
        spi_writedata(0x3F);
        spi_writedata(0x1F);

        spi_writecmd(0xFF);
        spi_writedata(0x77);
        spi_writedata(0x01);
        spi_writedata(0x00);
        spi_writedata(0x00);
        spi_writedata(0x13);

        spi_writecmd(0xEF);
        spi_writedata(0x08);

        spi_writecmd(0xFF);
        spi_writedata(0x77);
        spi_writedata(0x01);
        spi_writedata(0x00);
        spi_writedata(0x00);
        spi_writedata(0x00);

        spi_writecmd(0x11);

        delay(120);

        spi_writecmd(0x29);

        spi_writecmd(0x36);
        spi_writedata(0x08);

        spi_writecmd(0x3A);
        spi_writedata(0x70); // 55/50=16bit(RGB565);66=18bit(RGB666);77或默认不写3AH是=24bit(RGB888)
    }
    else if (type == 6)
    {
        // 网络查询，https://blog.csdn.net/qq_37859760/article/details/118311157
        spi_writecmd(0xFF);
        spi_writedata(0x77);
        spi_writedata(0x01);
        spi_writedata(0x00);
        spi_writedata(0x00);
        spi_writedata(0x13);

        spi_writecmd(0xEF);
        spi_writedata(0x08);

        spi_writecmd(0xFF);
        spi_writedata(0x77);
        spi_writedata(0x01);
        spi_writedata(0x00);
        spi_writedata(0x00);
        spi_writedata(0x10);

        spi_writecmd(0xC0);
        spi_writedata(0xE5);
        spi_writedata(0x02);

        spi_writecmd(0xC1);
        spi_writedata(0x0C);
        spi_writedata(0x0A);

        spi_writecmd(0xC2);
        spi_writedata(0x07);
        spi_writedata(0x0F);

        spi_writecmd(0xC3);
        spi_writedata(0x02);

        spi_writecmd(0xCC);
        spi_writedata(0x10);

        spi_writecmd(0xB0);
        spi_writedata(0x00);
        spi_writedata(0x08);
        spi_writedata(0x51);
        spi_writedata(0x0D);
        spi_writedata(0xCE);
        spi_writedata(0x06);
        spi_writedata(0x00);
        spi_writedata(0x08);
        spi_writedata(0x08);
        spi_writedata(0x1D);
        spi_writedata(0x02);
        spi_writedata(0xD0);
        spi_writedata(0x0F);
        spi_writedata(0x6F);
        spi_writedata(0x36);
        spi_writedata(0x3F);

        spi_writecmd(0xB1);
        spi_writedata(0x00);
        spi_writedata(0x10);
        spi_writedata(0x4F);
        spi_writedata(0x0C);
        spi_writedata(0x11);
        spi_writedata(0x05);
        spi_writedata(0x00);
        spi_writedata(0x07);
        spi_writedata(0x07);
        spi_writedata(0x1F);
        spi_writedata(0x05);
        spi_writedata(0xD3);
        spi_writedata(0x11);
        spi_writedata(0x6E);
        spi_writedata(0x34);
        spi_writedata(0x3F);

        spi_writecmd(0xFF);
        spi_writedata(0x77);
        spi_writedata(0x01);
        spi_writedata(0x00);
        spi_writedata(0x00);
        spi_writedata(0x11);

        spi_writecmd(0xB0);
        spi_writedata(0x4D);

        spi_writecmd(0xB1);
        spi_writedata(0x1C);

        spi_writecmd(0xB2);
        spi_writedata(0x87);

        spi_writecmd(0xB3);
        spi_writedata(0x80);

        spi_writecmd(0xB5);
        spi_writedata(0x47);

        spi_writecmd(0xB7);
        spi_writedata(0x85);

        spi_writecmd(0xB8);
        spi_writedata(0x21);

        spi_writecmd(0xB9);
        spi_writedata(0x10);

        spi_writecmd(0xC1);
        spi_writedata(0x78);

        spi_writecmd(0xC2);
        spi_writedata(0x78);

        spi_writecmd(0xD0);
        spi_writedata(0x88);

        delay(100);

        spi_writecmd(0xE0);
        spi_writedata(0x80);
        spi_writedata(0x00);
        spi_writedata(0x02);

        spi_writecmd(0xE1);
        spi_writedata(0x04);
        spi_writedata(0xA0);
        spi_writedata(0x00);
        spi_writedata(0x00);
        spi_writedata(0x05);
        spi_writedata(0xA0);
        spi_writedata(0x00);
        spi_writedata(0x00);
        spi_writedata(0x00);
        spi_writedata(0x60);
        spi_writedata(0x60);

        spi_writecmd(0xE2);
        spi_writedata(0x30);
        spi_writedata(0x30);
        spi_writedata(0x60);
        spi_writedata(0x60);
        spi_writedata(0x3C);
        spi_writedata(0xA0);
        spi_writedata(0x00);
        spi_writedata(0x00);
        spi_writedata(0x3D);
        spi_writedata(0xA0);
        spi_writedata(0x00);
        spi_writedata(0x00);
        spi_writedata(0x00);

        spi_writecmd(0xE3);
        spi_writedata(0x00);
        spi_writedata(0x00);
        spi_writedata(0x33);
        spi_writedata(0x33);

        spi_writecmd(0xE4);
        spi_writedata(0x44);
        spi_writedata(0x44);

        spi_writecmd(0xE5);
        spi_writedata(0x06);
        spi_writedata(0x3E);
        spi_writedata(0xA0);
        spi_writedata(0xA0);
        spi_writedata(0x08);
        spi_writedata(0x40);
        spi_writedata(0xA0);
        spi_writedata(0xA0);
        spi_writedata(0x0A);
        spi_writedata(0x42);
        spi_writedata(0xA0);
        spi_writedata(0xA0);
        spi_writedata(0x0C);
        spi_writedata(0x44);
        spi_writedata(0xA0);
        spi_writedata(0xA0);

        spi_writecmd(0xE6);
        spi_writedata(0x00);
        spi_writedata(0x00);
        spi_writedata(0x33);
        spi_writedata(0x33);

        spi_writecmd(0xE7);
        spi_writedata(0x44);
        spi_writedata(0x44);

        spi_writecmd(0xE8);
        spi_writedata(0x07);
        spi_writedata(0x3F);
        spi_writedata(0xA0);
        spi_writedata(0xA0);
        spi_writedata(0x09);
        spi_writedata(0x41);
        spi_writedata(0xA0);
        spi_writedata(0xA0);
        spi_writedata(0x0B);
        spi_writedata(0x43);
        spi_writedata(0xA0);
        spi_writedata(0xA0);
        spi_writedata(0x0D);
        spi_writedata(0x45);
        spi_writedata(0xA0);
        spi_writedata(0xA0);

        spi_writecmd(0xEB);
        spi_writedata(0x00);
        spi_writedata(0x01);
        spi_writedata(0x4E);
        spi_writedata(0x4E);
        spi_writedata(0xEE);
        spi_writedata(0x44);
        spi_writedata(0x00);

        spi_writecmd(0xED);
        spi_writedata(0xFF);
        spi_writedata(0xFF);
        spi_writedata(0x04);
        spi_writedata(0x56);
        spi_writedata(0x72);
        spi_writedata(0xFF);
        spi_writedata(0xFF);
        spi_writedata(0xFF);
        spi_writedata(0xFF);
        spi_writedata(0xFF);
        spi_writedata(0xFF);
        spi_writedata(0x27);
        spi_writedata(0x65);
        spi_writedata(0x40);
        spi_writedata(0xFF);
        spi_writedata(0xFF);

        spi_writecmd(0xEF);
        spi_writedata(0x10);
        spi_writedata(0x0D);
        spi_writedata(0x04);
        spi_writedata(0x08);
        spi_writedata(0x3F);
        spi_writedata(0x1F);

        spi_writecmd(0xFF);
        spi_writedata(0x77);
        spi_writedata(0x01);
        spi_writedata(0x00);
        spi_writedata(0x00);
        spi_writedata(0x13);

        spi_writecmd(0xE8);
        spi_writedata(0x00);
        spi_writedata(0x0E);

        spi_writecmd(0xFF);
        spi_writedata(0x77);
        spi_writedata(0x01);
        spi_writedata(0x00);
        spi_writedata(0x00);
        spi_writedata(0x00);

        spi_writecmd(0x11);

        delay(120);

        spi_writecmd(0xFF);
        spi_writedata(0x77);
        spi_writedata(0x01);
        spi_writedata(0x00);
        spi_writedata(0x00);
        spi_writedata(0x13);

        spi_writecmd(0xE8);
        spi_writedata(0x00);
        spi_writedata(0x0C);

        delay(10);

        spi_writecmd(0xE8);
        spi_writedata(0x00);
        spi_writedata(0x00);

        spi_writecmd(0xFF);
        spi_writedata(0x77);
        spi_writedata(0x01);
        spi_writedata(0x00);
        spi_writedata(0x00);
        spi_writedata(0x00);

        spi_writecmd(0x3A);
        spi_writedata(0x55);

        spi_writecmd(0x36);
        spi_writedata(0x00);

        spi_writecmd(0x35);
        spi_writedata(0x00);

        spi_writecmd(0x29);
    }
    else if (type == 7)
    {
        // 2.8寸圆屏-高信技术
        spi_writecmd(0xFF);
        spi_writedata(0x77);
        spi_writedata(0x01);
        spi_writedata(0x00);
        spi_writedata(0x00);
        spi_writedata(0x13);

        spi_writecmd(0xEF);
        spi_writedata(0x08);

        spi_writecmd(0xFF);
        spi_writedata(0x77);
        spi_writedata(0x01);
        spi_writedata(0x00);
        spi_writedata(0x00);
        spi_writedata(0x10);

        spi_writecmd(0xC0);
        spi_writedata(0x3B);
        spi_writedata(0x00);

        spi_writecmd(0xC1);
        spi_writedata(0x10);
        spi_writedata(0x0C);

        spi_writecmd(0xC2);
        spi_writedata(0x07);
        spi_writedata(0x0A);

        spi_writecmd(0xC7);
        spi_writedata(0x04);

        spi_writecmd(0xCC);
        spi_writedata(0x10);

        spi_writecmd(0xB0);
        spi_writedata(0x05);
        spi_writedata(0x12);
        spi_writedata(0x98);
        spi_writedata(0x0E);
        spi_writedata(0x0F);
        spi_writedata(0x07);
        spi_writedata(0x07);
        spi_writedata(0x09);
        spi_writedata(0x09);
        spi_writedata(0x23);
        spi_writedata(0x05);
        spi_writedata(0x52);
        spi_writedata(0x0F);
        spi_writedata(0x67);
        spi_writedata(0x2C);
        spi_writedata(0x11);

        spi_writecmd(0xB1);
        spi_writedata(0x0B);
        spi_writedata(0x11);
        spi_writedata(0x97);
        spi_writedata(0x0C);
        spi_writedata(0x12);
        spi_writedata(0x06);
        spi_writedata(0x06);
        spi_writedata(0x08);
        spi_writedata(0x08);
        spi_writedata(0x22);
        spi_writedata(0x03);
        spi_writedata(0x51);
        spi_writedata(0x11);
        spi_writedata(0x66);
        spi_writedata(0x2B);
        spi_writedata(0x0F);

        spi_writecmd(0xFF);
        spi_writedata(0x77);
        spi_writedata(0x01);
        spi_writedata(0x00);
        spi_writedata(0x00);
        spi_writedata(0x11);

        spi_writecmd(0xB0);
        spi_writedata(0x5D);

        spi_writecmd(0xB1);
        spi_writedata(0x2D);

        spi_writecmd(0xB2);
        spi_writedata(0x81);

        spi_writecmd(0xB3);
        spi_writedata(0x80);

        spi_writecmd(0xB5);
        spi_writedata(0x4E);

        spi_writecmd(0xB7);
        spi_writedata(0x85);

        spi_writecmd(0xB8);
        spi_writedata(0x20);

        spi_writecmd(0xC1);
        spi_writedata(0x78);

        spi_writecmd(0xC2);
        spi_writedata(0x78);

        spi_writecmd(0xD0);
        spi_writedata(0x88);

        spi_writecmd(0xE0);
        spi_writedata(0x00);
        spi_writedata(0x00);
        spi_writedata(0x02);

        spi_writecmd(0xE1);
        spi_writedata(0x06);
        spi_writedata(0x30);
        spi_writedata(0x08);
        spi_writedata(0x30);
        spi_writedata(0x05);
        spi_writedata(0x30);
        spi_writedata(0x07);
        spi_writedata(0x30);
        spi_writedata(0x00);
        spi_writedata(0x33);
        spi_writedata(0x33);

        spi_writecmd(0xE2);
        spi_writedata(0x11);
        spi_writedata(0x11);
        spi_writedata(0x33);
        spi_writedata(0x33);
        spi_writedata(0xF4);
        spi_writedata(0x00);
        spi_writedata(0x00);
        spi_writedata(0x00);
        spi_writedata(0xF4);
        spi_writedata(0x00);
        spi_writedata(0x00);
        spi_writedata(0x00);

        spi_writecmd(0xE3);
        spi_writedata(0x00);
        spi_writedata(0x00);
        spi_writedata(0x11);
        spi_writedata(0x11);

        spi_writecmd(0xE4);
        spi_writedata(0x44);
        spi_writedata(0x44);

        spi_writecmd(0xE5);
        spi_writedata(0x0D);
        spi_writedata(0xF5);
        spi_writedata(0x30);
        spi_writedata(0xF0);
        spi_writedata(0x0F);
        spi_writedata(0xF7);
        spi_writedata(0x30);
        spi_writedata(0xF0);
        spi_writedata(0x09);
        spi_writedata(0xF1);
        spi_writedata(0x30);
        spi_writedata(0xF0);
        spi_writedata(0x0B);
        spi_writedata(0xF3);
        spi_writedata(0x30);
        spi_writedata(0xF0);

        spi_writecmd(0xE6);
        spi_writedata(0x00);
        spi_writedata(0x00);
        spi_writedata(0x11);
        spi_writedata(0x11);

        spi_writecmd(0xE7);
        spi_writedata(0x44);
        spi_writedata(0x44);

        spi_writecmd(0xE8);
        spi_writedata(0x0C);
        spi_writedata(0xF4);
        spi_writedata(0x30);
        spi_writedata(0xF0);
        spi_writedata(0x0E);
        spi_writedata(0xF6);
        spi_writedata(0x30);
        spi_writedata(0xF0);
        spi_writedata(0x08);
        spi_writedata(0xF0);
        spi_writedata(0x30);
        spi_writedata(0xF0);
        spi_writedata(0x0A);
        spi_writedata(0xF2);
        spi_writedata(0x30);
        spi_writedata(0xF0);

        spi_writecmd(0xE9);
        spi_writedata(0x36);
        spi_writedata(0x01);

        spi_writecmd(0xEB);
        spi_writedata(0x00);
        spi_writedata(0x01);
        spi_writedata(0xE4);
        spi_writedata(0xE4);
        spi_writedata(0x44);
        spi_writedata(0x88);
        spi_writedata(0x40);

        spi_writecmd(0xED);
        spi_writedata(0xFF);
        spi_writedata(0x10);
        spi_writedata(0xAF);
        spi_writedata(0x76);
        spi_writedata(0x54);
        spi_writedata(0x2B);
        spi_writedata(0xCF);
        spi_writedata(0xFF);
        spi_writedata(0xFF);
        spi_writedata(0xFC);
        spi_writedata(0xB2);
        spi_writedata(0x45);
        spi_writedata(0x67);
        spi_writedata(0xFA);
        spi_writedata(0x01);
        spi_writedata(0xFF);

        spi_writecmd(0xEF);
        spi_writedata(0x08);
        spi_writedata(0x08);
        spi_writedata(0x08);
        spi_writedata(0x45);
        spi_writedata(0x3F);
        spi_writedata(0x54);

        spi_writecmd(0xFF);
        spi_writedata(0x77);
        spi_writedata(0x01);
        spi_writedata(0x00);
        spi_writedata(0x00);
        spi_writedata(0x00);

        spi_writecmd(0x11);
        delay(120); // ms

        spi_writecmd(0x3A);
        spi_writedata(0x66);

        spi_writecmd(0x36);
        spi_writedata(0x00);

        spi_writecmd(0x35);
        spi_writedata(0x00);

        spi_writecmd(0x29);
    }
}