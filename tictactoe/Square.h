
#ifndef Square_h
#define Square_h

#include "Arduino.h"

class Square
{
	public:
		Square(uint8_t analogPin, uint8_t position, uint8_t dataPin, uint8_t clockPin, uint8_t latchPin, uint8_t numOfRegisters);
		void calibrateCNY70(int maxGreen, int minGreen, int maxRed, int minRed);
		void setGreen();
		void setRed();
		void clear();
		char checkColor();
                int getIndex();
                int getPosition();
		
		

	private:
                int _analogPin;
                int _position;
                int _index; 
                int greenIndex;
                int redIndex;
                int colorValue;
                char color;
                int _maxGreen;
                int _minGreen;                
                int _maxRed;
                int _minRed;

		int calculateIndex();
		void writeValue(int index, int value);
                int getGreenIndex(int pos);
		

};

#endif
