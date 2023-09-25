#ifndef ultrasonic_h
#define ultrasonic_h

#define CM 28 // covert to cemtimeter

// use port b for ultrasonic sensor

#define trigPin 0 //portb 0
#define echoPin 1 //portb 1


unsigned long previousMicros;

void init_ultrasonic();
unsigned int timing();
unsigned int distanceRead();

#endif 

