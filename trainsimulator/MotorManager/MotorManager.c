/*
 * MotorManager.c
 *
 *  Created on: Dec 30, 2017
 *      Author: ojeju
 */

#include "pins_config.h"
#include "cmsis_os.h"

typedef int_fast16_t speed;

static void ReadSpeed(void);
static void CalculateSpeed(void);
static void UpdateSpeed(speed value);

static speed target_speed;
static speed acceleration;
static TIM_HandleTypeDef *Timer;

extern void InitMotorManager(TIM_HandleTypeDef *const  htim)
{
	Timer = htim;
}

extern void MotorManager()
{
	ReadSpeed();
	CalculateSpeed();
	osDelay(100);
}

static void ReadSpeed()
{
	GPIO_PinState position_minus1 = HAL_GPIO_ReadPin(SPEED_MINUS_1_IN);
	GPIO_PinState position_zero = HAL_GPIO_ReadPin(SPEED_0_IN);
	GPIO_PinState position_one = HAL_GPIO_ReadPin(SPEED_1_IN);
	GPIO_PinState position_two = HAL_GPIO_ReadPin(SPEED_2_IN);
	GPIO_PinState position_three = HAL_GPIO_ReadPin(SPEED_3_IN);
	GPIO_PinState position_four = HAL_GPIO_ReadPin(SPEED_4_IN);

	if (position_minus1)
	{
		target_speed = 0;
		acceleration = 2000;
	} else if (position_zero)
	{
		target_speed = 0;
		acceleration = 1000;
	} else if (position_one)
	{
		target_speed = 0xFFFF >> 8;
		acceleration = 1000;
	} else if (position_two)
	{
		target_speed = 0xFFFF >> 4;
		acceleration = 1000;
	} else if (position_three)
	{
		target_speed = 0xFFFF >> 2;
		acceleration = 1000;
	} else if (position_four)
	{
		target_speed = 0xFFFF;
		acceleration = 1000;
	}
}

static void CalculateSpeed()
{
	static speed current_speed;
	if (target_speed > current_speed + acceleration)
	{
		current_speed += acceleration;
		UpdateSpeed(current_speed);
	} else if (target_speed < current_speed - acceleration)
	{
		current_speed -= acceleration;
		UpdateSpeed(current_speed);
	}
}

speed tempspeed;
static void UpdateSpeed(speed value)
{
	TIM_OC_InitTypeDef sConfigOC;

	sConfigOC.OCMode = TIM_OCMODE_PWM1;
	sConfigOC.Pulse = tempspeed;
	sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
	sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;

	HAL_TIM_PWM_ConfigChannel(Timer, &sConfigOC, TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(Timer, TIM_CHANNEL_1);

	HAL_TIM_PWM_ConfigChannel(Timer, &sConfigOC, TIM_CHANNEL_2);
	HAL_TIM_PWM_Start(Timer, TIM_CHANNEL_2);
}
