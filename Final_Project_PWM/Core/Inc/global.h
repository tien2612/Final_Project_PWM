/*
 * global.h
 *
 *  Created on: Oct 24, 2022
 *      Author: Tien
 */

#ifndef INC_GLOBAL_H_
#define INC_GLOBAL_H_

// declare the duration of all default leds 
#define RED_TIME							500
#define GREEN_TIME							300
#define	YELLOW_TIME							200

#define NORMAL_MODE							1
#define RED_MODE							2
#define AMBER_MODE							3
#define GREEN_MODE							4
#define MAN_RED_MODE						5
#define MAN_AMBER_MODE						6
#define MAN_GREEN_MODE						7
#define CONFIRM_STATE						8

#define button_mode_is_pressed				10
#define button_add_is_pressed				11
#define button_confirm_is_pressed			12
#define button_reset_is_pressed				13

#define button_mode_is_pressed_1s			14
#define button_add_is_pressed_1s			15
#define button_confirm_is_pressed_1s		16
#define button_reset_is_pressed_1s			17

#define INIT_TRAFFIC_LIGHT					19			// Initital state for traffic light 2-way
#define RED_GREEN							20			// RED 1 ON - GREEN 2 ON
#define	RED_AMBER							21			// RED 1 ON - AMBER 2 ON
#define GREEN_RED							22			// GREEN 1 ON - RED 2 ON
#define AMBER_RED							23			// AMBER 1 ON - RED 2 ON

#define DURATION_1S							1000
#define DURATION_HALF_OF_SECOND				500

extern int man_red_time;
extern int man_green_time;
extern int man_amber_time;
extern int temp_value;

extern int led_status;
extern int light_time;
extern int light_time1;
extern int status;	// 4 modes of traffic light such as: mode MODE, ADD, CONFIRM and RESET
extern int index_led;

extern int flagForButtonPressOneQuarterSecond[4];
extern int AllowToExecuteAfterASecondPressed;
#endif /* INC_GLOBAL_H_ */
