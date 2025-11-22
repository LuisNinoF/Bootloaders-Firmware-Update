#ifndef __SD_CARD_DRIVER_H__
#define __SD_CARD_DRIVER_H__

#include "diskio.h"

#define CMD0	 (0x40+0)       /*GO_IDLE_STATE*/
#define CMD1	 (0x40+1)       /*SEND_OP_COND*/
#define CMD8     (0x40+8)     	/* SEND_IF_COND */
#define CMD9     (0x40+9)     	/* SEND_CSD */
#define CMD10    (0x40+10)    	/* SEND_CID */
#define CMD12    (0x40+12)    	/* STOP_TRANSMISSION */
#define CMD16    (0x40+16)    	/* SET_BLOCKLEN */
#define CMD17    (0x40+17)    	/* READ_SINGLE_BLOCK */
#define CMD18    (0x40+18)    	/* READ_MULTIPLE_BLOCK */
#define CMD23    (0x40+23)    	/* SET_BLOCK_COUNT */
#define CMD24    (0x40+24)    	/* WRITE_BLOCK */
#define CMD25    (0x40+25)    	/* WRITE_MULTIPLE_BLOCK */
#define CMD41    (0x40+41)    	/* SEND_OP_COND (ACMD) */
#define CMD55    (0x40+55)    	/* APP_CMD */
#define CMD58    (0x40+58)    	/* READ_OCR */

#define CMD0_CRC				0x95
#define CMD8_CRC				0x87

#define R1_IDLE_STATE_RESP		0x01

#define SD_RDY_FLAG				0xFF
#define SD_STOP_FLAG			0xFD

/*MMC Card Type*/

#define CT_MMC		0x01   /*MMC ver 3*/
#define CT_SD1		0x02   /*sd ver 1*/
#define CT_SD2		0x04   /*sd ver 2*/
#define CT_SDC		0x06   /*sd*/
#define CT_BLOCK    0x08   /*Block Addressing*/

DSTATUS sd_disk_init(BYTE pdrv);
DRESULT sd_disk_read(BYTE pdrv, BYTE* buff, DWORD sector, UINT count);
DRESULT sd_disk_write(BYTE pdrv, const BYTE* buff, DWORD sector, UINT count);
DRESULT sd_disk_ioctl (BYTE pdrv, BYTE ctrl, void* buff);
DSTATUS sd_disk_status (BYTE pdrv);



#endif
