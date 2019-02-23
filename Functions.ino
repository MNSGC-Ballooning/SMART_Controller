void CutB(){
  
  if (millis()-timerB>200){
  Serial.print('B');
  timerB=millis();  
}
}






void GetTempB() {  //in unlikely event that temp send takes more than 2 seconds, break
while(true && millis()-TempTimer<=2000){
 
  while(Serial.available()>0){
  Data=Xbee.read();
  }
  if (Data=='V'){
    logData(BTemp);  //ADD GPS TIMESTAMP
    BTemp="S: ";
    TempTimer=millis();
    Data='/';
    break;
  }
  if(Data=='/'){
    newTempChar=true;
  }
  else if(newTempChar && Data!='/')
  { 
    BTemp=BTemp+String(Data);
    newTempChar=false;
  } 
} 
}




void response(){
  while(Xbee.available()>0){
   Response=Xbee.read();
  
   
//Beacon might be redundant, can remove if we just want temperature requests as beacons
    
 if (Response=='F'){
     logData("B"); //BEACON FROM B ADD GPS TIME STAMP
     Response="";
  }
  


  else if (Response=='X' && !B_Done){
        logData("B Cut"); //ADD GPS TIMESTAMP
        B_Done=true;
        Response="";
}



  else if (Response=='C')
  {
    TempTimer=millis();
    GetTempB();
    newTempChar=true;
  Response="";
  }


  }
  Response="";
  
}



