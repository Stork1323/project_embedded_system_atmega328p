#ifndef ultrasonic_h
#define ultrasonic_h

// #define CM 28 // covert to cemtimeter

// use port b for ultrasonic sensor

#define trigPin_F 0 //portb 0
#define echoPin_F 1 //portb 1
#define trigPin_R 2 //portb 2
#define echoPin_R 3 //portb 3
#define trigPin_B 4 //portb 4
#define echoPin_B 5 //portb 5
#define trigPin_L 6 //portb 6
#define echoPin_L 7 //portb 7

#define forward_sensor 1
#define right_sensor 2
#define backward_sensor 3
#define left_sensor 4

unsigned char trigPin, echoPin;

unsigned long previousMicros;

void init_ultrasonic();
float readSensor(unsigned char numSensor);

#endif 

