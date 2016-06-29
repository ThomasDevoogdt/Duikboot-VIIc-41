/********************************************************************************
PPMdecode.h - Library for PPM decoding.
Created by Thomas D. & David D. , 2015.
With special thanks to Eddy Caron.
Released into the public domain.
********************************************************************************/

#include "Arduino.h"
#include "PPMdecode.h"

#define minPuls 1000		//탎
#define maxPuls 2000		//탎
#define maxSafeOffset 200	//탎
#define initTime 5000		//탎
#define initValue 50		//%

/********************************************************************************
Setup
********************************************************************************/

PPMdecode::PPMdecode(short pin, short channels)
{
	_pin = pin;
	_channels = channels;
	error = false;

	for (short i = 0; i < _channels; i++)
	{
		_defaultValue[i] = initValue;
		channel[i] = _defaultValue[i];
	}
	lastMs = 0;

	dr[reference] = this;
	attachInterrupt(_pin, voidList[reference], RISING);
	reference++;
}

void PPMdecode::SetDefaultValues(short defaultValue[maxChannel])
{
	for (short i = 0; i < _channels; i++)
	{
		_defaultValue[i] = defaultValue[i];
	}
}

void PPMdecode::PWMstore(){
	nowMs = micros();
	diffMs = nowMs - lastMs;
	if ((lastMs > 0) && (diffMs > initTime)){
		currentChannel = -1;
		synchronized = true;
		error = false;
	}
	else  if (synchronized && (currentChannel < (_channels + 1)) && (diffMs > minPuls) && (diffMs < maxPuls))
	{
		channel[currentChannel] = map(diffMs, minPuls, maxPuls, 0, 100);	
	}
	else if (synchronized && (currentChannel < (_channels + 1)) && !((diffMs > minPuls - maxSafeOffset) && (diffMs < maxPuls + maxSafeOffset))) 
	{
		error = true;
		currentChannel = 0;
		synchronized = false;

		for (short i = 0; i < _channels; i++)
		{
			channel[i] = _defaultValue[i];
		}
	}
	else if(!synchronized && currentChannel > 10){ //Maximum Failed Attempts
		error = true;
		currentChannel = 0;
		for (short i = 0; i < _channels; i++)
		{
			channel[i] = _defaultValue[i];
		}
	}
	
	currentChannel++;

	if (synchronized && currentChannel > _channels) //(currentChannel == (_channels + 1))
	{
		currentChannel = 0;
		synchronized = false;
	}
	lastMs = nowMs;
}

/********************************************************************************
Interrupt function
********************************************************************************/
static void update0(){ (*dr[0]).PWMstore(); }//I hate doing this, but the attachInterrupt needs this.
static void update1(){ (*dr[1]).PWMstore(); }
static void update2(){ (*dr[2]).PWMstore(); }
static void update3(){ (*dr[3]).PWMstore(); }
static void update4(){ (*dr[4]).PWMstore(); }
static void update5(){ (*dr[5]).PWMstore(); }

