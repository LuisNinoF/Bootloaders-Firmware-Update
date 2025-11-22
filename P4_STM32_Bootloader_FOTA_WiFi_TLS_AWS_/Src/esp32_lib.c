#include "esp32_lib.h"

/** STM32 -------------------->Slave Device
 *  5V------------------------>5V
 *  GND----------------------->GND
 *  TX PA9  (D8)----------------------->RX (D16)
 *  RX PA10 (D2)----------------------->TX (D17)
 * */

/*Declare function prototypes*/
static void esp32_reset(void);
static void esp32_startup_test(void);
static void esp32_sta_mode(void);
static void esp32_ap_connect(char *ssid, char *password);

/*Initialize esp32: reset, startup, sta mode (Wifi client), and ap connect (connect to Wifi)*/
void esp32_init(char *ssid, char *password)
{
	circular_buffer_init();
	esp32_reset();
	systick_delay_ms(500);

	esp32_startup_test();
	systick_delay_ms(500);
	esp32_sta_mode();
	systick_delay_ms(500);
	esp32_ap_connect(ssid, password);
	systick_delay_ms(500);
	/*DBG: success message*/
	buffer_send_string("ESP32 Init successful....\n\r",debug_port);
	systick_delay_ms(500);
}


/*Start up ESP32*/
static void esp32_startup_test(void)
{
	/*Clear ESP uart buffer*/
	buffer_clear(esp32_port);

	/*Send test command*/
	buffer_send_string("AT\r\n",esp32_port);

	/*Wait for "OK" response*/
	while(!(is_response("OK\r\n"))){}

	/*DBG: success message*/
	buffer_send_string("AT startup test successful....\n\r",debug_port);
}

/*Reset ESP32*/
static void esp32_reset(void)
{
	/*Clear ESP uart buffer*/
	buffer_clear(esp32_port);

	/*Send test command*/
	buffer_send_string("AT+RST\r\n",esp32_port);

	/*Delay for 1 second*/
	systick_delay_ms(1000);

	/*Wait for "OK" response*/
	while(!(is_response("OK\r\n"))){}

	/*DBG: success message*/
	buffer_send_string("Reset was successful....\n\r",debug_port);
}

/*Set the ESP32 as WIFI Client*/
static void esp32_sta_mode(void)
{
	/*Clear ESP uart buffer*/
	buffer_clear(esp32_port);

	/*Send test command*/
	buffer_send_string("AT+CWMODE=1\r\n",esp32_port);

	/*Wait for "OK" response*/
	while(!(is_response("OK\r\n"))){}

	/*DBG: success message*/
	buffer_send_string("STA Mode set successful....\n\r",debug_port);
}

/*Connect to WIFI*/
static void esp32_ap_connect(char *ssid, char *password)
{
	char data[80];

	/*Clear ESP uart buffer*/
	buffer_clear(esp32_port);

	buffer_send_string("Connecting to access point....\n\r",debug_port);

	/*Pust ssid, password and command into one string packet*/
	sprintf(data,"AT+CWJAP=\"%s\",\"%s\"\r\n",ssid,password);

	/*Send test command*/
	buffer_send_string(data,esp32_port);

	/*Wait for "OK" response*/
	while(!(is_response("OK\r\n"))){}

    sprintf(data,"Connected : \"%s\"\r\n",ssid);

	buffer_send_string(data,debug_port);
}
