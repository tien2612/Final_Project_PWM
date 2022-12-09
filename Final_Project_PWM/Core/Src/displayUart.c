/*
 * displayUart.c
 *
 *  Created on: 8 Dec 2022
 *      Author: xanlo
 */


#include <displayUart.h>
#include "main.h"
#include "global.h"

#include <stdio.h>

extern UART_HandleTypeDef huart2;
char str[50];

void updateDisplay(){
	HAL_UART_Transmit(&huart2, (uint8_t*)str, sprintf(str, "!7SEG:%d#\r\n",SEG7_CLOCK[0] / TIME_UNIT), 1000);
}

