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
#include "motor.h"

#define radius 10   // the minimum distance to run spiral algorithm
#define dis_min 5 // minimum distance that sensor sensed
#define R3 3     // forward sensor
#define R2 2     // right sensor
#define R1 1    // backward sensor
#define R0 0    // left sensor

#define switch 7 // select mode PD7
#define led 4 // PC4

#define pi 3.14159
#define time_sp_max 100 // max time to complete a circle in spriral mode 

//#define dir 1  // used to adjust the direction of robot
#define SPEED_MAX 200
#define SPEED_MIN 70
#define delta_speed 10

// initial mode
    unsigned int mode_servo = 0;

char *ulong_to_char(unsigned long num){
    char buffer [128];
    snprintf(buffer, sizeof(buffer), "%lu", num);
    char * num_string = buffer; //String terminator is added by snprintf
    return num_string;
}

void motor_run(unsigned char speed1, unsigned char speed2); // speed1: adjust speed of left wheel; speed2: adjust speed of right wheel

void init_pinchangeinterrupt(){
    PORTC |= (1 << switch);
    PCICR |= (1 << PCIE2);
    PCMSK2 |= (1 << PCINT23);
    sei();
}
ISR(PCINT2_vect){
    mode_servo = 1 - mode_servo;
    if (mode_servo == 1){
        servo(4820);
        uart_println("suck");
        _delay_ms(10);
        PORTC |= (1 << led);
    }
    else {
        servo(3310);
        uart_println("blow");
        _delay_ms(10);
        PORTC &= ~(1 << led);
    }
    //reti();
}

int main(void){

    // test power supply 
    DDRC |= (1 << led);
    
    PORTC &= ~(1 << led);
    
    //

    uart_init(); // using to debug
    init_ultrasonic();
    motor_init();
    role_on();
    init_pinchangeinterrupt();
    init();
    
    
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

    // testing
    // while (1){
    //     motor_run(SPEED_MAX, 70);
    // }
    // while (1){
    //     dis_forward = readSensor(forward_sensor);
    //     dis_right = readSensor(right_sensor);
    //     dis_backward = readSensor(backward_sensor);
    //     dis_left = readSensor(left_sensor);
    //     uart_println("forward: ");
    //     uart_println(ulong_to_char(dis_forward));
    //     uart_println("right: ");
    //     uart_println(ulong_to_char(dis_right));
    //     uart_println("backward: ");
    //     uart_println(ulong_to_char(dis_backward));
    //     uart_println("left: ");
    //     uart_println(ulong_to_char(dis_left));
    //     _delay_ms(100);
    // }
    // test spiral mode
    // unsigned char speed_wheel1 = 0, speed_wheel2 = 0;
    // int counter = 0;
    // while (1){
    //     uart_println("spiral mode: ");
    //     uart_println(ulong_to_char(counter));
    //     uart_println("---------");
    //         speed_wheel1 = SPEED_MAX / 2;
    //         speed_wheel2 = SPEED_MIN;
    //         motor_run(speed_wheel1, speed_wheel2);
    //         unsigned long start_sp;
    //         start_sp = millis();
    //         float ratio = speed_wheel2 / speed_wheel1;
    //         while (1) {
    //             if (speed_wheel2 >= SPEED_MAX){
    //                 break;
    //             }
    //             if (speed_wheel2 >= SPEED_MAX / 2 - 10){
    //                 speed_wheel1 = SPEED_MAX;
    //             }
    //             if ((millis() - start_sp) / 1000 >= speed_wheel2 / 20){
    //                 speed_wheel2 += 5;
    //                 uart_println("time: ");
    //                 uart_println(ulong_to_char((millis() - start_sp) / 1000));
    //                 uart_println("speed wheel 2");
    //                 uart_println(ulong_to_char(speed_wheel2));
                    
    //                 motor_run(speed_wheel1, speed_wheel2);
    //                 // uart_println(ulong_to_char(speed_wheel1));
    //                 // uart_println(ulong_to_char(speed_wheel2));
    //                 // uart_println("value millis: ");
    //                 // uart_println(ulong_to_char(millis()));
    //                 // uart_println("value start: ");
    //                 // uart_println(ulong_to_char(start_sp));
    //                 start_sp = millis();
    //                 ratio = speed_wheel2 / speed_wheel1;
    //             }
    //         }
    // }
    //-----------------------

    // while (1){
    //     motor_run(SPEED_MAX, SPEED_MAX);
    // }

    unsigned char speed_wheel1 = 0, speed_wheel2 = 0;
    motor_run(speed_wheel1, speed_wheel2);
    int time_ran; // variable to get random time
    bool random_check = false; // check whether robot was in random mode
    unsigned long start_r2;

    while (1) {


        // read distance from sensors
        dis_forward = readSensor(forward_sensor);
        dis_right = readSensor(right_sensor);
        dis_backward = readSensor(backward_sensor);
        dis_left = readSensor(left_sensor);


        //-----------------
        // check condition for spiral mode
        // if (dis_forward >= radius){
        //     check_register |= (1 << R3);
        // }
        // if (dis_right >= radius){
        //     check_register |= (1 << R2);
        // }
        // if (dis_backward >= radius){
        //     check_register |= (1 << R1);
        // }
        // if (dis_right >= radius){
        //     check_register |= (1 << R0);
        // }

        // test spiral mode
        // check_register = 0x0F;
        //
        if (dis_forward > radius && dis_right > radius && dis_backward > radius && dis_left > radius && random_check == true){ // check the area 
            motor_run(0, 0);
            _delay_ms(50);
            random_check = false;
            goto spiral_mode;
        }

        if (random_check == true){
            uart_println("out of random mode");
            // dis_forward = readSensor(forward_sensor);
            // dis_right = readSensor(right_sensor);
            // dis_left = readSensor(left_sensor);
            if (dis_forward <= dis_min || dis_right <= dis_min || dis_left <= dis_min){
                motor_run(0, 0);
                _delay_ms(50);
                random_check = false;

                
                start_r2 = millis();
                goto random_mode;
            }
            //random_check = false;
            continue;
        }
        //------------------------------------

        random_mode:
            uart_println("random mode");
            time_ran = rand() % 16; // value from 0 to 15
            uart_println(ulong_to_char(time_ran));
            time_ran *= 50; // convert 0ms to 1500ms 
            unsigned long start_r;
            start_r = millis();
            motor_run(0, SPEED_MIN);

            
            while (millis() - start_r < time_ran){ // random turn
            };
            motor_run(0,0); // stop turnning to measure
            //_delay_ms(100);
            dis_forward = readSensor(forward_sensor);
            dis_right = readSensor(right_sensor);
            dis_left = readSensor(left_sensor);
            if (dis_forward <= dis_min || dis_right <= dis_min || dis_left <= dis_min){
                motor_run(0, 0);
                //_delay_ms(100);
                if (millis() - start_r2 >= 1500){
                    PORTB |= ((1 << IN1) | (1 << IN3)); // go back
                    motor_run(160, 160);
                    uart_println("go back");
                    _delay_ms(1500);
                    PORTB &= ~((1 << IN1) | (1 << IN3)); 
                    motor_run(SPEED_MIN, 0);
                    _delay_ms(1500);
                    motor_run(0, 0);
                    _delay_ms(100);
                    start_r2 = millis();
                    goto skip;

                }
                goto random_mode;
            }
        skip:
            motor_run(SPEED_MIN, SPEED_MIN);
            random_check = true;

            
            
            

            continue;


        spiral_mode: 
            uart_println("spiral mode");
            speed_wheel1 = SPEED_MAX / 2;
            speed_wheel2 = SPEED_MIN / 2;
            motor_run(speed_wheel1, speed_wheel2);
            unsigned long start_sp;
            start_sp = millis();
            float ratio = speed_wheel2 / speed_wheel1;
            while (1) {
                dis_forward = readSensor(forward_sensor);
                dis_right = readSensor(right_sensor);
                dis_backward = readSensor(backward_sensor);
                dis_left = readSensor(left_sensor);
                if (dis_forward <= dis_min || dis_right <= dis_min || dis_left <= dis_min || speed_wheel2 >= SPEED_MAX - 50){
                    motor_run(0, 0);
                    _delay_ms(50);
                    //_delay_ms(100);
                    start_r2 = millis();
                    goto random_mode;
                }

                // test 
                // if (speed_wheel2 >= SPEED_MAX / 2){
                //     speed_wheel1 = 0;
                //     speed_wheel2 = 0;
                //     motor_run(speed_wheel1, speed_wheel2);
                //     break;
                // }
                //
                if ((millis() - start_sp) / 1000 >= speed_wheel2 / 70){
                    speed_wheel2 += delta_speed;
                    if (speed_wheel2 >= SPEED_MAX / 2){
                        speed_wheel1 = SPEED_MAX - 50;
                    }
                    motor_run(speed_wheel1, speed_wheel2);
                    // uart_println(ulong_to_char(speed_wheel1));
                    // uart_println(ulong_to_char(speed_wheel2));
                    // uart_println("value millis: ");
                    // uart_println(ulong_to_char(millis()));
                    // uart_println("value start: ");
                    // uart_println(ulong_to_char(start_sp));
                    start_sp = millis();
                    ratio = speed_wheel2 / speed_wheel1;
                }
            }
    }
}