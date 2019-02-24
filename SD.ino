//global variables for SD functions
File datalog, radiolog;
char datalogName[] = "Sensor00.csv";
bool datalogOpen = false, radiologOpen = false;

//standard SD initialization procedure
void SDsetup() {

  if (!SD.begin(chipSelect_pin)) {  //Display an error if SD communication fails
    while(true) { //Note that this error loop is never broken - check for slow blinking LEDs before flying
    //some error indicator
    }
  }
  else {  //file creation process
    for (byte i = 0; i < 100; i++) {
      datalogName[6] = '0' + i/10;
      datalogName[7] = '0' + i%10;
     
      if (!SD.exists(datalogName)) {  //make sure both file names are available before opening them
        openDatalog();
        break;
      }
    }
  }
}

//functions to change file state (open/closed) and handle the appropriate status LED
void openDatalog() {
  if (!datalogOpen) {
    datalog = SD.open(datalogName, FILE_WRITE);
    datalogOpen = true;

  }
}


void closeDatalog() {
  if (datalogOpen) {
    datalog.close();
    datalogOpen = false;
  }
}

//functions that handle both opening and closing of files when logging data to ensure it is saved properly
void logData(String data) {
  openDatalog();
  datalog.println(data); 
  closeDatalog();
    
}





