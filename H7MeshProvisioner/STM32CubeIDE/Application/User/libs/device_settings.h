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
// #define _SIMULATE_CONFIG_NODE

#if defined(MASTER) && !defined(SLAVE)
#define _MASTER
#elif !defined(MASTER) && defined(SLAVE)
#define _SLAVE
#else
#error "Conflicting master or slave device setting"
#endif

#ifdef MASTER
// edit these only if master device
#define COMM_CS_PORT_MASTER		GPIOE	 // port and pin used for output chip select (NSS) line in SPI (master)
#define COMM_CS_PIN_MASTER		GPIO_PIN_3

#define COMM_HS_PORT_MASTER		GPIOH	// port and pin used for handshake pin (input)
#define COMM_HS_PIN_MASTER		GPIO_PIN_15

#define COMM_NRST_PORT_MASTER	GPIOB
#define COMM_NRST_PIN_MASTER	GPIO_PIN_15

#define COMM_ERROR_PORT_MASTER	GPIOI
#define COMM_ERROR_PIN_MASTER	GPIO_PIN_2

#else
// edit these only if slave device
#define COMM_CS_PORT_SLAVE		GPIOA	// port and pin used for input chip select (NSS) line in SPI (slave)
#define COMM_CS_PIN_SLAVE		GPIO_PIN_5

#define COMM_HS_PORT_SLAVE		GPIOC	// port and pin used for handshake pin (output)
#define COMM_HS_PIN_SLAVE		GPIO_PIN_0

#define COMM_NRST_PORT_SLAVE	GPIOC
#define COMM_NRST_PIN_SLAVE		GPIO_PIN_0 // CHANGE!
#endif

#define COMM_EX_VER	"1.0"

/*
 * Each model is defined by exactly one bit:
 * 	- 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 -> no models defined
 * 	- 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 1 -> generic on/off model
 * 	- 0 0 0 0 0 0 0 0 0 0 0 0 0 0 1 0 -> generic level model
 * 	- 0 0 0 0 0 0 0 0 0 0 0 0 0 1 0 0 -> sensor model,
 * 	- 0 0 0 0 0 0 0 0 0 0 0 0 1 0 0 0 -> sensor setup model
 * 	- 0 0 0 0 0 0 0 0 0 0 0 1 0 0 0 0 -> light lightness model
 * 	- 0 0 0 0 0 0 0 0 0 0 1 0 0 0 0 0 -> light lightness setup model,
 * 	- 0 0 0 0 0 0 0 0 0 1 0 0 0 0 0 0 -> generic power on/off model
 * 	- 0 0 0 0 0 0 0 0 1 0 0 0 0 0 0 0 -> generic power on/off setup model
 * 	- 0 0 0 0 0 0 0 1 0 0 0 0 0 0 0 0 -> vendor model 1
 * 	- 0 0 0 0 0 0 1 0 0 0 0 0 0 0 0 0 -> undefined
 * 	As per limitation of the STM BLE Mesh library, the maximum
 * 	number of models on a node, which are active at the same time
 * 	is 5 (setup models and their counterpart count as ONE WHOLE model).
 */

#define NC_GENERIC_ON_OFF_MODEL				(1 << 0) // bit 0
#define NC_GENERIC_LEVEL_MODEL				(1 << 1) // bit 1
#define NC_SENSOR_MODEL						(1 << 2) // bit 2
#define NC_SENSOR_SETUP_MODEL				(1 << 3) // bit 2
#define NC_LIGH_LIGHTNESS_MODEL				(1 << 4) // bit 3
#define NC_LIGH_LIGHTNESS_SETUP_MODEL		(1 << 5) // bit 4
#define NC_GENERIC_POWER_ON_OFF_MODEL		(1 << 6) // bit 5
#define NC_GENERIC_POWER_SETUP_MODEL		(1 << 7) // bit 2
#define NC_VENDOR_MODEL_1					(1 << 8) // bit 6
#define NC_UNDEFINED_MODEL_1				(1 << 9) // bit 7

#define NC_RELAY_FEATURE					(1 << 0) // bit 0
#define NC_PROXY_FEATURE					(1 << 1) // bit 1
#define NC_FRIEND_FEATURE					(1 << 2) // bit 2
#define NC_EMBEDDED_PROVISIONER_FEATURE		(1 << 3) // bit 3
#define NC_UNDEFINED_FEATURE_1				(1 << 4) // bit 4
#define NC_UNDEFINED_FEATURE_2				(1 << 5) // bit 5
#define NC_UNDEFINED_FEATURE_3				(1 << 6) // bit 6
#define NC_UNDEFINED_FEATURE_4				(1 << 7) // bit 7

#endif /* INC_DEVICE_SETTINGS_H_ */
