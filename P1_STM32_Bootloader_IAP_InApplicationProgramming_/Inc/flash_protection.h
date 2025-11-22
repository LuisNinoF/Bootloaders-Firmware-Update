#ifndef FLASH_PROTECTION_H_
#define FLASH_PROTECTION_H_

#include <stdint.h>
#include "flash_driver.h"


#define OB_RDP_LEVEL_0 ((uint8_t)0xAA)
#define OB_RDP_LEVEL_1 ((uint8_t)0x55)

/*WARNING :  Enabling level 2 locks your debugger FOREVER*/
#define OB_RDP_LEVEL_2 ((uint8_t)0xCC)



typedef enum
{
	SECTOR_0 = 0x01,  // 0B 0000 0001
	SECTOR_1 = 0x02,  // OB 0000 0010
	SECTOR_2 = 0x04,  // OB 0000 0100
	SECTOR_3 = 0x08,  // OB 0000 1000
	SECTOR_4 = 0x10,  // OB 0001 0000
	SECTOR_5 = 0x20,  // OB 0010 0000
	SECTOR_6 = 0x40,  // OB 0100 0000
	SECTOR_7 = 0x80,  // OB 1000 0000

}sector_wrp_mask;


typedef struct
{
	uint32_t WRPSector;
	uint32_t RDPLevel;
	uint32_t BORLevel;
	uint32_t USERConfig;


}FLASH_OBProgramInitTypeDef;

void flash_obytes_get_config(FLASH_OBProgramInitTypeDef *curr_obytes);
void flash_set_rdp_level(uint8_t level);
void dis_wrp_all_sectors(void);
void sector_set_wrp(sector_wrp_mask sector_num);
uint8_t flash_obytes_get_rdp(void);
uint8_t flash_obytes_get_wrp(void);
uint8_t flash_obytes_get_user(void);
uint8_t flash_obytes_get_bor(void);

#endif
