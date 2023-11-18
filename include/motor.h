#ifndef motor_h
#define motor_h

#define servo_pin 1

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