

int ledRed = 24;
int ledGreen = 22;

// the setup routine runs once when you press reset:
void setup() {                
  // initialize the digital pin as an output.
  pinMode(ledRed, OUTPUT);     
  pinMode(ledGreen, OUTPUT);   
}

// the loop routine runs over and over again forever:
void loop() {
  int wait = 500;
  digitalWrite(ledRed, HIGH); 
  delay(wait);               
  digitalWrite(ledRed, LOW);    
  delay(wait);   
  digitalWrite(ledGreen, HIGH); 
  delay(wait);               
  digitalWrite(ledGreen, LOW);    
  delay(wait);     
}
