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

int pedestrian_active = 0;
extern TIM_HandleTypeDef htim3;

void pedestrian_scramble() {
		if (pedestrian_active) {
			set_led_color(PEDESTRIAN_LED, GREEN_COLOR);
			//if (SEG7_CLOCK[0] < 2) {
				__HAL_TIM_SetCompare(&htim3, TIM_CHANNEL_1, 10);
				HAL_Delay(1000);
				__HAL_TIM_SetCompare(&htim3, TIM_CHANNEL_1, 100);
				HAL_Delay(1000);
			//}
		} else {
			set_led_color(PEDESTRIAN_LED, RED_COLOR);
			__HAL_TIM_SetCompare(&htim3, TIM_CHANNEL_1, 0);
		}

		//if (is_button_pressed(3)) {
			if (get_led_color(TRAFFIC_1_LED) == RED_COLOR) {
				pedestrian_active = 1;
			}
			else pedestrian_active = 0;
		//}
}
