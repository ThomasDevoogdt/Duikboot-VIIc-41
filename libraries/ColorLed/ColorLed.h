#if defined(ARDUINO) && ARDUINO >= 100
#include <Arduino.h>
#else
#include <WProgram.h>
#endif

#ifndef ColorLed.h
#define ColorLed.h

#define MAXLED 8

struct color {
	unsigned short r;
	unsigned short g;
	unsigned short b;
};

class rgbLed
{
	public:
		rgbLed(int, int, int);
		void changeColor(String);
		void changeColor(struct color);
		void changeColor(short, short, short);
		int transitionTime;
		void updateLed();
		struct color currentColor;
		
		//Please don't touch
		static void updateLeds();
		static short ledCounter;
		
		
	private:
		int id;
		void setTarget(struct color);
		short _pin1, _pin2, _pin3;
		int progress;
		float steps;
		float difR, difG, difB;
		struct color targetColor;
		struct color sourceColor;
		//boolean ready;
	
		long nowMs, lastMs; 
		long diffMs;
};

static rgbLed *rgbLedPntr[MAXLED];
//static short ledCounter = 0;
//static boolean ready[MAXLED];
//static rgbLed *rgbLedPntr[MAXLED];

static void printSetup(struct color myRgbLed){
	String rgbString = "r: ";
	rgbString += myRgbLed.r;
	rgbString += " g: ";
	rgbString += myRgbLed.g;
	rgbString += " b: ";
	rgbString += myRgbLed.b;
	Serial.println(rgbString);
}

static struct color red =		{ 255, 0  , 0   };
static struct color green =		{ 0  , 255, 0   };
static struct color blue =		{ 0  , 0  , 255 };
static struct color black =		{ 0  , 0  , 0   };
static struct color white =		{ 255, 255, 255 };
static struct color grey =		{ 80 , 80 , 80  };
static struct color yellow =	{ 255, 255, 0   };



#endif