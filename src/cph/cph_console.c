#include "cph_cli.h"
#include "cph_console.h"


uint8_t uc_char;
uint8_t uc_flag;

uint8_t cph_console_tick(void)
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