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

#define UNDEFINED_COLOR						29
#define RED_COLOR							30
#define AMBER_COLOR							31
#define GREEN_COLOR							32

#define TRAFFIC_1_LED						40
#define TRAFFIC_2_LED						41
#define PEDESTRIAN_LED						42

void set_led_color(int led, int color);
int get_led_color(int led);

#endif /* INC_TRAFFIC_LIGHT_H_ */
