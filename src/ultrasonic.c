#include <avr/io.h>
#include <util/delay.h>
#include "my_time.h"
#include "ultrasonic.h"

// forward_sensor = pc0 pc1
// right_sensor = pd0 pd1
// backward_sensor = pd2 pd3
// left_sensor = pd4 pd5

void init_ultrasonic(){
    init(); // init timer0 to use millis() and micros() function in time.h
    DDRD |= (1 << DDD0) | (1 << DDD2) | (1 << DDD4); // set output for trigger pin
    DDRD &= ~((1 << DDD1) | (1 << DDD3) | (1 << DDD5)); // set input for echo pin
    DDRC |= (1 << DDC0);
    DDRC &= ~(1 << DDC1);
}

float readSensor(unsigned char numSensor)
{
    static unsigned long timeout = 40000UL;
    static unsigned long time;
        float pingTravelDistance, distanceToTarget;

    switch (numSensor)
    {
    case forward_sensor:
        trigPin = trigPin_F;
        echoPin = echoPin_F;
        goto switch_port;
        break;
    case backward_sensor:
        trigPin = trigPin_B;
        echoPin = echoPin_B;
        break;
    case right_sensor:
        trigPin = trigPin_R;
        echoPin = echoPin_R;
        break;
    case left_sensor:
        trigPin = trigPin_L;
        echoPin = echoPin_L;
        break;
    default:
        break;
    }

    PORTD &= ~(1 << trigPin);
    _delay_us(10);
    PORTD |= (1 << trigPin);
    _delay_us(10);
    PORTD &= ~(1 << trigPin);
    previousMicros = micros();
    while((micros() - previousMicros) <= timeout && !(PIND & (1 << echoPin))); //wait for the echo pin high or timeout
    previousMicros = micros();
    while((micros() - previousMicros) <= timeout && (PIND & (1 << echoPin))); // wait for the echo pin low or timeout
    time = micros() - previousMicros;
    _delay_ms(25);

    pingTravelDistance = (time*765.*5280.*12)/(3600.*1000000);
    distanceToTarget = pingTravelDistance/2;

    switch_port:
    PORTC &= ~(1 << trigPin);
    _delay_us(10);
    PORTC |= (1 << trigPin);
    _delay_us(10);
    PORTC &= ~(1 << trigPin);
    previousMicros = micros();
    while((micros() - previousMicros) <= timeout && !(PINC & (1 << echoPin))); //wait for the echo pin high or timeout
    previousMicros = micros();
    while((micros() - previousMicros) <= timeout && (PINC & (1 << echoPin))); // wait for the echo pin low or timeout
    time = micros() - previousMicros;
    _delay_ms(25);
    pingTravelDistance = (time*765.*5280.*12)/(3600.*1000000);
    distanceToTarget = pingTravelDistance/2;

    return  distanceToTarget;

}
