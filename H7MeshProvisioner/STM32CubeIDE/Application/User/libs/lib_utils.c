/*
 * lib_utils.c
 *
 *  Created on: Jan 21, 2025
 *      Author: Nejc
 */

#include <string.h>
#include <ctype.h>
#include "lib_utils.h"

/**
  * @brief  Print message to terminal.
  * @param  message			Pointer to the message which will be printed.
  * @param	payloadLength	Length of the message to be printed.
  * @param	printType		Message contains either characters or binary data.
  * @retval	None
  */
void debugMessage(char *message, uint16_t payloadLength, DEBUG_PRINT_TYPE printType) {

    char tmp[5];
    char output[256] = {0};
    int index = 0;
    int isBinary = 0;

    if (printType) {
    	// printing as binary
    	for (int i = 0; i<payloadLength; i++) {
    		if (!isBinary) {
    			if (message[i] == ':') {
    				// add ':' to the output and switch to binary mode
    				if (index < sizeof(output) - 1) {
    					output[index++] = ':';
    				}
    				isBinary = 1; // switch to binary processing
    				} else if (isprint((uint8_t) message[i])) {
    					// append printable character before ':' in string mode
    					if (index < sizeof(output) - 1) {
    						output[index++] = message[i];
    					}
    				}
    		} else {
    			// binary mode; always output as hex
    			snprintf(tmp, sizeof(tmp), "\\x%02X", (uint8_t)message[i]);
    			size_t len = strlen(tmp);
    			if (index + len < sizeof(output)) {
    				strcpy(&output[index], tmp);
    				index += len;
    			} else {
    				break; // prevent overflow
    			}
    		}
    	}
    } else {
    	// printing as string, which is passed in
    	memcpy(output, message, payloadLength);
    	index = payloadLength;
    }
    // always append \r\n to the output buffer at the end
    if (index + 2 < sizeof(output)) {
        output[index++] = '\r';
        output[index++] = '\n';
    }
    // terminate the output buffer
    if (index < sizeof(output)) {
        output[index] = '\0';
    } else {
        output[sizeof(output) - 1] = '\0';
    }
    // print to terminal defined by UART_DEBUG
    HAL_UART_Transmit(UART_DEBUG, (uint8_t *) output, index, 3000);

}
