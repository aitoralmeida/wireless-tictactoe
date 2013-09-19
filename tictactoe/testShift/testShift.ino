#include <Shiftduino.h>

Shiftduino sd(30, 31 , 32, 2);

int colorPin1 = 0; 
int colorPin2 = 1; 


void setup()
{
  Serial.begin(9600);  
  sd.clear();
}

void loop()
{
  int color0 = analogRead(0); 
  Serial.println(color0);
  int color1 = analogRead(1);
  
  if(color0 > 700 && color0 < 770) {
    sd.setPin(1, 0); //R1
    sd.setPin(0, 1); //G1
  } else if(color0 > 590 && color0 < 650){
    sd.setPin(1, 1); //R1
    sd.setPin(0, 0); //G1
  } else{
    sd.setPin(1, 0); //R1
    sd.setPin(0, 0); //G1
  }
  
  if(color1 > 700 && color1 < 770) {
    sd.setPin(8, 1); //G2
    sd.setPin(9, 0); //R2
  } else if(color1 > 590 && color1 < 650){
    sd.setPin(8, 0); //G2
    sd.setPin(9, 1); //R2
  } else{
    sd.setPin(8, 0); //G2
    sd.setPin(9, 0); //R2
  }
  
  /*
  sd.setPin(1, 1); //R1
  sd.setPin(0, 0); //G1
  sd.setPin(8, 0); //G2
  sd.setPin(9, 1); //R2
  */
}

