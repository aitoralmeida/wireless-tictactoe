/*
  Library to control the 74HC595
  More info at http://bildr.org/?s=74hc595
*/

#ifndef Shift_h
#define Shift_h

#include "Arduino.h"

class Shift
{
  public:
	void initialize();
	void clearRegisters();
	void writeRegisters();
	void setRegisterPin(int index, int value);
};

#endif