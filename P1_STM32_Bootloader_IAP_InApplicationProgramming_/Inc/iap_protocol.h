#ifndef INC_IAP_PROTOCOL_H_
#define INC_IAP_PROTOCOL_H_

#include <stdint.h>
#include <stdio.h>
#include "stm32f4xx.h"
#include "uart.h"
#include "bsp.h"

#define IAP_PROTOCOL_VERSION	0x02

#define DEBUG_UART	USART1
#define IAP_UART	USART2

#define DEFAULT_APP_ADDRESS			0x08008000

#define EMPTY_MEM 0xFFFFFFFF

typedef void(*func_ptr)(void);

typedef enum
{
	ACK_RESP 	= 0x6D,
	NACK_RESP 	= 0X9E
}ack_type;

typedef enum
{
	CMD_RESP_LEN 		= 0x01,
	CID_CMD_RESP_LEN 	= 0x02,
	HELP_CMD_RESP_LEN	= 0x08
}resp_len;

typedef enum
{
	VALID_ADDR 		= 0x00,
	INVALID_ADDR 	= 0x01
}addr_type;

typedef enum
{
	SECT_ERASE  = 0x01,		// Command to erase a sector of flash memory.
	SECT_WRITE	= 0x02		// Command to write data to a sector of flash memory.

}btl_cmd;

void iap_uart_write_data(uint8_t *p_buff, uint32_t len);
void iap_send_ack(uint32_t expected_bytes_len);
void iap_send_nack(void);

void handle_sect_erase_cmd(uint8_t *pbuff);
void handle_sect_write_cmd(uint8_t *pbuff);


#endif
