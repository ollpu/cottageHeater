#include <GSM.h>
#include <EEPROM.h>

#define simPIN "1111"

GSM gsm;
GSM_SMS sms;

//VARIABLES
//Temperatures are stored as bytes (for EEPROM), so for example, 25,5°C = 255. (This can be expanded)

byte currentTemp = 0;

//
boolean idle = false;
byte idleTrg = 0;
//
boolean target = false;
byte targetTrg = 0;
//
boolean warm = false;
byte warmWait = 0; //Hours of wait before starting target

byte warmTrg = 0;
byte warmTime = 0; //Days before turning target off

void setup() {
  //Wait for GSM connection. Will try to connect forever.
  boolean notConnected = true;
  while(notConnected) {
    if(gsm.begin(simPIN) == GSM_READY) {
      notConnected = false;
    } else {
      delay(1000);
    }
  }
  
}

void loop() {
}



//EEPROM save&load
//Writes unit status to EEPROM
void writeStatus() {
  
}
//Loads unit status from EEPROM
void loadStatus() {
  
}
