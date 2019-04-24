

#ifndef CPH_UART_H
#define CPH_UART_H

#include <asf.h>

#define UART1_SERIAL_BAUDRATE        115200
#define UART1_SERIAL_CHANNEL_MODE   UART_MR_CHMODE_NORMAL
#define UART1_SERIAL_MODE         UART_MR_PAR_NO

/* =============== UART1 =============== */ //(UART0 is defined but not UART1)
#define PINS_UART1          (PIO_PB2A_URXD1 | PIO_PB3A_UTXD1)
#define PINS_UART1_FLAGS    (PIO_PERIPH_A | PIO_DEFAULT)
#define PINS_UART1_MASK     (PIO_PB2A_URXD1 | PIO_PB3A_UTXD1)
#define PINS_UART1_PIO      PIOB
#define PINS_UART1_ID       ID_PIOB
#define PINS_UART1_TYPE     PIO_PERIPH_A
#define PINS_UART1_ATTR     PIO_DEFAULT


void uart_uart1_init(void);
uint32_t uart_uart1_rx_ready(void);
uint8_t uart_uart1_read(void);
void uart_uart1_write(uint8_t data);
void uart_uart1_write_bytes(char* data, int size);

#endif
