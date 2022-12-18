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
	if (is_button_pressed(0)) return 1;
	if (is_button_pressed(1)) return 2;
	if (is_button_pressed(2)) return 3;

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
//	HAL_UART_Transmit(&huart2, (uint8_t*)str, sprintf(str, "!7SEG:%d#\r\n",TIMES_INC), 1000);
	switch (mode) {
	case 0: // Inc red time mode
		LED_TIME[RED_STATUS] = LED_TIME[RED_STATUS] + (time_inc * TIME_UNIT);
		break;
	case 1: // Inc yellow time mode
		LED_TIME[YELLOW_STATUS] = LED_TIME[YELLOW_STATUS] + (time_inc * TIME_UNIT);
		break;
	case 2: // Inc green time
		LED_TIME [GREEN_STATUS] = LED_TIME[GREEN_STATUS] + (time_inc * TIME_UNIT);
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
		SEG7_CLOCK[VER_LED] = ((LED_TIME[0] / TIME_UNIT + TIMES_INC) % 99) * TIME_UNIT;
		SEG7_CLOCK[HOR_LED] = 0;
//		updateDisplay();
		break;
	case 1:							//YELLOW MODE
		if (timer1_flag == 1) {
			HAL_GPIO_TogglePin(Traffic_1_1_GPIO_Port, Traffic_1_1_Pin);
			HAL_GPIO_TogglePin(Traffic_1_2_GPIO_Port, Traffic_1_2_Pin);
			HAL_GPIO_TogglePin(Traffic_2_1_GPIO_Port, Traffic_2_1_Pin);
			HAL_GPIO_TogglePin(Traffic_2_2_GPIO_Port, Traffic_2_2_Pin);
			setTimer1(500);
		}
		SEG7_CLOCK[VER_LED] = ((LED_TIME[1]/TIME_UNIT + TIMES_INC) % 99) * TIME_UNIT;
		SEG7_CLOCK[HOR_LED] = 0;
//		updateDisplay();
		break;
	case 2:							//GREEN MODE
		if (timer1_flag == 1) {
			HAL_GPIO_WritePin(Traffic_1_1_GPIO_Port, Traffic_1_1_Pin, 0);
			HAL_GPIO_TogglePin(Traffic_1_2_GPIO_Port, Traffic_1_2_Pin);
			HAL_GPIO_WritePin(Traffic_2_1_GPIO_Port, Traffic_2_1_Pin, 0);
			HAL_GPIO_TogglePin(Traffic_2_2_GPIO_Port, Traffic_2_2_Pin);
			setTimer1(500);
		}
		SEG7_CLOCK[VER_LED] = ((LED_TIME[2]/TIME_UNIT + TIMES_INC) % 99) * TIME_UNIT;
		SEG7_CLOCK[HOR_LED] = 0;
//		updateDisplay();
		break;
	default:
		break;
	}
}

void traffic_processing() {
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
		state_handle();
		updateDisplay();
	}

	// Add button
	if (is_button_pressed(1) && index_mode != -1) {
		TIMES_INC++;
		state_handle();
		updateDisplay();
	}

	// Confirm button
	if (is_button_pressed(2) && index_mode != -1) {
		if (TIMES_INC != 0) {
			confirm_action(index_mode, TIMES_INC);
		}
		TIMES_INC = 0;
		index_mode = -1;
		status = 0;
	}

	// RESET when start
	if (start == 0) {
		reset();
		start = 1;
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
	default:
		break;
	}
}
