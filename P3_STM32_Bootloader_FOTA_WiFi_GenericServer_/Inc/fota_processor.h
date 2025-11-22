#ifndef __FOTA_PROCESSOR_H
#define __FOTA_PROCESSOR_H
#include <stdio.h>
#include <stddef.h>
#include "esp82xx_lib.h"
#include "flash_driver.h"


#define  MAX_FIRMWARE_SIZE		10500
#define  TEMP_RX_BUFF_SZ		11000
void firmware_update(void);
void firmware_parse(char *dst, const char *src, int size);

void jump_to_app(uint32_t address);


#define EMPTY_MEM		0xFFFFFFFF
typedef void (*func_ptr)(void);


#endif
