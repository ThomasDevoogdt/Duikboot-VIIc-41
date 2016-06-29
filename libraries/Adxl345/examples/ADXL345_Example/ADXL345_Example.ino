/*******************************************************
Name: ADXL345_Example
Function:  ADXL345加速度センサーモジュール応用例
Notes:
1.  Originally from  http://bildr.org/2011/03/adxl345-arduino/
2.  Arduino 1.0 以上必要
3.  Add Japanese comments and merge to Adxl345 libraries
    by アンドロ シティ 2012年9月25日  
    URL: http://www.androciti.com
    Wiki:http://wiki.androciti.com
    Email: info@androciti.com
******************************************************/
#include <Wire.h>
#include <ADXL345.h>

ADXL345 adxl; //variable adxl is an instance of the ADXL345 library

void setup(){
  Serial.begin(9600);
  adxl.powerOn();

  //set activity/ inactivity thresholds (0-255)
  adxl.setActivityThreshold(75); //62.5mg per increment
  adxl.setInactivityThreshold(75); //62.5mg per increment
  adxl.setTimeInactivity(10); // how many seconds of no activity is inactive?
 
  //look of activity movement on this axes - 1 == on; 0 == off 
  adxl.setActivityX(1);
  adxl.setActivityY(1);
  adxl.setActivityZ(1);
 
  //look of inactivity movement on this axes - 1 == on; 0 == off
  adxl.setInactivityX(1);
  adxl.setInactivityY(1);
  adxl.setInactivityZ(1);
 
  //look of tap movement on this axes - 1 == on; 0 == off
  adxl.setTapDetectionOnX(0);
  adxl.setTapDetectionOnY(0);
  adxl.setTapDetectionOnZ(1);
 
  //set values for what is a tap, and what is a double tap (0-255)
  adxl.setTapThreshold(50); //62.5mg per increment
  adxl.setTapDuration(15); //625μs per increment
  adxl.setDoubleTapLatency(80); //1.25ms per increment
  adxl.setDoubleTapWindow(200); //1.25ms per increment
 
  //set values for what is considered freefall (0-255)
  adxl.setFreeFallThreshold(7); //(5 - 9) recommended - 62.5mg per increment
  adxl.setFreeFallDuration(45); //(20 - 70) recommended - 5ms per increment
 
  //setting all interupts to take place on int pin 1
  //I had issues with int pin 2, was unable to reset it
  adxl.setInterruptMapping( ADXL345_INT_SINGLE_TAP_BIT,   ADXL345_INT1_PIN );
  adxl.setInterruptMapping( ADXL345_INT_DOUBLE_TAP_BIT,   ADXL345_INT1_PIN );
  adxl.setInterruptMapping( ADXL345_INT_FREE_FALL_BIT,    ADXL345_INT1_PIN );
  adxl.setInterruptMapping( ADXL345_INT_ACTIVITY_BIT,     ADXL345_INT1_PIN );
  adxl.setInterruptMapping( ADXL345_INT_INACTIVITY_BIT,   ADXL345_INT1_PIN );
 
  //register interupt actions - 1 == on; 0 == off  
  adxl.setInterrupt( ADXL345_INT_SINGLE_TAP_BIT, 1);
  adxl.setInterrupt( ADXL345_INT_DOUBLE_TAP_BIT, 1);
  adxl.setInterrupt( ADXL345_INT_FREE_FALL_BIT,  1);
  adxl.setInterrupt( ADXL345_INT_ACTIVITY_BIT,   1);
  adxl.setInterrupt( ADXL345_INT_INACTIVITY_BIT, 1);
}

void loop(){
  
  //Boring accelerometer stuff   
  int x,y,z;  
  adxl.readAccel(&x, &y, &z); //read the accelerometer values and store them in variables  x,y,z

  // Output x,y,z values - Commented out
  //Serial.print(x);
  //Serial.print(y);
  //Serial.println(z);


  //Fun Stuff!    
  //read interrupts source and look for triggerd actions
  
  //getInterruptSource clears all triggered actions after returning value
  //so do not call again until you need to recheck for triggered actions
   byte interrupts = adxl.getInterruptSource();
  
  // freefall
  if(adxl.triggered(interrupts, ADXL345_FREE_FALL)){
    Serial.println("Free-fall detected.");           //自由落下を検出しました。
    //add code here to do when freefall is sensed
  } 
  
  //inactivity
  if(adxl.triggered(interrupts, ADXL345_INACTIVITY)){
    Serial.println("No motion detected.");          //動きを検出しなかった。
     //add code here to do when inactivity is sensed
  }
  
  //activity
  if(adxl.triggered(interrupts, ADXL345_ACTIVITY)){
    Serial.println("Motion detected.");                 //動きを検出しました。
     //add code here to do when activity is sensed
  }
  
  //double tap
  if(adxl.triggered(interrupts, ADXL345_DOUBLE_TAP)){
    Serial.println("Double tap detected.");           //ダブルタップを検出しました。
     //add code here to do when a 2X tap is sensed
  }
  
  //tap
  if(adxl.triggered(interrupts, ADXL345_SINGLE_TAP)){
    Serial.println("Single tap detected.");            //シングルタップを検出しました。
     //add code here to do when a tap is sensed
  }  
}