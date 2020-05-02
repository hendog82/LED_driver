/*
 * button_if.c
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

#define Button_Pin     5
#define GPIO_INPUT_PIN_SEL  (1ULL<<Button_Pin)

static const char *TAG = "Button";


void button_isr_handler(void* arg)
{
    uint32_t gpio_num = (uint32_t) arg;
    xQueueSendFromISR(button, &gpio_num, NULL);
    return;
}


void Button_Task(void* arg)
{
    uint32_t io_num;
    for(;;) {
        if(xQueueReceive(button, &io_num, portMAX_DELAY)) {
           vTaskDelay(5);
        	printf("Current state = %d\n", state);
        	state ++;
        	if (state > 4){
        		state = 0;
        	}
        }
    }
}

void button_initialize(){
    //initialize button GPIO and ISR
    gpio_set_direction(Button_Pin, GPIO_MODE_INPUT);
    gpio_set_pull_mode(Button_Pin, GPIO_PULLUP_ONLY);
    gpio_set_intr_type(Button_Pin, GPIO_INTR_NEGEDGE);
    gpio_intr_enable(Button_Pin);
    gpio_install_isr_service(ESP_INTR_FLAG_LEVEL3);
    gpio_isr_handler_add(Button_Pin, button_isr_handler, (void*) Button_Pin);
}
