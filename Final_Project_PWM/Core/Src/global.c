/*
 * global.c
 *
 *  Created on: Oct 24, 2022
 *      Author: Tien
 */


#include "global.h"
#include "main.h"

// default light time for 3 states

int status = 0;
int start = 0;
int index_mode = 0;

int CURRENT_STATE[2] = {0, 2};
int SEG7_CLOCK[2] = {10000, 3000};
int led_seg_buffer[4] = {0};
int LED_TIME[3] = {10000, 3000, 7000};
int TIMES_INC = 0;

int man_red_time = 5;
int man_amber_time = 2;
int man_green_time = 3;

int temp_value = 0;
int light_time = GREEN_TIME / 100;
int light_time1 = RED_TIME / 100;
int led_status = RED_GREEN;
int index_led = 0;
int AllowToExecuteAfterASecondPressed = 0;

int flagForButtonPressOneQuarterSecond[4] = {0};
