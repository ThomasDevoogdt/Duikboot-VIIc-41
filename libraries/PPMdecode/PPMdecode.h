/********************************************************************************
PPMdecode.h - Library for PPM decoding.
Created by Thomas D. & David D. , 2015.
With special thanks to Eddy Caron.
Released into the public domain.
********************************************************************************/

#ifndef PPMdecode_h
#define PPMdecode_h

//#include "Arduino.h"

#if defined(ARDUINO) && ARDUINO >= 100
#include <Arduino.h>
#else
#include <WProgram.h>
#endif

#define maxPPMread 6
#define maxChannel 20

/********************************************************************************
Class
********************************************************************************/

class PPMdecode
{
public:
	PPMdecode(short pin, short channels);
	boolean synchronized;
	void PWMstore(); //Please do not use!
	void SetDefaultValues(short defaultValue[maxChannel]);
	short channel[maxChannel];
	boolean error;

private:
	short _pin, _channels;
	short _defaultValue[maxChannel];
	long nowMs, lastMs; 
	int diffMs;
	short currentChannel;
};

/********************************************************************************
Static
********************************************************************************/

//Please, dont use in your arduino sketch, this is static.
//copy of instances (needed for interrupts)

//I hate doing this, but the attachInterrupt needs this.
static PPMdecode(*dr[maxPPMread]);
static void update0(), update1(), update2(), update3(), update4(), update5();
static void(*voidList[maxPPMread])() = { update0, update1, update2, update3, update4, update5 };
static short reference = 0;

#endif
