#include <SoftwareSerial.h>
#include <TinyGPS++.h>
#include <SD.h>

/////////////Defs//////////////
//NAV and COMS
#define Xbee Serial
#define chipSelect_pin 6
#define gpsRx 7
#define gpsTx 8
//Control params
int cutTime = 100;
long maxAlt = 80000;

//////////////Class Defs////////////////
class Relay {
  protected:
    bool isOpen;
    int onPin;
    int offPin;
  public:
    Relay(int on, int off);
    const char* getRelayStatus();
    void init();
    void openRelay();
    void closeRelay();
};
class ASCENT_RATE{
  protected:
    float rate;
    float h_dot;
    float prevh;
    unsigned long prevt;
    float h_dotArr[5];
    float hQ[5];
    unsigned long tQ[5];
    float h_dotQ[5];
    float sum;
  public:
    ASCENT_RATE();
    void updateRate();
    void addHit();
    void checkHit();
    float getRate();
    float geth_dot();
    float getPrevh();
    float getPrevt();
    float getPrevh_dot();
    String getHDot();  
};
ASCENT_RATE vr = ASCENT_RATE();

////////////SMART Communication///////////
unsigned long timerB = 0;
unsigned long TempTimer=0;
String BTemp="S: ";
boolean B_Done=false;
boolean newTempChar=true;
char Response;
char Data;

///////////////Create GPS object/////////////////
SoftwareSerial gpsCom(gpsRx,gpsTx); //Create sofware serial object for gps communication
TinyGPSPlus GPS; //Create gps object


void setup() { 
  SDsetup();
  logData("SMART LOG START");
  Xbee.begin(9600);//Initiate xbee serial
  gpsCom.begin(4800); //Initiate gps serial  

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
