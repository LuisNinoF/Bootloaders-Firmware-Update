#include <stdint.h>
#include "uart.h"

#define GPIOAEN		(1U<<0)
#define UART2EN		(1U<<17)
#define UART1EN		(1U<<4)

#define SYS_UART_BAUDRATE		115200
#define DBG_UART_BAUDRATE		115200
#define SYS_FREQ				16000000
#define APB1_CLK				SYS_FREQ
#define APB2_CLK				SYS_FREQ
#define CR1_RE					(1U<<2)
#define CR1_TE					(1U<<3)
#define CR1_UE					(1U<<13)
#define SR_TXE					(1U<<7)
#define SR_RXNE					(1U<<5)


static void uart_set_baudrate(uint32_t periph_clk,uint32_t baudrate);
static void sys_uart_write(int ch);
static void debug_uart_write(int ch);
static uint16_t compute_uart_bd(uint32_t periph_clk,uint32_t baudrate);

int __io_putchar(int ch)
{
	debug_uart_write(ch);
	//sys_uart_write(ch);
	return ch;
}

void system_uart_init(void)
{
	/*Enable clock access to GPIOA*/
	RCC->AHB1ENR |= GPIOAEN;

	/*Set the mode of PA2 to alternate function mode*/
	GPIOA->MODER &=~(1U<<4);
	GPIOA->MODER |=(1U<<5);

	/*Set the mode of PA3 to alternate function mode*/
	GPIOA->MODER &=~(1U<<6);
	GPIOA->MODER |=(1U<<7);

	/*Set alternate function type to AF7(UART2_TX) for PA2*/
	GPIOA->AFR[0] |=(1U<<8);
	GPIOA->AFR[0] |=(1U<<9);
	GPIOA->AFR[0] |=(1U<<10);
	GPIOA->AFR[0] &=~(1U<<11);

	/*Set alternate function type to AF7(UART2_RX) for PA3*/
	GPIOA->AFR[0] |=(1U<<12);
	GPIOA->AFR[0] |=(1U<<13);
	GPIOA->AFR[0] |=(1U<<14);
	GPIOA->AFR[0] &=~(1U<<15);

	/*Enable clock access to UART2*/
     RCC->APB1ENR |=	UART2EN;

  	/*Disable UART Module before configuring it*/
     USART2->CR1 &= ~CR1_UE;

	/*Configure uart baudrate*/
     uart_set_baudrate(APB1_CLK,SYS_UART_BAUDRATE);

	/*Configure transfer direction*/
     USART2->CR1 = (CR1_TE | CR1_RE);

	/*Enable UART Module*/
     USART2->CR1 |= CR1_UE;
}

/*
 * functions to set and compute uart baudrate
 */
static uint16_t compute_uart_bd(uint32_t periph_clk,uint32_t baudrate)
{
	return((periph_clk + (baudrate/2U))/baudrate);
}

static void uart_set_baudrate(uint32_t periph_clk,uint32_t baudrate)
{
	USART2->BRR = compute_uart_bd(periph_clk,baudrate);
}

/*
 * functions to transmit and receive characters
 */
char sys_uart_read(void)
{
	/*Make sure read data register is not empty and ready to be read*/
	while(!(USART2->SR & SR_RXNE)){}

	/*Return the content of the data register*/
	return USART2->DR;
}

static void sys_uart_write(int ch)
{
	/*Make sure transmit data register is empty*/
	while(!(USART2->SR & SR_TXE)){}

	/*Write to transmit data register*/
	USART2->DR =(ch & 0xFF);
}

/*
 * functions to transmit and receive full messages
 */
void uart_transmit_buff(USART_TypeDef *UARTx, uint8_t *data_buff, uint16_t len)
{
	while(len > 0U)
	{
		while(!(UARTx->SR & SR_TXE)){}
		UARTx->DR = (uint8_t)(*data_buff & 0xFF);
		data_buff++;
		len--;
	}
}

void uart_receive_buff(USART_TypeDef *UARTx, uint8_t *data_buff, uint16_t len)
{
	while(len > 0U)
	{
		while(!(UARTx->SR & SR_RXNE)){}
		*data_buff = (uint8_t)(UARTx->DR &(uint8_t)0xFF);
		data_buff++;
		len--;
	}
}

/*
 * UART for debugging purposes
 * Using USART 1,
 *
 * USART 1 is connected to TX -> PA9 and RX -> PA10 (See alternate function mapping in datasheet)
 * In the board, PA9 -> D8 and PA10 -> D2 (See extension connectors in user manual)
 *
 * USART 1 is connected to APB2 bus
 *
 * Because we are using TX to send data from MC to PC, use PA9 -> D8 in MC to communicate to COM in PC
 *
 */

void debug_uart_init(void)
{
	/*Enable clock access to GPIOA*/
	RCC->AHB1ENR |= GPIOAEN;

	/*Set the mode of PA9 to alternate function mode*/
	GPIOA->MODER &=~(1U<<18);
	GPIOA->MODER |=(1U<<19);

	/*Set the mode of PA10 to alternate function mode*/
	GPIOA->MODER &=~(1U<<20);
	GPIOA->MODER |=(1U<<21);

	/*Set alternate function type to AF7(UART1_TX) for PA9*/
	GPIOA->AFR[1] |=(1U<<4);
	GPIOA->AFR[1] |=(1U<<5);
	GPIOA->AFR[1] |=(1U<<6);
	GPIOA->AFR[1] &=~(1U<<7);

	/*Set alternate function type to AF7(UART1_RX) for PA10*/
	GPIOA->AFR[1] |=(1U<<8);
	GPIOA->AFR[1] |=(1U<<9);
	GPIOA->AFR[1] |=(1U<<10);
	GPIOA->AFR[1] &=~(1U<<11);

	/*Enable clock access to UART1*/
     RCC->APB2ENR |=	UART1EN;

 	/*Disable UART Module before configuring it*/
     USART1->CR1 &= ~CR1_UE;

	/*Configure uart baudrate*/
     USART1->BRR = compute_uart_bd(APB2_CLK,DBG_UART_BAUDRATE);

	/*Configure transfer direction*/
     USART1->CR1 = (CR1_TE | CR1_RE);

	/*Enable UART Module*/
     USART1->CR1 |= CR1_UE;
}

static void debug_uart_write(int ch)
{
	/*Make sure transmit data register is empty*/
	while(!(USART1->SR & SR_TXE)){}

	/*Write to transmit data register*/
	USART1->DR =(ch & 0xFF);
}
