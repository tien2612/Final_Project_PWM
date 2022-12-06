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
int time_left = 10;
extern TIM_HandleTypeDef htim3;

void pedestrian_scramble() {
		if (pedestrian_active) {
			//set_led_color(Traffic_Pedes_1_GPIO_Port, Traffic_Pedes_1_Pin, GREEN_COLOR);
			if (time_left < 5) {
				__HAL_TIM_SetCompare(&htim3, TIM_CHANNEL_1, 10);
				time_left--;
			}

		}
		if (is_button_pressed(3)) {
			if (HAL_GPIO_ReadPin(GPIOB, Traffic_2_1_Pin) == 1 && HAL_GPIO_ReadPin(GPIOB, Traffic_2_1_Pin) == 0) {
				pedestrian_active = 1;
			}
			else pedestrian_active = 0;
		}
}
