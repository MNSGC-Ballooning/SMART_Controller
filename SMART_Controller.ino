#define Xbee Serial


#include <SD.h>

#define chipSelect_pin 6


unsigned long timerB = 0;
unsigned long TempTimer=0;




String BTemp="S: ";



boolean B_Done=false;
boolean newTempChar=true;



char Response;
char Data;

void setup() { 
  SDsetup();
  logData("SMART LOG START");
  Xbee.begin(9600);//Xbee  

}

void loop() {
 
//repeatedly call until !B_Done to ensure cut succesful
if (!B_Done){  //Heres where you can cut, B_Done true when get cut response from smart
  CutB();
}


//NO NEED TO CHANGE
if(millis()-TempTimer>10000){  //heres where you can request temperature

  Serial.print('P');  
  TempTimer=millis();
 

}
response();  //check for responses

}
