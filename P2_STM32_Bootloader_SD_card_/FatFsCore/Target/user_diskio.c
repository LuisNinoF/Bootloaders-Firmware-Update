#include "ff_gen_drv.h"
#include "sd_card_driver.h"
#include <string.h>


DSTATUS  TARGET_initialize(BYTE pdrv);
DRESULT TARGET_read(BYTE pdrv, BYTE* buff, DWORD sector, UINT count);
DRESULT TARGET_write(BYTE pdrv, const BYTE* buff, DWORD sector, UINT count);
DRESULT TARGET_ioctl (BYTE pdrv, BYTE ctrl, void* buff);
DSTATUS TARGET_status (BYTE pdrv);


Diskio_drvTypeDef TARGET_Driver =
{
		TARGET_initialize,
		TARGET_status,
		TARGET_read,
		TARGET_write,
		TARGET_ioctl
};

DSTATUS  TARGET_initialize(BYTE pdrv)
{
	return sd_disk_init(pdrv);
}


DRESULT TARGET_read(BYTE pdrv, BYTE* buff, DWORD sector, UINT count)
{
	return sd_disk_read( pdrv,buff,sector,count);

}

DRESULT TARGET_write(BYTE pdrv, const BYTE* buff, DWORD sector, UINT count)
{
	return  sd_disk_write(pdrv,buff,sector,count);

}

DRESULT TARGET_ioctl (BYTE pdrv, BYTE ctrl, void* buff)
{
	return sd_disk_ioctl(pdrv,ctrl,buff);

}


DSTATUS TARGET_status (BYTE pdrv)
{
	return sd_disk_status (pdrv);

}
