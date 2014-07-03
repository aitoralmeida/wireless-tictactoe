#include <Shiftduino.h>
#include "Square.h"

#include <Wire.h>
#include <GraphicDisplay.h>
#include <SM125.h>

#define DISPLAY_ADDRESS  0x51
#define BUTTONS_CHANGE   43
#define SM125_ADDRESS 0x46

// Baseboard screen
GraphicDisplay display;
// Baseboard RFID reader
SM125 sm125;

// Configuration for the shift registers
int numShifts = 3;
int datapin = 22;
int clockpin = 24;
int latchpin = 23;

Shiftduino _sd(datapin, clockpin, latchpin, numShifts);

// Unique board ID (TODO: right now is set manually for testing purposes)
int player_id = 1;
int rival_id = 0;

// Each of the squares of the tictactoe
Square * squares[9];

//int gameStatus = 0; //0 = ongoing game, 1 = you win, 2 = opponent wins
//int currentTurn = 0; //0 you, 1 opponent

// square status
int redStatus [] = {0,0,0,0,0,0,0,0,0};
int greenStatus [] = {0,0,0,0,0,0,0,0,0};

// conditions for victory
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
  Serial.println("Setup...");
  // Initialize the tictactoe board
  for (int i = 0; i < 9; i++){
    squares[i] = new Square(8 - i, i + 1, &_sd);
  }
  calibrateSensors();
  clearSquares();
  allGreen();
  delay(1000);
  clearSquares();
  // Initialize the baseboard peripherals 
  initializeHardwarePeripherals();
  // Show the welcome message
  doWelcome();  
  // Find other player
  rival_id = findPlayer(player_id);
}

// the loop routine runs over and over again forever:
void loop() {
  delay(200);

  
  /*
  if (!redVictory && !greenVictory){
    updateColorStatus();
  }
  
  redVictory = checkWin(redStatus);
  greenVictory = checkWin(greenStatus);
 
  if (redVictory){
    victoryLoop('r');
  } else if (greenVictory){
    victoryLoop('g');
  }
  */

}

/**********************************************
 *   BASEBOARD HARDWARE INITIALIZATION
 **********************************************/
 
void initializeHardwarePeripherals(){
  initializeRFID();
  initializeScreen();
}

void initializeRFID(){
  sm125.begin(SM125_ADDRESS);
}

void initializeScreen(){
  pinMode(BUTTONS_CHANGE, INPUT);
  display.begin(DISPLAY_ADDRESS, BUTTONS_CHANGE);
  //wait for display initialization
  do {
    delay(50);
  }
  while(!display.isReady());
  //get firmware version
  int ver = display.getVersion();
  switch(ver)
  {
  case 0x10:
    Serial.println("Version 1.0");
    break;
  }

  display.clear();
}

/**********************************************
 *   WELCOME SCREEN
 **********************************************/

void doWelcome(){
  display.printText("Welcome to the tictactoe game!", 6, 1, BLUE);
  delay(1000);
  display.clear();
  display.printText("Press green button to find another player...", 6, 1, GREEN);
  boolean doLoop = true;
  while(doLoop){
    if (display.buttonsChanged()) {
      byte reply = display.getButtons();
      if ((reply & 0x04) == 0x00){
          doLoop = false;
      }
    }  
  }
  display.clear();
  display.printText("Starting new game!", 6, 1, GREEN);
  
  
}

/**********************************************
 *   GAME INITIALIZATION WITH THE SERVER
 **********************************************/

int findPlayer(int player_id){
  //TODO
  Serial.println("finding player");
  return 113;

}


/**********************************************
 *   CNY70 SENSOR CALIBRATION
 **********************************************/

// The calibration changes from sensor to sensor
// The values are unique for each board and sensor
void calibrateSensors(){
  squares[0]->calibrateCNY70(460, 400, 360, 280);
  squares[1]->calibrateCNY70(240, 150, 145, 60);
  squares[2]->calibrateCNY70(290, 200, 150, 60);
  squares[3]->calibrateCNY70(420, 370, 260, 200);
  squares[4]->calibrateCNY70(260, 210, 160, 90);
  squares[5]->calibrateCNY70(380, 320, 230, 170);
  squares[6]->calibrateCNY70(350, 300, 230, 170);
  squares[7]->calibrateCNY70(480, 430, 380, 300);
  squares[8]->calibrateCNY70(340, 290, 230, 170);  
}


/**********************************************
 *   GAME LOGIC
 **********************************************/

void updateColorStatus(){
  for (int i=0; i < 9; i++){
    char color = squares[i]->checkColor();
    if (color == 'g'){
      squares[i]->setGreen();
      greenStatus[i] = 1;
    } else if (color == 'r'){
      squares[i]->setRed();
      redStatus[i] = 1;
    } else{
      squares[i]->clear();
      redStatus[i] = 0;
      greenStatus[i] = 0;
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

void checkColor(int s){
  int colorValue = squares[s]->getColorValue();
  Serial.println(colorValue);
}

void victoryLoop(char color){
  for (int i=0; i < 9; i++){
    clearSquares();
    if (color == 'g'){
      squares[i]->setGreen();
    } else if (color == 'r'){
      squares[i]->setRed();
    } 
    delay(100);
  }
  
  if (color == 'g'){
      allGreen();
  } else if (color == 'r'){
      allRed();
  } 
  delay(100);
  
}

void restartGame(){
  clearSquares();
  allGreen();
  delay(200);
  allRed();
  delay(200);
  allGreen();
  delay(600);
  clearSquares();

  redVictory = false;
  greenVictory = false;
}

/**********************************************
 *   COLOR CONTROL METHODS
 **********************************************/

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




