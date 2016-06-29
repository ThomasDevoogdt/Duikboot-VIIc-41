
#include "ColorLed.h"

// Provides ISR
//#include <avr/interrupt.h>
#include <stdlib.h>    

#define hexToInt(x) strtol(x,NULL,16)

rgbLed::rgbLed(int pin1, int pin2, int pin3){
	_pin1 = pin1;
	_pin2 = pin2;
	_pin3 = pin3;
	pinMode(_pin1, OUTPUT);
	pinMode(_pin1, OUTPUT);
	pinMode(_pin1, OUTPUT);
	/*
		//timer2   ticks every 16384 micros (16.384 millis)
		TCCR2B = 0x00;        //Disbale Timer2 while we set it up
		TCNT2 = 0xff;         //Reset Timer Count to 130 out of 255
		TIFR2 = 0x00;         //Timer2 INT Flag Reg: Clear Timer Overflow Flag
		TIMSK2 = 0x01;        //Timer2 INT Reg: Timer2 Overflow Interrupt Enable
		TCCR2A = 0x00;        //Timer2 Control Reg A: Wave Gen Mode normal
		TCCR2B = 0x07;        //Timer2 Control Reg B: Timer Prescaler set to 1024


		rgbLedPntr[ledCounter] = this;
		id = ledCounter;
		ledCounter++;
	*/
}

// input= #rrggbb or red;
void rgbLed::changeColor(String input){
	struct color myColor = { 
		(short)hexToInt(input.substring(1, 3).c_str()),
		(short)hexToInt(input.substring(3, 5).c_str()),
		(short)hexToInt(input.substring(5, 7).c_str())
	};
	setTarget(myColor);
}

void rgbLed::changeColor(struct color myColor){
	setTarget(myColor);
}

void rgbLed::changeColor(short r, short g, short b){

	struct color myColor = { r, g, b };
	setTarget(myColor);
}

void rgbLed::setTarget(struct color myColor){
	sourceColor = targetColor;
	targetColor = myColor;

	if (transitionTime == 0){
		analogWrite(_pin1, targetColor.r);
		analogWrite(_pin2, targetColor.g);
		analogWrite(_pin3, targetColor.b);
		
		currentColor = targetColor;
	}
	else{
		progress = 0;
		steps = transitionTime / 10; /// 16.384;
		difR = (targetColor.r > sourceColor.r) ? (targetColor.r - sourceColor.r) / steps : -( (sourceColor.r - targetColor.r) / steps);
		difG = (targetColor.g > sourceColor.g) ? (targetColor.g - sourceColor.g) / steps : -((sourceColor.g - targetColor.g) / steps);
		difB = (targetColor.b > sourceColor.b) ? (targetColor.b - sourceColor.b) / steps : -((sourceColor.b - targetColor.b) / steps);

		ready = false;
		//ready[id] = false;
	}
}

void rgbLed::updateLed(){
	nowMs = millis();
	diffMs = nowMs - lastMs;
	if(diffMs > 10){	
		if (steps < (progress+1)){ //done fading =
			currentColor = targetColor;

			ready = true;
			//ready[id] = true;
		}
		else{
			progress++;
			currentColor.r = (int)(progress*difR + sourceColor.r);
			currentColor.g = (int)(progress*difG + sourceColor.g);
			currentColor.b = (int)(progress*difB + sourceColor.b);
		}
		analogWrite(_pin1, currentColor.r);
		analogWrite(_pin2, currentColor.g);
		analogWrite(_pin3, currentColor.b);
		
		lastMs = nowMs;
	}
}
/*
ISR(TIMER2_OVF_vect) {
	for (int i = 0; i < ledCounter; i++){
		if (!ready[i]){
			rgbLedPntr[i]->updateLed();
		}
	}
};
*/