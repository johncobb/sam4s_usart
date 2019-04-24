#include <asf.h>

#include <stdio.h>
#include <string.h>
#include "cph_config.h"
#include "cph_millis.h"
#include "cph_util.h"
#include "cph_console.h"
#include "cph_cli.h"
#include "cph_uart.h"


clock_time_t f_log_timeout = 0;

// static void configure_console(void)
// {
// 	const usart_serial_options_t uart_serial_options = {
// 		.baudrate = CONF_UART_BAUDRATE,
// #ifdef CONF_UART_CHAR_LENGTH
// 		.charlength = CONF_UART_CHAR_LENGTH,
// #endif
// 		.paritytype = CONF_UART_PARITY,
// #ifdef CONF_UART_STOP_BITS
// 		.stopbits = CONF_UART_STOP_BITS,
// #endif
// 	};

// 	/* Configure console UART. */
//     sysclk_enable_peripheral_clock(CONSOLE_UART_ID);
// 	stdio_serial_init(CONF_UART, &uart_serial_options);
// }



int main(void)
{
    sysclk_init();
    board_init();
    delay_init();

    config_init();
    cph_millis_init();
    // cli_init();
    // configure_console();

    // TODO: Only uncomment when you have the USART setup and ready to go
    // puts("\r\n\r\nsam4d32c blinky demo...\r\n");

    uart_uart1_init();

    while(1) {

        char data[] = "Hello World\r\n";

        uart_uart1_write_bytes(data, sizeof(data));
        
        ioport_toggle_pin_level(LED0_GPIO);
        delay_ms(1000); 
    }
}

