#include <Shift.h>

Shift shift;

//LEDs
int red1 = 4;
int green1 = 3;
int red2 = 2;
int green2 = 1;
int red3 = 10;
int green3 = 9;

//Voltages for each color
float volRedLow= 0.0;
float volRedHigh = 1.0;
float volGreenLow= 1.0;
float volGreenHigh = 1.8;


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
  
  int sensorValue3 = analogRead(A7);
  // Convert the analog reading (which goes from 0 - 1023) to a voltage (0 - 5V):
  float voltage3 = sensorValue3 * (5.0 / 1023.0);
  adjust3(voltage3);
  
}

void adjust1(float v){
  if (v < volRedHigh && v > volRedLow){
    showRed1();
  }else if (v > volGreenLow && v < volGreenHigh) {
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

void adjust3(float v){
  if (v < 1){
    showRed3();
  }else if (v > 1.01 && v < 2.0) {
    showGreen3();
  } else {
    turnOff();
  }
}

void showRed1(){
  shift.setRegisterPin(green1, LOW);   
  shift.setRegisterPin(red1, HIGH);  
  shift.writeRegisters();
}

void showGreen1(){
  shift.setRegisterPin(green1, HIGH);   
  shift.setRegisterPin(red1, LOW);   
  shift.writeRegisters();
}

void showRed2(){
  shift.setRegisterPin(green2, LOW);   
  shift.setRegisterPin(red2, HIGH);  
  shift.writeRegisters();
}

void showGreen2(){
  shift.setRegisterPin(green2, HIGH);   
  shift.setRegisterPin(red2, LOW);   
  shift.writeRegisters();
}

void showRed3(){
  shift.setRegisterPin(green3, LOW);   
  shift.setRegisterPin(red3, HIGH);  
  shift.writeRegisters();
}

void showGreen3(){
  shift.setRegisterPin(green3, HIGH);   
  shift.setRegisterPin(red3, LOW);   
  shift.writeRegisters();
}

void turnOff(){
  shift.clearRegisters();
  shift.writeRegisters();  
}

