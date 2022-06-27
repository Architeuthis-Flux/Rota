

#include <EEPROMex.h>

//#include <Arduino.h>



//#include <EEPROM.h>

//#define OLD 0


#define ENTADDR 0x10  //1 byte, 32 slots, ends at 0x2F
#define ENTNUMADDR 0x04

#define TOTADDR 0x30  //2 bytes, 32 slots, ends at 0x6F
#define TOTLEN 0x03

#define COUNTADDR 0x00  //2 bytes, keep current total here
#define BACKUPCOUNT 0x7E

#define PULSEWIGGLE 5

#define SUBFLAG 0x05
#define CLEARFLAG 0x06

#define LASTMENU 0x07

#define dp0 PIN_PA5

#define dp1 PIN_PA6

#define dp2 PIN_PA7

#define ind PIN_PB5

signed char entries[32] = {0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF};


int totals[32] = {0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF};

unsigned long previousMillis = 0;
const long interval = 150; 
int dpCount = 0;

int duckCount = 0;


unsigned char subtractFlag = 0;
unsigned char clearFlag = 0;

int totalsDP = 0;

int zeroesInARow = 0;

volatile int rotaryPulse = 0;
unsigned long lastPulse = 0;

unsigned long avgPulse = 100;  //ms from rise to rise is actually 164 but ill give it some room
int pulseLen[11] = { 200, 200, 200, 200, 200, 200, 200, 200, 200, 200 };

int pulseCount = 0;

int finishedPulse = 0;

unsigned char lastMenu = EEPROM.readByte(LASTMENU);

//make sure to set BOD higher and read eeprom a few times at startup

const int truthBCD[10][4] = {

  { 0, 1, 0, 0 },
  { 1, 0, 0, 0 },
  { 0, 0, 0, 0 },
  { 1, 0, 0, 1 },
  { 0, 0, 0, 1 },
  { 1, 1, 1, 0 },
  { 0, 1, 1, 0 },
  { 1, 0, 1, 0 },
  { 0, 0, 1, 0 },
  { 1, 1, 0, 0 }

};

int dispDig0 = 0;
int dispDig1 = 0;
int dispDig2 = 0;

int blanking = 1;
int leftJustified = 0;

unsigned long currentMillis = millis();

int doneCountingTogg = 0;


void setup() {

splitBCD(-1);
EEPROM.readByte(0x0D);
  delay(10);

  //PORTB.PIN0CTRL = 0b00001010;

  //splitBCD(duckCount);

  pinMode(PIN_PA1, OUTPUT);
  pinMode(PIN_PA2, OUTPUT);
  pinMode(PIN_PA3, OUTPUT);
  pinMode(PIN_PA4, OUTPUT);

  pinMode(PIN_PA5, OUTPUT);
  pinMode(PIN_PA6, OUTPUT);
  pinMode(PIN_PA7, OUTPUT);

  pinMode(PIN_PB0, INPUT);

  pinMode(PIN_PB1, OUTPUT);
  pinMode(PIN_PB2, OUTPUT);
  pinMode(PIN_PB3, OUTPUT);
  pinMode(PIN_PB4, OUTPUT);

  pinMode(PIN_PB5, OUTPUT);


  pinMode(PIN_PC0, OUTPUT);
  pinMode(PIN_PC1, OUTPUT);
  pinMode(PIN_PC2, OUTPUT);
  pinMode(PIN_PC3, OUTPUT);
  analogSampleDuration(25);
 EEPROM.readByte(0x0D);
delay(5);  




    if (EEPROM.readByte(0x0D) != 0x69) {
    blanking = 0;
    splitBCD(000);
    blanking = 1;
    initEEPROM();
    duckCount = 0;
    EEPROM.updateInt(COUNTADDR, duckCount);
    splitBCD(duckCount);
      } else {

   
    duckCount = EEPROM.readInt(COUNTADDR);
    delay(8);
    duckCount = EEPROM.readInt(COUNTADDR);
    delay(8);
    /*
    duckCount = EEPROM.readInt(COUNTADDR);
    delay(8);
        duckCount = EEPROM.readInt(COUNTADDR);
    delay(8);

    */
     //loadHistory();
     splitBCD(duckCount);
    }


  subtractFlag = EEPROM.readByte(SUBFLAG );
delay(4);  


 if (lastMenu == 0x99){
   //EEPROM.updateByte(LASTMENU, 0x00);
  newDay();
 }
  
if (lastMenu == 0x09){
  //EEPROM.updateByte(LASTMENU, 0x09);
    showTotal();
    totalsDP = 1;
              //digitalWrite (dp2,HIGH);        
          //digitalWrite (dp1,HIGH);         
          //digitalWrite (dp0,HIGH);
          //digitalWrite (ind,HIGH);
    //delay(300);
}

if (lastMenu == 0xD9){
  EEPROM.updateByte(LASTMENU, 0x00);
    showHistory();
    //delay(300);
}
loadHistory();
}

void loop() {


readDial();
//splitBCD(subtractFlag);
totalsDP = 0;
splitBCD(duckCount);
//splitBCD(425);

    if (subtractFlag == 3){
        EEPROM.writeByte(CLEARFLAG, 0x01);
        initEEPROM();
          blanking = 0;
          for(int i = 0; i < 5; i++) {
          splitBCD(000);
          splitBCD(111);
          splitBCD(222);
          splitBCD(333);
          splitBCD(444);
          splitBCD(555);
          splitBCD(666);
          splitBCD(777);
          splitBCD(888);
          splitBCD(999);
          if (i%2 == 1){
            //digitalWrite (ind,LOW);
          }else {
          //digitalWrite (ind,HIGH);
          }
          }
          blanking = 1;
          splitBCD(0);
          
     }
      EEPROM.updateByte(CLEARFLAG, 0x00);

/*if (subtractFlag == 2){
          digitalWrite (dp2,HIGH);        
          digitalWrite (dp1,HIGH);         
          digitalWrite (dp0,HIGH);

}*/


}

void flashDP (void){

unsigned long currentMillis = millis();
//subtractFlag = EEPROM.readByte(SUBFLAG );
  if (currentMillis - previousMillis >= interval) {
   
    previousMillis = currentMillis;
    dpCount++;
    if (dpCount > 4) {
      dpCount = 0;                                                                             
    }
    if (totalsDP == 1){

          //digitalWrite (dp2,HIGH);        
          //digitalWrite (dp1,HIGH);         
          //digitalWrite (dp0,HIGH);
          //digitalWrite (ind,HIGH);

return;


    }else {

      //digitalWrite (ind,LOW);
    }
if (subtractFlag == 2) {
    switch((dpCount) % 4) {
          case 3:
          digitalWrite (dp2,LOW);        
          digitalWrite (dp1,LOW);         
          digitalWrite (dp0,LOW);
          break;

        case 2:
          digitalWrite (dp2,HIGH);        
          digitalWrite (dp1,HIGH);         
          digitalWrite (dp0,HIGH);
          break;
        case 1:
          digitalWrite (dp2,LOW);        
          digitalWrite (dp1,LOW);         
          digitalWrite (dp0,LOW);
          break;
        case 0:
          digitalWrite (dp2,HIGH);        
          digitalWrite (dp1,HIGH);         
          digitalWrite (dp0,HIGH);
          break;          

    }
  
} else if(subtractFlag == 1) {
  //splitBCD(subtractFlag);
    switch((dpCount) % 4) {
          
        case 3:
          digitalWrite (dp2,LOW);        
          digitalWrite (dp1,LOW);         
          digitalWrite (dp0,LOW);
          break;

        case 2:
          digitalWrite (dp2,LOW);        
          digitalWrite (dp1,LOW);         
          digitalWrite (dp0,HIGH);
          break;
        case 1:
          digitalWrite (dp2,LOW);        
          digitalWrite (dp1,HIGH);         
          digitalWrite (dp0,LOW);
          break;
        case 0:
          digitalWrite (dp2,HIGH);        
          digitalWrite (dp1,LOW);         
          digitalWrite (dp0,LOW);
          break;          

    } 

}else{


    switch((dpCount) % 4) {
          case 3:
          digitalWrite (dp2,LOW);        
          digitalWrite (dp1,LOW);         
          digitalWrite (dp0,LOW);
          break;

        case 2:
          digitalWrite (dp2,HIGH);        
          digitalWrite (dp1,LOW);         
          digitalWrite (dp0,LOW);
          break;
        case 1:
          digitalWrite (dp2,LOW);        
          digitalWrite (dp1,HIGH);         
          digitalWrite (dp0,LOW);
          break;
        case 0:
          digitalWrite (dp2,LOW);        
          digitalWrite (dp1,LOW);         
          digitalWrite (dp0,HIGH);
          break;          

    }

}



    
  }
  
}



int readDial (void) {

  pulseCount = 0;

  while(1){ 

    while (1) {
     
    if (readADC() == 2 && finishedPulse == 1) {


        int dialed = triageEntries(pulseCount);
        return dialed;

     finishedPulse = 0;
    }
    
    if (readADC()== 1 && finishedPulse == 0) {
        finishedPulse = 1;
 
        
    }

    if (readADC() == 0 && finishedPulse == 1) {
        finishedPulse = 0;

        
        break;
    }
    if (finishedPulse == 1){
      flashDP();
    }
    
    }     

  do {
    
      if (subtractFlag == 1) {

          pulseCount--;
      }else {
          pulseCount++;
          }

    int tempDuck = duckCount + pulseCount;

      if (tempDuck < 0) {
        tempDuck = 0;
        duckCount = 0;
        }
  
  
  
  
    totalsDP = 0;
    splitBCD(tempDuck );
finishedPulse = 0;
 }while(0);
  
}
}

int triageEntries (int menuSelect) {



switch (abs(menuSelect)) {
case 11:
splitBCD(999);
case 10:
///splitBCD(999);
//delay(100);
        if (EEPROM.readByte(CLEARFLAG) == 1){
          clearAllEntries();
          subtractFlag = 0;
          
        }else{
          subtractFlag ++;
        }        
       
       EEPROM.updateByte(SUBFLAG, subtractFlag);
       //EEPROM.updateByte(CLEARFLAG, 0x00);
       return 0;

case 0:
return 0;

case 9:
  
  if (lastMenu == 0x09){
//newDay();
  EEPROM.updateByte(LASTMENU, 0x99);
  //showHistory();
  return 0;


  } else if (lastMenu == 0x99) {
  EEPROM.updateByte(LASTMENU, 0xD9);
  //newDay();
  return 0;

  } else {
    EEPROM.updateByte(LASTMENU, 0x09);
    //showTotal();
    //delay(1000);
//newDay();
return 0;
} 
return 0;


default:
  





        int newDuck = duckCount+menuSelect;

        duckCount = newDuck;
          if (newDuck < 0) {
          newDuck = 0;
          duckCount = 0;}

        EEPROM.updateInt(COUNTADDR,newDuck );
        splitBCD(newDuck);
        totals[1] = newDuck;
        updateHistory();
        //digitalWrite(ind,HIGH);
      
       EEPROM.writeByte(LASTMENU, 0x00);
       subtractFlag = 0;
       EEPROM.updateByte(SUBFLAG, subtractFlag);


        //while(1) splitBCD(newDuck);
       return menuSelect;

      
}










}

void newDay (void) {

shiftHistory();
totals[1] = duckCount;
duckCount = 0;
  EEPROM.updateInt(COUNTADDR, duckCount);
splitBCD(duckCount);


}

void showHistory (void) {

loadHistory();
showTotal();
unsigned char days = EEPROM.readByte(TOTLEN);

unsigned long currentMillis = millis();
//subtractFlag = EEPROM.readByte(SUBFLAG );



  for (unsigned char i = days; i > 0; i--) {


      leftJustified = 1;
      splitBCD((days-i)+1);
//digitalWrite (ind,LOW);
//splitBCD(readADC());
      while (1) {
        currentMillis = millis();
if (readADC == 1) return;
  if (currentMillis - previousMillis >= 700) {
   
    previousMillis = currentMillis;
    break;
        

      }}
      

      leftJustified = 0;
//splitBCD(readADC());
        splitBCD(totals[i]);
        //digitalWrite (ind,HIGH);
             while (1) {
        currentMillis = millis();
if (readADC == 1) return;
  if (currentMillis - previousMillis >= 700) {
    
    previousMillis = currentMillis;
    break;


      }}
     
  }
splitBCD(duckCount);
}

void showTotal (void) {

  //EEPROM.writeByte(LASTMENU, 0x09);

  EEPROM.readBlock(TOTADDR, totals, 31);
  //delay(10);

int runningTotal = 0;
unsigned char tempLen = 0;

  for (unsigned char i = 1; i < 31; i++) {
      
      if (totals[i] == 0xFFFF) {
        tempLen = i-1;
        break;

      }else {
        runningTotal += totals[i]; 
      }
     

  }
  totals[0] = runningTotal;
  EEPROM.updateByte(TOTLEN, tempLen);
 // splitBCD(tempLen);
  //delay(200);

splitBCD(runningTotal);
updateHistory();
//delay(100);


}



void shiftHistory(void) {

loadHistory();
unsigned char tempTot = 0;

//if (EEPROM.readByte(ENTADDR) != 0xAA){

  for (unsigned char i = 32; i > 1; i--) {

    entries[i] = entries[i - 1];
    totals[i] = totals[i - 1];
  }
  entries[0] = 0x00;

  //totals[0] = 0xAA;
  totals[1] = 0x00;  
  //}  

  EEPROM.updateBlock(TOTADDR, totals, 31);
  delay(3);
  EEPROM.updateBlock(ENTADDR, entries, 31);
  delay(3);
}


void clearAllEntries(void) {


  for (int i = 0; i < 31; i++) {

    entries[i] = 0xFF;
    totals[i] = 0xFFFF;
  }
  updateHistory();

  initEEPROM();

  
}


void updateHistory(void) {

  EEPROM.updateBlock(TOTADDR, totals, 31);
  delay(10);
  EEPROM.updateBlock(ENTADDR, entries, 31);
  delay(10);
}


void loadHistory(void) {

  EEPROM.readBlock(TOTADDR, totals, 31);
  delay(10);
  EEPROM.readBlock(ENTADDR, entries, 31);
  delay(10);


}

void initEEPROM (void) {

    duckCount = 0;
    EEPROM.updateInt(COUNTADDR, duckCount);
    //EEPROM.updateByte(0x08, avgPulse);  
    //delay(4);  
    EEPROM.updateByte(TOTLEN, 0);
    subtractFlag = 0;
    EEPROM.updateByte(SUBFLAG, subtractFlag);
    //EEPROM.updateByte(0x08, 100);
    EEPROM.updateByte(0x0D,0x69);
    delay(4);
     
    
           
    //updateHistory();
    blanking = 0;
    splitBCD(000);
    blanking = 1;
 
}

int readADC (void) {

  int avg1 = analogRead(PIN_PB0);

  delayMicroseconds(1000);
  
  int avg2 = analogRead(PIN_PB0);

  delayMicroseconds(1000);
  
  int avg3 = analogRead(PIN_PB0);

    delayMicroseconds(1000);
  
  int avg4 = analogRead(PIN_PB0);

 int avgTot = (avg1+avg2+avg3+avg4)/4;

/*#ifdef OLD 1
if (avgTot > 380) {
  return 2;
} else if (avgTot > 160) {
  return 1;
} else {
  return 0;
}

#endif*/

if (avgTot > 400 && avgTot < 800) {
  return 2;
} else if (avgTot > 800) {
  return 1;
}  else {
  return 0;
}
}



void dispNix(void) {

  digitalWrite(PIN_PA1, truthBCD[dispDig0][0]);
  digitalWrite(PIN_PA2, truthBCD[dispDig0][1]);
  digitalWrite(PIN_PA3, truthBCD[dispDig0][2]);
  digitalWrite(PIN_PA4, truthBCD[dispDig0][3]);

  digitalWrite(PIN_PB1, truthBCD[dispDig1][0]);
  digitalWrite(PIN_PB2, truthBCD[dispDig1][1]);
  digitalWrite(PIN_PB3, truthBCD[dispDig1][2]);
  digitalWrite(PIN_PB4, truthBCD[dispDig1][3]);

  digitalWrite(PIN_PC0, truthBCD[dispDig2][0]);
  digitalWrite(PIN_PC1, truthBCD[dispDig2][1]);
  digitalWrite(PIN_PC2, truthBCD[dispDig2][2]);
  digitalWrite(PIN_PC3, truthBCD[dispDig2][3]);
}



void splitBCD(int dispNum) {

if (dispNum < 0 ){
  dispDig0 = 15;
  dispDig1 = 15;
  dispDig2 = 15;

}else{

  dispDig0 = dispNum % 10;
  dispDig1 = (dispNum / 10) % 10;
  dispDig2 = dispNum / 100;
}

  if (blanking >= 1) {

    if (dispDig1 == 0 && dispDig2 == 0) {
      dispDig1 = 15;
      dispDig2 = 15;
    }

    if (dispDig2 == 0) {
      dispDig2 = 15;
    }
  }

    if (leftJustified >= 1) {
      
    if (dispDig1 == 15 && dispDig2 == 15) {
      dispDig2 = dispDig0;
      dispDig0 = 15;
      dispDig1 = 15;
      
    }


    if (dispDig2 == 15) {
      dispDig2 = dispDig1;
      dispDig1 = dispDig0;
      dispDig0 = 15;
    }





  }
  EEPROM.updateByte(0x77, dispDig0);
  EEPROM.updateByte(0x7A, dispDig1);
  EEPROM.updateByte(0x7D, dispDig2);
  dispNix();
}














