#include <Shiftduino.h>
#include "Square.h"

#include <Wire.h>
#include <GraphicDisplay.h>
#include <SM125.h> //RFID
#include <SoundPlayer.h>

//Game ID
#define GAME_ID 0x01 //tictactoe
//Board ID
#define BOARD_ID 0x01 //TODO: Change it for each board


// Hardware addresses
#define DISPLAY_ADDRESS  0x51
#define BUTTONS_CHANGE   43
#define SM125_ADDRESS 0x46
// Sound files
#define WELCOME 0x1 //Welcome screen
#define YOU_WIN 0x2 //The Player wins :-)
#define YOU_LOSE 0x3 //The Rival wins :-(
#define VICTORY_LOOP 0x4 //Victory loop!!!
#define NO_NO 0x7 //Illegal move
// Image addresses
#define GAME_OVER 0x0049
#define WINNER 0x00DB
#define TICTACTOE 0x0092
#define DOWN_OFFSET 11 // How many pixels we are leaving for the status bar

// Communication msgs
String MSG_REGISTER = "REGISTER"; // id
String MSG_FIND_GAME = "FIND_GAME"; // id
String MSG_DO_MOVE =  "DO_MOVE"; // id movement(0..8)
String MSG_GET_MOVE = "GET_MOVE"; // id
String END_CHAR = "\0d"; 
String SEP_CHAR_COMMANDS = " ";
String SEP_CHAR_PARAMETERS = ",";
// Server commands
String CMD_WAITING_PLAYERS = "WAITING_FOR_PLAYERS";
String CMD_RIVAL = "RIVAL"; // rival_id turn(0/1)
String CMD_NOT_REGISTERED = "NOT_REGISTERED";
String CMD_WAIT_FOR_MOVE = "WAIT_FOR_MOVE";
String CMD_WAIT_FOR_RIVAL = "WAIT_FOR_RIVAL";
String CMD_WAIT_FOR_TURN = "WAIT_FOR_TURN";
String CMD_RIVAL_MOVE = "RIVAL_MOVE"; // # rival_id move
String CMD_ACK = "ACK"; // rival_id move


// Baseboard screen
GraphicDisplay display;
// Baseboard RFID reader
SM125 sm125;
// Baseboard sound player
SoundPlayer soundPlayer;

// Configuration for the shift registers
int numShifts = 3;
int datapin = 22;
int clockpin = 24;
int latchpin = 23;

Shiftduino _sd(datapin, clockpin, latchpin, numShifts);

// Unique board ID (TODO: right now is set manually for testing purposes)
String player_id = "1000";
// Rival board id
String rival_id = "0";
// Who starts playing: 0 the player, 1 the rival.
String start_turn = "2";

// Current turn
int turn = 0;

// Each of the squares of the tictactoe
Square * squares[9];

//int gameStatus = 0; //0 = ongoing game, 1 = you win, 2 = opponent wins
//int currentTurn = 0; //0 you, 1 opponent

// square status
int redStatus [] = {0,0,0,0,0,0,0,0,0};
int greenStatus [] = {0,0,0,0,0,0,0,0,0};
int previousGreenStatus [] = {0,0,0,0,0,0,0,0,0};

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

// hardware initialization
void setup() {  
  Serial.begin(9600);
  Serial.println("Setup...");
  // 1 - Initialize the tictactoe board
  for (int i = 0; i < 9; i++){
    squares[i] = new Square(8 - i, i + 1, &_sd);
  }
  calibrateSensors();
  clearSquares();
  allGreen();
  delay(1000);
  clearSquares();
  // 2 - Initialize the baseboard peripherals 
  initializeHardwarePeripherals();
  // 3 - Show the welcome message
  doWelcome();  
  // 4 - Register the player in the server
  registerPlayer();
  // 5 - Find other player
  display.printText("Waiting for players...", 6, 1, GREEN);
  findPlayer();
}

void loop() {
  delay(200);
  boolean waitForMove = true;
  
  if ((turn > 0) || (turn == 0 && start_turn=="0")){
    // ************************************************************************
    // ******** Player turn ***************************************************
    // ************************************************************************
    for (int i = 0; i < 9; i++){
      previousGreenStatus[i] = greenStatus[i];
    }
    // 1 -Update color status
    //   -Check player move, wait until it happens
    printMsg("Your turn.");
      
    int player_move = 0;
    
    while (waitForMove){
      updateBoardStatus();  
      player_move = checkPlayerMove();
      if (player_move == -1){
        // No move, wait and check again
        delay(30);
      }else if (player_move > -1){
        // Legal move, continue
        waitForMove = false;
      } else if (player_move == -2){
        // Player has moved 2 pieces, illegal move      
        msgAndWait("You moved two pieces! Correct you move.");
        soundPlayer.play(NO_NO);      
      } else if (player_move == -3){
        // Player has moved the piece into an square already used.     
        msgAndWait("You moved your piece into an already used square! Correct you move.");
        soundPlayer.play(NO_NO);  
      }
    }
  
    // 2- Send move to server  
    sendMove(player_move);
    
    // 3 -Check victory conditions
    greenVictory = checkWin(greenStatus);
    if (greenVictory){
      //printMsg("A winner is you");
      display.drawImage(0x0000, WINNER, 0, DOWN_OFFSET); 
      soundPlayer.play(YOU_WIN);
      victoryLoop('g');
    }
  }
  
  if ((turn > 0) || (turn == 0 && start_turn=="1")){
    // ************************************************************************
    // ******* Rival turn *****************************************************
    // ************************************************************************
    printMsg("Wait for your rival's move.");
    
    // 1- Check rival move, wait until it happens
    int rival_move = -1;
    rival_move = getMove();
      
    // 2- Update color status
    if (rival_move > -1 && rival_move  < 9){
      redStatus[rival_move] = 1;
      squares[rival_move]->setRed();
    }
    
    // 3 -Check victory conditions
    redVictory = checkWin(redStatus);
    if (redVictory){
      //printMsg("You lose!");
      display.drawImage(0x0000, GAME_OVER, 0, DOWN_OFFSET); 
      soundPlayer.play(YOU_LOSE); 
      victoryLoop('r');
    }
  }
}

/**********************************************
 *   BASEBOARD HARDWARE INITIALIZATION
 **********************************************/
 
void initializeHardwarePeripherals(){
  initializeScreen();
  display.printText("Initializing...", 6, 1, GREEN);
  initializeRFID();
  initializeSoundPlayer();
  initializeBoard();
}

void initializeBoard(){
  // TODO program it once the protocol is defined in the board
  // Send game ID
  // Send board ID
}

void initializeRFID(){
  sm125.begin(SM125_ADDRESS);
}

void initializeSoundPlayer(){
  soundPlayer.begin(0x61);
  int ver = soundPlayer.getVersion();  
  switch(ver)
  {
  case 0x10:
    Serial.println("Version: 1.0)");
    break;
  }
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
 *   SCREEN PRONTS
 **********************************************/
 


void doWelcome(){
  display.clear();
  soundPlayer.stop();
  //display.printText("Welcome to the tictactoe game!", 6, 1, BLUE);
  display.drawImage(0x0000, TICTACTOE, 0, DOWN_OFFSET);
  soundPlayer.play(WELCOME);
  delay(1000);
  display.clear();
  display.printText("Press the green button to find another player...", 6, 1, GREEN);
  boolean doLoop = true;
  while(doLoop){
    if (display.buttonsChanged()) {
      byte reply = display.getButtons();
      // Green button
      if ((reply & 0x04) == 0x00){
          doLoop = false;
          soundPlayer.stop();
      }
    }  
  }
  display.clear();
  display.printText("Starting a new game!", 6, 1, GREEN);
  delay(300); 
  
}

void msgAndWait(String msg){
  display.clear();
  display.printText(msg, 6, 1, RED);
  boolean doLoop = true;
  while(doLoop){
    if (display.buttonsChanged()) {
      byte reply = display.getButtons();
      // Green button
      if ((reply & 0x04) == 0x00){
          doLoop = false;
      }
    }  
  }
  display.clear(); 
}

void printMsg(String msg){
  display.clear();
  display.printText(msg, 6, 1, BLUE);
}

/**********************************************
 *   COMMUNICATION WITH THE GAME SERVER
 **********************************************/
 
void registerPlayer(){
  Serial.println("Registering player...");
  sendMsg(MSG_REGISTER + SEP_CHAR_COMMANDS + player_id + END_CHAR);
}

void findPlayer(){
  Serial.println("Finding other player...");
  boolean notFound = true;
  while(notFound){
    String res = sendAndWait(MSG_FIND_GAME + SEP_CHAR_COMMANDS  + player_id + END_CHAR);
    String command = getCommand(res);
    
    // if waiting_for_player -> delay 100
    // if not_registered -> register
    // if rival -> set start_turn, set rival_id, notFound = false
    if (command == CMD_WAIT_FOR_RIVAL){
      Serial.println("Waiting for rivals...");
      delay(1000);
    } else if (command == CMD_NOT_REGISTERED){ // This shouldn't happen
      Serial.println("Not registered");
      registerPlayer();
    } else if (command == CMD_RIVAL){
      Serial.println("Rival found");
      String parameters = getParameters(res);
      rival_id = getParameter(parameters, 1);
      start_turn = getParameter(parameters, 2);
      notFound = false;
    }
  }
}

int getMove(){
  Serial.println("Getting rival's move...");
  boolean notMove = true;
  int rivalMove = -1;
  
  while(notMove){
    String res = sendAndWait(MSG_GET_MOVE + SEP_CHAR_COMMANDS + player_id + END_CHAR);
    String command = getCommand(res);
    
    //if wait_for_move -> delay 1000
    //if rival_move -> notFound = false, rival_move = res 2
    if (command == CMD_WAIT_FOR_MOVE){
      Serial.println("Waiting for rival's move...");
      delay(1000);
    } else if (command == CMD_RIVAL_MOVE){
      String parameters = getParameters(res);
      String strMove = getParameter(parameters, 2);      
      rivalMove = strMove.toInt();
      notMove = false;
    }    
  }
    
  return rivalMove;
}

void sendMove(int player_move){
  Serial.println("Sending move...");
  boolean notReceived = true;
  
  //If ack -> notReceived = false
  //If wait_for_rival -> delay 1000
  while(notReceived){}
    String res = sendAndWait(MSG_DO_MOVE + SEP_CHAR_COMMANDS + player_id + SEP_CHAR_PARAMETERS  + player_move +  END_CHAR);
    String command = getCommand(res);
    if (command = CMD_ACK){
      Serial.println("Sent");
      notReceived = false;
    } else if (command == CMD_WAIT_FOR_RIVAL){
      Serial.println("Waiting for the rival to retrieve previous move...");
      delay(1000);
    }  
}

void sendMsg(String msg){
  //TODO
}
String sendAndWait(String msg){ 
  //TODO
  return "foo"; 
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

// -1 -> no move
// -2 -> illegal move, 2 pieces moved
// -3 -> illegal move, moved into a square used by the rival
// 0..8 -> legal move
int checkPlayerMove(){
  int totalMoves = 0;
  int player_move = 0;
  char color;
  for(int i = 0; i < 9; i++){
    color = squares[i]->checkColor();
    if (previousGreenStatus[i] != greenStatus[i]){
      totalMoves++;
      player_move = i;
    }
  }
  
  if (totalMoves == 0) {
    // The player has not done any move
    player_move = -1;
  } else if (totalMoves > 1) {
    // The player has moved 2 pieces, which is an illegal move
    player_move = -2;
  }
  else if (color == 'r'){
    // The player has moved his piece into an square already used by the rival
    player_move = -3;
  }
  
  return player_move;  
}

void updateBoardStatus(){
  for (int i=0; i < 9; i++){
    char color = squares[i]->checkColor();
    if (color == 'g'){
      squares[i]->setGreen();
      greenStatus[i] = 1;
    }
  }
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
    } else{
      squares[i]->clear();
      redStatus[i] = 0;
      greenStatus[i] = 0;
    }
  }  
}

boolean isSame(int array1[], int array2[]){
  boolean same = true;
  int size1 = sizeof(array1);
  int size2 = sizeof(array2);
    if (size1 == 9 && size2 == 9){
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
  } else {
    same = false;
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
  boolean doVictory = true;
  soundPlayer.play(VICTORY_LOOP);
  while (doVictory){
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
    
    printMsg("Press green button for another game");
    if (display.buttonsChanged()) {
      byte reply = display.getButtons();
      // Green button
      if ((reply & 0x04) == 0x00){
          doVictory = false;
          soundPlayer.stop();
          // new game!
          restartGame(); 
      }
    }
  }
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
  
  // initialize rival id
  rival_id = 0;

  // initialize status
  for (int i=0; i < 9; i++){
    redStatus[i] = 0;
    greenStatus[i] = 0;
    previousGreenStatus[i] = 0;
  } 
  
  // initialize squares
  for (int i = 0; i < 9; i++){
    squares[i] = new Square(8 - i, i + 1, &_sd);
  }
  
  // initialize victory
  redVictory = false;
  greenVictory = false;
  
  // Show the welcome message
  doWelcome();  
  
  // Find other player
  findPlayer();
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

/**********************************************
 *   COMMUNICATION METHODS
 **********************************************/

// getCommand
//
// Get the command string of the message
String getCommand(String message) {

  int index = message.indexOf(' ');
  if (index != -1)
    return message.substring(0, index);
  else
    return message;
}

// getParameters
//
// Get the parameters of the message
String getParameters(String message) {

  int index = message.indexOf(' ');
  if (index != -1)
    return message.substring(index + 1);
  else
    return "";
}

// numberOfParameters
//
// Get tne number of parameters
int numberOfParameters(String parameters) {

  if (parameters.equals(""))
    return 0;
  else {
    String temp = parameters;
    int count = 0, index;
    do {
      index = temp.indexOf(',');
      temp = temp.substring(index + 1);
      count++;
    }
    while (index >= 0);
    return count;
  }
}

// getParamenter
//
// Get a parameter defined for its position (1, 2, 3...)
String getParameter(String parameters, int position) {

  String temp = parameters, parameter;
  int count = 0, index;
  do {
    index = temp.indexOf(',');
    parameter = temp.substring(0, index);
    temp = temp.substring(index + 1);
    count++;
  }
  while (count != position);
  return parameter;
}




