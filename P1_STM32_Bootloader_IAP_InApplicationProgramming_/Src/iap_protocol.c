#include "iap_protocol.h"
#include "flash_protection.h"

/*Macros*/
#define NUM_OF_CMD 			8
#define MAX_NUM_OF_SECTS	8
#define LAST_SECT_NUM		7

/*Function to transmit the ACK or NACK and number of expected bytes (length)*/
void iap_send_ack(uint32_t expected_bytes_len)
{
	const uint8_t ACK_BUFFSIZE = 2;
	uint8_t ack_buff[ACK_BUFFSIZE];

	ack_buff[0] = ACK_RESP;
	ack_buff[1] = expected_bytes_len;

	//Reminder: #define IAP_UART USART2
	uart_transmit_buff(IAP_UART, ack_buff, ACK_BUFFSIZE);
	// same as: iap_uart_write_data(ack_buff, ACK_BUFFSIZE);

}
void iap_send_nack(void)
{
	uint8_t nack = NACK_RESP;
	//Reminder: #define IAP_UART USART2
	uart_transmit_buff(IAP_UART,&nack,1);
	// same as: iap_uart_write_data(&nack,1);
}

/*Same function as uart_transmit_buff but only for IAP_UART, which is USART2 */
void iap_uart_write_data(uint8_t *p_buff, uint32_t len)
{
	//Reminder: #define IAP_UART	USART2
	uart_transmit_buff(IAP_UART, p_buff, len);
}

/*Auxiliary function to check if address is in flash range*/
uint8_t _addr_valid(uint32_t addr)
{
	if(addr >= FLASH_BASE && addr <= FLASH_END)
	{
		return VALID_ADDR;
	}
	else
	{
		return INVALID_ADDR;
	}
}

/*Auxiliary function for the erase handle*/
static uint8_t execute_flash_erase(uint8_t sector_number, uint8_t number_of_sectors)
{
	FLASH_EraseInitTypeDef flash_erase_handle;
	uint32_t sect_arr;
	DEV_StatusTypeDef status;

	if((number_of_sectors > MAX_NUM_OF_SECTS) || (sector_number > LAST_SECT_NUM))
	{
		return DEV_ERROR;
	}

	if(sector_number <= LAST_SECT_NUM)
	{
		uint8_t remaining_sectors = MAX_NUM_OF_SECTS - sector_number;
		if(number_of_sectors > remaining_sectors)
		{
			number_of_sectors = remaining_sectors;
		}
		flash_erase_handle.TypeErase = FLASH_TYPEERASE_SECTORS;
		flash_erase_handle.Sector = sector_number;
		flash_erase_handle.NbSectors = number_of_sectors;
	}

	flash_unlock();
	flash_erase_handle.VoltageRange = FLASH_VOLTAGE_RANGE_3;
	status = flash_ex_erase(&flash_erase_handle, &sect_arr);

	return status;
}

/*Erase handle*/
void handle_sect_erase_cmd(uint8_t *pbuff)
{
	printf("handle_secterase_cmd \n");

	iap_send_ack(CMD_RESP_LEN);						// Send ack byte & expected length

	DEV_StatusTypeDef status = DEV_OK;
	status = execute_flash_erase(pbuff[2], pbuff[3]);
	iap_uart_write_data(&status, CMD_RESP_LEN);

	printf("First sector to erase is: %d, and number of sectors is: %d \n", pbuff[2], pbuff[3]);
	printf("Sector erase status: %#x \n", status);

}


/*Auxiliary function to write function*/
static DEV_StatusTypeDef mem_write(uint8_t *pbuff, uint32_t mem_address, uint32_t len)
{
	DEV_StatusTypeDef status = DEV_OK;
	flash_unlock();
	for(int i = 0; i < len; i++)
	{
		status = flash_program(FLASH_TYPEPROGRAM_BYTE, mem_address + i, pbuff[i]);
	}
	flash_lock();
	return status;
}

/*Write handle*/
void handle_sect_write_cmd(uint8_t *pbuff)
{
	iap_send_ack(CMD_RESP_LEN);						// Send ack byte & expected length

	DEV_StatusTypeDef status = DEV_OK;
	uint8_t payload_len = pbuff[6];
	uint32_t mem_addr =  *((uint32_t *)(&pbuff[2]));

	if(_addr_valid(mem_addr) == VALID_ADDR)
	{
		status = mem_write(&pbuff[7], mem_addr, payload_len);
		iap_uart_write_data(&status, CMD_RESP_LEN);
	}
	else
	{
		status = DEV_ERROR;
		iap_uart_write_data(&status, CMD_RESP_LEN);
	}
}

