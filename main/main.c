/*
 * main.c
 *
 *      Author: hendog82
 */
#include <stdio.h>
#include <stdint.h>

#include "esp_system.h"
#include "esp_spi_flash.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "driver/gpio.h"
#include "sdkconfig.h"
#include "esp_log.h"
#include "driver/rmt.h"
#include "soc/rmt_struct.h"
#include "nvs_flash.h"
#include "esp_types.h"
#include "driver/periph_ctrl.h"
#include "driver/timer.h"
#include "timer_if.h"
#include "button_if.h"
#include "LED_if.h"

static const char *TAG = "main";


//#define RMT_GPIO 5						//LED strip connected to GPIO5
#define RMT_TX_CHANNEL RMT_CHANNEL_0

#define LED_GPIO 18
#define t0h_ticks 14
#define t0l_ticks 40
#define t1h_ticks 40
#define t1l_ticks 14
#define Button_Pin     5
#define GPIO_INPUT_PIN_SEL  (1ULL<<Button_Pin)


xQueueHandle button = NULL;
xQueueHandle timer_queue = NULL;
uint8_t state = 0;
uint32_t timer_interval = 100;//time between color changes (ms)

void app_main(void)
{
	nvs_flash_init();

    rmt_config_t config = RMT_DEFAULT_CONFIG_TX(18, RMT_TX_CHANNEL);
    // default clock is 80MHz so this is 40MHz
    config.clk_div = 2;

    //initialize timer, task, and interrupt handler
    timer_queue = xQueueCreate(10, sizeof(uint32_t));
    timer_initialize(timer_interval);
    xTaskCreate(timer_task, "timer_evt_task", 2048, NULL, 5, NULL);


    //initialize button, task and interrupt handler
    button = xQueueCreate(10, sizeof(uint32_t));
    xTaskCreate(Button_Task, "Button_Task", 2048, NULL, 10, NULL);
    button_initialize();
    //setup RMT for LED strip
    rmt_config(&config);
    rmt_driver_install(RMT_TX_CHANNEL, 0, 0);
    while(1) {

    	vTaskDelay(100);
    }
}








