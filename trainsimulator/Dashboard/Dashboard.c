/*
 * DashboardManager.c
 *
 *  Created on: Dec 30, 2017
 *      Author: ojeju
 */

#include "pins_config.h"
#include "cmsis_os.h"

static xSemaphoreHandle GreenButtonSemaphore;
static xSemaphoreHandle RedButtonSemaphore;
static xSemaphoreHandle BlackButtonSemaphore;

void WatchdogTask()
{
	for (;;)
	{
		osDelay(10 * 1000);
		HAL_GPIO_WritePin(LED_RED_OUT, GPIO_PIN_SET);
		const BaseType_t BUZZER_TIMEOUT = (5 * 1000 / portTICK_RATE_MS);
		if (pdTRUE == xSemaphoreTake(RedButtonSemaphore, BUZZER_TIMEOUT))
		{
			HAL_GPIO_WritePin(LED_RED_OUT, GPIO_PIN_RESET);
		} else
		{
			HAL_GPIO_WritePin(BUZZER_OUT, GPIO_PIN_SET);
			const BaseType_t STOP_MOTOR_TIMEOUT = (5 * 1000 / portTICK_RATE_MS);
			if (pdFALSE
					== xSemaphoreTake(RedButtonSemaphore, STOP_MOTOR_TIMEOUT))
			{
				//ToDo Stop Motor
				while (pdFALSE == xSemaphoreTake(RedButtonSemaphore, BUZZER_TIMEOUT));
			}
			HAL_GPIO_WritePin(BUZZER_OUT, GPIO_PIN_RESET);
		}
	}
}

void RelayTask()
{
	for (;;)
	{
		if (pdTRUE == xSemaphoreTake(GreenButtonSemaphore, (TickType_t ) 0))
		{
			HAL_GPIO_WritePin(RELAY_OUT, GPIO_PIN_SET);
			HAL_GPIO_WritePin(LED_BLUE_OUT, GPIO_PIN_SET);
			HAL_GPIO_WritePin(LED_GREEN_OUT, GPIO_PIN_SET);
			osDelay(4 * 1000);
			HAL_GPIO_WritePin(RELAY_OUT, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(LED_BLUE_OUT, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(LED_GREEN_OUT, GPIO_PIN_RESET);
			osDelay(2 * 1000);
		}
	}
}

void BuzzerTask()
{
	for (;;)
	{
		if (pdTRUE == xSemaphoreTake(BlackButtonSemaphore, (TickType_t )0))
		{
			for (int i = 0; i < 4; i++)
			{
				HAL_GPIO_WritePin(BUZZER_OUT, GPIO_PIN_SET);
				osDelay(100);
				HAL_GPIO_WritePin(BUZZER_OUT, GPIO_PIN_RESET);
				osDelay(100);
			}
		}
	}
}

const struct BUTTON_HANDLER_T
{
	GPIO_TypeDef* GPIOx;
	uint16_t GPIO_Pin;
	void (*Action)(void);
	osThreadId task;
} Buttons[] = { { BUTTON_GREEN_IN, RelayTask }, { BUTTON_BLACK_IN, BuzzerTask },
		{ BUTTON_RED_IN, WatchdogTask }, };

#define NUM_OF_BUTTONS sizeof(Buttons)/sizeof(Buttons[0])

extern void Dashboard_Init()
{
	int button_index;

	GreenButtonSemaphore = xSemaphoreCreateBinary();
	RedButtonSemaphore = xSemaphoreCreateBinary();
	BlackButtonSemaphore = xSemaphoreCreateBinary();

	for (button_index = 0; button_index < NUM_OF_BUTTONS; button_index++)
	{
		struct BUTTON_HANDLER_T *button = &Buttons[button_index];
		osThreadDef(buttontask, button->Action, osPriorityNormal, 0, 128);
		button->task = osThreadCreate(osThread(buttontask), NULL);
	}
}

extern void GreenButtonPressed()
{
	const BaseType_t sHigherPriorityTaskWoken = pdFALSE;
	xSemaphoreGiveFromISR(GreenButtonSemaphore, &sHigherPriorityTaskWoken);
}

extern void RedButtonPressed()
{
	const BaseType_t sHigherPriorityTaskWoken = pdFALSE;
	xSemaphoreGiveFromISR(RedButtonSemaphore, &sHigherPriorityTaskWoken);
}

extern void BlackButtonPressed()
{
	const BaseType_t sHigherPriorityTaskWoken = pdFALSE;
	xSemaphoreGiveFromISR(BlackButtonSemaphore, &sHigherPriorityTaskWoken);
}
