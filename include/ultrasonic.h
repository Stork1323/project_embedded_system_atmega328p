#ifndef ultrasonic_h
#define ultrasonic_h

// #define CM 28 // covert to cemtimeter

// use port b for ultrasonic sensor

#define trigPin_F 0 //portc 0
#define echoPin_F 1 //portc 1
#define trigPin_R 0 //portd 0
#define echoPin_R 1 //portd 1
#define trigPin_B 2 //portd 2
#define echoPin_B 3 //portd 3
#define trigPin_L 4 //portd 4
#define echoPin_L 5 //portd 5

#define forward_sensor 1
#define right_sensor 2
#define backward_sensor 3
#define left_sensor 4

unsigned char trigPin, echoPin;

unsigned long previousMicros;

void init_ultrasonic();
float readSensor(unsigned char numSensor);

#endif 

