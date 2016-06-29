#include <ColorLed.h>
rgbLed myLed = rgbLed(3, 5, 6);

char buffer[16];
String match;

void setup() {
  Serial.begin(250000);
  myLed.transitionTime = 2000;
}

void loop() {
  if (Serial.available() > 0) {
    Serial.readBytes(buffer, 16);
    match = "";
    for (short i = 0; i < 7; i++) {
      match += buffer[i];
      buffer[i] = 'f';
    }
    myLed.changeColor(match);
    Serial.println(match);
  }


  short red = myLed.currentColor.r;
  short green = myLed.currentColor.g;
  short blue = myLed.currentColor.b;
  String log = (String)
               "RED: "
               + red
               + ((abs(red) < 100) ? " " : "")//text align
               + ((abs(red) < 10) ? " " : "")//text align
               + " GREEN: "
               + green
               + ((abs(green) < 100) ? " " : "")//text align
               + ((abs(green) < 10) ? " " : "")//text align
               + " BLUE: "
               + blue
               + ((abs(blue) < 100) ? " " : "")//text align
               + ((abs(blue) < 10) ? " " : "")//text align
               + "";
  Serial.println(log);
  rgbLed::updateLeds();
}







