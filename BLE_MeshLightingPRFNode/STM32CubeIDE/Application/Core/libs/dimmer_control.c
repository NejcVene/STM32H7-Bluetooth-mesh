/*
 * dimmer_control.c
 *
 *  Created on: Dec 2, 2024
 *      Author: Nejc
 */

#include "dimmer_control.h"

DC_Dimmer_t *DC_CreateControl(TIM_HandleTypeDef *timer, uint32_t channel) {

	DC_Dimmer_t *tmp;

	if (!timer || !(tmp = (DC_Dimmer_t *) malloc(sizeof(DC_Dimmer_t)))) {
		return NULL;
	}
	tmp->timer = timer;
	tmp->channel = channel;

	return tmp;

}

void DC_FreeControl(DC_Dimmer_t *dimmer) {

	if (dimmer) {
		free(dimmer);
	}

}

int16_t DC_MapSliderToGenericLevelRange(int16_t sliderValue) {

	return (int16_t) ((65535 * sliderValue) / 100) - 32768;

}

uint16_t DC_CalcNewDutyCycle(DC_Dimmer_t *dimmer, int16_t levelSet) {

	uint8_t dutyPercent = 0;
	uint16_t newDutyCycle = 0;

	dutyPercent = (levelSet + 32768) * 100 / 65535;
	newDutyCycle = (dutyPercent * (dimmer->timer->Init.Period + 1)) / 100;

	return newDutyCycle;

}

void DC_SetTimerCCR(DC_Dimmer_t *dimmer, uint16_t dutyCycle) {

	__HAL_TIM_SET_COMPARE(dimmer->timer, dimmer->channel, dutyCycle);

}
