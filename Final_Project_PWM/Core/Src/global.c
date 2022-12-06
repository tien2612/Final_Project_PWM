/*
 * global.c
 *
 *  Created on: Oct 24, 2022
 *      Author: Tien
 */


#include "global.h"
#include "main.h"

// default light time for 3 states
int man_red_time = 5;
int man_amber_time = 2;
int man_green_time = 3;

int temp_value = 0;
int light_time = GREEN_TIME / 100;
int light_time1 = RED_TIME / 100;
int status = NORMAL_MODE;
int led_status = RED_GREEN;
int index_led = 0;
int AllowToExecuteAfterASecondPressed = 0;

int flagForButtonPressOneQuarterSecond[4] = {0};
