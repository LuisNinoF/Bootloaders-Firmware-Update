
#include "sd_card_spi.h"
#include  "sd_card_driver.h"
#include "timebase.h"
#include "diskio.h"
#include <stdbool.h>

extern SPI_HandleTypeDef hspi1;
extern volatile uint16_t g_tmr_cnt1;
extern volatile uint16_t g_tmr_cnt2;

#define SD_CARD_SPI_HANDLE		&hspi1
#define SD_SPI_TIMEOUT			10

#define CMD_PACKET_SZ			6

static volatile DSTATUS g_disk_stat = STA_NOINIT;

static uint8_t g_power_flg  = 0;

/*Card type MMC = 0. SDC = 1*/
static uint8_t g_card_type;

/*Slave select*/
static void slave_select(void)
{
	/*Set CS pin low*/
	cs_pin_reset();

	/*Delay for 1ms*/
	delay(5);
}

/*Slave deselect*/
static void slave_deselect(void)
{
	/*Set CS pin high*/
	cs_pin_set();

	/*Delay for 1ms*/
	delay(5);
}

/*Transmit a byte of data*/
static void sd_spi_tx_byte(uint8_t data)
{
	/*Wait for TXE(transmit buffer empty) flag to be set*/
	while(!(hspi1.Instance->SR & (SPI_FLAG_TXE))){}

	/*Transmit byte*/
	spi_transmit(SD_CARD_SPI_HANDLE,&data,1,SD_SPI_TIMEOUT);
}

/*Transmit buffer of data*/
static void sd_spi_tx_buff(uint8_t *buffer,uint16_t len)
{
	/*Wait for TXE(transmit buffer empty) flag to be set*/
	while(!(hspi1.Instance->SR & (SPI_FLAG_TXE))){}

	/*Transmit byte*/
	spi_transmit(SD_CARD_SPI_HANDLE,buffer,len,SD_SPI_TIMEOUT);
}

static uint8_t sd_spi_rx_byte(void)
{
	uint8_t temp;
	uint8_t rcv_data;

	temp =  0xFF;

	/*Wait for TXE(transmit buffer empty) flag to be set*/
	while(!(hspi1.Instance->SR & (SPI_FLAG_TXE))){}

	/*Transmit byte*/
	spi_transmit(SD_CARD_SPI_HANDLE,&temp,1,SD_SPI_TIMEOUT);

	delay(5);

	/*Receive byte*/
	spi_receive(SD_CARD_SPI_HANDLE,&rcv_data,1,SD_SPI_TIMEOUT);

	return rcv_data;

}

/*Receive byte using pointer*/

static void sd_spi_rx_byte_ptr(uint8_t *buff)
{
	*buff = sd_spi_rx_byte();
}

static void sd_pwr_on(void)
{
	/*Array to hold command packet*/
	uint8_t cmd_packet[CMD_PACKET_SZ];

	uint32_t cnt =  1000;

	/*Deselect slave*/
	slave_deselect();

	/*Transmit byte to wakeup*/
	for(int i = 0; i < 10; i++)
	{
		sd_spi_tx_byte(0xFF);
	}
	delay(5);

	/*Select slave*/
	slave_select();
	delay(5);

	/*Prepare cmd packet*/
	cmd_packet[0] =  CMD0;
	cmd_packet[1] =  0;
	cmd_packet[2] =  0;
	cmd_packet[3] =  0;
	cmd_packet[4] =  0;
	cmd_packet[5] =  CMD0_CRC;

	sd_spi_tx_buff(cmd_packet,sizeof(cmd_packet));

	/*Wait for response*/
	while((sd_spi_rx_byte()!= R1_IDLE_STATE_RESP)&& cnt)
	{
		cnt--;
	}

	/*Deselect slave*/
	slave_deselect();
	delay(5);

	/*Set power flag*/
	g_power_flg = 1;

}


/*Power off*/
static void sd_pwr_off(void)
{
	g_power_flg = 0;
}


static uint8_t sd_get_pwr(void)
{
	return g_power_flg;
}




/*Wait for SD Ready*/
static uint8_t sd_rdy_wait(void)
{
	uint8_t res;
	/*Timout of 500ms*/
	g_tmr_cnt2 = 50;

	/*0xFF is received when SD is ready*/
	do{
		res =  sd_spi_rx_byte();
	}while((res != SD_RDY_FLAG) && g_tmr_cnt2);

	return res;
}

/*Receive data block*/
static bool sd_spi_rx_data_blk(uint8_t *buff, uint32_t len)
{

	uint8_t flg;
	/*set 200ms timout*/
	g_tmr_cnt1 = 20;

	/*Keep looping until response is received or timeout occurs*/
	do{
		flg =  sd_spi_rx_byte();
	}while((flg == SD_RDY_FLAG) && g_tmr_cnt1);

	do{
		sd_spi_rx_byte_ptr(buff++);
	}while(len--);

	/*Discard CRC*/
	 sd_spi_rx_byte();
	 sd_spi_rx_byte();

	 return 1;

}

/*Transmit data block*/
static bool sd_spi_tx_data_blk(const uint8_t *buff, uint8_t flg)
{
	uint8_t itr = 0;
	uint8_t resp;

	/*Check for SD Ready*/
	if(sd_rdy_wait() != SD_RDY_FLAG)
	{
		return 0;
	}

	/*Transmit flag*/
	sd_spi_tx_byte(flg);

	/*If flag passed is not Stop then transmit data*/
	if( flg != SD_STOP_FLAG)
	{
		sd_spi_tx_buff((uint8_t *)buff, 512);
		/*Discard CRC*/
		 sd_spi_rx_byte();
		 sd_spi_rx_byte();

		 /*Receive response*/
		 while(itr <= 50)
		 {
			 resp = sd_spi_rx_byte();
				if((resp & 0x1F) == 0x05)
				{
						 break;
				}

				itr++;
		 }

		 while(sd_spi_rx_byte() ==  0){}
	}

	if((resp & 0x1F) == 0x05)
	{
		return 1;
	}


	return 0;
}

static uint8_t sd_send_cmd(uint8_t cmd, uint32_t arg)
{
	uint8_t crc;
	uint8_t res;
	uint8_t cnt = 10;

	/*Check for SD Ready*/
	if(sd_rdy_wait() != SD_RDY_FLAG)
	{
		return 0xFF;
	}

	/*Transmit command*/
	sd_spi_tx_byte(cmd);

	/*Transmit Arguments*/
	sd_spi_tx_byte((uint8_t)(arg >> 24)); 	/* Argument[31..24] */
	sd_spi_tx_byte((uint8_t)(arg >> 16)); 	/* Argument[23..16] */
	sd_spi_tx_byte((uint8_t)(arg >> 8)); 	/* Argument[15..8] */
	sd_spi_tx_byte((uint8_t)arg); 			/* Argument[7..0] */

	/*Prepare CRC*/
	if(cmd ==  CMD0)
	{
		crc = CMD0_CRC;
	}
	else if(cmd ==  CMD8)
	{
		crc = CMD8_CRC;
	}
	else
	{
		crc = 1;
	}
	/*Transmit crc*/
	sd_spi_tx_byte(crc);

	/*Check for STOP_TRANSMISSION command*/
	/*Skip stuff byte*/
	if(cmd  == CMD12)
	{
		sd_spi_rx_byte();
	}

	/*Receive response*/
	do{

		res =  sd_spi_rx_byte();
	}while((res & 0x80) && --cnt);


	return res;
}



DSTATUS sd_disk_init(BYTE pdrv)
{
    printf("\r\n---- sd_disk_init START ----");

    uint8_t ocr[4];
    uint8_t type = 0;
    uint8_t n;

    /* no disk */
    if (g_disk_stat & STA_NODISK)
    {
        printf("\r\nnSTA_NODISK set");
        return g_disk_stat;
    }

    /* power on */
    sd_pwr_on();
    printf("\r\nPower on...");

    /* slave select */
    slave_select();
    printf("\r\nSelecting CS...");
    delay(500);

    /* Send GO_IDLE_STATE command */
    printf("\r\nSending CMD0...");
    uint8_t r_cmd0 = sd_send_cmd(CMD0, 0);
    printf("\r\nCMD0 response = 0x%02X", r_cmd0);

    if (r_cmd0 == 1)
    {
        g_tmr_cnt1 = 100;

        /* High Capacity or Standard SD card: send CMD8 */
        printf("\r\nSending CMD8...");
        uint8_t r_cmd8 = sd_send_cmd(CMD8, 0x1AA);
        printf("\r\nCMD8 response = 0x%02X", r_cmd8);

        if (r_cmd8 == 1)
        {
            /* Read 4 bytes of OCR */
            for (n = 0; n < 4; n++)
            {
                ocr[n] = sd_spi_rx_byte();
            }
            printf("\r\nCMD8 OCR = %02X %02X %02X %02X", ocr[0], ocr[1], ocr[2], ocr[3]);

            /* Check voltage acceptance */
            if (ocr[2] == 0x01 && ocr[3] == 0xAA)
            {
                //printf("\r\nocr[2] == 0x01 && ocr[3] == 0xAA");

                /* Initialize card by sending ACMD41 (CMD55 + CMD41) */
                do {
                    printf("\r\nSending CMD55...");
                    sd_send_cmd(CMD55, 0);

                    printf("\r\nSending CMD41...");
                    uint8_t r_cmd41 = sd_send_cmd(CMD41, 1UL << 30);
                    printf("\r\nCMD41 response = 0x%02X", r_cmd41);

                    if (r_cmd41 == 0)
                    {
                        //printf("\r\nInitialization complete");
                        break;
                    }
                    //printf("\r\ng_tmr_cnt1 is %d", g_tmr_cnt1);
                } while (g_tmr_cnt1);

                /* Read OCR register with CMD58 */
                if (g_tmr_cnt1 && sd_send_cmd(CMD58, 0) == 0)
                {
                    for (n = 0; n < 4; n++)
                    {
                        ocr[n] = sd_spi_rx_byte();
                    }
                    printf("\r\nCMD58 OCR = %02X %02X %02X %02X", ocr[0], ocr[1], ocr[2], ocr[3]);

                    /* Determine SD card version (V2 block or standard) */
                    type = (ocr[0] & 0x40) ? CT_SD2 | CT_BLOCK : CT_SD2;
                }
            }
        }
        else
        {
            /* SD v1 or MMC card initialization */
            type = ((sd_send_cmd(CMD55, 0) <= 1) && (sd_send_cmd(CMD41, 0) <= 1)) ? CT_SD1 : CT_MMC;

            do
            {
                if (type == CT_SD1)
                {
                    if ((sd_send_cmd(CMD55, 0) <= 1) && (sd_send_cmd(CMD41, 0) == 0))
                    {
                        break; /* initialized */
                    }
                }
                else
                {
                    if (sd_send_cmd(CMD1, 0) == 0)
                    {
                        break; /* initialized */
                    }
                }
            } while (g_tmr_cnt1);

            /* Set block length to 512 bytes */
            if (!g_tmr_cnt1 || (sd_send_cmd(CMD16, 512) != 0))
            {
                type = 0; /* Initialization failed */
            }
        }
    }

    g_card_type = type;

    /* Deselect slave and issue dummy clocks */
    slave_deselect();
    sd_spi_rx_byte();

    if (type)
    {
        g_disk_stat &= ~STA_NOINIT;
    }
    else
    {
        /* Initialization failed */
        sd_pwr_off();
    }

    return g_disk_stat;
}





DRESULT sd_disk_read(BYTE pdrv, BYTE* buff, DWORD sector, UINT count)
{

	/*Check status*/
	if (g_disk_stat & STA_NOINIT) {

		return RES_NOTRDY;
	}

	if (!(g_card_type & CT_SD2)) {
		sector *= 512;
	}

	/*Select slave*/
	slave_select();

	if(count ==  1)
	{
		/*Read Single Block*/
		if ((sd_send_cmd(CMD17, sector) == 0) && sd_spi_rx_data_blk(buff, 512))
		{
				count = 0;
		}
	}
	else
	{
		/*Read Multiple Blocks*/
		if (sd_send_cmd(CMD18, sector) == 0)
		{
			do {
				if (!sd_spi_rx_data_blk(buff, 512))
				{
						break;
				}
					buff += 512;
				} while (--count);

			/*Stop Transmission*/
			sd_send_cmd(CMD12, 0);

		}
	}

	/* Idle */
	slave_deselect();
	sd_spi_rx_byte();

	 if(count)
	 {
		 return RES_ERROR;
	 }
	 else
	 {
		 return RES_OK;
	 }
}

DRESULT sd_disk_write(BYTE pdrv, const BYTE* buff, DWORD sector, UINT count)
{
	/*Check status*/
	if (g_disk_stat & STA_NOINIT) {

		return RES_NOTRDY;
	}

	if (!(g_card_type & CT_SD2)) {
		sector *= 512;
	}

	 /*Check for write protection*/
	if(g_disk_stat & STA_PROTECT)
	{
	   return RES_WRPRT;
	}

	/*Select slave*/
	slave_select();


	/*Single write*/
   if(count ==  1)
   {

		/*In order to initiate a block write, a start token must be sent to the card.
		 *  For a single block write, the start token is 0xFE, and for a multi-block write,
		 *   the start token is 0xFC. This allows the card to begin writing a single block
		 *   of data to the specified sector. When performing a multi-block write,
		 *    it is important to ensure that a start token of 0xFC is sent for each block being written.*/

		if ((sd_send_cmd(CMD24, sector) == 0) && sd_spi_tx_data_blk(buff, 0xFE))
		{
			count = 0;
		}
   }

   /*Multi write*/
   else
   {
		if (g_card_type & CT_SD1)
		{
			sd_send_cmd(CMD55, 0);
			sd_send_cmd(CMD23, count); /* ACMD23 */
		}
		if (sd_send_cmd(CMD25, sector) == 0)
		{
			do {
				/*0xFC :  Multi-block write*/
				if(!sd_spi_tx_data_blk(buff, 0xFC))
				{
					break;
				}
				buff += 512;
			} while (--count);

			/*Stop transmisssion  :  0xFD*/
			if(!sd_spi_tx_data_blk(0, 0xFD))
			{
				count = 1;
			}
		}
     }

	/* Idle */
	slave_deselect();
	sd_spi_rx_byte();

	 if(count)
	 {
		 return RES_ERROR;
	 }
	 else
	 {
		 return RES_OK;
	 }
}


DRESULT sd_disk_ioctl (BYTE pdrv, BYTE ctrl, void* buff)
{
    DRESULT res;                                 // variable to store the result of the function
    BYTE n, csd[16], *ptr = buff;              // variable n and an array csd with size of 16, and a pointer ptr to buff
    WORD csize;                                 // variable csize

    res = RES_ERROR;                            // initialize the result to RES_ERROR


    if(ctrl  ==  CTRL_POWER)
    {
    	switch(*ptr)
    	{
        case 0:                                 // if the value is 0
            sd_pwr_off();                      // call the function sd_pwr_off()
            res = RES_OK;                      // update the result to RES_OK
            break;
        case 1:                                 // if the value is 1
            sd_pwr_on();                       // call the function sd_pwr_on()
            res = RES_OK;                      // update the result to RES_OK
            break;

        case 2:                                 // if the value is 2
            *(ptr + 1) = sd_get_pwr();         // call the function sd_get_pwr() and store the result at the location pointed by (ptr + 1)
            res = RES_OK;                      // update the result to RES_OK
            break;

        default:                                // if the value is not 0, 1 or 2
            res = RES_PARERR;                  // update the result to RES_PARERR
    	}
    }
    else
    {
        if (g_disk_stat & STA_NOINIT){

        	return RES_NOTRDY;  // check if the disk has been initialized and return RES_NOTRDY if it has not
        }

        slave_select();                        // call the function slave_select()

        switch(ctrl)
        {
        case GET_SECTOR_COUNT:
           	/* Get the Card Specific Data (CSD)*/

			if ((sd_send_cmd(CMD9, 0) == 0) && sd_spi_rx_data_blk(csd, 16))
			{
                if ((csd[0] >> 6) == 1)
                {
                    /* SDC V2 */
                    csize = csd[9] + ((WORD) csd[8] << 8) + 1;  // calculate the value of csize
                    *(DWORD*) buff = (DWORD) csize << 10;       // store the calculated value of csize to the location pointed by buff
                }
                else
					{
						/* MMC or SDC V1 */
						n = (csd[5] & 15) + ((csd[10] & 128) >> 7) + ((csd[9] & 3) << 1) + 2;
						csize = (csd[8] >> 6) + ((WORD) csd[7] << 2) + ((WORD) (csd[6] & 3) << 10) + 1;
						*(DWORD*) buff = (DWORD) csize << (n - 9);
					}
					res = RES_OK;
			}
			break;
        case GET_SECTOR_SIZE:

        	//Set sector size to 512
        	*(WORD*)buff = 512;
			res = RES_OK;
			break;

        case CTRL_SYNC:

			if (sd_rdy_wait() == 0xFF){
				res = RES_OK;
			}
			break;

        case MMC_GET_CSD:
        	/* Get the Card Specific Data (CSD)*/
			if (sd_send_cmd(CMD9, 0) == 0 && sd_spi_rx_data_blk(ptr, 16))
			{
				res = RES_OK;
			}
			break;

		case MMC_GET_CID:
			/*Get the Card Identification (CID)*/
			if (sd_send_cmd(CMD10, 0) == 0 && sd_spi_rx_data_blk(ptr, 16))
			{
				res = RES_OK;
			}
			break;


			if (sd_send_cmd(CMD58, 0) == 0)
			{
				for (n = 0; n < 4; n++)
				{
					*ptr++ = sd_spi_rx_byte();
				}
				res = RES_OK;
			}

		default:
			res = RES_PARERR;

        }

		slave_deselect();
		sd_spi_rx_byte();
    }

    return res;
}


DSTATUS sd_disk_status (BYTE pdrv)
{

	return g_disk_stat;

}
