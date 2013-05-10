#include <Shift.h>

Shift shift;

void setup() {  
  Serial.begin(9600);
  
  shift.initialize();
}

// the loop routine runs over and over again forever:
void loop() {
  
  int sensorValue1 = analogRead(A0);
  // Convert the analog reading (which goes from 0 - 1023) to a voltage (0 - 5V):
  float voltage1 = sensorValue1 * (5.0 / 1023.0);
  adjust1(voltage1);
  
  int sensorValue2 = analogRead(A4);
  // Convert the analog reading (which goes from 0 - 1023) to a voltage (0 - 5V):
  float voltage2 = sensorValue2 * (5.0 / 1023.0);
  adjust2(voltage2);
  Serial.println(voltage2);
  
}

void adjust1(float v){
  if (v < 1){
    showRed1();
  }else if (v > 1.01 && v < 2.0) {
    showGreen1();
  } else {
    turnOff();
  }
}

void adjust2(float v){
  if (v < 1){
    showRed2();
  }else if (v > 1.01 && v < 2.0) {
    showGreen2();
  } else {
    turnOff();
  }
}

void showRed1(){
  shift.setRegisterPin(3, LOW);   
  shift.setRegisterPin(4, HIGH);  
  shift.writeRegisters();
}

void showGreen1(){
  shift.setRegisterPin(3, HIGH);   
  shift.setRegisterPin(4, LOW);   
  shift.writeRegisters();
}

void showRed2(){
  shift.setRegisterPin(1, LOW);   
  shift.setRegisterPin(2, HIGH);  
  shift.writeRegisters();
}

void showGreen2(){
  shift.setRegisterPin(1, HIGH);   
  shift.setRegisterPin(2, LOW);   
  shift.writeRegisters();
}

void turnOff(){
  shift.clearRegisters();
  shift.writeRegisters();  
}
