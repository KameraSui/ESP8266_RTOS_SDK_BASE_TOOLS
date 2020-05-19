/*
 * ESPRSSIF MIT License
 *
 * Copyright (c) 2017 <ESPRESSIF SYSTEMS (SHANGHAI) PTE LTD>
 *
 * Permission is hereby granted for use on ESPRESSIF SYSTEMS ESP8266 only, in which case,
 * it is free of charge, to any person obtaining a copy of this software and associated
 * documentation files (the "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the Software is furnished
 * to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all copies or
 * substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 */

#include "esp_common.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "espressif/c_types.h"
#include "espressif/esp_misc.h"
#include "lwip/sockets.h"
#include "uart.h"

static os_timer_t timer;

/******************************************************************************
 * FunctionName : user_rf_cal_sector_set
 * Description  : SDK just reversed 4 sectors, used for rf init data and paramters.
 *                We add this function to force users to set rf cal sector, since
 *                we don't know which sector is free in user's application.
 *                sector map for last several sectors : ABCCC
 *                A : rf cal
 *                B : rf init data
 *                C : sdk parameters
 * Parameters   : none
 * Returns      : rf cal sector
*******************************************************************************/
uint32 user_rf_cal_sector_set(void)
{
    flash_size_map size_map = system_get_flash_size_map();
    uint32 rf_cal_sec = 0;

    switch (size_map) {
        case FLASH_SIZE_4M_MAP_256_256:
            rf_cal_sec = 128 - 5;
            break;

        case FLASH_SIZE_8M_MAP_512_512:
            rf_cal_sec = 256 - 5;
            break;

        case FLASH_SIZE_16M_MAP_512_512:
        case FLASH_SIZE_16M_MAP_1024_1024:
            rf_cal_sec = 512 - 5;
            break;

        case FLASH_SIZE_32M_MAP_512_512:
        case FLASH_SIZE_32M_MAP_1024_1024:
            rf_cal_sec = 1024 - 5;
            break;
        case FLASH_SIZE_64M_MAP_1024_1024:
            rf_cal_sec = 2048 - 5;
            break;
        case FLASH_SIZE_128M_MAP_1024_1024:
            rf_cal_sec = 4096 - 5;
            break;
        default:
            rf_cal_sec = 0;
            break;
    }

    return rf_cal_sec;
}

typedef struct{
    void * buf_cp;
    uint16 len;
}PackageMSG_t;

xQueueHandle packageReadQuene;

#define mem_malloc(s)    \
    ({  \
        static const char mem_debug_file[] ICACHE_RODATA_ATTR STORE_ATTR = __FILE__;    \
        pvPortMalloc(s, mem_debug_file, __LINE__, false);  \
    })

#define mem_zalloc(s)    \
    ({  \
        static const char mem_debug_file[] ICACHE_RODATA_ATTR STORE_ATTR = __FILE__;    \
        pvPortZalloc(s, mem_debug_file, __LINE__);  \
    })

#define mem_free(s) \
do{\
    static const char mem_debug_file[] ICACHE_RODATA_ATTR STORE_ATTR = __FILE__;    \
    vPortFree(s, mem_debug_file, __LINE__);\
}while(0)


void _main_thread(void *p){
    printf("main_thread\n");

    while(1){
        vTaskDelay(1000 / portTICK_RATE_MS);

        printf("main \n");

    }
}



void	ICACHE_FLASH_ATTR	_set115200()
{
	UART_ConfigTypeDef uart_config;
    uart_config.baud_rate    = BIT_RATE_115200;
    uart_config.data_bits     = UART_WordLength_8b;
    uart_config.parity          = USART_Parity_None;
    uart_config.stop_bits     = USART_StopBits_1;
    uart_config.flow_ctrl      = USART_HardwareFlowControl_None;
    uart_config.UART_RxFlowThresh = 120;
    uart_config.UART_InverseMask = UART_None_Inverse;
    UART_ParamConfig(UART0, &uart_config);
}
/******************************************************************************
 * FunctionName : user_init
 * Description  : entry of user application, init user function here
 * Parameters   : none
 * Returns      : none
*******************************************************************************/
void user_init(void)
{
    _set115200();
    printf("SDK version:%s\n", system_get_sdk_version());

    packageReadQuene =  xQueueCreate( 200, sizeof(PackageMSG_t));

    //wifi_station_disconnect
    signed portBASE_TYPE ret = xTaskCreate(_main_thread,
                      "sniffer_task",
                      400,
                      NULL,
                      1,
                      NULL);
    if (ret != pdPASS)  {
        printf("create thread  2 failed\n");
        return ;
    }
}
