/////////////////Class members////////////////
/////////////////////////
//ASCENT_RATE functions//
/////////////////////////
ASCENT_RATE::ASCENT_RATE(){
  rate=0;
  h_dot=0;
  prevh=0;
  prevt=0;
  for(int i=0;i<5;i++){
    h_dotArr[i]=0;
    hQ[i]=0;
    tQ[i]=0;
    h_dotQ[i]=0;
  }
  sum=0;
  
}
void ASCENT_RATE::updateRate(){
 rate=((GPS.altitude.feet()-prevh)/(getGPStime()-prevt))*60; //h_dot in feet per minute
 prevh=GPS.altitude.feet();
 prevt=getGPStime();
 //Serial.println("Rate: " + String(rate));
 //Serial.println("Alt: " + String(prevh));
 //Serial.println("Time: " + String(prevt));
}
void ASCENT_RATE::addHit(){
  //Serial.println("Adding hit");
  for(int i=0;i<5;i++){
  if(h_dotArr[i]==0){
    h_dotArr[i]=rate;
    break;
  }
 }
 if(h_dotArr[4]!=0){
   for(int i=0;i<5;i++){
     sum+=h_dotArr[i];
     h_dotArr[i]=0; //set h dot array element equal to zero after adding to sum to prepare for next five value average
   }
   h_dot=sum/5;
   sum=0;
 }
 for(int i=0;i<5;i++){
  h_dotQ[i]=0; //set questionable array equal to zero everytime normal update happens
 } 
}
void ASCENT_RATE::checkHit(){
  //Serial.println("Checking hit");
  for(int i=0;i<5;i++){
    if(h_dotQ[i]==0){
      h_dotQ[i]=rate;
      break;
    }
  }
  if(h_dotQ[4]!=0){
   for(int i=0;i<5;i++){
     sum+=h_dotQ[i];
     h_dotQ[i]=0;      //set normal and questionable array to zero after questionable array is determined to be correct
     h_dotArr[i]=0;
   }
   h_dot=sum/5;
   sum=0;
 }
}
float ASCENT_RATE::getRate(){
  return rate;
}
float ASCENT_RATE::geth_dot(){
  return h_dot;
}
float ASCENT_RATE::getPrevh(){
  return prevh;
}
float ASCENT_RATE::getPrevt(){
  return prevt;
}
String ASCENT_RATE::getHDot() {return (String(h_dot));}

//////////////SMART communication functions/////////////
void GetTempB() {  //in unlikely event that temp send takes more than 2 seconds, break
  while(true && millis()-TempTimer<=2000){
    while(Serial.available()>0){
      Data=Xbee.read();
    }
    if (Data=='V'){
      logData(flightTimeStr()+ "," + gpsStr()+ "," + BTemp);  //ADD GPS TIMESTAMP
      BTemp="S: ";
      TempTimer=millis();
      Data='/';
      break;
    }
    if(Data=='/'){
      newTempChar=true;
    }
    else if(newTempChar && Data!='/'){ 
      BTemp=BTemp+String(Data);
      newTempChar=false;
    } 
  } 
}

void response(){
  while(Xbee.available()>0){
    Response=Xbee.read(); 
  //Beacon might be redundant, can remove if we just want temperature requests as beacons  
//    if (Response=='F'){
//       logData("B"); //BEACON FROM B ADD GPS TIME STAMP
//       Response="";
//    }
    if (Response=='X' && !B_Done){
          logData("B Cut"); //ADD GPS TIMESTAMP
          B_Done=true;
          Response="";
    }
    else if (Response=='C'){
      TempTimer=millis();
      GetTempB();
      newTempChar=true;
      Response="";
    }
  }
  Response="";
}

void CutB(){
  if (millis()-timerB>200){
    Serial.print('B');
    timerB=millis();  
  }
}

////////////////////State switch function///////////////////
void stateSwitch(){
  if(vrInit &&  GPS.altitude.feet()!=0){
    if(vr.geth_dot()>=5000 || vr.geth_dot()<=-5000){
      //Serial.println("GPS Jump Detected");
    }
    else if(vr.geth_dot() > 250){
      stackState = STATE_SMART_ASCENT;
      stateString = "ASCENT";
    }
    else if(vr.geth_dot()<=-2000){
      stackState = STATE_SMART_DESCENT;
      stateString = "FAST DESCENT";
    }
    else{
      
    } 
  } 
}


