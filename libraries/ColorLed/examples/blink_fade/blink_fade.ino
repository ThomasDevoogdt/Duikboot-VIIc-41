#include <Event.h>
#include <Timer.h>
#include <ColorLed.h>

Timer t;                          //Timer aanmaken
rgbLed myLed = rgbLed(3, 5, 6);   //Led aanmaken, RGB
boolean state = false;            //Blink

void setup() {
  myLed.transitionTime = 2000;    //Led omschakeltijd.
  t.every(2500, timer2500);       //1000ms timer
}

void loop() {
  myLed.updateLed();              //Led updaten.
  t.update();                     //Timer updaten.
}

void timer2500() {
  state = !state;
  if (state)
    myLed.changeColor("#FFFFFF"); //Color 1
  else
    myLed.changeColor("#000000"); //Color 2
}