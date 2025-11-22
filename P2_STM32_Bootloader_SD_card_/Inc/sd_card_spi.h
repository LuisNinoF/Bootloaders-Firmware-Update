#ifndef __SD_CARD_SPI_H__
#define __SD_CARD_SPI_H__


#include "stm32f4xx.h"
#include "spi.h"

void cs_pin_set(void);
void cs_pin_reset(void);
void sd_cs_pin_init(void);
void sd_card_spi_init(void);

#endif
