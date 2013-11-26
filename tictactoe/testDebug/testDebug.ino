/*
  Debug Test
  Turns on an LED on for one second, then off for one second, repeatedly.
 
  This example need a special version of pins_arduino.h that have to be located
  in Arduino folder ../hardware/arduino/variants/mega/
 
  This example code is in the public domain.
 */

#define DEBUG1  70
#define DEBUG2  71
#define DEBUG3  72

int led = DEBUG1;

void setup() {                

  // initialize led pin as outputs.
  pinMode(led, OUTPUT);     
}

void loop() {

  digitalWrite(led, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(1000);               // wait for a second
  digitalWrite(led, LOW);    // turn the LED off by making the voltage LOW
  delay(1000);               // wait for a second
}
