#include <avr/io.h>
#include <util/delay.h>
#include "my_time.h"
#include "ultrasonic.h"

void init_ultrasonic(){
    init(); // init timer0 to use millis() and micros() function in time.h
    DDRB |= (1 << DDB0) | (1 << DDB2) | (1 << DDB4) | (1 << DDB6); // set output for trigger pin
    DDRB &= ~((1 << DDB1) | (1 << DDB3) | (1 << DDB5) | (1 << DDB7)); // set input for echo pin
}

float readSensor(unsigned char numSensor)
{
    static unsigned long timeout = 40000UL;
    static unsigned long time;

    switch (numSensor)
    {
    case forward_sensor:
        trigPin = trigPin_F;
        echoPin = echoPin_F;
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

    PORTB &= ~(1 << trigPin);
    _delay_us(10);
    PORTB |= (1 << trigPin);
    _delay_us(10);
    PORTB &= ~(1 << trigPin);
    previousMicros = micros();
    while((micros() - previousMicros) <= timeout && !(PINB & (1 << echoPin))); //wait for the echo pin high or timeout
    previousMicros = micros();
    while((micros() - previousMicros) <= timeout && (PINB & (1 << echoPin))); // wait for the echo pin low or timeout
    time = micros() - previousMicros;
    _delay_ms(25);
    float pingTravelDistance, distanceToTarget;
    pingTravelDistance = (time*765.*5280.*12)/(3600.*1000000);
    distanceToTarget = pingTravelDistance/2;
    return  distanceToTarget;

}
