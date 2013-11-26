
#include <Shiftduino.h>

Shiftduino sd(22, 24 , 23, 3);
boolean testRed[] = {0, 1, 0, 1, 0, 1, 0, 1,
                    0, 1, 0, 1, 0, 1, 0, 1,
                    0, 1, 0, 1, 0, 1, 0, 1};
                    
boolean testGreen[] = {1, 0, 1, 0, 1, 0, 1, 0,
                      1, 0, 1, 0, 1, 0, 1, 0,
                      1, 0, 1, 0, 1, 0, 1, 0};
                      
boolean leds = false;
boolean cny = false;

void setup() {
  Serial.begin(9600);  
  sd.clear();  
  sd.setPins(testGreen);
  delay(1000);
  sd.clear();
}

void loop() {
  if (leds){
    testLeds();
  } else if (cny){
    testCNY70();
  } else{
    victoryLoop();
  }
  
}

void testCNY70(){
  // Used analog pins for CNY70: [A8, A0]
  sd.clear();  
  // Change the analog ping to test each one
  int color = analogRead(A8);
  Serial.println(color);
  delay(300);
}

void testLeds(){
  sd.clear();
  sd.setPins(testRed);
  delay(300);
  sd.clear();
  sd.setPins(testGreen);
  delay(300);
}

void victoryLoop(){
  
  boolean values[] = {0, 1, 0, 1, 0, 1, 0, 1,
                    0, 1, 0, 1, 0, 1, 0, 1,
                    0, 1, 0, 1, 0, 1, 0, 1};
  int pos = 0;
  while(true){
    if (pos > 24){ //8*3
      pos = 0;
    }
    for (int i = 0; i < 24; i++){
      if(i==pos){
        values[i] = 1;
      } else{
        values[i] = 0;
      }
    }
    sd.setPins(values);
    delay(300);
    pos++;
    
  }

}
