/*
 * input_processing.c
 *
 *  Created on: Dec 8, 2022
 *      Author: xanlo
 */


#include "main.h"
#include "input_reading.h"
#include "input_processing.h"
#include "global.h"
#include "software_timer.h"
#include "displayUart.h"
#include "traffic_light.h"

enum ButtonState{BUTTON_RELEASED, BUTTON_PRESSED, BUTTON_PRESSED_MORE_THAN_1_SECOND} ;
enum ButtonState buttonState = BUTTON_RELEASED;

extern UART_HandleTypeDef huart2;
char str[50];

int WhichButtonIsPressed() {
	if (is_button_pressed(2)){
		return 3;
	}
	if (is_button_pressed(1)){
		return 2;
	}
	if (is_button_pressed(0)){
		return 1;
	}

	return 0; // None of these buttons are pressed
}

int WhichButtonIsPressedMoreThan1S() {
	if (is_button_pressed_1s(2)){
		return 3;
	}
	if (is_button_pressed_1s(1)){
		return 2;
	}
	if (is_button_pressed_1s(0)){
		return 1;
	}

	return 0; // None of these buttons are pressed
}

void clear_vertical() {
	clear_led(TRAFFIC_1_LED);
}

void clear_horizontal() {
	clear_led(TRAFFIC_2_LED);
}

void vertical_processing() {
	clear_vertical();
	switch (CURRENT_STATE[0]) {
	case 0:
		set_led_color(TRAFFIC_1_LED, RED_COLOR);
		break;
	case 1:
		set_led_color(TRAFFIC_1_LED, AMBER_COLOR);
		break;
	case 2:
		set_led_color(TRAFFIC_1_LED, GREEN_COLOR);
		break;
	default:
		break;
	}
}

void horizontal_processing() {
	clear_horizontal();
	switch (CURRENT_STATE[1]) {
	case 0:
		set_led_color(TRAFFIC_2_LED, RED_COLOR);
		break;
	case 1:
		set_led_color(TRAFFIC_2_LED, AMBER_COLOR);
		break;
	case 2:
		set_led_color(TRAFFIC_2_LED, GREEN_COLOR);
		break;
	default:
		break;
	}
}

void state_update(int idx) {
	if (idx == 0) {
		switch (CURRENT_STATE[idx]) {
		case 0:
			CURRENT_STATE[idx] = 2;
			break;
		case 1:
			CURRENT_STATE[idx] = 0;
			break;
		case 2:
			CURRENT_STATE[idx] = 1;
			break;
		default:
			break;
		}
	} else {
		CURRENT_STATE[idx] = (CURRENT_STATE[idx] - 1 + 3) % 3;
	}
	SEG7_CLOCK[idx] = LED_TIME[CURRENT_STATE[idx]];
}

void check_state() {
	if (SEG7_CLOCK[VER_LED] <= 0) {
		state_update(VER_LED);
		vertical_processing();
	}

	if (SEG7_CLOCK[HOR_LED] <= 0) {
		state_update(HOR_LED);
		horizontal_processing();
	}
}

void update_clock() {
	if (timer2_flag == 1) {
		SEG7_CLOCK[VER_LED] = SEG7_CLOCK[VER_LED] - TIME_UNIT;
		SEG7_CLOCK[HOR_LED] = SEG7_CLOCK[HOR_LED] - TIME_UNIT;
		check_state();
		updateDisplay();
		setTimer2(1000);
	}
}

void reset() {
	LED_TIME[RED_STATUS] = NORMAL_RED ;
	LED_TIME[YELLOW_STATUS] = NORMAL_YELLOW;
	LED_TIME[GREEN_STATUS] = NORMAL_GREEN;
	CURRENT_STATE[VER_LED] = RED_STATUS;
	CURRENT_STATE[HOR_LED] = GREEN_STATUS;
	SEG7_CLOCK[VER_LED] = LED_TIME[CURRENT_STATE[VER_LED]];
	SEG7_CLOCK[HOR_LED] = LED_TIME[CURRENT_STATE[HOR_LED]];
}


void confirm_action(int mode, int time_inc) {
	switch (mode) {
	case 0: // Inc red time mode
		LED_TIME[RED_STATUS] = tmpBuffer[RED_STATUS];
		LED_TIME [GREEN_STATUS] = tmpBuffer[GREEN_STATUS];
		break;
	case 1: // Inc yellow time mode
		LED_TIME[YELLOW_STATUS] = tmpBuffer[YELLOW_STATUS];
		LED_TIME[RED_STATUS] = tmpBuffer[RED_STATUS];
		break;
	case 2: // Inc green time
		LED_TIME [GREEN_STATUS] = tmpBuffer[GREEN_STATUS];
		LED_TIME[RED_STATUS] = tmpBuffer[RED_STATUS];
		break;
	default:
		break;
	}
}

void add_clock(){
	switch (index_mode) {
	case 0:
		tmpBuffer[RED_STATUS] = tmpBuffer[RED_STATUS] + TIME_UNIT;
		tmpBuffer[GREEN_STATUS] = tmpBuffer[GREEN_STATUS] + TIME_UNIT;
		if (tmpBuffer[RED_STATUS] > 99000) {
			tmpBuffer[RED_STATUS] = 0;
		}
		if (tmpBuffer[GREEN_STATUS] > 99000) {
			tmpBuffer[GREEN_STATUS] = 0;
		}
		break;
	case 1:
		tmpBuffer[YELLOW_STATUS] = tmpBuffer[YELLOW_STATUS] + TIME_UNIT;
		tmpBuffer[RED_STATUS] = tmpBuffer[RED_STATUS] + TIME_UNIT;
		if (tmpBuffer[YELLOW_STATUS] > 99000) {
			tmpBuffer[YELLOW_STATUS] = 0;
		}
		if (tmpBuffer[RED_STATUS] > 99000) {
			tmpBuffer[RED_STATUS] = 0;
		}
		break;
	case 2:
		tmpBuffer[GREEN_STATUS] = tmpBuffer[GREEN_STATUS] + TIME_UNIT;
		tmpBuffer[RED_STATUS] = tmpBuffer[RED_STATUS] + TIME_UNIT;
		if (tmpBuffer[GREEN_STATUS] > 99000) {
			tmpBuffer[GREEN_STATUS] = 0;
		}
		if (tmpBuffer[RED_STATUS] > 99000) {
			tmpBuffer[RED_STATUS] = 0;
		}
		break;
	default:
		break;
	}
}

void state_handle() {
	switch (index_mode) {
	case 0:							//RED MODE
		if (timer1_flag == 1) {
			HAL_GPIO_WritePin(Traffic_1_2_GPIO_Port, Traffic_1_2_Pin, 0);
			HAL_GPIO_TogglePin(Traffic_1_1_GPIO_Port, Traffic_1_1_Pin);
			HAL_GPIO_WritePin(Traffic_2_2_GPIO_Port, Traffic_2_2_Pin, 0);
			HAL_GPIO_TogglePin(Traffic_2_1_GPIO_Port, Traffic_2_1_Pin);
			setTimer1(500);
		}
		SEG7_CLOCK[VER_LED] = tmpBuffer[0];
		SEG7_CLOCK[HOR_LED] = 0;
		break;
	case 1:							//YELLOW MODE
		if (timer1_flag == 1) {
			HAL_GPIO_TogglePin(Traffic_1_1_GPIO_Port, Traffic_1_1_Pin);
			HAL_GPIO_TogglePin(Traffic_1_2_GPIO_Port, Traffic_1_2_Pin);
			HAL_GPIO_TogglePin(Traffic_2_1_GPIO_Port, Traffic_2_1_Pin);
			HAL_GPIO_TogglePin(Traffic_2_2_GPIO_Port, Traffic_2_2_Pin);
			setTimer1(500);
		}
		SEG7_CLOCK[VER_LED] = tmpBuffer[1];
		SEG7_CLOCK[HOR_LED] = 0;
		break;
	case 2:							//GREEN MODE
		if (timer1_flag == 1) {
			HAL_GPIO_WritePin(Traffic_1_1_GPIO_Port, Traffic_1_1_Pin, 0);
			HAL_GPIO_TogglePin(Traffic_1_2_GPIO_Port, Traffic_1_2_Pin);
			HAL_GPIO_WritePin(Traffic_2_1_GPIO_Port, Traffic_2_1_Pin, 0);
			HAL_GPIO_TogglePin(Traffic_2_2_GPIO_Port, Traffic_2_2_Pin);
			setTimer1(500);
		}
		SEG7_CLOCK[VER_LED] = tmpBuffer[2];
		SEG7_CLOCK[HOR_LED] = 0;
		break;
	default:
		break;
	}
}

void traffic_processing() {
	if (timer4_flag == 1) {
		status = 0;
		index_mode = -1;
		clearTimer4();
	}
	switch (status) {
	case 0: //INIT
		CURRENT_STATE[VER_LED] = RED_STATUS;
		CURRENT_STATE[HOR_LED] = GREEN_STATUS;
		SEG7_CLOCK[VER_LED] = LED_TIME[CURRENT_STATE[VER_LED]];
		SEG7_CLOCK[HOR_LED] = LED_TIME[CURRENT_STATE[HOR_LED]];
		updateDisplay();
		vertical_processing();
		horizontal_processing();
		status = 1;
		clearTimer2();
	    setTimer2(1000);	//Update clock after each 1s
		break;
	case 1: // Normal state
		update_clock();
		break;
	case 2: // Modify state
		state_handle();
		break;
	default:
		break;
	}
}

void input_processing() {

	if (timer3_flag != 1) {
		return;
	}

	// Switch button
	if (is_button_pressed(0)) {
		clear_vertical();
		clear_horizontal();
		status = 2;
		index_mode = (index_mode + 1);
		TIMES_INC = 0;
		if (index_mode >= 3) {
			status = 0;
			index_mode = -1;
		}
		if (index_mode != -1) {
			clearTimer4(); // After 10s auto switch to auto mode
			setTimer4(10000);
			tmpBuffer[0] = LED_TIME[0];
			tmpBuffer[1] = LED_TIME[1];
			tmpBuffer[2] = LED_TIME[2];
			state_handle();
			updateDisplay();
		}
	}

	// Add button
	if (is_button_pressed(1) && index_mode != -1) {
		TIMES_INC = TIMES_INC + 1;
		clearTimer4();
		setTimer4(10000);
		add_clock();
		state_handle();
		updateDisplay();
	}

	// Long press add button
	if (is_button_pressed_1s(1) && index_mode != -1) {
		if (timer3_flag == 1) {
			clearTimer4();
			setTimer4(10000);
			add_clock();
			state_handle();
			updateDisplay();
			setTimer3(100);
		}
	}

	// Confirm button
	if (is_button_pressed(2) && index_mode != -1) {
		if (TIMES_INC != 0) {
			confirm_action(index_mode, TIMES_INC);
		}
		clearTimer4();
		TIMES_INC = 0;
		index_mode = -1;
		status = 0;
	}


}

void fsm_simple_button_run() {
	switch (buttonState) {
	case BUTTON_RELEASED:
		if (WhichButtonIsPressed()) {
			buttonState = BUTTON_PRESSED;
			input_processing();
		}
	case BUTTON_PRESSED:
		if (!WhichButtonIsPressed()) {
			buttonState = BUTTON_RELEASED;
		}
		if (WhichButtonIsPressedMoreThan1S()) {
			buttonState = BUTTON_PRESSED_MORE_THAN_1_SECOND;
			setTimer3(100);
		}
	case BUTTON_PRESSED_MORE_THAN_1_SECOND:
		if (!WhichButtonIsPressed()) {
			buttonState = BUTTON_RELEASED;
		}
		if (WhichButtonIsPressedMoreThan1S()) {
			input_processing();
		}
	default:
		break;
	}
}
