#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdio.h>
#include "serial.h"
#include "time.h"
#include "ultrasonic.h"

char *ulong_to_char(unsigned long num){
    char buffer [128];
    snprintf(buffer, sizeof(buffer), "%lu", num);
    char * num_string = buffer; //String terminator is added by snprintf
    return num_string;
}


int main(void){
    init(); // init timer0 to use millis() and micros() function in time.h
    uart_init();
    init_ultrasonic();
     
    // ultrasonic 
    // using portb0 for trigger pin and portb1 for echo pin 
    while (1) {
        unsigned int temp;
        temp = distanceRead();
        uart_println(ulong_to_char(temp));
        _delay_ms(100);
    }

}