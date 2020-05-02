/*
 * LED_if.h
 *
 *      Author: hendog82
 */

#ifndef MAIN_LED_IF_H_
#define MAIN_LED_IF_H_


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

extern uint8_t state;
extern uint32_t cnt;

void transmit_colors(int length);

void buffer_filler(int length);


#endif /* MAIN_LED_IF_H_ */
