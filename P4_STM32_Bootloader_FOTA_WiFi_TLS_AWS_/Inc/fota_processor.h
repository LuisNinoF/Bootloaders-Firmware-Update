#ifndef __FOTA_PROCESSOR_H
#define __FOTA_PROCESSOR_H

#include <stdio.h>
#include <string.h>

#include "esp32_client.h"
#include "flash_driver.h"


#define  MAX_FIRMWARE_SIZE		10500
#define  TEMP_RX_BUFF_SZ		11000


int firmware_parser(const char *src_buffer, size_t src_buffer_size, char *dest_buffer, size_t dest_buffer_size);


#define EMPTY_MEM		0xFFFFFFFF
typedef void (*func_ptr)(void);


#endif
