/*
 * MotorManager.c
 *
 *  Created on: Dec 30, 2017
 *      Author: ojeju
 */

#include "pins_config.h"
#include "cmsis_os.h"

typedef int_fast16_t Speed_t;

static void CheckSpeedSettings(void);
static void CalculateSpeed(void);
static void UpdateSpeed();
static void CheckDirectionSettings(void);

static Speed_t TargetSpeed;
static Speed_t CurrentSpeed;
static Speed_t Acceleration;
static TIM_HandleTypeDef *Timer;

extern void InitMotorDriver(TIM_HandleTypeDef * const htim)
{
	Timer = htim;
}

extern void MotorDriver()
{
	CheckSpeedSettings();
	CheckDirectionSettings();
	CalculateSpeed();
	UpdateSpeed();
	osDelay(100);
}

static void CheckSpeedSettings()
{
	const struct SPEED_CFG_T
	{
		GPIO_TypeDef* GPIOx;
		uint16_t GPIO_Pin;
		Speed_t target_speed;
		Speed_t acceleration;
	} SpeedCfg[] = {{SPEED_MINUS_1_IN, 0, 1000},
			{SPEED_0_IN, 0, 1000},
			{SPEED_1_IN, 0xFFFF/8, 1000},
			{SPEED_2_IN, 0xFFFF/3, 1000},
			{SPEED_3_IN, 0xFFFF/2, 1000},
			{SPEED_4_IN, 0xFFFF, 1000}
	};

	const BaseType_t NUM_OF_CFG = sizeof(SpeedCfg) / sizeof(SpeedCfg[0]);

	for(BaseType_t i=0; i<NUM_OF_CFG; i++)
	{
		struct SPEED_CFG_T const *cfg = &SpeedCfg[i];
		GPIO_PinState pin_state = HAL_GPIO_ReadPin(cfg->GPIOx, cfg->GPIO_Pin);
		if (pin_state == GPIO_PIN_SET)
		{
			TargetSpeed = cfg->target_speed;
			Acceleration = cfg->acceleration;
			break;
		}
	}
}

static void CalculateSpeed()
{
	if (TargetSpeed > CurrentSpeed + Acceleration)
	{
		CurrentSpeed += Acceleration;
	} else if (TargetSpeed < CurrentSpeed - Acceleration)
	{
		CurrentSpeed -= Acceleration;
	}
}

static void UpdateSpeed()
{
	TIM_OC_InitTypeDef sConfigOC;

	sConfigOC.OCMode = TIM_OCMODE_PWM1;
	sConfigOC.Pulse = CurrentSpeed;
	sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
	sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;

	HAL_TIM_PWM_ConfigChannel(Timer, &sConfigOC, TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(Timer, TIM_CHANNEL_1);

	HAL_TIM_PWM_ConfigChannel(Timer, &sConfigOC, TIM_CHANNEL_2);
	HAL_TIM_PWM_Start(Timer, TIM_CHANNEL_2);
}

void CheckDirectionSettings()
{
	if (CurrentSpeed == 0U)
	{
		if (HAL_GPIO_ReadPin(POWER_I_IN))
		{
			HAL_GPIO_WritePin(MOTOR_OUT1, GPIO_PIN_SET);
			HAL_GPIO_WritePin(MOTOR_OUT2, GPIO_PIN_RESET);
		} else if (HAL_GPIO_ReadPin(POWER_I_IN))
		{
			HAL_GPIO_WritePin(MOTOR_OUT1, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(MOTOR_OUT2, GPIO_PIN_SET);
		}
	}
}
