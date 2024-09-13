/*
 * hw_spi.h
 *
 *  Created on: Jul 17, 2024
 *      Author: Nejc
 */

#ifndef APPLICATION_CORE_LIBS_HW_SPI_H_
#define APPLICATION_CORE_LIBS_HW_SPI_H_

#include "main.h"
#include "stm32_seq.h"
#include "serial_if.h"
#include "appli_mesh.h"
#include "communication_ex.h"

void CommEx_Protocol_Init(void);
void CommEx_Protocol_Task(void);

#endif /* APPLICATION_CORE_LIBS_HW_SPI_H_ */
