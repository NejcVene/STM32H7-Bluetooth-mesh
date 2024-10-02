/*
 * device_settings.h
 *
 *  Created on: Sep 1, 2024
 *      Author: Nejc
 */

#ifndef INC_DEVICE_SETTINGS_H_
#define INC_DEVICE_SETTINGS_H_

#define NODE
#define _DEBUG

#define NC_GENERIC_ON_OFF_MODEL				(1 << 0) // bit 0
#define NC_GENERIC_LEVEL_MODEL				(1 << 1) // bit 1
#define NC_SENSOR_MODEL						(1 << 2) // bit 2
#define NC_LIGH_LIGHTNESS_MODEL				(1 << 3) // bit 3
#define NC_LIGH_LIGHTNESS_SETUP_MODEL		(1 << 4) // bit 4
#define NC_UNDEFINED_MODEL_1				(1 << 5) // bit 5
#define NC_UNDEFINED_MODEL_2				(1 << 6) // bit 6
#define NC_UNDEFINED_MODEL_3				(1 << 7) // bit 7

#define NC_RELAY_FEATURE					(1 << 0) // bit 0
#define NC_PROXY_FEATURE					(1 << 1) // bit 1
#define NC_FRIEND_FEATURE					(1 << 2) // bit 2
#define NC_EMBEDDED_PROVISIONER_FEATURE		(1 << 3) // bit 3
#define NC_UNDEFINED_FEATURE_1				(1 << 4) // bit 4
#define NC_UNDEFINED_FEATURE_2				(1 << 5) // bit 5
#define NC_UNDEFINED_FEATURE_3				(1 << 6) // bit 6
#define NC_UNDEFINED_FEATURE_4				(1 << 7) // bit 7

#if defined(MASTER) && !defined(SLAVE) && !defined(NODE)
#define _MASTER
#elif !defined(MASTER) && defined(SLAVE) && !defined(NODE)
#define _SLAVE
#elif !defined(MASTER) && !defined(SLAVE) && defined(NODE)
#define _NODE
#else
#error "Conflicting master or slave device setting"
#endif

#if defined(_MASTER)
// edit these only if master device
#define COMM_CS_PORT_MASTER		GPIOE	 // port and pin used for output chip select (NSS) line in SPI (master)
#define COMM_CS_PIN_MASTER		GPIO_PIN_3

#define COMM_HS_PORT_MASTER		GPIOH	// port and pin used for handshake pin (input)
#define COMM_HS_PIN_MASTER		GPIO_PIN_15

#define COMM_ERR_PORT_MASTER	GPIOH
#define COMM_ERR_PIN_MASTER		GPIO_PIN_15
#elif defined(_SLAVE)
// edit these only if slave device
#define COMM_CS_PORT_SLAVE		GPIOA	// port and pin used for input chip select (NSS) line in SPI (slave)
#define COMM_CS_PIN_SLAVE		GPIO_PIN_5

#define COMM_HS_PORT_SLAVE		GPIOC	// port and pin used for handshake pin (output)
#define COMM_HS_PIN_SLAVE		GPIO_PIN_0

#define COMM_ERR_PORT_SLAVE		GPIOC
#define COMM_ERR_PIN_SLAVE		GPIO_PIN_0 // CHANGE!
#elif defined(_NODE)
#define NC_NODE_CONFIG_FEATURES (NC_RELAY_FEATURE | NC_PROXY_FEATURE | NC_FRIEND_FEATURE)
#define NC_NODE_CONFIG_MODELS	(NC_GENERIC_ON_OFF_MODEL | NC_GENERIC_LEVEL_MODEL | NC_LIGH_LIGHTNESS_MODEL)
#endif

#endif /* INC_DEVICE_SETTINGS_H_ */
