#ifndef F_CPU
#define F_CPU 16000000UL
#endif

#ifndef BAUD
#define BAUD 9600
#include <util/setbaud.h>
#endif

void uart_init(void);
void uart_write(char c);
char uart_read(void);
void uart_println(char *s);
