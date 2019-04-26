
#include "cph_uart.h"
#include "cph_cli.h"


uint8_t uc_char;
uint8_t uc_flag;

uart_cfg_t uart_cfg;
uart_event_handler_t uart_evt;

// uart_config
// void uart_config(void) {

//     const sam_uart_opt_t sam_uart_opt = {
//         sysclk_get_cpu_hz(),
//         UART1_SERIAL_BAUDRATE, 
//         UART1_SERIAL_MODE
//     };


//     const uart_cfg_t u_cfg = {
//         sam_uart_opt,
//         UART1,
//         ID_UART1,
//         PINS_UART1_PIO,
//         PINS_UART1_TYPE,
//         PINS_UART1_MASK,
//         PINS_UART1_ATTR,
//         UART1_IRQn,
//         UART_IER_RXRDY
//     };

//     sysclk_init();
//     pio_configure(u_cfg.p_pio, u_cfg.pio_type, u_cfg.ul_mask, u_cfg.ul_attribute);
//     pmc_enable_periph_clk(u_cfg.uart_id);
//     uart_init(u_cfg.p_uart, &u_cfg.sam_uart_opt);

//     uart_enable_interrupt(u_cfg.p_pio, u_cfg.ul_source);   //Interrupt reading ready
//     NVIC_EnableIRQ(u_cfg.ul_irq);
    
// }

void lib_uart_cfg(uart_cfg_t *uart_cfg) {

    uart_cfg_t u_cfg = *uart_cfg;

    sysclk_init();
    pio_configure(u_cfg.p_pio, u_cfg.pio_type, u_cfg.ul_mask, u_cfg.ul_attribute);
    pmc_enable_periph_clk(u_cfg.uart_id);
    uart_init(u_cfg.p_uart, &u_cfg.sam_uart_opt);

    uart_enable_interrupt(u_cfg.p_pio, u_cfg.ul_source);   //Interrupt reading ready
    NVIC_EnableIRQ(u_cfg.ul_irq);
    
}



void uart_set_ondatareceive_func(uart_ondatareceive_func_t func)
{
    // LOGT("modem_set_ondatareceive_func: \r\n");
    // uart_evt.on_datareceive = func;
}


void lib_uart_tick(uart_cfg_t * u_cfg) {
    if (lib_uart_rxready(u_cfg)) {
        uint8_t data = lib_uart_read(u_cfg);
        //uart_evt.on_datareceive(data, 1);
    }
}


uint32_t lib_uart_rxready(uart_cfg_t * u_cfg) {
    return uart_is_tx_ready(u_cfg->p_uart);
}

uint8_t lib_uart_read(uart_cfg_t * u_cfg) {
 	if (uart_is_rx_ready(u_cfg->p_uart)) {
		uc_flag = uart_read(u_cfg->p_uart, &uc_char);
		if (!uc_flag) {
            return uc_char;
		}
	}   
}

void lib_uart_write(uart_cfg_t * u_cfg, uint8_t data) {
   while (!(u_cfg->p_uart->UART_SR & UART_SR_TXRDY));
   uart_write(u_cfg->p_uart, data);
}

void lib_uart_writebytes(uart_cfg_t * u_cfg, char* data, int size) {

	for (int i=0; i<size; i++) {

        lib_uart_write(u_cfg, *(data++));
	}
}

void UART1_Handler() {
   uint32_t dw_status = uart_get_status(UART1);
   

   if(dw_status & UART_SR_RXRDY) {
      uint8_t received_byte;
      uart_read(UART1, &received_byte);
      uart_evt.on_datareceive(received_byte, 1);
   }
}