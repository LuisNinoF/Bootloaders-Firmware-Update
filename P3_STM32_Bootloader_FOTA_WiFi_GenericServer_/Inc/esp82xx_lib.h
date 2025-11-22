#ifndef __ESP82XX_LIB_H
#define __ESP82XX_LIB_H

#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "esp82xx_driver.h"
#include "circular_buffer.h"
#include "timebase.h"

void esp8266_init(char *ssid, char *password);
void esp82xx_get_version_file(char *dest_buffer);
void esp82xx_get_firmware(char *dest_buffer, const char *firmware_file);

#define esp82xx_port		SLAVE_DEV_PORT
#define debug_port			DEBUG_PORT

#endif
