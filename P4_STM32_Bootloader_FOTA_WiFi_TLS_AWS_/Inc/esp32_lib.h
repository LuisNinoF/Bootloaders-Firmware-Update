#ifndef _ESP32_LIB_H
#define _ESP32_LIB_H

#include <string.h>
#include <stdio.h>
#include <stdint.h>
#include "circular_buffer.h"
#include "timebase.h"

#define debug_port	DEBUG_PORT		// being DEBUG_PORT = 0
#define esp32_port 	SLAVE_DEV_PORT 	// being SLAVE_DEV_PORT = 1

void esp32_init(char *ssid, char *password);


#endif
