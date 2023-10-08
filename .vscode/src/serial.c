#include <avr/io.h>
#include "serial.h"

void uart_init(void) {
    UBRR0H = UBRRH_VALUE;
    UBRR0L = UBRRL_VALUE;

#if USE_2X
    UCSR0A |= _BV(U2X0);
#else
    UCSR0A &= ~(_BV(U2X0));
#endif

    UCSR0C = _BV(UCSZ01) | _BV(UCSZ00); /* 8-bit data */
    UCSR0B = _BV(RXEN0) | _BV(TXEN0);   /* Enable RX and TX */
}

/* Sends 1 byte of data to the serial Monitor
 * Waits for USART Data Register to be empty by checking if the UDRE flag is set
 */
void uart_write(char c) {
    loop_until_bit_is_set(UCSR0A, UDRE0); /* Wait until data register empty. */
    UDR0 = c;
}

/* Recieves  1 byte of data from the serial Monitor
 * Waits for UDR0 Data Register to have data that can be read by checking the RXC0 flag
 */
char uart_read(void) {
    loop_until_bit_is_set(UCSR0A, RXC0); /* Wait until data exists. */
    return UDR0;
}

/*
 * Prints string with new line
 */

void uart_println(char *s) {
    // loop through entire string
    while (*s) {
        uart_write(*s);
        s++;
    }
    uart_write('\n');
}