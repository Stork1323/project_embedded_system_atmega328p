#ifndef motor_h
#define motor_h

#define servo_pin 1
#define PWM1_PIN 3 //pd3
#define PWM2_PIN 3 //pb3
#define IN1 4
#define IN3 5
#define role_pin 0
//#define uint8_t unsigned char 
typedef unsigned char uint8_t;
//typedef unsigned short int uint16_t;

unsigned int new_value;

void motor_run(unsigned char speed1, unsigned char speed2); // speed1 adjust speed of left wheel, speed2 adjust speed of right wheel
void motor_init();
void timer1_init();
void timer0_init();
void role_off();
void role_on();
void servo(unsigned int x);
// void thuan_1();
// void thuan_2();
// void nghich_1();
// void nghich_2();

#endif