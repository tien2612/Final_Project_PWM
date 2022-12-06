/*
 * traffic_light.h
 *
 *  Created on: Dec 6, 2022
 *      Author: nguye
 */

#ifndef INC_TRAFFIC_LIGHT_H_
#define INC_TRAFFIC_LIGHT_H_

#include "main.h"
#include "global.h"

#define RED_COLOR							30
#define AMBER_COLOR							31
#define GREEN_COLOR							32

void set_led_color(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin1, GPIO_TypeDef *GPIOy, uint16_t GPIO_Pin2, int COLOR);

#endif /* INC_TRAFFIC_LIGHT_H_ */
