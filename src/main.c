/*
 * main.c
 * 
 * Created on: April 22, 2019
 * Author: John Cobb
 */

#include <asf.h>
#include <stdio.h>
#include <string.h>
#include "cph_config.h"
#include "cph_millis.h"
#include "cph_util.h"
#include "cph_console.h"
#include "cph_cli.h"
#include "cph_uart.h"
#include "stdio_serial.h"

void app_init(void);
bool app_timeout(clock_time_t);
void app_settimer(clock_time_t *, clock_time_t);
void load_uart_config(void);
void app_message(uart_cfg_t *, char *);

sam_uart_opt_t sam_uart_opt;
uart_cfg_t u_cfg;

typedef struct {
    clock_time_t log_timer;
    clock_time_t led_timer;
} app_timer_t;

volatile app_timer_t app_timer;


void uart_ondatareceived(uint8_t data, uint32_t len);

void uart_ondatareceived(uint8_t data, uint32_t len) {

    lib_uart_write(&u_cfg, data);
}

void app_init(void) {
    // initialize timers to zero
    app_timer.log_timer = 0;
    app_timer.led_timer = 0;
}

void app_uart_init(void) {

    sam_uart_opt.ul_mck = sysclk_get_cpu_hz();
    sam_uart_opt.ul_baudrate = UART1_SERIAL_BAUDRATE;
    sam_uart_opt.ul_mode = UART1_SERIAL_MODE;

    u_cfg.sam_uart_opt = sam_uart_opt;
    u_cfg.p_uart = UART1;
    u_cfg.uart_id = ID_UART1;
    u_cfg.p_pio = PINS_UART1_PIO;
    u_cfg.pio_type = PINS_UART1_TYPE;
    u_cfg.ul_mask = PINS_UART1_MASK;
    u_cfg.ul_attribute = PINS_UART1_ATTR;
    u_cfg.ul_irq = UART1_IRQn;
    u_cfg.ul_source = UART_IER_RXRDY;
    u_cfg.on_datareceive = uart_ondatareceived;
}


uint8_t uc_char;
uint8_t uc_flag;

int main(void)
{
    
    sysclk_init();
    board_init();
    delay_init();
    config_init();
    cph_millis_init();

    /* init uart */
    app_uart_init();

    /* configure uart */
    lib_uart_cfg(&u_cfg);

    /* initialize variables and callbacks for our task processor */
    app_init();

    app_message(&u_cfg, "Welcome to sam4s_uart example.\r\n");
    while(1) {

        if ((app_timeout(app_timer.log_timer))) {
            // send a message every second
            // app_message(&u_cfg);
            // reset timer

            app_message(&u_cfg, "Hello World!\r\n");
            app_settimer(&app_timer.log_timer, 1000);
        }

        if ((app_timeout(app_timer.led_timer))) {
            // toggle led
            ioport_toggle_pin_level(LED0_GPIO);
            app_settimer(&app_timer.led_timer, 100);
        }

        lib_uart_tick(&u_cfg);
        delay_ms(50);
    }
}

void app_message(uart_cfg_t *uart_cfg, char *msg) {
    // buffer used to sprintf timestamp into message
    char buffer[64] = {0};
    // data used to store the message
    char data[] = "%lu    %s\r\n";

    // copy the timstamp into data and store into buffer
    sprintf(buffer, data, g_cph_millis, msg);

    // write to uart1
    // uart_uart1_write_bytes(buffer, sizeof(data));
    lib_uart_writebytes(uart_cfg, buffer, sizeof(buffer));   
}

void app_settimer(clock_time_t *timer, clock_time_t timeout) {
    *timer = g_cph_millis + timeout;
}

bool app_timeout(clock_time_t timer) {
    if (g_cph_millis >= timer) {
        return true;
    }

    return false;
}

// #define USART_SERIAL                     UART1
// #define USART_SERIAL_BAUDRATE            115200
// #define USART_SERIAL_CHAR_LENGTH         US_MR_CHRL_8_BIT
// #define USART_SERIAL_PARITY              US_MR_PAR_NO
// #define USART_SERIAL_STOP_BIT            false

// void config_usart(void) {
//     const sam_usart_opt_t usart_console_settings = {
//         USART_SERIAL_BAUDRATE,
//         USART_SERIAL_CHAR_LENGTH,
//         USART_SERIAL_PARITY,
//         USART_SERIAL_STOP_BIT,
//         US_MR_CHMODE_NORMAL
//     };

//     sysclk_enable_peripheral_clock(USART_SERIAL_ID);

//     usart_init_rs232(USART_SERIAL, &usart_console_settings, sysclk_get_main_hz());
//     usart_enable_tx(USART_SERIAL);
//     usart_enable_rx(USART_SERIAL);

//     while (1) {
//         usart_putchar('A', USART_SERIAL);
//         delay_ms(50);
//     }
// }

