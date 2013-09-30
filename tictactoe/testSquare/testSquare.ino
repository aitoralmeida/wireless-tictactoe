int redPin = 46;  
int greenPin = 47;     
int colorPin = 0; 
int color = 0;

void setup()
{
  pinMode(redPin, OUTPUT); 
  pinMode(greenPin, OUTPUT);
  Serial.begin(9600);  
}

void red()
{
  digitalWrite(redPin, HIGH); 
  digitalWrite(greenPin, LOW);  
}

void green()
{
  digitalWrite(redPin, LOW); 
  digitalWrite(greenPin, HIGH);  
}

void none()
{
  digitalWrite(redPin, LOW); 
  digitalWrite(greenPin, LOW);  
}


void loop()
{
  color = analogRead(colorPin); 
  Serial.println(color);
  //Piezas plastico pintadas

  if (color < 560 && color > 510 ){
    green();
  } else if(color < 510 && color > 450 ){
    red();
  }else{
    none();
  }

  
  //Piezas esca imprimidas
  /*
  if (color < 265 && color > 250 ){
    green();
  } else if(color < 250 && color > 240 ){
    red();
  }else{
    none();
  }
  */
}
