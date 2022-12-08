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
#include "displayuart.h"

enum ButtonState{BUTTON_RELEASED, BUTTON_PRESSED, BUTTON_PRESSED_MORE_THAN_1_SECOND} ;
enum ButtonState buttonState = BUTTON_RELEASED;

int WhichButtonIsPressed() {
	if (is_button_pressed(0)) return 1;
	if (is_button_pressed(1)) return 2;
	if (is_button_pressed(2)) return 3;

	return 0; // None of these buttons are pressed
}

void clear_vertical() {
	HAL_GPIO_WritePin(GPIOA, Traffic_1_1_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOB, Traffic_1_2_Pin, GPIO_PIN_SET);

}

void clear_horizontal() {
	HAL_GPIO_WritePin(GPIOB, Traffic_2_1_Pin | Traffic_2_2_Pin, GPIO_PIN_SET);
}

void vertical_processing() {
	clear_vertical();
	switch (CURRENT_STATE[0]) {
	case 0:
		HAL_GPIO_WritePin(GPIOA, Traffic_1_1_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOB, Traffic_1_2_Pin, GPIO_PIN_RESET);
		break;
	case 1:
		HAL_GPIO_WritePin(GPIOA, Traffic_1_1_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOB, Traffic_1_2_Pin, GPIO_PIN_SET);
		break;
	case 2:
		HAL_GPIO_WritePin(GPIOA, Traffic_1_1_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOB, Traffic_1_2_Pin, GPIO_PIN_SET);
		break;
	default:
		break;
	}
}

void horizontal_processing() {
	clear_horizontal();
	switch (CURRENT_STATE[1]) {
	case 0:
		HAL_GPIO_WritePin(GPIOB, Traffic_2_1_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOB, Traffic_2_2_Pin, GPIO_PIN_RESET);
		break;
	case 1:
		HAL_GPIO_WritePin(GPIOB, Traffic_2_1_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOB, Traffic_2_2_Pin, GPIO_PIN_SET);
		break;
	case 2:
		HAL_GPIO_WritePin(GPIOB, Traffic_2_1_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOB, Traffic_2_2_Pin, GPIO_PIN_SET);
		break;
	default:
		break;
	}
}

void state_update(int idx) {
	if (idx == 0) {
		CURRENT_STATE[idx] = (CURRENT_STATE[idx] + 1) % 3;
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
	LED_TIME[RED_STATUS] = NORMAL_RED * TIME_UNIT;
	LED_TIME[YELLOW_STATUS] = NORMAL_YELLOW * TIME_UNIT;
	LED_TIME[GREEN_STATUS] = NORMAL_GREEN * TIME_UNIT;
	CURRENT_STATE[VER_LED] = RED_STATUS;
	CURRENT_STATE[HOR_LED] = GREEN_STATUS;
	SEG7_CLOCK[VER_LED] = LED_TIME[CURRENT_STATE[VER_LED]];
	SEG7_CLOCK[HOR_LED] = LED_TIME[CURRENT_STATE[HOR_LED]];
}

void restart(){
	status = 0;
}

void confirm_action(int mode) {
	switch (mode) {
	case 0: //Normal mode -> do nothing
		return;
	case 1: // Inc red time mode
		LED_TIME[RED_STATUS] = LED_TIME[RED_STATUS] + (TIMES_INC * TIME_UNIT);
		LED_TIME [GREEN_STATUS] = LED_TIME[GREEN_STATUS] + (TIMES_INC * TIME_UNIT);
		break;
	case 2: // Inc yellow time mode
		LED_TIME[RED_STATUS] = LED_TIME[RED_STATUS] + (TIMES_INC * TIME_UNIT);
		LED_TIME[YELLOW_STATUS] = LED_TIME[YELLOW_STATUS] + (TIMES_INC * TIME_UNIT);
		break;
	case 3: // Inc green time
		LED_TIME[RED_STATUS] = LED_TIME[RED_STATUS] + (TIMES_INC * TIME_UNIT);
		LED_TIME [GREEN_STATUS] = LED_TIME[GREEN_STATUS] + (TIMES_INC * TIME_UNIT);
		break;
	default:
		break;
	}

}

void state_handle() {
	switch (index_mode) {
	case 0:
		SEG7_CLOCK[VER_LED] = 0;
		SEG7_CLOCK[HOR_LED] = 0;
		break;
	case 1:
		SEG7_CLOCK[VER_LED] = 1 * TIME_UNIT;
		SEG7_CLOCK[HOR_LED] = LED_TIME[0] + TIMES_INC * TIME_UNIT;
		break;
	case 2:
		SEG7_CLOCK[VER_LED] = 2  * TIME_UNIT;
		SEG7_CLOCK[HOR_LED] = LED_TIME[1] + TIMES_INC * TIME_UNIT;
		break;
	case 3:
		SEG7_CLOCK[VER_LED] = 3  * TIME_UNIT;
		SEG7_CLOCK[HOR_LED] = LED_TIME[2] + TIMES_INC * TIME_UNIT;
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
		status = 2;
		index_mode = (index_mode + 1) % 4;
		TIMES_INC = 0;
	}

	// Add button
	if (is_button_pressed(1) && index_mode != 0) {
		TIMES_INC++;
	}

	// Confirm button
	if (is_button_pressed(2) && index_mode != 0) {
		if (TIMES_INC != 0) {
			confirm_action(index_mode);
		}
		TIMES_INC = 0;
		index_mode = 0;
		restart();
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
