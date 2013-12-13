

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
//int _index; 

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
Square::Square(uint8_t analogPin, uint8_t pos, uint8_t dataPin, uint8_t clockPin, uint8_t latchPin, uint8_t numOfRegisters){

	sd = new Shiftduino(dataPin, clockPin , latchPin, numOfRegisters);
	
	_analogPin = analogPin;
	_position = pos;
	
	//_index = calculateIndex();
	
	greenIndex = getGreenIndex(pos);
	redIndex = greenIndex+1;
	
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
        clear();
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

int Square::getIndex(){
        return _index;
}

int Square::getPosition(){
        return _position;
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

/*
This one is a bit tricky. The green and red pin index of each square depend on the square postion ([1..9]),
but they are not consecutive, becouse there is a "gap" in each shift register. 

Register 1: Squares 1, 2 and 3
Register 2: Squares 4, 5 and 6
Register 3: Sqyares 7, 8 and 9

So the pins for the led will be (green and red):
Square 1: 0 and 1
Square 2: 2 and 3
Square 3: 4 and 5
-gap-
Square 4: 8 and 9
Square 5: 10 and 11
Square 6: 12 and 13
-gap
Square 7: 16 and 17
Square 8: 18 and 19
Square 9: 20 and 21

*/
int Square::getGreenIndex(int pos){
  if (pos == 1){
    return 0;
  }
  else if (pos == 2){
    return 2;
  }
  else if (pos == 3){
    return 3;
  }
  else if (pos == 4){
    return 4;
  }
  else if (pos == 5){
    return 10;
  }
  else if (pos == 6){
    return 12;
  }
  else if (pos == 7){
    return 16;
  }
  else if (pos == 8){
    return 18;
  }
  else if (pos == 9){
    return 20;
  }
  
}

      




