/*
 * *** Purpose:
 * Demonstrate a bootloader that updates STM32 firmware by downloading a binary file from an AWS S3 bucket over WIFI using HTTPS (TLS) and forwarding it over UART to the microcontroller.
 *
 * *** How it works:
 * A new firmware binary file is stored on a AWS S3 bucket.
 * The ESP32 connects to the WIFI network, downloads the firmware via HTTPS request secured with TLS, and sends it to the Microcontroller over UART.
 * The bootloader receives the binary file and programs it into internal flash memory.
 *
 * The user only needs to press the board button during reset to start the update.
 * The new application is executed after updating the firmware and restarting the device.
 * If the button is not pressed, the bootloader jumps to the existing application.
 *
 * *** Note of use:
 * You must update your Server address, FIRMWARE file name, and WIFI credentials in the code.
 *
 * * *** Copyrights:
 * Created by Luis Nino
 * Last revision 2025.11.20
 *
 */

// Include drivers
#include <stdint.h>
#include "stm32f4xx.h"

#include "bsp.h"
#include "fpu.h"
#include "timebase.h"
#include "uart.h"
#include "adc.h"
//#include "spi.h"
#include "circular_buffer.h"
#include "esp32_lib.h"
#include "fota_processor.h"

/*Define SERVER address*/
/*refer to "esp32_client.c"*/

/*Define firmware address and file name in the server*/
#define NEW_FIRMWARE_START_ADDRESS	0x08008000
#define FIRMWARE 					"/embedded-expert-fota-aws-bucket/new_firmware_fota_aws.bin"

/*Define WIFI credentials*/
#define _SSID		"YOUR-WIFI-CONNECTION"
#define _PASSWORD	"YOUR-WIFI-PASSWORD"

/*Declare global variables*/
char version_buff[50] = {0};
char firmware_buffer[MAX_FIRMWARE_SIZE] = {0};
char temp_rx_buffer[TEMP_RX_BUFF_SZ] = {0};

/*Declare function prototypes*/
void firmware_update(void);
void jump_to_app(uint32_t address);

/* Run Main*/
int main(void)
{
	/*Initialize drivers*/
	led_init();
	button_init();
	fpu_enable();
	timebase_init();
	debug_uart_init();
	esp_uart_init();
	pa1_adc_init();
	start_conversion();

	if(get_btn_state())
	{
		/*initialize esp32: set up and connect to Wifi network*/
		esp32_init(_SSID, _PASSWORD);

		/*update firmware*/
		firmware_update();

	}
	else
	{
		/*jump to new firmware address*/
		systick_delay_ms(500);
		jump_to_app(NEW_FIRMWARE_START_ADDRESS);
	}
}

/*Function to update firmware from the server binary file to the new firmware address in the flash memory*/
void firmware_update(void)
{
	char DGB_MSG[60] = {0};

	/*copy firmware from rx_buffer_1 (already there because of uart rx interrupt) to temp_rx_buffer*/
	esp32_client_get_file(temp_rx_buffer, FIRMWARE);

	/*get firmware valid data only from temp_rx_buffer*/
	int ret_firmware_parser = firmware_parser(temp_rx_buffer, sizeof(temp_rx_buffer), firmware_buffer, sizeof(firmware_buffer));

	systick_delay_ms(500);
	snprintf(DGB_MSG, sizeof(DGB_MSG),"ret_firmware_parser = %d \n\r", ret_firmware_parser);

	buffer_send_string(DGB_MSG, debug_port);
	systick_delay_ms(500);

	/*write firmware to flash memory*/
	flash_write_data_byte(NEW_FIRMWARE_START_ADDRESS,(uint8_t *) firmware_buffer, sizeof(firmware_buffer));

	/*success message*/
	buffer_send_string("Firmware updated successfully. Restart system to run new firmware.\n\r",debug_port);
}

/*Function to jump to indicated address as argument*/
void jump_to_app(uint32_t address)
{
	uint32_t app_start_address;
	func_ptr jump_to_app;

	/*disable global interrupts and systick*/
	__disable_irq();
	SysTick->CTRL = 0;

	/*disable and re-enable clock for AHB*/
	RCC->AHB1RSTR  =  0xFFFFFFFF;
	RCC->AHB1RSTR  =  0x00000000;

	/*set reset handler address as pointer, set the main stack pointer (MSP) and jump to new firmware address*/
	if(  (*(uint32_t *)address) != EMPTY_MEM)
	{
		app_start_address =  *(uint32_t *)(address + 4);
		jump_to_app =  (func_ptr) app_start_address;
		__set_MSP(*(uint32_t *)address);

		jump_to_app();
	}
	/*set error message*/
	else
	{
		buffer_send_string("ERR: Could not jump to indicated address because there is no firmware there\n\r",debug_port);
	}
}

