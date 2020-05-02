/*
 * timer_if.c
 *
 *      Author: hendog82
 */

#include <stdio.h>
#include "esp_types.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "driver/periph_ctrl.h"
#include "driver/timer.h"
#include "timer_if.h"
#include "button_if.h"
#include "LED_if.h"
static const char *TAG = "Timer";


uint32_t timer_id = 0;
uint32_t cnt = 0;

/*
 * Timer ISR Handler
 */
void timer_isr(void *para)
{
    //Clear the interrupt
    timer_group_clr_intr_status_in_isr(0, 0);
    //reset timer
    timer_set_counter_value(0, 0, 0x00000000ULL);
    timer_group_enable_alarm_in_isr(0, 0);
    //Call timer task
    xQueueSendFromISR(timer_queue, &timer_id, NULL);
}

/*
 * Initialize timer with interrupt
 */
void timer_initialize(uint32_t interval_msec)
{
	//configure timer
	timer_config_t config = {
        .divider = 40000, //each clock tick is .5ms
        .counter_dir = TIMER_COUNT_UP,
        .counter_en = TIMER_PAUSE,
        .alarm_en = TIMER_ALARM_EN,
        .auto_reload = true,
    };
	//initialize timer
    timer_init(0, 0, &config);
    timer_set_counter_value(0, 0, 0x00000000ULL);
    timer_set_alarm_value(0, 0, interval_msec * 2 );//multiplty by two because clock is .5ms
    timer_enable_intr(0, 0);
    timer_isr_register(0, 0, timer_isr, (void *) 0, ESP_INTR_FLAG_LEVEL1, NULL);
    //start timer
    timer_start(0, 0);

}

/*
 * Timer Task
 */
void timer_task(void *arg)
{
    while (1) {
        uint32_t id;
        if(xQueueReceive(timer_queue, &id, portMAX_DELAY)){
        	transmit_colors(150);
        	buffer_filler(150);
        	cnt++;
        }


    }
}

