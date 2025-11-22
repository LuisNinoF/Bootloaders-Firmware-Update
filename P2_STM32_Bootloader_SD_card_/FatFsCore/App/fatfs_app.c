#include "fatfs_app.h"


#define PATH_LEN		4

char USER_PATH[PATH_LEN];


void fatfs_init(void)
{
	FATFS_LinkDriver(&TARGET_Driver, USER_PATH);
}

DWORD get_fatfs_time(void)
{
	//Do something...
	return 0;
}
