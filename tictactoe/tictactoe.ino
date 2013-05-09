
int ledRed1 = 50;
int ledGreen1 = 52;

int ledRed2 = 22;
int ledGreen2 = 24;

void setup() {                
  Serial.begin(9600);
  
  pinMode(ledRed1, OUTPUT);     
  pinMode(ledGreen1, OUTPUT); 
  
  pinMode(ledRed2, OUTPUT);     
  pinMode(ledGreen2, OUTPUT);  
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
    turnOff1();
  }
}

void adjust2(float v){
  if (v < 1){
    showRed2();
  }else if (v > 1.01 && v < 2.0) {
    showGreen2();
  } else {
    turnOff2();
  }
}

void showRed1(){
  digitalWrite(ledGreen1, LOW);
  digitalWrite(ledRed1, HIGH);   
}

void showGreen1(){
  digitalWrite(ledRed1, LOW);
  digitalWrite(ledGreen1, HIGH);   
}

void turnOff1(){
  digitalWrite(ledRed1, LOW);
  digitalWrite(ledGreen1, LOW);   
}


void showRed2(){
  digitalWrite(ledGreen2, LOW);
  digitalWrite(ledRed2, HIGH);   
}

void showGreen2(){
  digitalWrite(ledRed2, LOW);
  digitalWrite(ledGreen2, HIGH);   
}

void turnOff2(){
  digitalWrite(ledRed2, LOW);
  digitalWrite(ledGreen2, LOW);   
}
