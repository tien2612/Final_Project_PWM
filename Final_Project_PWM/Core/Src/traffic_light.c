/*
 * traffic_light.c
 *
 *  Created on: Dec 6, 2022
 *      Author: Tien
 */


#include "traffic_light.h"

void helper_set_led_color(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin1, GPIO_TypeDef *GPIOy, uint16_t GPIO_Pin2, int COLOR) {
	switch (COLOR) {
		case GREEN_COLOR:
			GPIOx->BSRR = GPIO_Pin1;
			GPIOy->BSRR = (uint32_t)GPIO_Pin2 << 16u;
			break;
		case AMBER_COLOR:
			GPIOx->BSRR = GPIO_Pin1;
			GPIOy->BSRR = GPIO_Pin2;
			break;
		case RED_COLOR:
			GPIOx->BSRR = (uint32_t)GPIO_Pin1 << 16u;
			GPIOy->BSRR = GPIO_Pin2;
			break;
	}
}

void set_led_color(int led, int color) {
	switch (led) {
		case TRAFFIC_1_LED:
			helper_set_led_color(Traffic_1_1_GPIO_Port, Traffic_1_1_Pin,
					Traffic_1_2_GPIO_Port, Traffic_1_2_Pin, color);
			break;
		case TRAFFIC_2_LED:
			helper_set_led_color(Traffic_2_1_GPIO_Port, Traffic_2_1_Pin,
					Traffic_2_1_GPIO_Port, Traffic_2_2_Pin, color);
			break;
		case PEDESTRIAN_LED:
			helper_set_led_color(Traffic_1_1_GPIO_Port, Traffic_1_1_Pin,
					Traffic_1_2_GPIO_Port, Traffic_1_2_Pin, color);
			break;
	}
}

int helper_get_led_color(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin1, GPIO_TypeDef *GPIOy, uint16_t GPIO_Pin2) {
	GPIO_PinState bitstatus1;
	GPIO_PinState bitstatus2;

	if ((GPIOx->IDR & GPIO_Pin1) != (uint32_t)GPIO_PIN_RESET) bitstatus1 = GPIO_PIN_SET;
	else bitstatus1 = GPIO_PIN_RESET;

	if ((GPIOy->IDR & GPIO_Pin2) != (uint32_t)GPIO_PIN_RESET) bitstatus2 = GPIO_PIN_SET;
	else bitstatus2 = GPIO_PIN_RESET;

	if (!bitstatus1 && bitstatus2) return GREEN_COLOR;
	else if (bitstatus1 && !bitstatus2) return RED_COLOR;
	else if (bitstatus1 && bitstatus2) return AMBER_COLOR;

	return UNDEFINED_COLOR;
}

int get_led_color(int led) {
	switch(led) {
	case TRAFFIC_1_LED:
		return helper_get_led_color(Traffic_1_1_GPIO_Port, Traffic_1_1_Pin, Traffic_1_2_GPIO_Port, Traffic_1_2_Pin);
		break;
	case TRAFFIC_2_LED:
		return helper_get_led_color(Traffic_2_1_GPIO_Port, Traffic_2_1_Pin, Traffic_2_1_GPIO_Port, Traffic_2_2_Pin);
		break;
	case PEDESTRIAN_LED:
		return helper_get_led_color(Traffic_Pedes_1_GPIO_Port, Traffic_Pedes_1_Pin, Traffic_Pedes_2_GPIO_Port, Traffic_Pedes_2_Pin);
		break;
	default:
		return UNDEFINED_COLOR;
		break;
	}

	return UNDEFINED_COLOR;
}



