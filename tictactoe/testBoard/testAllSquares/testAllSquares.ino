
#include <Shiftduino.h>

Shiftduino sd(22, 24 , 23, 3);
boolean testRed[] = {0, 1, 0, 1, 0, 1, 0, 1,
                    0, 1, 0, 1, 0, 1, 0, 1,
                    0, 1, 0, 1, 0, 1, 0, 1};
                    
boolean testGreen[] = {1, 0, 1, 0, 1, 0, 1, 0,
                      1, 0, 1, 0, 1, 0, 1, 0,
                      1, 0, 1, 0, 1, 0, 1, 0};
                      
boolean leds = true;

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
  } else{
    testCNY70();
  }
  
}

void testCNY70(){
  // Used analog pins for CNY70: [A8, A0]
  sd.clear();  
  // Change the analog ping to test each one
  int color = analogRead(A0);
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
