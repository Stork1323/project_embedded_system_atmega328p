
#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include "serial.h"



int main(void){
    uart_init();
  
    DDRB = (0 << PB0);
    while(1){
        if (PINB & 0b00000001){
            uart_println("OK\n");
            _delay_ms(100);
        }
        uart_println("don;t receive any signal\n");
        _delay_ms(100);
    }
}