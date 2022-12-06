/*
 * traffic_light.c
 *
 *  Created on: Dec 6, 2022
 *      Author: Tien
 */


#include "traffic_light.h"

void set_led_color(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin1, GPIO_TypeDef *GPIOy, uint16_t GPIO_Pin2, int COLOR) {
	switch (COLOR) {
		case RED_COLOR:
			GPIOx->BSRR = GPIO_Pin1;
			GPIOy->BSRR = (uint32_t)GPIO_Pin2 << 16u;
			break;
		case AMBER_COLOR:
			GPIOx->BSRR = GPIO_Pin1;
			GPIOy->BSRR = GPIO_Pin2;
			break;
		case GREEN_COLOR:
			GPIOx->BSRR = GPIO_Pin1;
			GPIOy->BSRR = GPIO_Pin2;
			break;
	}
}
