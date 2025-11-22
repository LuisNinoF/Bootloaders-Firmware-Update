/*
 * *** Purpose:
 * Demonstrate a bootloader to update the firmware using In Application Programming (IAP) via UART Protocol.
 * Enables remote firmware updates without external programming tools.
 *
 * *** How it works
 * The user can enter the bootloader mode by pressing the the board button during restart.
 * In the bootloader mode, the device (Microcontroller) receives commands from the host (PC) via UART
 * to either erase a given sector in memory or to write a new firmware to memory
 * from a binary file stored in the host.
 *
 * *** Copyrights:
 * Created by Luis Nino
 * Last revision 2025.11.17
 */

/*Include drivers*/
#include <stdint.h>
#include "stm32f4xx.h"
#include "bsp.h"
#include "fpu.h"
#include "timebase.h"
#include "uart.h"
#include "adc.h"
#include "iap_protocol.h"

/*Macros and global variables*/
#define RX_PCK_LEN	300
uint8_t RX_PCK_BUFF[RX_PCK_LEN];

/*Declare function prototypes*/
static void process_host_cmds(void);
void jump_to_app(uint32_t addr_value);

/*Run main*/
int main(void)
{
	/*Initialize drivers*/
	led_init();
	button_init();
	fpu_enable();
	timebase_init();
	system_uart_init();		// to communicate HOST - TARGET for IAP (TX and RX)
	debug_uart_init();		// to communicate to PC for debugging purposes (TX only required)
	pa1_adc_init();
	start_conversion();

	/*If the button of the boards is pressed at restart, go to bootloader mode and read the host commands*/
	if(get_btn_state())
	{
		printf("Button pressed, going to Bootloader mode\n\r");
		process_host_cmds();
	}
	/*otherwise, run default application, if it exists at DEFAULT_APP_ADDRESS*/
	else
	{
		printf("Button not pressed, execute Application\n\r");
		jump_to_app(DEFAULT_APP_ADDRESS);
	}
}

/*Function to process the host commands*/
static void process_host_cmds(void)
{
	uint8_t expected_num_of_bytes = 0;
	while(1)
	{
		/*Reset RX Buffer */
		for (int i = 0; i < RX_PCK_LEN; i++)
		{
			RX_PCK_BUFF[i] = 0;
		}

		/*Receive first bytes. They contain the number of expected bytes */
		uart_receive_buff(IAP_UART, RX_PCK_BUFF, 1);
		expected_num_of_bytes = RX_PCK_BUFF[0];

		/*Tell the UART to receive the expected number of bytes */
		uart_receive_buff(IAP_UART, &RX_PCK_BUFF[1], expected_num_of_bytes);

		/*Go to to either erase or write handle depending on the host command*/
		if (RX_PCK_BUFF[1] == SECT_ERASE)
		{
			handle_sect_erase_cmd(RX_PCK_BUFF);
		}
		else if (RX_PCK_BUFF[1] == SECT_WRITE)
		{
			handle_sect_write_cmd(RX_PCK_BUFF);
		}
		else
		{
			printf("Wrong command \n\r");
		}
	}
}


/*Function to jump to a different part of the memory, where the firmware is stored*/
void jump_to_app(uint32_t addr_value)
{
	uint32_t app_start_address;
	func_ptr jump_to_app;

	if((*(uint32_t *)addr_value) != EMPTY_MEM)
	{
		printf("Jumping to indicated address \n\r");
		app_start_address = *(uint32_t *)(addr_value +4);
		jump_to_app = (func_ptr) app_start_address;
		__set_MSP(*(uint32_t *) addr_value);

		jump_to_app();
	}
	else
	{
		printf("No application found at location \n\r");
	}
}

