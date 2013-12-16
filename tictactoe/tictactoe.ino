#include <Shiftduino.h>
#include "Square.h"

int numShifts = 3;
int datapin = 22;
int clockpin = 24;
int latchpin = 23;

Shiftduino _sd(datapin, clockpin, latchpin, numShifts);
Square * squares[9];

//int gameStatus = 0; //0 = ongoing game, 1 = you win, 2 = opponent wins
//int currentTurn = 0; //0 you, 1 opponent

int redStatus [] = {0,0,0,0,0,0,0,0,0};
int greenStatus [] = {0,0,0,0,0,0,0,0,0};

int winConditions [][9] = {{1,1,1,0,0,0,0,0,0},
                              {0,0,0,1,1,1,0,0,0},
                              {0,0,0,0,0,0,1,1,1},
                              {1,0,0,1,0,0,1,0,0},
                              {0,1,0,0,1,0,0,1,0},
                              {0,0,1,0,0,1,0,0,1},
                              {1,0,0,0,1,0,0,0,1},
                              {0,0,1,0,1,0,1,0,0}
};

boolean redVictory = false;
boolean greenVictory = false;

void setup() {  
  Serial.begin(9600);
  for (int i = 0; i < 9; i++){
    squares[i] = new Square(8 - i, i + 1, &_sd);
  }
  calibrateSensors();
  clearSquares();
  allGreen();
  delay(1000);
  clearSquares(); 
  
}

// the loop routine runs over and over again forever:
void loop() {
  if (!redVictory && !greenVictory){
    updateColorStatus();
  }
  
  redVictory = checkWin(redStatus);
  greenVictory = checkWin(greenStatus);
 
  if (redVictory){
    clearSquares();
    allRed();
  } else if (greenVictory){
    clearSquares();
    allGreen();
  }
  
  //Serial.println();  
}


//The calibration changes from sensor to sensor
void calibrateSensors(){
  squares[0]->calibrateCNY70(450, 400, 350, 290);
  squares[1]->calibrateCNY70(230, 150, 140, 70);
  squares[2]->calibrateCNY70(280, 200, 140, 70);
  squares[3]->calibrateCNY70(410, 370, 250, 210);
  squares[4]->calibrateCNY70(250, 210, 150, 100);
  squares[5]->calibrateCNY70(370, 320, 220, 180);
  squares[6]->calibrateCNY70(350, 300, 220, 180);
  squares[7]->calibrateCNY70(470, 430, 370, 310);
  squares[8]->calibrateCNY70(330, 290, 220, 180);  
}

void updateColorStatus(){
  for (int i=0; i < 9; i++){
    char color = squares[i]->checkColor();
    if (color == 'g'){
      squares[i]->setGreen();
      greenStatus[i] = 1;
    } else if (color == 'r'){
      squares[i]->setRed();
      redStatus[i] = 1;
    } else if (color == 'n'){
      squares[i]->clear();
    }
  }  
}

boolean isSame(int array1[], int array2[]){
  boolean same = true;
  for (int i=0; i < 9; i++){
    if (same==false){
      break;
    }
    if (array1[i] == array2[i]){
      same = true;
    } else {
      same = false;
    }
  }  
  return same;
} 


boolean checkWin(int statusArray[]){
  boolean win = false;
  for (int i=0; i < 8; i++){
    if (win){
      break;
    }
    //Mask to only take the needed values
    int result[] = {0,0,0,0,0,0,0,0,0};
    for (int j=0; j < 9; j++){
      result[j] = statusArray[j] * winConditions[i][j];
    }  
    win = isSame(result, winConditions[i]); 
  }
  return win;
  
}

void clearSquares(){
  for (int i=0; i < 9; i++){
    squares[i]->clear();
  }
}

void allRed(){
  for (int i=0; i < 9; i++){
    squares[i]->setRed();
  }
}

void allGreen(){
  for (int i=0; i < 9; i++){
    squares[i]->setGreen();
  }
}

void printStatus(){
  Serial.println("Red");
  for (int i=0; i < 9; i++){
    Serial.println(redStatus[i]);
  }
  Serial.println();
  
  Serial.println("Green");
  for (int i=0; i < 9; i++){
    Serial.println(greenStatus[i]);
  }
  Serial.println();
}


