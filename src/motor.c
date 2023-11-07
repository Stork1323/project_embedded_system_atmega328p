
#include <avr/io.h>
#include <util/delay.h>
#include <motor.h>

#define PWM1_PIN 1
#define PWM2_PIN 2
#define IN1 3
#define IN3 4
#define role_pin 0
//#define uint8_t unsigned char 
typedef unsigned char uint8_t;
//typedef unsigned short int uint16_t;
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
    PORTC |= (1<< role_pin);
    _delay_ms(500);
}
void role_off()
{
    PORTC &= ~(1<< role_pin);
    _delay_ms(500);
}
void motor_init()
{
    DDRB |= (1<< PWM1_PIN) | (1<< PWM2_PIN); // PWM1_PIN = IN2, PWM2_PIN = IN4 
    DDRB |= (1 << IN1) | (1 << IN3);
    PORTB &= ~((1 << IN1) | (1 << IN3));
}
void motor_run(unsigned char speed1, unsigned char speed2)
{
    OCR1A = speed1;
    OCR1B = speed2;
    TCCR1A |= (1<< WGM10) | (1<< COM1A1) | (1<< COM1B1);
    TCCR1B |= (1<< WGM12) | (1<< CS11) | (1<< CS10); // che do FPWM top la 0CR1A

}