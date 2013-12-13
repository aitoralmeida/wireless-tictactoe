
#ifndef Square_h
#define Square_h

#include "Arduino.h"
#include <Shiftduino.h>

class Square
{
	public:
		Square(uint8_t analogPin, uint8_t pos, Shiftduino * reg);
                void calibrateCNY70(int maxGreen, int minGreen, int maxRed, int minRed);
		void setGreen();
		void setRed();
		void clear();
		char checkColor();
                int getPosition();
                int getGreenIndex();
                int getRedIndex();
		
		

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
                Shiftduino * sd;

		int calculateIndex();
		void writeValue(int index, int value);
                int calculateGreenIndex(int pos);
		

};

#endif
