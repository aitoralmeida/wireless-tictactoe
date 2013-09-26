#include <Shiftduino.h>

int numShifts = 3;
Shiftduino sd(30, 31 , 32, numShifts);

//cny70 pins
int colorPins[9] = {0,1,2,3,4,5,6,7,8};

//read color values
int color[9];

//Color limits
int redLow = 0;
int redHigh = 0;
int greenLow = 0;
int greenHigh = 0;

//Reds, 0 or 1
int reds[9] = {0,0,0,0,0,0,0,0,};

//Greens, 0 or 1
int greens[9] = {0,0,0,0,0,0,0,0,};

int winConditions[][9] ={{1,1,1,0,0,0,0,0,0},{0,0,0,1,1,1,0,0,0},
                        {0,0,0,0,0,0,1,1,1},{1,0,0,1,0,0,1,0,0},
                        {0,1,0,0,1,0,0,1,0},{0,0,1,0,0,1,0,0,1},
                        {1,0,0,0,1,0,0,0,1},{0,0,1,0,1,0,1,0,0}};

//Led pins
int redPins[9] = {0,2,4,6,8,10,12,14,16};
int greenPins[9] = {1,3,5,7,9,11,13,15,17};



void setup() {  
  Serial.begin(9600);
  sd.clear();
}

// the loop routine runs over and over again forever:
void loop() {
  checkColors();
  setLeds();
}

void setLeds(){
  for(int i = 0; i < 9; i++){
    if (reds[i] == 1){
      sd.setPin(redPins[i],1);
      sd.setPin(greenPins[i],0);
    } else if(greens[i]== 1){
      sd.setPin(redPins[i],0);
      sd.setPin(greenPins[i],1);
    } else{
      sd.setPin(redPins[i],0);
      sd.setPin(greenPins[i],0);
    }
  }
}

void checkColors(){
  for(int i = 0; i < 9; i++){
    int value = analogRead(colorPins[i]);  
    char color = analyzeColor(value);
    if (color = 'r'){
      reds[i] = 1;
      greens[i] = 0;
    } else if (color = 'g'){
      reds[i] = 0;
      greens[i] = 1;
    } else{
      reds[i] = 0;
      greens[i] = 0;
    }
   
  }
}

char analyzeColor(int value){
  char color = 'x';
  if (color > redLow && color< redHigh){
    color = 'r';
  } else if (color > greenLow && color< greenHigh){
    color = 'g';
  }
  return color;
}


