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
  loadStatus();
  
}

void loop() {
  performEEPROM();
  
  //This is a very passive use-case, so it doesn't really matter if the arduino is just sitting around.
  delay(1000);
}



//EEPROM-------------------------------------------------------------------------------------------------------
long lastWriteMillis;
void performEEPROM() {
  //Don't save to EEPROM so often -- save the EEPROM!
  if (millis() > lastWriteMillis + 60000) {
    writeStatus();
    lastWriteMillis = millis();
  }
}

//Writes unit status to EEPROM
//tempvars to check for change - to preserve the EEPROMs life
boolean idleOld = false;
byte idleTrgOld = 0;
boolean targetOld = false;
byte targetTrgOld = 0;
boolean warmOld = false;
byte warmWaitOld = 0;
byte warmTrgOld = 0;
byte warmTimeOld = 0;

//EEPROM address mapping:
//0: idle (binary)
//1: idleTrg (byte)
//2: target (binary)
//3: targetTrg (byte)
//4: warm (binary)
//5: warmWait (byte)
//6: warmTrg (byte)
//7: warmTime (byte)

void writeStatus() {
  //Sorry for the repetition here, but I find it's the most efficient (runtime-wise).
  if (idle != idleOld) {
    idleOld = idle;
    EEPROM.write(0, idle);
  }
  if (idleTrg != idleTrgOld) {
    idleTrgOld = idleTrg;
    EEPROM.write(1, idleTrg);
  }
  if (target != targetOld) {
    targetOld = target;
    EEPROM.write(2, target);
  }
  if (targetTrg != targetTrgOld) {
    targetTrgOld = targetTrg;
    EEPROM.write(3, targetTrg);
  }
  if (warm != warmOld) {
    warmOld = warm;
    EEPROM.write(4, warm);
  }
  if (warmWait != warmWaitOld) {
    warmWaitOld = warmWait;
    EEPROM.write(5, warmWait);
  }
  if (warmTrg != warmTrgOld) {
    warmTrgOld = warmTrg;
    EEPROM.write(6, warmTrg);
  }
  if (warmTime != warmTimeOld) {
    warmTimeOld = warmTime;
    EEPROM.write(7, warmTime);
  }
}

//Loads unit status from EEPROM
void loadStatus() {
  idle = EEPROM.read(0);
  idleOld = idle;
  idleTrg = EEPROM.read(1);
  idleTrgOld = idleTrg;
  target = EEPROM.read(2);
  targetOld = target;
  targetTrg = EEPROM.read(3);
  targetTrgOld = targetTrg;
  warm = EEPROM.read(4);
  warmOld = warm;
  warmWait = EEPROM.read(5);
  warmWaitOld = warmWait;
  warmTrg = EEPROM.read(6);
  warmTrgOld = warmTrg;
  warmTime = EEPROM.read(7);
  warmTimeOld = warmTime;
}


