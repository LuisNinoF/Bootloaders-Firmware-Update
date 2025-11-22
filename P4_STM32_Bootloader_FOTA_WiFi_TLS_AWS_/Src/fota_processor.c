#include "fota_processor.h"

#define END_DELIMITER "\r\nCLOSED\r\n"
#define UNWANTED_PREFIX "\r\n\r\n+IPD,"
#define UNWANTED_PREFIX_DELIMITER ':'

/*helper function to find substring within a source buffer*/
const char* find_substring(const char *src, const char *sub, size_t src_size)
{
	size_t sub_len = strlen(sub);
	for(size_t i = 0; i <= src_size - sub_len; i++)
	{
		if(strncmp(&src[i], sub, sub_len) == 0)
		{
			return &src[i];
		}
	}
	return NULL;
}

/*function to get firmware valid data only from temp_rx_buffer*/
int firmware_parser(const char *src_buffer, size_t src_buffer_size, char *dest_buffer, size_t dest_buffer_size)
{
	/*find end delimiter*/
	const char *end = find_substring(src_buffer, END_DELIMITER, src_buffer_size);
	if(!end)
	{
		buffer_send_string("Error: End delimiter not found in the src buffer \r\n", debug_port);
		return -1;
	}

	/*process and copy data while skipping unwanted sequences*/
	size_t firmware_length = end - src_buffer;
	size_t src_index = 0;
	size_t dest_index = 0;

	while(src_index < firmware_length && dest_index < dest_buffer_size -1)
	{
		if(src_index + strlen(UNWANTED_PREFIX) < firmware_length &&
				strncmp(&src_buffer[src_index], UNWANTED_PREFIX, strlen(UNWANTED_PREFIX)) == 0)
		{
			src_index += strlen(UNWANTED_PREFIX);
			while(src_index < firmware_length && src_buffer[src_index] != UNWANTED_PREFIX_DELIMITER)
			{
				src_index++;
			}
			src_index++;
			continue;
		}
		dest_buffer[dest_index++] = src_buffer[src_index++];
	}

	dest_buffer[dest_index] = '\0';

	if(src_index < firmware_length)
	{
		buffer_send_string("Error: dst buffer is too small for firmware data \r\n", debug_port);
		return -1;
	}

	return dest_index;

}
