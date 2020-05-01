/*
 * button_if.h
 *
 *  Created on: May 1, 2020
 *      Author: henrygilbert
 */

#ifndef MAIN_BUTTON_IF_H_
#define MAIN_BUTTON_IF_H_

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
#include "LED_if.h"

extern xQueueHandle button;

void button_isr_handler(void* arg);

void Button_Task(void* arg);
void button_initialize(void);


#endif /* MAIN_BUTTON_IF_H_ */
