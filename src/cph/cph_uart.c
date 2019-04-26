
#include "cph_uart.h"
#include "cph_cli.h"


uint8_t uc_char;
uint8_t uc_flag;

uart_cfg_t uart_cfg;
uart_event_handler_t uart_evt;

enum {
    APP_UART1 = 0,
    APP_UART2,
    APP_UART3
} app_uart;
// typedef struct uart_cfg {
//     sam_uart_opt_t sam_uart_opt;
//     uint32_t *p_pio;
//     pio_type_t pio_type;
//     uint32_t ul_mask;
//     uint32_t ul_attribute;
// } uart_cfg_t;



// uart_config
void uart_config(void) {

    const sam_uart_opt_t sam_uart_opt = {
        sysclk_get_cpu_hz(),
        UART1_SERIAL_BAUDRATE, 
        UART1_SERIAL_MODE
    };


    const uart_cfg_t u_cfg = {
        sam_uart_opt,
        UART1,
        ID_UART1,
        PINS_UART1_PIO,
        PINS_UART1_TYPE,
        PINS_UART1_MASK,
        PINS_UART1_ATTR,
        UART1_IRQn,
        UART_IER_RXRDY
    };

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
    uart_evt.on_datareceive = func;
}


void uart_uart1_tick(void) {
    if (uart_uart1_rx_ready()) {
        uint8_t data = uart_uart1_read();
        uart_evt.on_datareceive(data, 1);
    }
}


void uart_uart1_init(void) {
    uart_config();
    return;

    sysclk_init();

     // set the pins to use the uart peripheral
    pio_configure(PINS_UART1_PIO, PINS_UART1_TYPE, PINS_UART1_MASK, PINS_UART1_ATTR);

    //enable the uart peripherial clock
    pmc_enable_periph_clk(ID_UART1);

    const sam_uart_opt_t uart1_settings = { sysclk_get_cpu_hz(), UART1_SERIAL_BAUDRATE, UART1_SERIAL_MODE };


    uart_init(UART1,&uart1_settings);      //Init UART1 and enable Rx and Tx

    // uart_enable_interrupt(UART1, UART_IER_RXRDY);   //Interrupt reading ready
    // NVIC_EnableIRQ(UART1_IRQn);

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

        uart_uart1_write(*(data++));
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