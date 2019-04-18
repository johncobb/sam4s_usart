#include <string.h>
#include <stdbool.h>
#include "cph_config.h"
#include "cph_cli.h"


void init_buffer();
void init_lines();
void cli_clear_buffer();
uint8_t cli_data_available();
uint8_t cli_data_read(void);
bool handle_data();
uint8_t cli_parse(char *token, char **out);
void cli_handle_command(char *cmd, char * parm);


CLI_BUFFER cli_buffer = {{0},0,0};

#define CLI_MAX_CHARS		128

char HEX_DIGITS[] = "0123456789abcdef";

char cli_lines[CLI_MAX_CHARS+1];
char cli_line_buffer[CLI_MAX_CHARS+1];
int cli_line_index = 0;


void cli_init()
{
	init_buffer();
	init_lines();
}

void init_buffer()
{
	cli_line_index = 0;
	memset(cli_line_buffer, '\0', sizeof(cli_line_buffer));
}

void init_lines()
{
	memset(cli_lines, '\0', sizeof(cli_lines));
}

void cli_read_device(void)
{
    uint8_t uc_char;
	uint8_t uc_flag;

	if (uart_is_rx_ready(CONSOLE_UART)) {
		uc_flag = uart_read(CONSOLE_UART, &uc_char);
		if (!uc_flag) {
			cli_put_char(uc_char);
		}
	}
}

void cli_tick()
{
	// read the serial port
	cli_read_device();

	if (cli_data_available()) {
		if (handle_data()) {

			//config_test();

			char *saveptr;
			char *cmd, *parm;

			cmd = strtok_r(cli_lines, CLI_DELIM, &saveptr);
			parm = strtok_r(NULL, CLI_DELIM, &saveptr);

			// printf("cmd=%s\r\n", cmd);
			// printf("parm=%s\r\n", parm);

			if (parm != NULL)
				cli_handle_command(cmd, parm);


			// reset cli_lines
			init_lines();

		}
	}

}

//printf("motor0: %d\r\n", atoi(parm));
void cli_handle_command(char *cmd, char *parm)
{

	if (strcmp(cmd, "test") == 0) {
		// int integerX = atoi(parm);
		// float floatY = atof(parm);
		// printf("user input: %s %f\r\n", cmd, atof(parm));
		// printf("user input: %s %d\r\n", cmd, atoi(parm));

		printf("console input: %s", cmd);
	}

}

// check to see if we have a new line
bool handle_data()
{

	char c = cli_data_read();

	// ignore null terminated strings
	if(c == '\0') return false;
	// prevent buffer overrun
	if(cli_line_index >= CLI_MAX_CHARS) return false;

	// store character in cli_line_buffer
	cli_line_buffer[cli_line_index] = c;
	cli_line_index++;

	// check for end of line
	if(c == CLI_TKEND) {
		// copy new message into buffer
		strcpy(cli_lines, cli_line_buffer);
		init_buffer();
		return true;
	}

	return false;
}

void cli_put_char(unsigned char c)
{
	int i = (unsigned int)(cli_buffer.head + 1) % CLI_RX_BUFFER_SIZE;

	// if we should be storing the received character into the location
	// just before the tail (meaning that the head would advance to the
	// current location of the tail), we're about to overflow the buffer
	// and so we don't write the character or advance the head.
	if (i != cli_buffer.tail) {
		cli_buffer.buffer[cli_buffer.head] = c;
		cli_buffer.head = i;
	}
}

uint8_t cli_parse(char *token, char **out)
{
	uint8_t *ptr = NULL;
	// TODO: review warning
	if((ptr == strstr(cli_lines, token)))
	{
		if(out != NULL) *out = ptr;
		return CLI_TKFOUND;
	}
	else
		return CLI_TKNOTFOUND;
}



void cli_clear_buffer()
{
	memset(&cli_buffer, 0, sizeof(CLI_BUFFER));
}

uint8_t cli_data_available()
{

	return (uint8_t)(CLI_RX_BUFFER_SIZE + cli_buffer.head - cli_buffer.tail) % CLI_RX_BUFFER_SIZE;
}

uint8_t cli_data_read(void)
{
	// if the head isn't ahead of the tail, we don't have any characters
	if (cli_buffer.head == cli_buffer.tail) {
		return -1;
	} else {
		uint8_t c = cli_buffer.buffer[cli_buffer.tail];
		cli_buffer.tail = (unsigned int)(cli_buffer.tail + 1) % CLI_RX_BUFFER_SIZE;
		return c;
	}
}