/*
 * timer_if.h
 *
 *  Created on: May 1, 2020
 *      Author: henrygilbert
 */

#ifndef MAIN_TIMER_IF_H_
#define MAIN_TIMER_IF_H_

#include <stdio.h>
#include "esp_types.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "driver/periph_ctrl.h"
#include "driver/timer.h"
#include "button_if.h"
#include "LED_if.h"

extern xQueueHandle timer_queue;
extern uint32_t timer_interval;

/*
 * Timer ISR Handler
 */
void timer_isr(void *para);

/*
 * Initialize timer with interrupt
 */
void timer_initialize(uint32_t interval_msec);

/*
 * Timer Task
 */
void timer_task(void *arg);

#endif /* MAIN_TIMER_IF_H_ */
