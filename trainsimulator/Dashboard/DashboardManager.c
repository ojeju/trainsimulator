/*
 * DashboardManager.c
 *
 *  Created on: Dec 30, 2017
 *      Author: ojeju
 */

#include "pins_config.h"
#include "cmsis_os.h"

void TurnOnRelay()
{
	vTaskSuspend(NULL);
	for (;;)
	{
		HAL_GPIO_WritePin(RELAY_OUT, GPIO_PIN_SET);
		osDelay(2 * 1000);
		HAL_GPIO_WritePin(RELAY_OUT, GPIO_PIN_RESET);
		osDelay(2 * 1000);

		vTaskSuspend(NULL);
	}
}

struct BUTTON_HANDLER_T
{
	GPIO_TypeDef* GPIOx;
	uint16_t GPIO_Pin;
	void (*Action)(void);
	osThreadId task;
} Buttons[] = { { BUTTON_GREEN_IN, TurnOnRelay }, };

#define NUM_OF_BUTTONS sizeof(Buttons)/sizeof(Buttons[0])

static void CheckButtonState(void);

extern void Init_DashboardManager()
{
	int button_index;
	for (button_index = 0; button_index < NUM_OF_BUTTONS; button_index++)
	{
		struct BUTTON_HANDLER_T *button = &Buttons[button_index];
		osThreadDef(buttontask, button->Action, osPriorityNormal, 0, 128);
		button->task = osThreadCreate(osThread(buttontask), NULL);
	}
}

extern void DashboardManager()
{
	CheckButtonState();
}

static void CheckButtonState()
{
	int button_index;
	for (button_index = 0; button_index < NUM_OF_BUTTONS; button_index++)
	{
		struct BUTTON_HANDLER_T *button = &Buttons[button_index];
		if (HAL_GPIO_ReadPin(button->GPIOx, button->GPIO_Pin))
		{
			vTaskResume(button->task);
		}
	}

}
