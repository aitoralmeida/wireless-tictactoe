#include <Shiftduino.h>

Shiftduino sd(22, 24 , 23, 3);

int colorPin1 = 0; 
int colorPin2 = 1; 


void setup()
{
  Serial.begin(9600);  
  sd.clear();
}

void loop()
{
  int color0 = analogRead(A8); 
  
  int color1 = analogRead(A6);
  Serial.println(color1);
  
  if(color0 > 190 && color0 < 225) {
    sd.setPin(0, 0); //R1
    sd.setPin(1, 1); //G1
  } else if(color0 > 135 && color0 < 145){
    sd.setPin(0, 1); //R1
    sd.setPin(1, 0); //G1
  } else{
    sd.setPin(1, 0); //R1
    sd.setPin(0, 0); //G1
  }
  
  if(color1 > 190 && color1 < 225) {
    sd.setPin(2, 1); //G2
    sd.setPin(3, 0); //R2
  } else if(color1 > 80 && color1 < 120){
    sd.setPin(2, 0); //G2
    sd.setPin(3, 1); //R2
  } else{
    sd.setPin(2, 0); //G2
    sd.setPin(3, 0); //R2
  }
  
  /*
  sd.setPin(1, 1); //R1
  sd.setPin(0, 0); //G1
  sd.setPin(8, 0); //G2
  sd.setPin(9, 1); //R2
  */
}

