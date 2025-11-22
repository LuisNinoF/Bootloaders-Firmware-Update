#ifndef __UART_H__
#define __UART_H__

#include "stm32f4xx.h"

void system_uart_init(void);
void debug_uart_init(void);
char sys_uart_read(void);
void uart_receive_buff(USART_TypeDef *UARTx, uint8_t *data_buff, uint16_t len);
void uart_transmit_buff(USART_TypeDef *UARTx, uint8_t *data_buff, uint16_t len);


#endif
