void printValues()
{
    for (int i; i < channels; i++) {
      int tmp = PPM[i];
      String ChannelString = (String)
                        "CH"
                        + String(i)
                        + ": "
                        + String(tmp)
                        + " "
                        + ((tmp < 1000) ? " " : "")//text align
                        + ((tmp < 100) ? " " : "")//text align
                        + ((tmp < 10) ? " " : "");//text align
      Serial.print(ChannelString);
    }
  
  //Serial.print("Fl1: " + String(firstFlag) + " ");
  //Serial.print("Fl2: " + String(secondFlag) + " ");
  //Serial.print("TOut: " + String(outTank) + " ");
  //Serial.print("voltage: "+ String(voltageAlarm) + " ");
  //Serial.print("Water: "+ String(waterCheckAlarm) + " ");
  
  switch(inTank) {
    case off: Serial.print("TIn: off   "); break;
    case left: Serial.print("TIn: left  "); break;
    case right: Serial.print("TIn: right "); break;
  }
  
  switch(outTank) {
    case off: Serial.print("TIn: off   "); break;
    case left: Serial.print("TIn: left  "); break;
    case right: Serial.print("TIn: right "); break;
  }
  
/*
  String myStringOutTank = (String)
                           "Tout: "
                           //+ ((outTankVal < 0) ? "" : " ")//text align
                           + String(outTankValReal)
                           + " "
                           + ((abs(outTankValReal) < 1000) ? " " : "")//text align
                           + ((abs(outTankValReal) < 100) ? " " : "")//text align
                           + ((abs(outTankValReal) < 10) ? " " : "");//text align
  Serial.print(myStringOutTank);
*/
/*
  String myStringInsTank = (String)
                           "Tins: "
                           //+ ((outTankVal < 0) ? "" : " ")//text align
                           + String(insTankValReal)
                           + " "
                           + ((abs(insTankValReal) < 1000) ? " " : "")//text align
                           + ((abs(insTankValReal) < 100) ? " " : "")//text align
                           + ((abs(insTankValReal) < 10) ? " " : "");//text align
  Serial.print(myStringInsTank);
*/
/*
  String myStringESC = (String)
                       "ESC: "
                       //+ ((speedSetValReal < 0) ? "" : " ")//text align
                       + String(speedSetValReal)
                       + " "
                       + ((abs(speedSetValReal) < 1000) ? " " : "")//text align
                       + ((abs(speedSetValReal) < 100) ? " " : "")//text align
                       + ((abs(speedSetValReal) < 10) ? " " : "");//text align
  Serial.print(myStringESC);
*/

  

/*
  adxl.readAccel(&x, &y, &z);
  int angel[3] = {x, y, z};
  for (int i; i < 3; i++) {
    int tmp = angel[i];
    String myString = (String)
                      ((i == 0) ? "X: " : "")
                      + ((i == 1) ? "Y: " : "")
                      + ((i == 2) ? "Z: " : "")
                      + ((tmp < 0) ? "" : " ")//text align
                      + String(tmp)
                      + " "
                      + ((abs(tmp) < 1000) ? " " : "")//text align
                      + ((abs(tmp) < 100) ? " " : "")//text align
                      + ((abs(tmp) < 10) ? " " : "");//text align
    Serial.print(myString);
  }
*/
  int PIDInputInt = PIDInput;
  String PIDInputString = (String)
                        "PID Inp: "
                        + ((PIDInputInt < 0) ? "" : " ")//text align
                        + String(PIDInputInt)
                        + " "
                        + ((abs(PIDInputInt) < 1000) ? " " : "")//text align
                        + ((abs(PIDInputInt) < 100) ? " " : "")//text align
                        + ((abs(PIDInputInt) < 10) ? " " : "")//text align
                        + "cm ";
  Serial.print(PIDInputString);
  
  int PIDSetpointInt = PIDSetpoint;
    String PIDSetpointString = (String)
                        "PID Setp: "
                        + ((PIDSetpointInt < 0) ? "" : " ")//text align
                        + String(PIDSetpointInt)
                        + " "
                        + ((abs(PIDSetpointInt) < 1000) ? " " : "")//text align
                        + ((abs(PIDSetpointInt) < 100) ? " " : "")//text align
                        + ((abs(PIDSetpointInt) < 10) ? " " : "")//text align
                        + "cm ";
  Serial.print(PIDSetpointString);

/*
  String myStringDrukAsk = (String)
                           "Diepte Ask: "
                           + ((diepteAsk < 0) ? "" : " ")//text align
                           + String(diepteAsk)
                           + " "
                           + ((abs(diepteAsk) < 1000) ? " " : "")//text align
                           + ((abs(diepteAsk) < 100) ? " " : "")//text align
                           + ((abs(diepteAsk) < 10) ? " " : "")//text align
                           + "cm ";
  Serial.print(myStringDrukAsk);
*/
/*
  String myStringDrukDiff = (String)
                            "Diepte diff: "
                            + ((diepteDiff < 0) ? "" : " ")//text align
                            + String(diepteDiff)
                            + " "
                            + ((abs(diepteDiff) < 1000) ? " " : "")//text align
                            + ((abs(diepteDiff) < 100) ? " " : "")//text align
                            + ((abs(diepteDiff) < 10) ? " " : "")//text align
                            + "cm ";
  Serial.print(myStringDrukDiff);
*/
/*
  String myStringDuikDiff = (String)
                            "Diepte duik diff: "
                            + ((duikDiff < 0) ? "" : " ")//text align
                            + String(duikDiff)
                            + " "
                            + ((abs(duikDiff) < 1000) ? " " : "")//text align
                            + ((abs(duikDiff) < 100) ? " " : "")//text align
                            + ((abs(duikDiff) < 10) ? " " : "")//text align
                            + "cm ";
  Serial.print(myStringDuikDiff);
*/
/*
  String myStringdirectAngleReal = (String)
                            "Direct. Angl.: "
                            + ((directAngleReal < 0) ? "" : " ")//text align
                            + String(directAngleReal)
                            + " "
                            + ((abs(directAngleReal) < 1000) ? " " : "")//text align
                            + ((abs(directAngleReal) < 100) ? " " : "")//text align
                            + ((abs(directAngleReal) < 10) ? " " : "")//text align
                            + " ";
  Serial.print(myStringdirectAngleReal);
  
    String myStringdirectVBAngleReal = (String)
                            "Direct. Angl. vb: "
                            + ((directVBAngleReal < 0) ? "" : " ")//text align
                            + String(directVBAngleReal)
                            + " "
                            + ((abs(directVBAngleReal) < 1000) ? " " : "")//text align
                            + ((abs(directVBAngleReal) < 100) ? " " : "")//text align
                            + ((abs(directVBAngleReal) < 10) ? " " : "")//text align
                            + " ";
  Serial.print(myStringdirectVBAngleReal);
  
    String myStringdirectVFAngleReal = (String)
                            "Direct. Angl. vf: "
                            + ((directVFAngleReal < 0) ? "" : " ")//text align
                            + String(directVFAngleReal)
                            + " "
                            + ((abs(directVFAngleReal) < 1000) ? " " : "")//text align
                            + ((abs(dirctVFAngleReal) < 100) ? " " : "")//text align
                            + ((abs(directVFAngleReal) < 10) ? " " : "")//text align
                            + " ";
  Serial.print(myStringdirectVFAngleReal);
  //directVBAngleReal
  */
  /*
  short red = myLed.currentColor.r;
  short green = myLed.currentColor.g;
  short blue = myLed.currentColor.b;
  String ledLOG = (String)
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
  Serial.print(ledLOG);
*/

  Serial.println("");
}

