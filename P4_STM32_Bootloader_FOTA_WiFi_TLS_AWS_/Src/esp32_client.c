#include "esp32_client.h"

#define TEMP_BUFF_LNG_SZ		600
#define TEMP_BUFF2_SHT_SZ		30

/*Server address*/
#define SERVER_ADDRESS "s3.eu-north-1.amazonaws.com"

/*Macros for commonly used strings in the function*/
#define SSL_START_COMMAND "AT+CIPSTART=\"SSL\",\"%s\",443\r\n"
#define OK_RESPONSE "OK\r\n"
#define SEND_PROMPT ">"
#define SEND_OK_RESPONSE "SEND OK\r\n"
#define END_OF_HEADERS "\r\n\r\n"
#define LINE_TERMINATOR "\r\n"
#define CLOSED_RESPONSE "CLOSED\r\n"
#define CIPSEND_COMMAND "AT+CIPSEND=%d\r\n"
#define CIPCLOSE		"AT+CIPCLOSE\r\n"

#define HTTPS_GET_REQUEST_FIRM 	"GET %s HTTP/1.1\r\nHost: %s\r\nConnection: close\r\n\r\n"

/*get firmware from server*/
void esp32_client_get_file(char *dest_buffer, const char *storage_url)
{
	/*Buffer to hold HTTPS GET request and other string*/
	char request_buffer[TEMP_BUFF_LNG_SZ] = {0};
	char send_command_buffer[TEMP_BUFF2_SHT_SZ] = {0};

	/*Clear esp uart buffer*/
	buffer_clear(esp32_port);

	/*Establish a SSL connection to the server on port 443, which is a secure connection*/
	snprintf(request_buffer, sizeof(request_buffer),SSL_START_COMMAND,SERVER_ADDRESS);
	buffer_send_string(request_buffer,esp32_port);

	/*Wait until connection is successfully established*/
	while(!is_response(OK_RESPONSE)){}

	/*Prepare the HTTPS GET request to retrieve the file*/
	snprintf(request_buffer, sizeof(request_buffer),HTTPS_GET_REQUEST_FIRM, storage_url, SERVER_ADDRESS);

	/*Get the length of the prepared request*/
	int request_length = strlen(request_buffer);

	/*Prepare the AT+CIPSEND command with the request length */
	snprintf(send_command_buffer,sizeof(send_command_buffer),CIPSEND_COMMAND,request_length);

	/*Send the AT+CIPSEND command */
	buffer_send_string(send_command_buffer,esp32_port);

	/*Wait for the ESP to be ready to receive the HTTP GET request ">"*/
	while(!is_response(SEND_PROMPT)){}

	/*Send the actual HTTP GET request to the server*/
	buffer_send_string(request_buffer,esp32_port);

	/*Wait to confirm that the data was sent*/
	while(!is_response(SEND_OK_RESPONSE)){}

	/*Wait for the HTTP response header to finish*/
	while(!is_response(END_OF_HEADERS)){}

	/*Copy the data to destination buffer*/
	while(!copy_up_to_string(CLOSED_RESPONSE,dest_buffer)){}

	/*Close SSL connection*/
	buffer_send_string(CIPCLOSE, esp32_port);
}
