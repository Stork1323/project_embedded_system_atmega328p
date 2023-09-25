#include <avr/io.h>
#include <util/delay.h>
#include "time.h"
#include "ultrasonic.h"

void init_ultrasonic(){
    DDRB |= (1 << PB0); // set output for trigger pin
    DDRB &= (!(1 << PB1)); // set input for echo pin
}

unsigned int timing(){
    
    static unsigned long timeout = 40000UL;

    PORTB &= !(1 << PB0);
    _delay_us(2);
    PORTB |= (1 << PB0);
    _delay_us(10);
    PORTB &= !(1 << PB0);
    _delay_ms(30);

    previousMicros = micros();
    while((micros() - previousMicros) <= timeout && !(PINB & (1 << PB1))); //wait for the echo pin high or timeout
    previousMicros = micros();
    while((micros() - previousMicros) <= timeout && (PINB & (1 << PB1))); // wait for the echo pin low or timeout

    return micros() - previousMicros;
}

unsigned int distanceRead(){
    return timing() / CM / 2;
}