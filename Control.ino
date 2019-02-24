#define STATE_SMART_INIT 0x00     //00000000
#define STATE_SMART_ASCENT 0x01   //00000001
#define STATE_SMART_DESCENT 0x02  //00000010

//Control variables
uint8_t stackState;
String stateString = "";
static bool vrInit = false;
unsigned long Timer = cutTime*60000;

void smartTest (){
  static unsigned long prevTimes;
  static byte maxed = 0;
  static bool init = false;
  if(!init){
    stackState = STATE_SMART_INIT;
    stateString = "INITIALIZATION";
    init=true;
    //Serial.println("Initializing...");
  }
  if(millis()>Timer){
    CutB();
  }
  
  /////////////////State switcher/////////////////
  stateSwitch();
  
  /////////////////State Machine//////////////////
  if(millis()-prevTimes>=1000 && GPS.altitude.feet()!=vr.getPrevh()){
    vr.updateRate();
    prevTimes = millis();

    //Initialization phase
    if(stackState == STATE_SMART_INIT && !vrInit){
      if(vr.getRate()>250 && vr.getRate()<2000){
        vr.addHit();
      }
      else{
        vr.checkHit();
      }
      if(GPS.altitude.feet()>5000){
        vrInit=true;
      }
    }
    
    //Ascent
    if(stackState==STATE_SMART_ASCENT){
      if(vr.getRate()>250 && vr.getRate()<2000){
        vr.addHit();
      }
      else{
        vr.checkHit();
      }
      if(GPS.altitude.feet()>maxAlt){
        CutB();
      }
    }
    else{
      //do nothing
    } 
  } 
}


