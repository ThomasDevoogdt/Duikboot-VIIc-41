/*******************************************************************************************************************
  GP Duikboot VII c/41 2013-2015
  by:       * Michiel Caron
              Christophe Dupont
              Joachim Pauwels
              Thomas Devoogdt
  links:    * http://viic41.blogspot.be/
              https://www.youtube.com/channel/UCqY24WBCvpEZ_ps5mDEgGUQ
*******************************************************************************************************************/
/*ESC Calibratie*/
/*  Throtle omhoog blijven houden.
    Ontvanger aanleggen.
    Wachten tot 3x tuut.
    Throtle beneden, boven, beneden,...
*/

/*INCLUDE*/
#include <ADXL345.h>      //Library by Adafruit: https://github.com/adafruit/Adafruit_ADXL345
#include <PPMdecode.h>    //Library by Thomas Devoogdt: https://github.com/ThomasDavidDev/PPMdecode
#include <ColorLed.h>     //Library by Thomas Devoogdt: 
#include <Wire.h>         //Library by Nicholas Zambetti: https://github.com/arduino/Arduino/tree/master/hardware/arduino/avr/libraries/Wire
#include <Servo.h>        //Library by Michael Margolis: https://github.com/arduino/Arduino/tree/master/libraries/Servo
#include <Event.h>        //Library by Simon Monk: http://www.doctormonk.com/2012/01/arduino-timer-library.html
#include <Timer.h>        //Library by Simon Monk: http://www.doctormonk.com/2012/01/arduino-timer-library.html

/*Define's*/
//#define led 13

#define gyroServoOffset 30  //Max hoek die door servo's worden gecorrigeerd.
#define speedOffset 5       //Zone waarbinnen speedStop actief is. (voor joystick)
#define servoOffset 7       //Zone waarbinnen servoOffset actief is. (voor joystick)

#define speedMin  15      //ESC max snelheid links.
#define speedStop 90      //ESC geen beweging.
#define speedMax  165     //ESC max snelheid rechts.

#define directAngleStart    155  //Servo's neutrale positie
#define directVBAngleStart  20
#define directVFAngleStart  20

#define directAngleMin  129 //Servo Mapping Direction Angle
#define directAngleMax  180
#define directAngleVAngleMin  0 //Servo Mapping Vertical Direction Angle
#define directAngleVAngleMax  40

#define maxDiepteBuitenTank 25    //Max diepte waarbij pompen nog nut heeft.
#define snorkel           20      //Diepte Snorkel
#define delayTankOutDown  36000   //Pomptijd omlaag
#define delayTankOutUp    36000   //Pomptijd omhoog
#define delayTankInsDown  1000    //Pomptijd binnentank
#define delayTankInsUp    500     //Pomptijd binnentank
#define maxDiepte         150     //cm
#define minDuikSnelheid   -1      //1cm / 5s  
#define criDuikSnelheid   -10     //10cm / 5s Kritieke duiksnelheid

//Vervang de getallen in PPM om de afstandsbediening in te stellen.
#define PPMdirectionHorizontal  PPM[1]  //... (vul de juiste joystick aan)
#define PPMdirectionVertical    PPM[0]  //...
#define PPMspeed                PPM[2]  //...
#define PPMbinnenTank           PPM[3]  //...
#define PPMbuitenTank           PPM[5]  //...

#define ADXL345Calibration  20    //Calibration Gyro - Axel

#define VoltageMin          940
#define VoltageHysterese    950

/*Timer.h*/
Timer t;

/*PPM_INIT*/
#define inter 0       //Let op! int.0 staat verbonden op !pin 2! zie: http://arduino.cc/en/Reference/attachInterrupt .
#define channels 6    //In te stellen op het aantal in te lezen kanalen.
PPMdecode myPPMdecode = PPMdecode(inter, channels); //Aanmaken van een nieuw object.
short defaultValue[channels] = { 50, 50, 50, 50, 50, 0 };
int PPM[channels] = { 50, 50, 50, 50, 50, 0 };
int duikReal;

/*ADXL345_INIT (Hoek)*/
ADXL345 adxl = ADXL345();     //Aanmaken van een nieuw object.
int x, y, z;      //Sensor Val.
int xR, yR, zR;   //Real
int xO, yO, zO;   //Offset

/*DRUK_INIT*/
int diepteReal;
int drukCal;
#define drukPin 0
#define diepte (uint16_t)(abs((analogRead(drukPin)-drukCal)*2.5))
/* Berekening: vb: p = 200kPa, p0 = 101.325kPa, Vs = 5V,  ρ = 1027kg/m³
   VOUT = Vs* (0.004 x P-0.04) ± Error = 5 * (0.004 x 200.000-0.04) + 0 = 3.8V  (Zie DataSheet!)
   VOUT = Vs* (0.004 x P-0.04) ± Error = 5 * (0.004 x 101.325-0.04) + 0 = 1.8265V
   Devisions = (2^10 - 1) x ΔV / Vs = (2^10 - 1) x (3.8 - 1.8265) / 5 = 403.7772816div
   p=p0 + ρgh zodat h = (p - p0) / (ρ x g) = (200000 - 101325) / (9.81 * 1027) = 9.79m = 979.42cm
   C = 979.42/403.78 = 2.425636975 ~ 2.5
*/

/*Voltage_Check*/
int voltageReal;
#define voltagePin 1
boolean voltageAlarm = LOW;

/*RGBLed*/
rgbLed myLed = rgbLed(4, 12, 13);
//RED   = PIN 4
//GREEN = PIN 12
//BLUE  = PIN 13



/*Variabelen*/
int speedSetVal       = speedStop;            //Motoren
int speedSetValReal   = speedStop;
int directAngle       = directAngleStart;     //Servo richting links rechts.
int directAngleReal   = directAngleStart;
int directVBAngle     = directVBAngleStart;   //Servo richting achteraan, Omlaag - Omhoog.
int directVBAngleReal = directVBAngleStart;
int directVFAngle     = directVFAngleStart;   //Servo richting vooraan, Omlaag - Omhoog.
int directVFAngleReal = directVFAngleStart;

int calAngle; //Draaihoek om de stabiliteit bij te sturen. (gyro afhankelijk)

int diepteAsk;              //Gevraagde diepte.
int diepteDiff;             //Verschil met gevraagde en werkelijke diepte.
int diepteLast;             //Vorige diepte.
int duikDiff;               //Hoogte verschil per tijdseenheid.

boolean insTankFlag = false;  //Binnentank in werking?
boolean outTankFlag = false;  //Buitentank in werking?
boolean firstFlag = false;    //Duikprocedure starten.
boolean secondFlag = false;   //Snorkeldiepte bereikt met buitentank.
boolean thirdFlag = false;    //Snorkeldiepte bereikt met binnentank.

boolean outTankValA = false;  //HBrug Buiten tank
boolean outTankValB = false;
boolean inTankValA = false;   //HBrug Binnen tank
boolean inTankValB = false;

boolean blinkLed = false;     //Led pin 13 blink.

/*Servo*/
Servo direct;           //Richting Links Rechts
Servo directVB;         //Richting Achteraan, Omlaag - Omhoog
Servo directVF;         //Richting Vooran, Omlaag - Omhoog
Servo speedSet;         //ESC's Motoren
Servo insTank;          //ESC Binnen Tank
Servo outTank;          //ESC Buiten Tank

/*SETUP*/
void setup() {
  Serial.begin(250000);            //INIT_Serial.
  Serial.println("Welcome!");
  Serial.println("");

  adxl.autoPreset(1);              //INIT_ADXL345.
  xO = 0;  yO = 0;  zO = 0;
  delay(500);
  for (int i = 0; i < ADXL345Calibration; i++) { //Begrens alle waarden tussen 0 & 100
    adxl.readAccel(&x, &y, &z);
    xO += x;
    yO += y;
    zO += z;
    delay(50);
  }
  xO /= -ADXL345Calibration;
  yO /= -ADXL345Calibration;
  zO /= -ADXL345Calibration;

  drukCal = analogRead(drukPin);   //Pressure Calibration. (eventueel vast zetten)

  myPPMdecode.SetDefaultValues(defaultValue);

  pinMode(7, OUTPUT);     //inTankValA
  pinMode(8, OUTPUT);     //inTankValB
  pinMode(9, OUTPUT);     //outTankValA
  pinMode(10, OUTPUT);    //outTankValB

  pinMode(12, OUTPUT);    //RED
  pinMode(13, OUTPUT);    //BLUE

  myLed.transitionTime = 250;

  direct.attach(3);                           //Richting Links Rechts
  directVB.attach(5);                         //Richting Achteraan, Omlaag - Omhoog
  directVF.attach(6);                         //Richting Vooraan, Omlaag - Omhoog
  speedSet.attach(11, 1000, 2000);            //ESC's Motoren

  delay(500);

  t.every(50, timer50);     //50ms timer
  t.every(750, timer750);   //750ms timer
  t.every(1000, timer1000); //1000ms timer
}

/*******************************************************************************************************************
  loop
*******************************************************************************************************************/

void loop() {
  ppmRemap();         //Schaalt alle waardes tussen 0 en 100%.

  servoCalc();        //Berekent de hoek van de servo's en stuurt ze aan.

  escCalc();          //Berekent de snelheid van de motoren.

  tankCalc();         //Berekent de snelheid van de pompen en regelt hun werking.

  voltageCheck();     //Controleert de spanning van de batterijen.

  myLed.updateLed();  //Kleuren Led update.

  t.update();         //Timer update
}

/*******************************************************************************************************************
  subroutines
*******************************************************************************************************************/

void ppmRemap() {
  PPM[0] = map(myPPMdecode.channel[0], 4, 84, 0, 100); //Alles tussen 0 en 100 mappen
  PPM[1] = map(myPPMdecode.channel[1], 6, 87, 0, 100);
  PPM[2] = map(myPPMdecode.channel[2], 4, 84, 0, 100);
  PPM[3] = map(myPPMdecode.channel[3], 8, 89, 0, 100);
  PPM[4] = map(myPPMdecode.channel[4], 6, 87, 0, 100);
  PPM[5] = map(myPPMdecode.channel[5], 8, 87, 0, 100);

  for (int i; i < channels; i++) { //Begrens alle waarden tussen 0 & 100
    if (PPM[i] < 0) PPM[i] = 0;
    if (PPM[i] > 100) PPM[i] = 100;
  }

  if (myPPMdecode.error) {
    PPM[0] = 50;
    PPM[1] = 50;
    PPM[2] = 50;
    PPM[3] = 50;
    PPM[4] = 50;
    PPM[5] = 0;
  }
}

void servoCalc() {
  int directV = PPMdirectionVertical; //Channel 0, val: 0 to 100
  if (directV - 50 > servoOffset) {
    directVFAngle = map(directV, 50 + speedOffset, 100, directAngleVAngleMin, (directAngleVAngleMin + directAngleVAngleMax) / 2);
    directVBAngle = map(directV, 50 + speedOffset, 100, (directAngleVAngleMin + directAngleVAngleMax) / 2, directAngleVAngleMin);
  }
  else {
    directVFAngle = map(directV, 0, 50 - speedOffset, (directAngleVAngleMin + directAngleVAngleMax) / 2, directAngleVAngleMax);
    directVBAngle = map(directV, 0, 50 - speedOffset, directAngleVAngleMax, (directAngleVAngleMin + directAngleVAngleMax) / 2);
  }
  directAngle = map(PPMdirectionHorizontal, 0, 100, directAngleMin, directAngleMax);
}

void escCalc() {
  int speedSet = PPMspeed; //Channel 0, val: 0 to 100
  if (abs(speedSet - 50) < speedOffset)
    speedSetVal = speedStop; // map: 35 to 65 --> 90
  else if (speedSet - 50 > speedOffset)
    speedSetVal = map(speedSet, 50 + speedOffset, 100, speedStop, speedMax); // map: 65 to 100 --> 90 to 165
  else
    speedSetVal = map(speedSet, 0, 50 - speedOffset, speedMin, speedStop); // map: 0 to 35 --> 15 to 90
}

void tankCalc() {
  //Buiten Tank
  if (duikReal > 75 && !firstFlag) firstFlag = true;
  if (duikReal < 25 && firstFlag) firstFlag = false;

  if (firstFlag && ((!outTankFlag && !secondFlag) || (!outTankValA && outTankValB && secondFlag)) && diepteReal < maxDiepteBuitenTank) {
    outTankFlag = true;

    outTankValA = true; //Inpompen
    outTankValB = false;

    t.after(delayTankOutDown, delayCall1);
  }
  if (!firstFlag && ((!outTankFlag && secondFlag) || (outTankValA && !outTankValB  && !secondFlag)) && diepteReal < maxDiepteBuitenTank) {
    outTankFlag = true;

    outTankValA = false;  //Uitpompen
    outTankValB = true;

    t.after(delayTankOutUp, delayCall2);
  }

  //Binnen Tank H-Bridge
  int insTank = PPMbinnenTank; //Channel 3, val: 0 to 100
  if (abs(insTank - 50) < 25) {
    //off
    inTankValA = false;
    inTankValB = false;
  }
  else if (insTank - 50 > 25) {
    //left
    inTankValA = false;
    inTankValB = true;
  }
  else {
    //right
    inTankValA = true;
    inTankValB = false;
  }
}

void voltageCheck() {
  if (fading(analogRead(voltagePin), &voltageReal, 1) < VoltageMin && !voltageAlarm) {
    voltageAlarm = true;
  }
  if (fading(analogRead(voltagePin), &voltageReal, 1) > VoltageHysterese && voltageAlarm) {
    voltageAlarm = false;
  }
}

int fading(int input, int *real, int val) {   //Berekent de fading.
  if (*real < input && *real + val < input) *real += val;
  else if (*real > input && *real - val > input) *real -= val;
  else *real += input - *real;
  return *real;
}

void timer50() {  //50ms timer
  //hoek uitlezen + pieken weg werken
  adxl.readAccel(&x, &y, &z);
  fading(x + xO, &xR, 1);
  fading(y + yO, &yR, 1);
  fading(z + zO, &zR, 1);

  //fading ESC
  speedSet.write(fading(speedSetVal, &speedSetValReal, 4));
  direct.write(fading(directAngle, &directAngleReal, 15));
  directVB.write(fading(directVBAngle, &directVBAngleReal, 15));
  directVF.write(fading(directVFAngle, &directVFAngleReal, 15));

  fading(PPMbuitenTank, &duikReal, 3);
  fading(diepte, &diepteReal, 3);

  //Inside Tank and Outside Tank
  digitalWrite(7, inTankValA); //inTankValA
  digitalWrite(8, inTankValB); //inTankValB
  digitalWrite(9, outTankValA); //outTankValA
  digitalWrite(10, outTankValB); //outTankValB

  printValues(); //Log
}

void timer1000() {   //1000ms timer
  //blinkLed = !blinkLed;
  //digitalWrite(led, blinkLed);
}

void timer750() {   //750ms timer
  if (voltageAlarm) { //RED Blink
    blinkLed = !blinkLed;
    if (blinkLed) myLed.changeColor(red);
    else myLed.changeColor(white);
    //digitalWrite(12, blinkLed); //RED
    //digitalWrite(13, LOW); //BLUE
  }
  else {    //Fade from blue to red
    short red = map(voltageReal, VoltageMin, 1023, 255, 0);
    short green = 0;
    short blue = map(voltageReal, VoltageMin, 1023, 0, 255);
    myLed.changeColor(red, green, blue);
    //digitalWrite(12, HIGH); //RED
    //digitalWrite(13, LOW); //BLUE
  }
  //voltageReal
}

void delayCall1() {
  outTankFlag = false;
  if (outTankValA || !outTankValB) secondFlag = HIGH;
  outTankValA = LOW;
  outTankValB = LOW;
}

void delayCall2() {
  outTankFlag = false;
  if (!outTankValA || outTankValB) secondFlag = LOW;
  outTankValA = HIGH;
  outTankValB = HIGH;
}

