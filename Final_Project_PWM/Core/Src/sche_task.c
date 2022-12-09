/*
 * task.c
 *
 *  Created on: Nov 28, 2022
 *      Author: Tien
 */
#include <stdio.h>
#include "main.h"
#include "sche_task.h"
#include "global.h"
#include "input_reading.h"
#include "traffic_light.h"
#include "software_timer.h"

int pedestrian_active = 0;
int delay = TIME_UNIT;
int FREQ = 10;
int FREQ_STEP = 0;
int DELAY_STEP = TIME_UNIT / 10;
int time_allow_pedestrian = 0;
int flag_button = 0;
extern TIM_HandleTypeDef htim3;

float extra_step = 1;

void pedestrian_scramble() {
		if (is_button_pressed(3)) {
			if (get_led_color(TRAFFIC_1_LED) == RED_COLOR) {
				pedestrian_active = 1;
				flag_button = 1;
			}
		}

		if (get_led_color(TRAFFIC_1_LED) == RED_COLOR && flag_button) {
			pedestrian_active = 1;
			/* Calculate number of step for increase frequency and decrease delay time */
			FREQ_STEP = 120 / (SEG7_CLOCK[0] / TIME_UNIT);
			DELAY_STEP = 400 / (SEG7_CLOCK[0] / TIME_UNIT);
			time_allow_pedestrian = SEG7_CLOCK[0];
		}
		else {
			pedestrian_active = 0;
			set_led_color(PEDESTRIAN_LED, RED_COLOR);
		}
		if (pedestrian_active) {
			set_led_color(PEDESTRIAN_LED, GREEN_COLOR);
			/* Buzzer frequency */
			__HAL_TIM_SetCompare(&htim3, TIM_CHANNEL_1, 0);
			HAL_Delay(delay);

			/* If 2/3 time has passed, need more extra step*/
			if (SEG7_CLOCK[0] >= 2 * time_allow_pedestrian / 3) {
				extra_step += 0.09;
				__HAL_TIM_SetCompare(&htim3, TIM_CHANNEL_1, FREQ += FREQ_STEP);
				if (delay > extra_step * DELAY_STEP) HAL_Delay(delay -= extra_step * DELAY_STEP);
				else HAL_Delay(delay);
			} else {
				extra_step += 0.3;
				__HAL_TIM_SetCompare(&htim3, TIM_CHANNEL_1, FREQ += extra_step * FREQ_STEP);
				if (delay > extra_step * DELAY_STEP) HAL_Delay(delay -= extra_step * DELAY_STEP);
				else if (delay >= extra_step * 10) HAL_Delay(delay = - extra_step * 20);
				else HAL_Delay(delay);
			}

		} else {
			set_led_color(PEDESTRIAN_LED, RED_COLOR);
			__HAL_TIM_SetCompare(&htim3, TIM_CHANNEL_1, 0);
			/* Reset delay & start freq */
			delay = TIME_UNIT;
			FREQ = 10;
			extra_step = 1;
			flag_button = 0;
		}
}
