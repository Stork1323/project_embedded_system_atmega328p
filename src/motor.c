
#include <avr/io.h>
#include <util/delay.h>
#include "motor.h"



void timer1_init()
{
    TCCR1A |= (1<< WGM10) | (1<< WGM11);
    TCCR1B |= (1<< WGM12) | (1<< WGM13); // che do FPWM top la 0CR1A
    TCCR1A |= (1<< COM1A1) | (1<< COM1B1); // che do non-inverting PWM 
    TCCR1B |= (1<< CS11) | (1<< CS10); // prescaler / 64  
}
// void thuan_1()
// {
//     PORTB |= (1<< IN1); 
// }
// void nghich_1()
// {
//     PORTB = 0b00010000;
// }
// void thuan_2()
// {
//     PORTB |= (1<< IN3);
// }
// void nghich_2()
// {
//     PORTB = 0b00001000;
// }
void timer0_init()
{
    TCCR0A |= (1<< COM0A1) | (1<< WGM01) | (1<< WGM00);
    TCCR0B |= (1<< WGM02) | (1<< CS00) | (1<< CS01);
}
void role_on()
{
    DDRB |= (1 << role_pin);
    PORTB |= (1<< role_pin);
    _delay_ms(500);
}
void role_off()
{
    PORTB &= ~(1<< role_pin);
    _delay_ms(500);
}
void motor_init()
{
    DDRD |= (1<< PWM1_PIN); // PWM1_PIN = IN2, PWM2_PIN = IN4 
    DDRB |= (1<< PWM2_PIN);
    DDRB |= (1 << IN1) | (1 << IN3);
    PORTB &= ~((1 << IN1) | (1 << IN3));
}
void motor_run(unsigned char speed1, unsigned char speed2)
{
    OCR2A = speed1;
    OCR2B = speed2;
    TCCR2A |= (1<< WGM20) | (1<< COM2A1) | (1<< COM2B1) | (1<< WGM21) ;
    TCCR2B |= (1<< CS22) | (1 << CS20); 

}
void servo(unsigned int x)
{
    DDRB |= (1<< servo_pin);
    TCCR1A |= (1<< WGM11) | (1<< COM1A1);
    TCCR1B |= (1<< WGM12) | (1<< WGM13) | (1<< CS11); // prescaler /8
    OCR1A = 1060;
    new_value = OCR1A;
    ICR1 = 40000;
    if(new_value != x)
    {
        OCR1A = x;
        new_value = OCR1A;
    }
    else
    {
        return;
    }
// 1060 0 do 4820 180 do ct: angel*21+1000
}