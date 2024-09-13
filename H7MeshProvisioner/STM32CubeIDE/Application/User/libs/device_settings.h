/*
 * device_settings.h
 *
 *  Created on: Sep 1, 2024
 *      Author: Nejc
 */

#ifndef INC_DEVICE_SETTINGS_H_
#define INC_DEVICE_SETTINGS_H_

#define MASTER
#define _DEBUG

#if defined(MASTER) && !defined(SLAVE)
#define _MASTER
#elif !defined(MASTER) && defined(SLAVE)
#define _SLAVE
#else
#error Conflicting master or slave device setting
#endif

#ifdef MASTER
// edit these only if master device
#define COMM_CS_PORT_MASTER		GPIOE	 // port and pin used for output chip select (NSS) line in SPI (master)
#define COMM_CS_PIN_MASTER		GPIO_PIN_3

#define COMM_HS_PORT_MASTER		GPIOH	// port and pin used for handshake pin (input)
#define COMM_HS_PIN_MASTER		GPIO_PIN_15

#define COMM_ERR_PORT_MASTER	GPIOH
#define COMM_ERR_PIN_MASTER		GPIO_PIN_15
#else
// edit these only if slave device
#define COMM_CS_PORT_SLAVE		GPIOA	// port and pin used for input chip select (NSS) line in SPI (slave)
#define COMM_CS_PIN_SLAVE		GPIO_PIN_5

#define COMM_HS_PORT_SLAVE		GPIOC	// port and pin used for handshake pin (output)
#define COMM_HS_PIN_SLAVE		GPIO_PIN_0

#define COMM_ERR_PORT_SLAVE		GPIOC
#define COMM_ERR_PIN_SLAVE		GPIO_PIN_0 // CHANGE!
#endif

#endif /* INC_DEVICE_SETTINGS_H_ */
