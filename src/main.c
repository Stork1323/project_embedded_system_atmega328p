#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "serial.h"
#include "ultrasonic.h"
#include <time.h>
#include "my_time.h"

#define radius 20   // the minimum distance to run spiral algorithm
#define R3 3     // forward sensor
#define R2 2     // right sensor
#define R1 1    // backward sensor
#define R0 0    // left sensor

#define pi 3.14159
#define time_sp_max 10 // max time to complete a circle in spriral mode 

//#define dir 1  // used to adjust the direction of robot
#define SPEED_MAX 128

char *ulong_to_char(unsigned long num){
    char buffer [128];
    snprintf(buffer, sizeof(buffer), "%lu", num);
    char * num_string = buffer; //String terminator is added by snprintf
    return num_string;
}

void motor_run(unsigned char speed1, unsigned char speed2); // speed1: adjust speed of left wheel; speed2: adjust speed of right wheel


int main(void){

    // test power supply 
    DDRD |= (1 << DDD2);
    PORTD |= (1 << PD2);
    //

    uart_init(); // using to debug
    init_ultrasonic();
    
    time_t time_v;
    srand((unsigned) time(&time_v));  // initialize random number generator
     
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
    motor_run(speed_wheel1, speed_wheel2);
    int time_ran; // variable to get random time

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
        if (check_register == 0x0F){ // check the area 
            goto spiral_mode;
        }
        //------------------------------------

        random_mode:

            time_ran = rand() % 16; // value from 0 to 15
            time_ran *= 100; // convert 0ms to 1500ms 
            int start_r;
            start_r = millis();
            motor_run(0, SPEED_MAX);
            while (millis() - start_r > time_ran){ // random turn
            };
            motor_run(0,0); // stop turnning to measure
            dis_forward = readSensor(forward_sensor);
            if (dis_forward <= 2){
                goto random_mode;
            }
            motor_run(SPEED_MAX, SPEED_MAX);
            continue;


        spiral_mode: 

            speed_wheel1 = SPEED_MAX;
            speed_wheel2 = 1;
            motor_run(speed_wheel1, speed_wheel2);
            int start_sp;
            start_sp = millis();
            float ratio = speed_wheel2 / speed_wheel1;
            while (1) {
                dis_forward = readSensor(forward_sensor);
                if (dis_forward <= 2 || speed_wheel2 >= SPEED_MAX / 2){
                    speed_wheel1 = 0;
                    speed_wheel2 = 0;
                    motor_run(speed_wheel1, speed_wheel2);
                    goto random_mode;
                }
                if ((millis() - start_sp) >= ratio * 2 * time_sp_max * 1000){
                    speed_wheel2 += 5;
                    motor_run(speed_wheel1, speed_wheel2);
                    start_sp = millis();
                    ratio = speed_wheel2 / speed_wheel1;
                }
            }
    }

}