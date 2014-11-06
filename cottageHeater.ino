#include <GSM.h>
#include <EEPROM.h>
#include <avr/pgmspace.h>

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





//Thermistor lookup table (from 0 to 30,0)
const int thermistor[] PROGMEM = { 0, 1, 2, 3, 4, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 65, 66, 67, 68, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 86, 87, 88, 88, 89, 90, 91, 92, 93, 94, 95, 96, 97, 98, 99, 100, 101, 102, 102, 103, 104, 105, 106, 107, 108, 109, 110, 111, 112, 113, 114, 114, 115, 116, 117, 118, 119, 120, 121, 122, 123, 124, 124, 125, 126, 127, 128, 129, 130, 131, 132, 133, 134, 134, 135, 136, 137, 138, 139, 140, 141, 142, 143, 143, 144, 145, 146, 147, 148, 149, 150, 151, 151, 152, 153, 154, 155, 156, 157, 158, 159, 159, 160, 161, 162, 163, 164, 165, 166, 167, 167, 168, 169, 170, 171, 172, 173, 174, 175, 175, 176, 177, 178, 179, 180, 181, 182, 182, 183, 184, 185, 186, 187, 188, 189, 190, 190, 191, 192, 193, 194, 195, 196, 197, 197, 198, 199, 200, 201, 202, 203, 204, 205, 205, 206, 207, 208, 209, 210, 211, 212, 212, 213, 214, 215, 216, 217, 218, 219, 220, 220, 221, 222, 223, 224, 225, 226, 227, 228, 228, 229, 230, 231, 232, 233, 234, 235, 235, 236, 237, 238, 239, 240, 241, 242, 243, 243, 244, 245, 246, 247, 248, 249, 250, 251, 252, 252, 253, 254, 255, 256, 257, 258, 259, 260, 260, 261, 262, 263, 264, 265, 266, 267, 268, 269, 269, 270, 271, 272, 273, 274, 275, 276, 277, 278, 279, 279, 280, 281, 282, 283, 284, 285, 286, 287, 288, 289, 289, 290, 291, 292, 293, 294, 295, 296, 297, 298, 299, 300 };

#define thermistorPin 0


//Returns the room temperature * 10 in celcius
int getTemp() {
  int value = analogRead(thermistorPin) - 238;
  if (value >= 0 && value <= 324) {
    return pgm_read_word(&(thermistor[value]));
  } else /*Out of range*/ return 0;
}



