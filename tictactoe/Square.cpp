

#include "Arduino.h"
#include <Shiftduino.h>
#include "Square.h"

char RED = 'r';
char GREEN = 'g';
char NONE = 'n';

int _analogPin;
int _position; // from 1 to 9

//Squares 1 to 3 are in shiftreg 1
//Squares 4 to 6 are in shiftreg 2
//Squares 7 to 9 are in shiftreg 3
//In each shiftreg there is one unused square position
int _index; 

//Each square has two leds, green and red
//The index is the position of the red led and index-1 the position of the green led 
int greenIndex;
int redIndex;

int colorValue;
char color; //g = green, r = red, n = none

//Calibration for the CNY70
int _maxGreen;
int _minGreen;

int _maxRed;
int _minRed;

Shiftduino * sd;


//************* PUBLIC ************************
Square::Square(uint8_t analogPin, uint8_t position, uint8_t dataPin, uint8_t clockPin, uint8_t latchPin, uint8_t numOfRegisters){

	sd = new Shiftduino(dataPin, clockPin , latchPin, numOfRegisters);
	
	_analogPin = analogPin;
	_position = position;
	
	_index = calculateIndex();
	
	greenIndex = _index - 1;
	redIndex = _index;
	
	//Reset the leds
	clear();
	
	_maxGreen = 0;
	_minGreen = 0;
	_maxRed = 0;
	_minRed = 0;
	colorValue = 0;
	color = 'n';	
}

void Square::calibrateCNY70(int maxGreen, int minGreen, int maxRed, int minRed){
	_maxGreen = maxGreen;
	_minGreen = minGreen;
	_maxRed = maxRed;
	_minRed = minRed;
}

void Square::setGreen(){
	writeValue(greenIndex, 1);
	writeValue(redIndex, 0);
}

void Square::setRed(){
	writeValue(greenIndex, 0);
	writeValue(redIndex, 1);
}

void Square::clear(){
	writeValue(greenIndex, 0);
	writeValue(redIndex, 0);
}   

char Square::checkColor(){
	colorValue = analogRead(_analogPin);
	if (colorValue >= _minGreen && colorValue <= _maxGreen){
		color = GREEN;
	} else if (colorValue >= _minRed && colorValue <= _maxRed){
		color = RED;
	} else {
		color = NONE;
	}
	
	return color;
}



//************* PRIVATE ************************

int Square::calculateIndex(){
	int index = 0;
	if (_position < 4){
		index = _position;
	}else if (_position > 3 && _position < 7){
		index = _position + 1;
	}else if (_position > 6){
		index = _position + 2;
	}
	
	return index;
} 

void Square::writeValue(int index, int value){
	sd->setPin(index, value);
}

      




