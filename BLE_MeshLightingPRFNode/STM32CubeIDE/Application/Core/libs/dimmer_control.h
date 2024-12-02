/*
 * dimmer_control.h
 *
 *  Created on: Dec 2, 2024
 *      Author: Nejc
 */

#ifndef APPLICATION_CORE_LIBS_DIMMER_CONTROL_H_
#define APPLICATION_CORE_LIBS_DIMMER_CONTROL_H_

#include "main.h"

#define STEP_SIZE	1000 // should be cca. one percent

typedef struct {
	TIM_HandleTypeDef *timer;
	uint32_t channel;
} DC_Dimmer_t;

DC_Dimmer_t *DC_CreateControl(TIM_HandleTypeDef *timer, uint32_t channel);
void DC_FreeControl(DC_Dimmer_t *dimmer);
int16_t DC_MapSliderToGenericLevelRange(int16_t sliderValue);
uint16_t DC_CalcNewDutyCycle(DC_Dimmer_t *dimmer, int16_t levelSet);
void DC_SetTimerCCR(DC_Dimmer_t *dimmer, uint16_t dutyCycle);

#endif /* APPLICATION_CORE_LIBS_DIMMER_CONTROL_H_ */
