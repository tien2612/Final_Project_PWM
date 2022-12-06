/*
 * input_reading.h
 *
 *  Created on: Oct 25, 2022
 *      Author: Tien
 */

#ifndef INC_INPUT_READING_H_
#define INC_INPUT_READING_H_
#include "main.h"
// we aim to work with more than one buttons
#define N0_OF_BUTTONS 4

void button_reading(void);
unsigned char is_button_pressed(unsigned char index);
unsigned char is_button_pressed_3s(unsigned char index);
unsigned char is_button_pressed_1s_while_holding(unsigned char index);
#endif /* INC_INPUT_READING_H_ */


















