
#include "cph_uart.h"
#include "cph_cli.h"


uint8_t uc_char;
uint8_t uc_flag;

void uart_uart1_init(void) {
    sysclk_init();

     // set the pins to use the uart peripheral
    pio_configure(PINS_UART1_PIO, PINS_UART1_TYPE, PINS_UART1_MASK, PINS_UART1_ATTR);

    //enable the uart peripherial clock
    pmc_enable_periph_clk(ID_UART1);

    const sam_uart_opt_t uart1_settings = { sysclk_get_cpu_hz(), UART1_SERIAL_BAUDRATE, UART1_SERIAL_MODE };

    uart_init(UART1,&uart1_settings);      //Init UART1 and enable Rx and Tx

    uart_enable_interrupt(UART1,UART_IER_RXRDY);   //Interrupt reading ready
    NVIC_EnableIRQ(UART1_IRQn);

}

uint32_t uart_uart1_rx_ready(void) {
    return uart_is_rx_ready(UART1);
}

uint8_t uart_uart1_read(void) {
 	if (uart_is_rx_ready(UART1)) {
		uc_flag = uart_read(UART1, &uc_char);
		if (!uc_flag) {
            return uc_char;
		}
	}   
}

void uart_uart1_write(uint8_t data) {
   while (!(UART1->UART_SR & UART_SR_TXRDY));
   uart_write(UART1, data);
}

void uart_uart1_write_bytes(char* data, int size) {

	for (int i=0; i<size; i++) {
        while (!(UART1->UART_SR & UART_SR_TXRDY));
        uart_write(UART1, data[i]);
	}
}

void UART1_Handler() {
   uint32_t dw_status = uart_get_status(UART1);

   if(dw_status & UART_SR_RXRDY) {
      uint8_t received_byte;
      uart_read(UART1, &received_byte);
    //   cli_put_char(received_byte);
      uart_uart1_write(received_byte);
   }
}