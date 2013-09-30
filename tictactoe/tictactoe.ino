#include <Shiftduino.h>

int numShifts = 3;
Shiftduino sd(30, 31 , 32, numShifts);

//cny70 pins
int colorPins[9] = {0,1,2,3,4,5,6,7,8};

//read color values
int color[9];

//Color limits
int redLow = 45;
int redHigh = 55;
int greenLow = 100;
int greenHigh = 115;

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

//Game control variables
int gameStatus = 0; //0 = ongoing game, 1 = you win, 2 = opponent wins
int currentTurn = 0; //0 you, 1 opponent




void setup() {  
  Serial.begin(9600);
  sd.clear();
}

// the loop routine runs over and over again forever:
void loop() {
  if (gameStatus == 0){ 
    checkColors();
    setLeds();
    gameStatus = checkVictoryConditions(); 
  } else if (gameStatus == 1){
    victoryLoop(0,1);
  } else if (gameStatus == 2){
    victoryLoop(1,0);
  }
}

//Blink leds when endgame is reached
void victoryLoop(int redValue, int greenValue){
  sd.clear();
  while(true){ //TODO change this
    for (int i = 0; i < 9; i++){
      sd.setPin(redPins[i],redValue);
      sd.setPin(greenPins[i],greenValue);
      delay(50);
      sd.setPin(redPins[i],0);
      sd.setPin(greenPins[i],0);  
    }
  }
}

//Set led values to used chip
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

//Checks the color of the used chip
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

//identifies a color from the cny70 value
char analyzeColor(int value){
  char color = 'x';
  if (color > redLow && color< redHigh){
    color = 'r';
  } else if (color > greenLow && color< greenHigh){
    color = 'g';
  }
  return color;
}

//0 = ongoing game, 1 = you win, 2 = opponent wins
int checkVictoryConditions(){

}


