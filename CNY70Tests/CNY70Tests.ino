
int ledRed = 24;
int ledGreen = 22;

void setup() {                
  Serial.begin(9600);
  pinMode(ledRed, OUTPUT);     
  pinMode(ledGreen, OUTPUT);   
}

// the loop routine runs over and over again forever:
void loop() {
  
  int sensorValue = analogRead(A15);
  // Convert the analog reading (which goes from 0 - 1023) to a voltage (0 - 5V):
  float voltage = sensorValue * (5.0 / 1023.0);
  // print out the value you read:
  Serial.println(voltage);
  if (voltage < 1){
    showRed();
  }else if (voltage > 1.01 && voltage < 2.0) {
    showGreen();
  } else {
    turnOff();
  }

//  digitalWrite(ledRed, HIGH);   
}

void showRed(){
  digitalWrite(ledGreen, LOW);
  digitalWrite(ledRed, HIGH);   
}

void showGreen(){
  digitalWrite(ledRed, LOW);
  digitalWrite(ledGreen, HIGH);   
}

void turnOff(){
  digitalWrite(ledRed, LOW);
  digitalWrite(ledGreen, LOW);   
}
