/*
 * LED_if.c
 *
 *  Created on: May 1, 2020
 *      Author: henrygilbert
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

#define RMT_TX_CHANNEL RMT_CHANNEL_0

#define t0h_ticks 14
#define t0l_ticks 40
#define t1h_ticks 40
#define t1l_ticks 14

static const char *TAG = "LED";

const rmt_item32_t send_zero = {{{ t0h_ticks, 1, t0l_ticks, 0 }}}; //code to transmit a 0
const rmt_item32_t send_one = {{{ t1h_ticks, 1, t1l_ticks, 0 }}}; //code to transmit a 1

uint8_t red = 0xAA;
uint8_t green = 0x55;
uint8_t blue = 0xFF;
uint8_t color_buffer[0];

void buffer_filler(int length){
	if (state == 0){
		if (cnt % 2){
			for(int i=0; i<length; i++){
				color_buffer[i*3] = 0x00;
				color_buffer[i*3+1] = 0xFF;
				color_buffer[i*3+2] = 0x00;
			}
		}else{
			for(int i=0; i<length; i++){
				color_buffer[i*3] = 0xFF;
				color_buffer[i*3+1] = 0xFF;
				color_buffer[i*3+2] = 0xFF;
			}
		}
	}else if(state == 1){
		for(int i=0; i<length; i++){
			green = red;
			color_buffer[i*3] = green;
			red = blue;
			color_buffer[i*3+1] = red;
			blue += 0x55;
			color_buffer[i*3+2] = blue;
		}
	}else if(state == 2){
		for(int i=0; i<length; i++){
			color_buffer[i*3] = (cnt*10+ i * 25);
			color_buffer[i*3+1] = (cnt*10+170+ i * 25);
			color_buffer[i*3+2] = (cnt*10+85 + i * 25);
		}
	}else if(state ==3){
		for(int i=0; i<length; i++){
			color_buffer[i*3] = (cnt*10);
			color_buffer[i*3+1] = (cnt*10);
			color_buffer[i*3+2] = (cnt*10);
		}
	}else{
		for(int i=0; i<length; i++){
			color_buffer[i*3] = (cnt%2)*255;
			color_buffer[i*3+1] = 0xFF;
			color_buffer[i*3+2] = (cnt%2)*255;
		}
	}
}


void transmit_colors(int length){
	rmt_item32_t  *buff_out;
	uint32_t req_length = length * 8 * 3 * sizeof(rmt_item32_t);
	buff_out = (rmt_item32_t*)malloc(req_length);
	for(int i=0; i<length*3; i++){
		for (int j = 0; j < 8; j++){
			if (color_buffer[i] & (1 << (7 - j))) {//MSB first
				buff_out[(i*8)+j] =  send_one;
			} else {
				buff_out[(i*8)+j] =  send_zero;
			}
		}
	}
	esp_err_t ret_val = rmt_write_items(RMT_TX_CHANNEL, buff_out, length*3*8, true);

    if(ret_val != ESP_OK) {
    	ESP_LOGE(TAG, "[0x%x] rmt_write_items failed", ret_val);
    }
	free(buff_out);
	return;

}
