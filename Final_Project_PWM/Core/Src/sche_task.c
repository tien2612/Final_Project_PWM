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

int pedestrian_active = 0;
extern TIM_HandleTypeDef htim3;

void pedestrian_scramble() {
		if (pedestrian_active) {
			//set_led_color(Traffic_Pedes_1_GPIO_Port, Traffic_Pedes_1_Pin, GREEN_COLOR);
			if (SEG7_CLOCK[0] < 2) {
				__HAL_TIM_SetCompare(&htim3, TIM_CHANNEL_1, 10);
				time_left--;
			}

		}
		if (is_button_pressed(3)) {
			if (CURRENT_STATE[0] == 0) {
				pedestrian_active = 1;
			}
			else pedestrian_active = 0;
		}
}
