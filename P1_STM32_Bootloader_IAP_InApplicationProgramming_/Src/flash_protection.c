#include "flash_protection.h"




#define FLASH_GET_FLAG(FLAG) 		((FLASH->SR) & (FLAG))

#define FLASH_OPT_KEY1		0x08192A3BU
#define FLASH_OPT_KEY2		0x4C5D6E7FU


/* Flash interface register + FLASH_OPTCR_Offset +  0 Byte  */
#define OPTCR_BYTE0_ADDRESS		0x40023C14U

/* Flash interface register + FLASH_OPTCR_Offset +  1Byte :  RDP*/
#define OPTCR_BYTE1_ADDRESS		0x40023C15U

/* Flash interface register + FLASH_OPTCR_Offset +  2Byte :  WRP*/
#define OPTCR_BYTE2_ADDRESS		0x40023C16U

/* Flash interface register + FLASH_OPTCR_Offset +  3Byte : SPRMOD*/
#define OPTCR_BYTE3_ADDRESS		0x40023C17U

#define START_OF_WRP_BYTE		16
#define START_OF_RDP_BYTE		8

uint8_t flash_obytes_get_wrp(void)
{
	return (*(__IO uint8_t *)(OPTCR_BYTE2_ADDRESS));
}

uint8_t flash_obytes_get_rdp(void)
{
	return (*(__IO uint8_t *)(OPTCR_BYTE1_ADDRESS));

}

/*User bots WDG_SW, RST_STOP, RST_STDBY*/
uint8_t flash_obytes_get_user(void)
{
	return ((uint8_t)(FLASH->OPTCR & 0xE0));
}


uint8_t flash_obytes_get_bor(void)
{
	return ((uint8_t)(FLASH->OPTCR & 0x0C));

}

DEV_StatusTypeDef flash_obytes_lock(void)
{
	FLASH->OPTCR |= FLASH_OPTCR_OPTLOCK;

	return DEV_OK;
}

DEV_StatusTypeDef flash_obytes_unlock(void)
{
	if(( FLASH->OPTCR & FLASH_OPTCR_OPTLOCK) != RESET)
	{
		FLASH->OPTKEYR = FLASH_OPT_KEY1;
		FLASH->OPTKEYR = FLASH_OPT_KEY2;

	}
	else
	{
		return DEV_ERROR;
	}

	return DEV_OK;
}

void sector_set_wrp(sector_wrp_mask sector_num)
{


	/*Unlock option byte*/
	flash_obytes_unlock();

	/*Make sure flash is not busy*/
	while(FLASH_GET_FLAG(FLASH_SR_BSY) != RESET){}

	/*Enable WRPi by setting SPRMOD to 0*/
	FLASH->OPTCR &=~(1U<<31);

	/*Set WRP for desired sector*/
	FLASH->OPTCR &=~(sector_num << START_OF_WRP_BYTE);

	/*Enable OPTSTART*/
	FLASH->OPTCR |=(1U<<1);

	/*Make sure flash is not busy*/
	while(FLASH_GET_FLAG(FLASH_SR_BSY) != RESET){}

	flash_obytes_lock();
}


void dis_wrp_all_sectors(void)
{

	/*Unlock option byte*/
	flash_obytes_unlock();

	/*Make sure flash is not busy*/
	while(FLASH_GET_FLAG(FLASH_SR_BSY) != RESET){}

	/*Enable WRPi by setting SPRMOD to 0*/
	FLASH->OPTCR &=~(1U<<31);

	/*Set WRP for desired sector*/
	FLASH->OPTCR |= (0xFF << START_OF_WRP_BYTE);

	/*Enable OPTSTART*/
	FLASH->OPTCR |=(1U<<1);

	/*Make sure flash is not busy*/
	while(FLASH_GET_FLAG(FLASH_SR_BSY) != RESET){}

	flash_obytes_lock();
}

void flash_set_rdp_level(uint8_t level)
{

	/*Unlock option byte*/
	flash_obytes_unlock();

	/*Make sure flash is not busy*/
	while(FLASH_GET_FLAG(FLASH_SR_BSY) != RESET){}

	/*Set RDP level*/
	FLASH->OPTCR |= (level << START_OF_RDP_BYTE);

	/*Enable OPTSTART*/
	FLASH->OPTCR |=(1U<<1);

	/*Make sure flash is not busy*/
	while(FLASH_GET_FLAG(FLASH_SR_BSY) != RESET){}

	flash_obytes_lock();
}


void flash_obytes_get_config(FLASH_OBProgramInitTypeDef *curr_obytes)
{
	curr_obytes->WRPSector = (uint8_t) flash_obytes_get_wrp();
	curr_obytes->RDPLevel  =  (uint8_t) flash_obytes_get_rdp();
	curr_obytes->USERConfig =  (uint8_t) flash_obytes_get_user();
	curr_obytes->BORLevel   = (uint8_t) flash_obytes_get_bor();
}
