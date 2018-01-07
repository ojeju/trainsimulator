/*
 * pins_config.h
 *
 *  Created on: Jan 6, 2018
 *      Author: ojeju
 */

#ifndef PINS_CONFIG_H_
#define PINS_CONFIG_H_

/*
 * pins_config.h
 *
 *  Created on: Dec 30, 2017
 *      Author: ojeju
 */


#include "stm32f1xx_hal.h"

#define GREEN_METAL_LED GPIOA, GPIO_PIN_12
#define RED_METAL_LED GPIOC, GPIO_PIN_5

#define GREEN_LED GPIOC, GPIO_PIN_9
#define RED_LED GPIOC, GPIO_PIN_8
#define BLUE_LED GPIOC, GPIO_PIN_6

#define GREEN_BTN GPIOB, GPIO_PIN_5
#define RED_BTN GPIOA, GPIO_PIN_10
#define BLUE_BTN GPIOC, GPIO_PIN_4

#define PWR_I GPIOA, GPIO_PIN_1
#define PWR_II GPIOA, GPIO_PIN_0

#define TOGGLE_I GPIOB, GPIO_PIN_4
#define TOGGLE_II GPIOB, GPIO_PIN_13

#define BUZZER GPIOB, GPIO_PIN_9

#define RELAY GPIOB, GPIO_PIN_8

#define MOTOR GPIO, GPIO_PIN_
#define MOTOR_IN1 GPIOB, GPIO_PIN_14
#define MOTOR_IN2 GPIOB, GPIO_PIN_15

#define SPEDOMOTER GPIO, GPIO_PIN_

#define SPEED_3 GPIOC, GPIO_PIN_0
#define SPEED_0 GPIOC, GPIO_PIN_3
#define SPEED_2 GPIOC, GPIO_PIN_1
#define SPEED_5 GPIOC, GPIO_PIN_2
#define SPEED_4 GPIOB, GPIO_PIN_0
#define SPEED_1 GPIOA, GPIO_PIN_4


#endif /* PINS_CONFIG_H_ */
