#ifndef __CIRCULAR_BUFFER_H
#define __CIRCULAR_BUFFER_H

#include "uart.h"


#define UART_BUFFER_SIZE 	11000
#define INIT_VAL			0

typedef enum
{
	DEBUG_PORT = 0,
	SLAVE_DEV_PORT

}portType;

typedef struct
{
		unsigned char buffer[UART_BUFFER_SIZE];
		__IO uint32_t head;
		__IO uint32_t tail;

}circular_buffer;

void buffer_send_string(const char *s,portType uart);
void circular_buffer_init(void);

void buffer_clear(portType uart);
int buffer_peek(portType uart);
int buffer_read(portType uart);
void buffer_write(unsigned char c, portType uart);
int is_data(portType uart);
int is_response(char *str);
void get_strs(uint8_t num_of_chars,char *dest_buffer);

int8_t copy_up_to_string(char * str, char * dest_buffer);

#endif



