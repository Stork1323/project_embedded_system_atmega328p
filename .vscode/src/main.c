#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdbool.h>
#include <stdio.h>
#include "serial.h"
#include "ultrasonic.h"
#include "time.h"

#define radius 20   // the minimum distance to run spiral algorithm
#define R3 3     // forward sensor
#define R2 2     // right sensor
#define R1 1    // backward sensor
#define R0 0    // left sensor

#define pi 3.14159

#define dir 1  // used to adjust the direction of robot
#define SPEED_MAX 1024

char *ulong_to_char(unsigned long num){
    char buffer [128];
    snprintf(buffer, sizeof(buffer), "%lu", num);
    char * num_string = buffer; //String terminator is added by snprintf
    return num_string;
}

void motor_run(bool direction, unsigned char speed1, unsigned char speed2); // speed1: adjust speed of left wheel; speed2: adjust speed of right wheel


int main(void){

    // test power supply 
    DDRD |= (1 << DDD2);
    PORTD |= (1 << PD2);
    //

    uart_init(); // using to debug
    init_ultrasonic();
     
    // ultrasonic 
    // pb0 trigger 1
    // pb1 echo 1      => forward sensor
    // pb2 trigger 2
    // pb3 echo 2      => right sensor
    // pb4 trigger 3
    // pb5 echo 3       => backward sensor
    // pb6 trigger 4
    // pb7 echo 4       => left sensor


    unsigned char check_register; // using to contain bits of qualified sensors, check to run spiral algorithm
                                    // bit 3 is forward sensor
                                    // bit 2 is right sensor
                                    // bit 1 is backward sensor
                                    // bit 0 is left sensor

    float dis_forward; // distance that forward sensor measured
    float dis_right; // distance that right sensor measured
    float dis_backward; // distance that backward sensor measured
    float dis_left; // distance that left sensor measured

    unsigned char speed_wheel1 = 0, speed_wheel2 = 0;
    motor_run(dir, speed_wheel1, speed_wheel2);

    while (1) {
        // read distance from sensors
        dis_forward = readSensor(forward_sensor);
        dis_right = readSensor(right_sensor);
        dis_backward = readSensor(backward_sensor);
        dis_left = readSensor(left_sensor);

        //-----------------
        // check condition for spiral mode
        if (dis_forward >= radius){
            check_register |= (1 << R3);
        }
        if (dis_right >= radius){
            check_register |= (1 << R2);
        }
        if (dis_backward >= radius){
            check_register |= (1 << R1);
        }
        if (dis_right >= radius){
            check_register |= (1 << R0);
        }
        if (check_register == 0x0F){
            goto spiral_mode;
        }
        //------------------------------------
        random_mode:

        spiral_mode: 
            speed_wheel1 = SPEED_MAX / 2;
            speed_wheel2 = 100;
            motor_run(dir, speed_wheel1, speed_wheel2);
            unsigned long start; // variable store current time
            // every voltage motor will spin 40 rpm
            // 
            float v_wheel1 = (speed_wheel1 / SPEED_MAX) * 100 * 12 * (40 / 60) * 2*pi;
            float v_wheel2 = (speed_wheel2 / SPEED_MAX) * 100 * 12 * (40 / 60) * 2*pi;
            float duration = (2*pi*15) / ()
            start = millis();
            while (1) {
                dis_forward = readSensor(forward_sensor);
                if (dis_forward <= 2 || speed_wheel2 >= (SPEED_MAX / 2)){
                    speed_wheel1 = 0;
                    speed_wheel2 = 0;
                    motor_run(dir, speed_wheel1, speed_wheel2);
                    goto random_mode;
                }
                if (((millis() - start) / 1000) >= duration){
                    speed_wheel2 += 50;
                    if (speed_wheel2 >= (SPEED_MAX / 4)){
                        speed_wheel1 = SPEED_MAX;
                    }
                    motor_run(dir, speed_wheel1, speed_wheel2);
                    start = millis();
                }
            }
            
    }

}