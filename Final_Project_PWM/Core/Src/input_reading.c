/*
 * input_reading.c
 *
 *  Created on: Oct 25, 2022
 *      Author: Tien
 */

#include "main.h"
#include "input_reading.h"
#include "global.h"

//#include "7SEG.h"
// timer interrupt duration is 10ms , so to pass 1 second ,
// we need to jump to the interrupt service routine 100 time
#define DURATION_FOR_AUTO_INCREASING 			100
#define	DURATION_FOR_AUTO_RUNNING_MODE		  	25
#define	DURATION_FOR_DEBOUNDING 				5
#define BUTTON_IS_PRESSED GPIO_PIN_RESET
#define BUTTON_IS_RELEASED GPIO_PIN_SET
// FSM state for input processing
// the buffer that the final result is stored after debouncing
static GPIO_TypeDef *buttonPort[N0_OF_BUTTONS] = {
		B_1_GPIO_Port,
		B_2_GPIO_Port,
		B_3_GPIO_Port,
		B_Pedes_GPIO_Port
};

static uint16_t buttonPin[N0_OF_BUTTONS] = {
		B_1_Pin,
		B_2_Pin,
		B_3_Pin,
		B_Pedes_Pin
};

// debouncing
static GPIO_PinState buttonBuffer[N0_OF_BUTTONS];
// we define two buffers for debouncing
static GPIO_PinState debounceButtonBuffer1[N0_OF_BUTTONS];
static GPIO_PinState debounceButtonBuffer2[N0_OF_BUTTONS];
// we define a flag for a button pressed more than 1 second .
static uint8_t flagForButtonPress1s[N0_OF_BUTTONS];
// we define counter for automatically increasing the value
// after the button is pressed more than 1 second .
static uint16_t counterForButtonPress1s[N0_OF_BUTTONS];
static uint16_t counterForButtonPressOneQuarterSecond[N0_OF_BUTTONS];

void button_reading(void) {
	for (int i = 0; i < N0_OF_BUTTONS; i++) {
		debounceButtonBuffer2[i] = debounceButtonBuffer1[i];
		debounceButtonBuffer1[i] = HAL_GPIO_ReadPin(buttonPort[i], buttonPin[i]);

		if (debounceButtonBuffer1[i] == debounceButtonBuffer2[i]) {
			//valid input, can read now
			buttonBuffer[i] = debounceButtonBuffer1[i];
			if (buttonBuffer[i] == BUTTON_IS_PRESSED) {
				if ((counterForButtonPressOneQuarterSecond[i] < DURATION_FOR_AUTO_RUNNING_MODE)
						&& AllowToExecuteAfterASecondPressed)
				{
					counterForButtonPressOneQuarterSecond[i]++;
				} else {
					counterForButtonPressOneQuarterSecond[i] = 0;
					flagForButtonPressOneQuarterSecond[i] = 1;
				}
				if(counterForButtonPress1s[i] < DURATION_FOR_AUTO_INCREASING) {
					counterForButtonPress1s[i]++;
				} else {
					// the flag is turned on when 1 second has passed
					// since the button is pressed .
					flagForButtonPress1s[i] = 1;
				}
			}

			else {
				counterForButtonPress1s[i] = 0;
				counterForButtonPressOneQuarterSecond[i] = 0;
				flagForButtonPress1s[i] = 0;
				flagForButtonPressOneQuarterSecond[i] = 0;
			}
		}
	}
}
// Check if a button is pressed or not
unsigned char is_button_pressed(uint8_t index) {
	if (index >= N0_OF_BUTTONS) return 0;
	return (buttonBuffer[index] == BUTTON_IS_PRESSED);
}

// Check for button is pressed more than a second or not
unsigned char is_button_pressed_1s (unsigned char index ) {
	if(index >= N0_OF_BUTTONS ) return 0xff ;
	return (flagForButtonPress1s[index] == 1) ;
}

unsigned char is_button_pressed_one_quarter_second(unsigned char index) {
	if(index >= N0_OF_BUTTONS) return 0xff ;
	return (flagForButtonPressOneQuarterSecond[index] == 1) ;
}
