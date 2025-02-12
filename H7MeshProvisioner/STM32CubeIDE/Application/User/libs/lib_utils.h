/*
 * lib_utils.h
 *
 *  Created on: Jan 21, 2025
 *      Author: Nejc
 */

#ifndef APPLICATION_USER_LIBS_LIB_UTILS_H_
#define APPLICATION_USER_LIBS_LIB_UTILS_H_

#include <stdint.h>
#include "main.h"
#include "device_settings.h"

#ifdef _MASTER
extern UART_HandleTypeDef huart3;
#define UART_DEBUG &huart3
#else
extern UART_HandleTypeDef huart1;
#define UART_DEBUG &huart1
#endif

typedef enum {
	PRINT_CHAR = 0,
	PRINT_BINARY
} DEBUG_PRINT_TYPE;

void debugMessage(char *message, uint16_t payloadLenght, DEBUG_PRINT_TYPE printType);

#endif /* APPLICATION_USER_LIBS_LIB_UTILS_H_ */
