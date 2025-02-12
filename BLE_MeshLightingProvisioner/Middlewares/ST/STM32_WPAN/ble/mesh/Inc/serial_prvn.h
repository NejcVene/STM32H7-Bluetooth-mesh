/**
******************************************************************************
* @file    serial_prvn.h
* @author  BLE Mesh Team
* @brief   Header file for the serial interface file 
******************************************************************************
* @attention
*
* Copyright (c) 2018-2021 STMicroelectronics.
* All rights reserved.
*
* This software is licensed under terms that can be found in the LICENSE file
* in the root directory of this software component.
* If no LICENSE file comes with this software, it is provided AS-IS.
*
******************************************************************************
*/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __SERIAL_PRVN_H
#define __SERIAL_PRVN_H

/* Includes ------------------------------------------------------------------*/
#include "types.h"

/* Exported macro ------------------------------------------------------------*/
#define COMMAND_OFFSET      5
/* Exported variables  ------------------------------------------------------- */
/* Exported Functions Prototypes ---------------------------------------------*/
void SerialPrvn_Process(char *rcvdStringBuff, uint16_t rcvdStringSize, uint8_t *resultBuffer, int *cmdResposneElsewhere);
void SerialPrvn_ProvisioningStatusUpdateCb(uint8_t flagPrvningInProcess, MOBLEUINT16 nodeAddress);
neighbor_params_t *getNeighborTable(void);
#endif /* __SERIAL_PRVN_H */



