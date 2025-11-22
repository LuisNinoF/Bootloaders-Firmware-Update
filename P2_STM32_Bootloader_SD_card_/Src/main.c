/*
 * *** Purpose:
 *  Demonstrate a bootloader to update the firmware by reading a binary file stored on an SD card accessed via SPI.
 *
 * *** How it works:
 * A binary file with the new firmware is stored in a SD card.
 * The new firmware is read via SPI from the SD card and written to the internal flash memory.
 * The user only needs to insert the SD card and press the board button during restart to start the update.
 * The new application is executed at the end of the firmware update.
 *
 * *** Copyrights:
 * Created by Luis Nino
 * Last revision 2025.11.17
 */

#include <stdio.h>
#include "stm32f4xx.h"
#include "fpu.h"
#include "uart.h"
#include "timebase.h"
#include "adc.h"
#include "string.h"
#include "ff.h"

#include "sd_card_driver.h"
#include "fatfs_app.h"
#include "sd_card_spi.h"
#include "flash_driver.h"


/*Define firmware address and file name in the SD card*/
#define MAIN_APP_START_ADDRESS 0x08008000
#define FIRMWARE 	"main_firmware.bin"

/*Declare global variables*/
#define EMPTY_MEM  0xFFFFFFFF
typedef void (*func_ptr)(void);

/*Declare function prototypes*/
int update_main_firmware(void);
void jump_to_app(uint32_t addr_value);
void dump_flash(uint32_t addr, int bytes);

/*Run Main*/
int main()
{
	/*Enable FPU*/
	fpu_enable();

	/*Initialize debug UART*/
	debug_uart_init();

	/*Initialize timebase*/
	timebase_init();

	/*Initialize BSP*/
	led_init();
	button_init();

	/*Initialize ADC*/
	pa1_adc_init();

	/*Start ADC*/
	start_conversion();

	/*Initialize SD*/
	sd_cs_pin_init();
	sd_card_spi_init();

	/*Initialize fatfs*/
	fatfs_init();

	printf("\r\nInit completed");
	delay(500);

	/*If the blue button on the board is pressed at restart,
	 *the firmware will be updated using the file "main_firmware.bin" in the SD Card*/
	if(get_btn_state())
	{
		update_main_firmware();
		jump_to_app(MAIN_APP_START_ADDRESS);
		return 1;
	}
	/*Otherwise, the current firmware will run*/
	else
	{
		jump_to_app(MAIN_APP_START_ADDRESS);
	}

    return 0;
}



/*Function to jump to indicated address as argument*/
void jump_to_app(uint32_t addr_value)
{
	uint32_t app_start_address;
	func_ptr jump_to_app;

	if((*(uint32_t *)addr_value) != EMPTY_MEM)
	{
		printf("\r\nJumping to indicated address");
		app_start_address = *(uint32_t *)(addr_value +4);
		jump_to_app = (func_ptr) app_start_address;
		__set_MSP(*(uint32_t *) addr_value);

		jump_to_app();
	}
	else
	{
		printf("\r\nNo application found at location");
	}
}




/*Function to update the main firmware
 * at address MAIN_APP_START_ADDRESS in sector 2, using the file in FIRMWARE macro*/
int update_main_firmware(void)
{
	int size;
	int firmware_len;
	unsigned int bytes_read;

	/*Allocate memory for FATFS and FIL structures*/
    FATFS *fat_fs = (FATFS *) malloc(sizeof(FATFS));
    FIL *file = (FIL *)malloc(sizeof(FIL));

    if(!fat_fs || !file)
    {
    	printf("\r\nERR: Memory Allocation Failed!");
    	free(fat_fs);
    	free(file);
    	return 1;
    }
    /*Mount*/
    FRESULT result =  f_mount(fat_fs, "", 1);
    if(result != FR_OK )
    {
    	printf("\r\nERR: SD Card Cannot be Mounted!");
    	free(fat_fs);
    	free(file);
    	return 1;
    }
    else
    {
    	printf("\r\nSD Card Mounted. Wait for 30 seconds");
    }

    /*Open the firmware file from SD Card*/
      result =  f_open(file, FIRMWARE, FA_READ);
      if(result != FR_OK )
      {
      	printf("\r\nERR: Firmware File Not Found!");
      	free(fat_fs);
      	free(file);
    	return 1;
      }
      else
      {
    	  printf("\r\nFirmware opened");
      }

      /*Get the size of the file*/
      size =  f_size(file);

      /*Allocate memory for firmware bytes*/
      uint8_t * firmware_bytes = (uint8_t *)calloc(size, sizeof(uint8_t));
      if(!firmware_bytes)
      {
      	printf("\r\nERR: Memory Allocation Failed!");
      	free(fat_fs);
      	free(file);
    	return 1;
      }

      /*Calculate the length of the firmware in words(32-bit)*/
      firmware_len =  size / 4;

      /*Allocate memory for firmware bytes*/
       uint32_t * firmware_words = (uint32_t *)calloc(firmware_len, sizeof(uint32_t));
       if(!firmware_words)
       {
       	printf("\r\nERR: Memory Allocation Failed!");
       	free(fat_fs);
       	free(file);
    	return 1;
       }

       /*Read the firmware file into firmware_bytes buffer*/
        f_read(file,firmware_bytes,size, &bytes_read);

        /*Convert firmware bytes to 32-bit words:*/
        for(int word_index = 0;  word_index < firmware_len; word_index++)
        {
        	firmware_words[word_index] = (firmware_bytes[word_index *4] |
        								 (firmware_bytes[word_index *4 + 1] << 8) |
										 (firmware_bytes[word_index *4 + 2] << 16) |
		 	 	 	 	 	 	 	 	 (firmware_bytes[word_index *4 + 3] << 24));
        }

        printf("\r\nReady to erase flash sector");

       /*Unlock FLASH*/
       flash_unlock();

       /*Erase the necessary sector*/
       flash_sector_erase(2,FLASH_VOLTAGE_RANGE_3);

       /*Write firmware to FLASH*/
       flash_write_to_addr(MAIN_APP_START_ADDRESS,firmware_words,firmware_len);

       /*Free allocated memory*/
     	free(fat_fs);
     	free(file);
      	free(firmware_words);
      	free(firmware_bytes);

      	/*Debug auxiliary function*/
      	//dump_flash(MAIN_APP_START_ADDRESS, 128);

      	/*Firmware updated*/
      	printf("\r\nFirmware update completed \n\r");

      	return 0;
}




/*Debug auxiliary function to print the firmware written during the update firmware process*/
void dump_flash(uint32_t addr, int bytes)
{
	printf("\r\nRead the new main firmware:");
    for (int i = 0; i < bytes; i++) {
        if (i % 16 == 0)
        {
        	printf("\r\n0x%08X: ", addr + i);
        }
        printf("%02X ", *(uint8_t *)(addr + i));
    }
    printf("\n");
}
