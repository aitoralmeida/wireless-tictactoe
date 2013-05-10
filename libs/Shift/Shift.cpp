/*
  Library to control the 74HC595
  More info at http://bildr.org/?s=74hc595
*/

#include "Arduino.h"
#include "Shift.h"

int SER_Pin = 38;   //pin 14 on the 75HC595
int RCLK_Pin = 39;  //pin 12 on the 75HC595
int SRCLK_Pin = 40; //pin 11 on the 75HC595

//How many of the shift registers - change this
#define number_of_74hc595s 2 

//do not touch
#define numOfRegisterPins number_of_74hc595s * 8

boolean registers[numOfRegisterPins];

void Shift::initialize(){
  pinMode(SER_Pin, OUTPUT);
  pinMode(RCLK_Pin, OUTPUT);
  pinMode(SRCLK_Pin, OUTPUT);


  //reset all register pins
  clearRegisters();
  writeRegisters();
}               


//set all register pins to LOW
void Shift::clearRegisters(){
  for(int i = numOfRegisterPins - 1; i >=  0; i--){
     registers[i] = LOW;
  }
} 


//Set and display registers
//Only call AFTER all values are set how you would like (slow otherwise)
void Shift::writeRegisters(){

  digitalWrite(RCLK_Pin, LOW);

  for(int i = numOfRegisterPins - 1; i >=  0; i--){
    digitalWrite(SRCLK_Pin, LOW);

    int val = registers[i];

    digitalWrite(SER_Pin, val);
    digitalWrite(SRCLK_Pin, HIGH);

  }
  digitalWrite(RCLK_Pin, HIGH);

}

//set an individual pin HIGH or LOW
void Shift::setRegisterPin(int index, int value){
  registers[index] = value;
}