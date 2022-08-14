

/*


here's the special function menu map

note - I'm calling moving the dial just a little to turn the display on a "click"

when inside a special function, dial anything to exit (preferably 0 because it's only reading intermittently when in menus, so it's most likely to register)

0 -- enter/exit menu or funtion   shows 111 when you're in the menu)
1 -- stopwatch                    click to start, click again to see last time.
2 -- show battery voltage         briefly turns off the display to get a more accurate reading (not under load)
3 -- new day                      this stores the total since the last time you started a new day. maximum 63 days
4 -- does nothing
5 -- enter a count by digits      enter 3 digits (hundreds - tens - ones) to add to your daily total
6 -- show history                 click through each day with first the day number (on the left) then the total for that day
7 -- toggle day/total display     either shows the count for that day or the running total of all days (default is total)
8 -- golf mode?                   maybe I'll add a mode where it will keep score for golf or whatever
9 -- clear everything             dial 9 again to confirm. dial anything else to go back

999999 -- emergency clear         dial 9 six times to emergency clear everything in case something gets messed up same as dialing 10 9 9


*/

#include <EEPROM.h>

#define ENTADDR 0x10 // 1 byte, 32 slots, ends at 0x2F
#define ENTNUMADDR 0x04

#define TOTADDR 0x30 // 2 bytes, 32 slots, ends at 0x6F
#define TOTLEN 0x03

#define COUNTADDRH 0x01 // keep current total here High Byte
#define COUNTADDRL 0x02 // keep current total here Low Byte

#define BACKUPCOUNTH 0xF1
#define BACKUPCOUNTL 0xF2

#define TOTALCOUNTH 0x0B
#define TOTALCOUNTL 0x0C

#define SPECIALSELECTION 0x09

#define SPECIALMENUFLAG 0xE0

#define STOPWATCHFLAG 0xE1
#define VOLTAGEFLAG 0xE2
#define NEWDAYFLAG 0xE3
#define ENTERTOTALFLAG 0xE5
#define SHOWHISTORYFLAG 0xE6
#define DAYTOTALDISPLAYFLAG 0xE7
#define GOLFMODEFLAG 0xE8 // I might come up with a low power mode someday
#define CLEAREVERYTHINGFLAG 0xE9

#define GOLFSUBTRACTFLAG 0xED
#define GOLFHOLE 0xEA
#define GOLFPLAYER 0xEB
#define GOLFNUMPLAYERS 0xEC
#define TEMPPLAYER 0xEE
#define TEMPHOLE 0xEF

#define PLAYER1SCORE 0xD1
#define PLAYER2SCORE 0xD2
#define PLAYER3SCORE 0xD3
#define PLAYER4SCORE 0xD4

#define GOLFTOTALFLAG 0xD0

#define TIMEDIGITSTART 0xC0 // move this to not wear out the EEPROM

#define SUBFLAG 0x05
#define CLEARFLAG 0x06

#define dp0 PIN_PD5
#define dp1 PIN_PD6
#define dp2 PIN_PD7
#define ind PIN_PA3

#define pulse1 PIN_PA0
#define pulse2 PIN_PA1
#define pulse3 PIN_PA2

#define MEMLEN 64

int8_t entries[33] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

uint8_t totals[2][MEMLEN + 1] = {{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}};
// totals[0][] is High Byte    totals [1][] is Low Byte

uint8_t playerScore[4][20] = {{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}};

unsigned long previousMillis = 0;
const long interval = 150;
long intervalStopwatch = 10;
int dpCount = 0;

int mil = 1;

int duckCount = 0;

uint8_t duckCountH = 0;
uint8_t duckCountL = 0;

uint8_t dayTotal = 0;

uint16_t totalCount = 0;

uint8_t totalCountH = 0;
uint8_t totalCountL = 0;

uint8_t subtractFlag = 0;
uint8_t clearFlag = 0;

int pulseCount = 0;

uint8_t totlen = 0;
uint8_t entlen = 0;

// make sure to set BOD higher and read eeprom a few times at startup

const int truthBCD[10][4] = {

    {0, 0, 0, 0},
    {0, 0, 0, 1}, //
    {0, 0, 1, 0},
    {0, 0, 1, 1},
    {0, 1, 0, 0},
    {0, 1, 0, 1},
    {0, 1, 1, 0}, //
    {0, 1, 1, 1},
    {1, 0, 0, 0},
    {1, 0, 0, 1}

};

int enteredTotal1 = EEPROM.read(0xCA);
int enteredTotal2 = EEPROM.read(0xCB);
int enteredTotal3 = EEPROM.read(0xCC);

int golfHole = EEPROM.read(GOLFHOLE);
int golfPlayer = EEPROM.read(GOLFPLAYER);
uint8_t golfMode = EEPROM.read(GOLFMODEFLAG);
uint8_t golfNumPlayers = EEPROM.read(GOLFNUMPLAYERS);

int dispDig0 = 0;
int dispDig1 = 0;
int dispDig2 = 0;

uint8_t blanking = 1;
uint8_t leftJustified = 0;
uint8_t centered = 0;

void setup()
{
  splitBCD(-1);      //-1 shows nothing
  EEPROM.read(0x0D); // check for first startup (but this read gets thrown away because EEPROM can be weird)

  pinMode(PIN_PF0, OUTPUT); // this enables power to the HV supply
  digitalWriteFast(PIN_PF0, HIGH);

  pinMode(PIN_PD5, OUTPUT);
  pinMode(PIN_PD6, OUTPUT);
  pinMode(PIN_PD7, OUTPUT);
  digitalWriteFast(PIN_PD5, LOW);
  digitalWriteFast(PIN_PD6, LOW);
  digitalWriteFast(PIN_PD7, LOW);

  pinMode(PIN_PA4, OUTPUT);
  pinMode(PIN_PA5, OUTPUT);
  pinMode(PIN_PA6, OUTPUT);
  pinMode(PIN_PA7, OUTPUT);

  pinMode(pulse1, INPUT_PULLUP);
  pinMode(pulse2, INPUT);
  pinMode(pulse3, INPUT);
  pinMode(PIN_PA2, INPUT);

  pinMode(PIN_PA3, OUTPUT);

  pinMode(PIN_PD1, OUTPUT);
  pinMode(PIN_PD2, OUTPUT);
  pinMode(PIN_PD3, OUTPUT);
  pinMode(PIN_PD4, OUTPUT);

  pinMode(PIN_PF1, INPUT);

  pinMode(PIN_PC0, OUTPUT);
  pinMode(PIN_PC1, OUTPUT);
  pinMode(PIN_PC2, OUTPUT);
  pinMode(PIN_PC3, OUTPUT);

  analogReadResolution(10);
  analogSampleDuration(500);

  if (EEPROM.read(0x0D) != 0x69 || EEPROM.read(CLEARFLAG) >= 0x03 || digitalRead(pulse2) == 1) // sets everything up on first startup or emergency clear (6 9's in a row) or super emergengy hardware clear by pulling pulse 2 high
  {

    voltageDisplay(5);

    initEEPROM();
    duckCount = 0;
    EEPROM.update(COUNTADDRH, 0);
    EEPROM.update(COUNTADDRL, 0);
  }
  else
  {

    duckCountH = EEPROM.read(COUNTADDRH);
    duckCountL = EEPROM.read(COUNTADDRL);

    totalCountH = EEPROM.read(TOTALCOUNTH);
    totalCountL = EEPROM.read(TOTALCOUNTL);

    totlen = EEPROM.read(TOTLEN);
    entlen = EEPROM.read(ENTNUMADDR);

    bytes2dc();
  }
  if (EEPROM.read(DAYTOTALDISPLAYFLAG) == 0xDA) // show daily count or total count
  {
    dayTotal = 1;
  }
  else
  {
    dayTotal = 0;
  }

  if (EEPROM.read(SPECIALMENUFLAG) == 0xFF) // if it's 0xFF then we aren't in a special menu
  {
    if (dayTotal == 1)
    {
      splitBCD(duckCount);
    }
    else
    {
      splitBCD(totalCount);
    }
  }

  subtractFlag = EEPROM.read(SUBFLAG);

  if (EEPROM.read(SPECIALMENUFLAG) == 0xAA || EEPROM.read(SPECIALMENUFLAG) == 0x18)
  {
    specialMenu(); // this function takes input from the dial
  }
  if (EEPROM.read(SPECIALMENUFLAG) != 0xFF && EEPROM.read(SPECIALMENUFLAG) != 0x18)
  {
    checkSpecialMenuFlags(); // this one doesn't
  }

  loadHistory();
}

void loop()
{

  if (golfMode != 0x01)
  {
    if (dayTotal == 1)
    {
      splitBCD(duckCount);
    }
    else
    {
      splitBCD(totalCount);
    }
  }
  readDial(0);

  writedc();
}

void checkSpecialMenuFlags(void)
{

  uint8_t smFlagVal = EEPROM.read(SPECIALMENUFLAG);

  switch (smFlagVal)
  {
  case 0xFF:
    return;

  case 0x01:
    if (EEPROM.read(STOPWATCHFLAG) == 0xFF)
    {
      EEPROM.update(STOPWATCHFLAG, 0x01);
    }

    stopwatch();
    while (1)
      ;

  case 0x02:
    while (digitalRead(pulse1) == 0)
    {
      voltageDisplay(8);

      for (int i = 0; i < 40; i++)
      { // this works as a sort of interrupt to get out of voltage display

        if (digitalRead(pulse1) == 1)
        {
          EEPROM.update(SPECIALMENUFLAG, 0xFF);
          while (1)
            ;
        }
        else
        {
          delay(20);
        }
      }
    }
    return;

  case 0x03:
    EEPROM.update(SPECIALMENUFLAG, 0xFF);
    newDay();
    return;

  case 0x05:

    enterStartingTotal();

    while (1)
      ;

  case 0x06:
    if (golfMode == 1)
    {
      EEPROM.update(SHOWHISTORYFLAG, 0xEE);
      showGolfHistory();
      return;
    }

    if (EEPROM.read(SHOWHISTORYFLAG) == 0xFF)
    {
      EEPROM.update(SHOWHISTORYFLAG, EEPROM.read(TOTLEN));
      EEPROM.update(SPECIALMENUFLAG, 0x06);
      EEPROM.update(SPECIALSELECTION, 0x06);
    }

    showHistory();

    return;

  case 0x07:
    EEPROM.update(SPECIALMENUFLAG, 0xFF);
    if (EEPROM.read(DAYTOTALDISPLAYFLAG) != 0xDA)
    {
      EEPROM.update(DAYTOTALDISPLAYFLAG, 0xDA);
      dayTotal = 1;
    }
    else
    {
      EEPROM.update(DAYTOTALDISPLAYFLAG, 0xFF);
      dayTotal = 0;
    }
    return;

  case 0x08:

    if (EEPROM.read(GOLFMODEFLAG) == 0x01)
    {
      golfMode = 0;
      EEPROM.update(SPECIALMENUFLAG, 0xFF);
      EEPROM.update(GOLFMODEFLAG, 0xFF);
      EEPROM.update(DAYTOTALDISPLAYFLAG, 0xFF);
      dayTotal = 0;
      initEEPROM();
    }
    else
    {
      EEPROM.update(DAYTOTALDISPLAYFLAG, 0xDA); // golf mode defaults to showing days (holes)
      dayTotal = 1;
      EEPROM.update(SPECIALMENUFLAG, 0x18);
      duckCount = 0;
      totalCount = 0;
      writedc();
      clearAllEntries();
    }

    specialMenu();
    return;

  case 0x09:

    EEPROM.update(CLEAREVERYTHINGFLAG, 0x99);
    EEPROM.update(SPECIALMENUFLAG, 0xAA);
    subtractFlag = 2;
    EEPROM.update(SUBFLAG, 0x02);
    splitBCD(totalCount);
    specialMenu();
  }

  if (EEPROM.read(SHOWHISTORYFLAG) > 0x00 && EEPROM.read(SHOWHISTORYFLAG) < 0x40)
  {

    showHistory();
  }
}

void specialMenu(void)
{

  uint8_t specialSelection = 0;
  uint8_t smFlagVal = EEPROM.read(SPECIALMENUFLAG);
  golfMode = 0;

  if (EEPROM.read(CLEAREVERYTHINGFLAG) == 0x99)
  {

    while (1)
    {

      specialSelection = abs(readDial(9));

      if (specialSelection > 0)
      {
        break;
      }
    }

    if (specialSelection == 9) // clear confirmed
    {
      subtractFlag = 0;
      EEPROM.update(SUBFLAG, subtractFlag);
      EEPROM.update(SPECIALMENUFLAG, 0xFF);
      EEPROM.update(CLEAREVERYTHINGFLAG, 0xFF);

      initEEPROM();
      return;
    }
    else // didn't confirm clear
    {

      subtractFlag = 0;
      EEPROM.update(SUBFLAG, subtractFlag);
      EEPROM.update(SPECIALMENUFLAG, 0xFF);
      EEPROM.update(CLEAREVERYTHINGFLAG, 0xFF);
      return;
    }
  }

  if (EEPROM.read(SUBFLAG) == 1 || EEPROM.read(SUBFLAG) == 2)
  {
    subtractFlag = 0;
    EEPROM.update(SUBFLAG, subtractFlag);
    return;
  }

  if (smFlagVal == 0xAA)
  {
    while (1)
    {
      splitBCD(111);
      specialSelection = readDial(1);
      if (specialSelection > 0)
      {
        break;
      }
    }

    if (specialSelection == 10)
    {
      EEPROM.update(SPECIALMENUFLAG, 0xFF);
      while (1)
        ;
    }

    EEPROM.update(SPECIALMENUFLAG, specialSelection);

    while (1)
      ;
  }
  else if (smFlagVal == 0x18)
  {
    askHowManyPlayers();
  }
  else
  {
    checkSpecialMenuFlags();
  }
}

void askHowManyPlayers(void)
{
  int animationCount = 0;
  int golfSelection = 0;
  unsigned long previousMillisg = 0;

  while (animationCount < 6 && digitalRead(pulse1) == 0)
  {

    unsigned long currentMillisg = millis();

    if (currentMillisg - previousMillisg >= 120)
    {

      previousMillisg = currentMillisg;
      animationCount++;
    }
    switch (animationCount)
    {

    case 0:
      leftJustified = 1;
      splitBCD(7);

      digitalWriteFast(dp2, LOW);
      digitalWriteFast(dp1, LOW);
      digitalWriteFast(dp0, HIGH);
      digitalWriteFast(ind, LOW);
      leftJustified = 0;
      break;
    case 1:
      leftJustified = 1;
      splitBCD(7);

      digitalWriteFast(dp2, LOW);
      digitalWriteFast(dp1, LOW);
      digitalWriteFast(dp0, HIGH);
      digitalWriteFast(ind, LOW);
      leftJustified = 0;
      break;
    case 2:
      leftJustified = 1;
      splitBCD(1);
      digitalWriteFast(dp2, LOW);
      digitalWriteFast(dp1, HIGH);
      digitalWriteFast(dp0, LOW);
      digitalWriteFast(ind, LOW);
      leftJustified = 0;
      break;
    case 3:
      leftJustified = 1;
      splitBCD(1);
      digitalWriteFast(dp2, HIGH);
      digitalWriteFast(dp1, LOW);
      digitalWriteFast(dp0, LOW);
      digitalWriteFast(ind, LOW);
      leftJustified = 0;
      break;
    case 4:
      leftJustified = 1;
      splitBCD(1);
      digitalWriteFast(dp2, LOW);
      digitalWriteFast(dp1, LOW);
      digitalWriteFast(dp0, LOW);
      digitalWriteFast(ind, LOW);
      leftJustified = 0;
      break;
    case 5:
      leftJustified = 1;
      splitBCD(1);
      digitalWriteFast(dp2, LOW);
      digitalWriteFast(dp1, LOW);
      digitalWriteFast(dp0, LOW);
      digitalWriteFast(ind, LOW);
      leftJustified = 0;
      break;
    case 6:
      leftJustified = 1;
      splitBCD(1);
      digitalWriteFast(dp2, LOW);
      digitalWriteFast(dp1, LOW);
      digitalWriteFast(dp0, LOW);
      digitalWriteFast(ind, LOW);
      leftJustified = 0;
      break;
    }
  }
  while (1)
  {
    splitBCD(1);

    golfPlayerDP(0, 1);

    golfSelection = readDial(18);
    if (golfSelection > 0)
    {
      break;
    }
    else
    {
      golfPlayerDP(0, 1);
    }
  }
  if (golfSelection > 4)
  {

    while (1)
      ;
  }

  if (golfSelection == 10)
  {
    EEPROM.update(SPECIALMENUFLAG, 0xFF);
    while (1)
      ;
  }

  golfNumPlayers = golfSelection;
  EEPROM.update(GOLFNUMPLAYERS, golfNumPlayers);
  EEPROM.update(SPECIALMENUFLAG, 0xFF);
  EEPROM.update(GOLFPLAYER, 0x00);
  EEPROM.update(GOLFHOLE, 0x00);

  if (EEPROM.read(GOLFMODEFLAG) != 0x01)
  {
    golfMode = 1;
    EEPROM.update(GOLFMODEFLAG, 0x01);
  }

  clearAllEntries();
  while (1)
    ;
}

void golfPlayerDP(int playerNumber, int select)
{

  if (select == 1)
  {
    if (millis() % 180 > 100)
    {
      
      digitalWriteFast(dp2, LOW);
      digitalWriteFast(dp1, LOW);
      digitalWriteFast(dp0, LOW);
      digitalWriteFast(ind, LOW);
      
    }
    else
    {
      switch (playerNumber)
      {
      case -1:
        digitalWriteFast(dp2, LOW);
        digitalWriteFast(dp1, LOW);
        digitalWriteFast(dp0, HIGH);
        digitalWriteFast(ind, LOW);

      case 0:
        digitalWriteFast(dp2, LOW);
        digitalWriteFast(dp1, LOW);
        digitalWriteFast(dp0, HIGH);
        digitalWriteFast(ind, LOW);

        break;
      case 1:
        digitalWriteFast(dp2, LOW);
        digitalWriteFast(dp1, HIGH);
        digitalWriteFast(dp0, HIGH);
        digitalWriteFast(ind, LOW);

        break;
      case 2:
        digitalWriteFast(dp2, HIGH);
        digitalWriteFast(dp1, HIGH);
        digitalWriteFast(dp0, HIGH);
        digitalWriteFast(ind, LOW);

        break;
      case 3:

        digitalWriteFast(dp2, HIGH);
        digitalWriteFast(dp1, HIGH);
        digitalWriteFast(dp0, HIGH);
        digitalWriteFast(ind, HIGH);

        break;
      }
    }
    //} else {
    // mil = 1;
  }
  else
  {
    switch (playerNumber)
    {

    case 0:
      digitalWriteFast(dp2, LOW);
      digitalWriteFast(dp1, LOW);
      digitalWriteFast(dp0, HIGH);
      digitalWriteFast(ind, LOW);

      break;
    case 1:
      digitalWriteFast(dp2, LOW);
      digitalWriteFast(dp1, HIGH);
      digitalWriteFast(dp0, LOW);
      digitalWriteFast(ind, LOW);

      break;
    case 2:
      digitalWriteFast(dp2, HIGH);
      digitalWriteFast(dp1, LOW);
      digitalWriteFast(dp0, LOW);
      digitalWriteFast(ind, LOW);

      break;
    case 3:

      digitalWriteFast(dp2, LOW);
      digitalWriteFast(dp1, LOW);
      digitalWriteFast(dp0, LOW);
      digitalWriteFast(ind, HIGH);

      break;
    }
  }
}

void flashDP(void)
{

  unsigned long currentMillis = millis();
  // subtractFlag = EEPROM.readByte(SUBFLAG );
  if (currentMillis - previousMillis >= interval)
  {

    previousMillis = currentMillis;
    dpCount++;

    if (dpCount > 4)
    {
      dpCount = 0;
    }

    else
    {

      // digitalWriteFast (ind,LOW);
    }
    if (subtractFlag == 2)
    {
      // splitBCD(dpCount);
      switch ((dpCount) % 4)
      {
      case 3:
        digitalWriteFast(dp2, LOW);
        digitalWriteFast(dp1, LOW);
        digitalWriteFast(dp0, LOW);
        digitalWriteFast(ind, HIGH);
        break;

      case 2:
        digitalWriteFast(dp2, HIGH);
        digitalWriteFast(dp1, HIGH);
        digitalWriteFast(dp0, HIGH);
        digitalWriteFast(ind, LOW);
        break;
      case 1:
        digitalWriteFast(dp2, LOW);
        digitalWriteFast(dp1, LOW);
        digitalWriteFast(dp0, LOW);
        digitalWriteFast(ind, HIGH);
        break;
      case 0:
        digitalWriteFast(dp2, HIGH);
        digitalWriteFast(dp1, HIGH);
        digitalWriteFast(dp0, HIGH);
        digitalWriteFast(ind, LOW);
        break;
      }
    }
    else if (subtractFlag == 1)
    {
      // splitBCD(subtractFlag);
      switch ((dpCount) % 4)
      {

      case 3:
        digitalWriteFast(dp2, LOW);
        digitalWriteFast(dp1, LOW);
        digitalWriteFast(dp0, LOW);
        digitalWriteFast(ind, HIGH);
        break;

      case 2:
        digitalWriteFast(dp2, LOW);
        digitalWriteFast(dp1, LOW);
        digitalWriteFast(dp0, HIGH);
        digitalWriteFast(ind, LOW);
        break;
      case 1:
        digitalWriteFast(dp2, LOW);
        digitalWriteFast(dp1, HIGH);
        digitalWriteFast(dp0, LOW);
        digitalWriteFast(ind, LOW);
        break;
      case 0:
        digitalWriteFast(dp2, HIGH);
        digitalWriteFast(dp1, LOW);
        digitalWriteFast(dp0, LOW);
        digitalWriteFast(ind, LOW);
        break;
      }
    }
    else
    {

      switch ((dpCount) % 4)
      {
      case 3:
        digitalWriteFast(dp2, LOW);
        digitalWriteFast(dp1, LOW);
        digitalWriteFast(dp0, LOW);
        digitalWriteFast(ind, HIGH);
        break;

      case 2:
        digitalWriteFast(dp2, HIGH);
        digitalWriteFast(dp1, LOW);
        digitalWriteFast(dp0, LOW);
        digitalWriteFast(ind, LOW);
        break;
      case 1:
        digitalWriteFast(dp2, LOW);
        digitalWriteFast(dp1, HIGH);
        digitalWriteFast(dp0, LOW);
        digitalWriteFast(ind, LOW);
        break;
      case 0:
        digitalWriteFast(dp2, LOW);
        digitalWriteFast(dp1, LOW);
        digitalWriteFast(dp0, HIGH);
        digitalWriteFast(ind, LOW);
        break;
      }
    }
  }
}

int readDial(int special)
{

  pulseCount = 0;
  int tempDuck = 0;
  int tempTotal = 0;

  int lastPulse = 1;
  int p1 = 1;
  int p3 = 1;

  if (special == 18)
  {
    while (digitalRead(pulse3) == 1)
    {
      golfPlayerDP(0, 1);
    }
  }

  if (golfMode == 1)
  {
    leftJustified = 1;
    if (golfHole < 0)
    {
      golfHole = 0;
      EEPROM.update(GOLFHOLE, golfHole);
    }
    splitBCD(golfHole + 1);
    leftJustified = 0;

    golfPlayerDP(golfPlayer, 0);

    if (dayTotal == 0)
    {
      delay(250);
      splitBCD(EEPROM.read(PLAYER1SCORE + golfPlayer));
    }
  }
  else if (special == 0 || subtractFlag == 2 || special == 9)
  {
    flashDP();
  }

  while (digitalRead(pulse3) == 0)
  {
    if (special == 18)
    {

      golfPlayerDP(pulseCount - 1, 1);
    }

    if (golfMode == 1)
    {

      golfPlayerDP(golfPlayer, 0);
    }
    else if (special == 0 || subtractFlag == 2)
    {
      flashDP();
    }

    delay(8);
    p3 = digitalRead(pulse3);
    p1 = digitalRead(pulse1);

    if (lastPulse == 0 && p1 == 1 && p3 == 0)
    {
      // digitalWrite(ind, HIGH);
      lastPulse = 1;
      // splitBCD(000);
      if (subtractFlag == 1 && special != 9)
      {

        pulseCount--;
      }
      else
      {
        pulseCount++;
      }

      tempDuck = duckCount + pulseCount;
      tempTotal = totalCount + pulseCount;

      if (tempTotal < 0)
      {
        tempDuck = 0;
        duckCount = 0;
        totalCount = 0;
        subtractFlag = 0;
        EEPROM.update(SUBFLAG, subtractFlag);
        splitBCD(0);
        writedc();
        while (1)
          ;
      }
      else
      {
        tempDuck = duckCount + pulseCount;
        tempTotal = totalCount + pulseCount;
      }

      // totalsDP = 0;

      if (special == 0 && golfMode != 0x01)
      {
        if (dayTotal == 1)
        {
          splitBCD(tempDuck);
        }
        else
        {
          splitBCD(tempTotal);
        }
      }
      else if (special == 0 && golfMode == 0x01)
      {
        if (pulseCount == 0 && dayTotal == 1)
        {
          splitBCD(-1);
        }
        else
        {

          if (dayTotal == 1)
          {
            splitBCD(tempDuck);
          }
          else
          {
            splitBCD(EEPROM.read(PLAYER1SCORE + golfPlayer) + tempDuck);
          }
        }
      }

      else if (special == 1)
      {
        centered = 1;
        splitBCD(pulseCount);

        centered = 0;
      }
      else if (special == 2)
      {
        // leftJustified = 1;
        if (pulseCount == 10)
        {

          // splitBCD(000);
        }
        else
        {
          splitBCD(pulseCount * 100);
        }
        // splitBCD(pulseCount) ;
        // leftJustified = 0;
      }
      else if (special == 3)
      {
        if (enteredTotal1 == 0)
        {

          splitBCD(pulseCount * 10);
        }

        if (pulseCount == 10)
        {

          splitBCD((enteredTotal1 * 100) + (pulseCount * 10));
        }
        else
        {
          splitBCD((enteredTotal1 * 100) + (pulseCount * 10));
          // leftJustified = 0;
        }
      }
      else if (special == 4)
      {
        if (enteredTotal1 == 0)
        {

          splitBCD((enteredTotal2 * 10) + pulseCount);
        }

        splitBCD((enteredTotal1 * 100) + (enteredTotal2 * 10) + pulseCount);
      }
      else if (special == 9)
      {

        splitBCD(totalCount);
        flashDP();
      }
      else if (special == 18)
      {
        // centered = 1;

        splitBCD(pulseCount);
        if (pulseCount == 0)
        {
          golfPlayerDP(0, 1);
        }
        else
        {
          golfPlayerDP(pulseCount - 1, 1);
        }

        // centered = 0;
      }

      delay(4);
    }

    if (p1 == 1)
    {
      lastPulse = 1;
    }
    else
    {
      lastPulse = 0;
    }
  }

  if (special == 0) // so it doesn't add to the total when using special menu
  {

    triageEntries(pulseCount);
  }

  return pulseCount;
}

int triageEntries(int menuSelect)
{

  menuSelect = abs(menuSelect);

  switch (abs(menuSelect))
  {
  case 11:
    splitBCD(999);
  case 10:
    if (EEPROM.read(SUBFLAG) == 1)
    {
      subtractFlag = 0;
      EEPROM.update(SUBFLAG, subtractFlag);
      return 0;
    }
    EEPROM.update(SPECIALMENUFLAG, 0xAA);
    specialMenu();
    return;

    return 0;

  case 0:
    return 0;

  case 9:

    if (golfMode == 1)
    {
      golfHole = EEPROM.read(GOLFHOLE);
      golfNumPlayers = EEPROM.read(GOLFNUMPLAYERS);
      golfPlayer = EEPROM.read(GOLFPLAYER);

      if (golfPlayer > 0x00)
      {

        golfPlayer--;
      }
      else
      {

        golfPlayer = golfNumPlayers - 1;
        golfHole--;
        if (golfHole < 0)
        {
          golfHole = 0;
          golfPlayer = 0;
          EEPROM.update(GOLFHOLE, golfHole);
          return;
        }
        EEPROM.update(GOLFHOLE, golfHole);
      }

      int entryToSubtract = EEPROM.read((TOTADDR + (golfHole * golfNumPlayers) + golfPlayer));
      int tempScore = EEPROM.read(PLAYER1SCORE + golfPlayer);
      digitalWriteFast(ind, HIGH);
      splitBCD(tempScore);

      tempScore = tempScore - entryToSubtract;

      if (tempScore < 0)
        tempScore = 0;

      splitBCD(tempScore);

      digitalWriteFast(ind, LOW);
      EEPROM.update(PLAYER1SCORE + golfPlayer, tempScore);

      playerScore[golfPlayer][golfHole] = 0x00;

      EEPROM.update(TOTADDR + (golfHole * golfNumPlayers) + golfPlayer, 0xFF);

      // EEPROM.update(PLAYER1SCORE + golfPlayer, EEPROM.read(ENTADDR));

      duckCount = 0;

      EEPROM.update(GOLFPLAYER, golfPlayer);
      golfPlayerDP(golfPlayer, 0);

      while (1)
        ;
    }
    else
    {

      if (EEPROM.read(SUBFLAG) == 1)
      {
        uint8_t clf = EEPROM.read(CLEARFLAG);
        clf++;
        EEPROM.update(CLEARFLAG, clf);
        subtractFlag = 0;
        EEPROM.update(SUBFLAG, subtractFlag);
        return 0;
      }
      else if (EEPROM.read(SUBFLAG) == 0)
      {
        subtractFlag = 1;
        EEPROM.update(SUBFLAG, subtractFlag);
      }
      return 0;
    }

  default:

    if (subtractFlag == 1)
    {
      duckCount = duckCount - menuSelect;
      totalCount = totalCount - menuSelect;
      if (duckCount <= 0)
      {
        duckCount = 0;
      }

      if (totalCount <= 0)
      {
        totalCount = 0;
      }

      subtractFlag = 0;
      EEPROM.update(SUBFLAG, subtractFlag);
    }
    else
    {

      duckCount = duckCount + menuSelect;
      totalCount = totalCount + menuSelect;
    }
    writedc();

    EEPROM.update(CLEARFLAG, 0x00);
    // EEPROM.update(ENTADDR, menuSelect);

    if (EEPROM.read(GOLFMODEFLAG) == 0x01)
    {

      golfHole = EEPROM.read(GOLFHOLE);
      golfPlayerDP(golfPlayer, 0);
      playerScore[golfPlayer][golfHole] = menuSelect;
      newDay();
      updateHistory();

      if (EEPROM.read(GOLFHOLE) >= 17 && EEPROM.read(GOLFPLAYER) + 1 >= EEPROM.read(GOLFNUMPLAYERS))
      {
        EEPROM.update(TEMPPLAYER, 0x00);
        EEPROM.update(GOLFTOTALFLAG, 0x02);
        EEPROM.update(SPECIALMENUFLAG, 0x06);
        EEPROM.update(SHOWHISTORYFLAG, 0xEE);
        return;
        while (1)
          ;
        showGolfHistory();
      }

      golfPlayer++;
      if (EEPROM.read(GOLFNUMPLAYERS) <= golfPlayer)
      {
        golfPlayer = 0;
        golfHole = EEPROM.read(GOLFHOLE);
        golfHole++;
        EEPROM.update(GOLFHOLE, golfHole);
      }
      EEPROM.update(GOLFPLAYER, golfPlayer);
    }
    else if (dayTotal == 1)
    {
      splitBCD(duckCount);
    }
    else
    {
      splitBCD(totalCount);
    }

    addEntry(abs(menuSelect));

    if (EEPROM.read(SUBFLAG) == 1)
    {
      subtractFlag = 0;
      EEPROM.update(SUBFLAG, subtractFlag);
      return 0;
    }

    // while(1) splitBCD(duckCount);

    return menuSelect;
  }
}

void shiftHistory(void)
{

  loadHistory();

  if (EEPROM.read(GOLFMODEFLAG) != 0x01)
  {
    totlen = EEPROM.read(TOTLEN);
    totlen++;
    EEPROM.update(TOTLEN, totlen);

    for (int i = MEMLEN ; i > 0; i--)
    {
      totals[0][i] = totals[0][i - 1];
      totals[1][i] = totals[1][i - 1];
    }

    totals[0][0] = duckCountH;
    totals[1][0] = duckCountL;

    for (int i = 0; i <= totlen && i < MEMLEN; i++)
    {
      EEPROM.update(TOTADDR + (i * 2), totals[0][i]);
      EEPROM.update(TOTADDR + (i * 2) + 1, totals[1][i]);
    }
  }
  else
  {

    // playerScore[golfPlayer][golfHole] = duckCountL;

    EEPROM.update(TOTADDR + (EEPROM.read(GOLFHOLE) * EEPROM.read(GOLFNUMPLAYERS)) + golfPlayer, playerScore[golfPlayer][EEPROM.read(GOLFHOLE)]);

    uint8_t tempGolfTotal = EEPROM.read(PLAYER1SCORE + golfPlayer);
    tempGolfTotal = tempGolfTotal + playerScore[golfPlayer][EEPROM.read(GOLFHOLE)];
    EEPROM.update(PLAYER1SCORE + golfPlayer, tempGolfTotal);
  }
}

void addEntry(int entry)
{

  

  entlen = EEPROM.read(ENTNUMADDR);
  entlen++;
  EEPROM.update(ENTNUMADDR, entlen);

  for (int i = 30; i > 0; i--)
  {

    entries[i] = entries[i - 1];
  }
  entries[0] = entry;

  for (int i = 0; i <= entlen && i < 32; i++)
  {

    EEPROM.update(ENTADDR + i, entries[i]);

  }
}

void clearAllEntries(void)
{

  for (int i = 0; i < MEMLEN; i++)
  {
    totals[0][i] = 0x00;
    totals[1][i] = 0x00;
  }

  for (int i = 0; i < 32; i++)
  {
    entries[i] = 0x00;
  }

  for (int j = 0; j < 4; j++)
  {
    for (int i = 0; i < 20; i++)
    {
      playerScore[j][i] = 0x00;
    }
  }

  EEPROM.update(TOTLEN, 0);
  EEPROM.update(ENTNUMADDR, 0);
  EEPROM.update(PLAYER1SCORE, 0);
  EEPROM.update(PLAYER2SCORE, 0);
  EEPROM.update(PLAYER3SCORE, 0);
  EEPROM.update(PLAYER4SCORE, 0);

  updateHistory();

  // initEEPROM();
}

void updateHistory(void)
{
  loadHistory();
  totlen = EEPROM.read(TOTLEN);
  entlen = EEPROM.read(ENTNUMADDR);

  if (EEPROM.read(GOLFMODEFLAG) == 0x01)
  {

    EEPROM.update(TOTADDR + (golfHole * golfNumPlayers) + golfPlayer, playerScore[golfPlayer][golfHole]);
  }
  else
  {

    for (int i = 0; i <= totlen && i < MEMLEN; i++)
    {
      EEPROM.update(TOTADDR + (i * 2), totals[0][i]);
      EEPROM.update(TOTADDR + (i * 2) + 1, totals[1][i]);
    }
  }

  for (int i = 0; i <= entlen && i < 32; i++)
  {
    EEPROM.update(ENTADDR + i, entries[i]);
  }
}

void loadHistory(void)
{

  for (int i = 0; i <= entlen && i < 32; i++)
  {
    entries[i] = EEPROM.read(ENTADDR + i);
  }

  if (EEPROM.read(GOLFMODEFLAG) == 0x01)
  {
    for (int j = 0; j < EEPROM.read(GOLFNUMPLAYERS); j++)
    {
      for (int i = 0; i < EEPROM.read(GOLFHOLE) && i < 20; i++)
      {
        playerScore[j][i] = EEPROM.read(TOTADDR + (golfNumPlayers * i) + j);
      }
    }
  }
  else
  {

    for (int i = 0; i <= totlen && i < MEMLEN; i++)
    {
      totals[0][i] = EEPROM.read(TOTADDR + (i * 2));
      totals[1][i] = EEPROM.read(TOTADDR + (i * 2) + 1);
    }
  }
}

void initEEPROM(void)
{
  subtractFlag = 0;
  EEPROM.update(SUBFLAG, subtractFlag);
  EEPROM.update(SPECIALMENUFLAG, 0xFF);

  duckCount = 0;
  EEPROM.update(COUNTADDRH, 0);
  EEPROM.update(COUNTADDRL, 0);

  totalCount = 0;
  EEPROM.update(TOTALCOUNTH, 0);
  EEPROM.update(TOTALCOUNTL, 0);

  EEPROM.update(TOTLEN, 0);
  EEPROM.update(ENTNUMADDR, 0);

  EEPROM.update(SPECIALSELECTION, 0x00);
  EEPROM.update(CLEAREVERYTHINGFLAG, 0xFF);
  EEPROM.update(ENTERTOTALFLAG, 0x05);
  EEPROM.update(CLEARFLAG, 0x00);

  EEPROM.update(GOLFHOLE, 0x00);
  EEPROM.update(GOLFPLAYER, 0x00);
  // EEPROM.update(GOLFMODEFLAG, 0xFF);
  EEPROM.update(GOLFNUMPLAYERS, 0x02);

  EEPROM.update(PLAYER1SCORE, 0x00);
  EEPROM.update(PLAYER2SCORE, 0x00);
  EEPROM.update(PLAYER3SCORE, 0x00);
  EEPROM.update(PLAYER4SCORE, 0x00);

  EEPROM.update(0xF0, 0x05);

  EEPROM.update(0x0D, 0x69);

  for (int i = 0; i < 4; i++)
  {
    EEPROM.update(TIMEDIGITSTART + i, 0x00);
    EEPROM.update(0xC8 + i, 0x00);
    // EEPROM.update(0xE1 + i, 0xFF);
  }

  clearAllEntries();
}

void dispNix(void)
{

  digitalWriteFast(PIN_PC0, truthBCD[dispDig0][3]);
  digitalWriteFast(PIN_PC1, truthBCD[dispDig0][2]);
  digitalWriteFast(PIN_PC2, truthBCD[dispDig0][1]);
  digitalWriteFast(PIN_PC3, truthBCD[dispDig0][0]);

  digitalWriteFast(PIN_PA4, truthBCD[dispDig1][3]);
  digitalWriteFast(PIN_PA5, truthBCD[dispDig1][2]);
  digitalWriteFast(PIN_PA6, truthBCD[dispDig1][1]);
  digitalWriteFast(PIN_PA7, truthBCD[dispDig1][0]);

  digitalWriteFast(PIN_PD1, truthBCD[dispDig2][3]);
  digitalWriteFast(PIN_PD2, truthBCD[dispDig2][2]);
  digitalWriteFast(PIN_PD3, truthBCD[dispDig2][1]);
  digitalWriteFast(PIN_PD4, truthBCD[dispDig2][0]);
}

void splitBCD(int dispNum)
{

  if (dispNum < 0)
  {
    dispDig0 = 15;
    dispDig1 = 15;
    dispDig2 = 15;
  }
  else
  {

    dispDig0 = dispNum % 10;
    dispDig1 = (dispNum / 10) % 10;
    dispDig2 = dispNum / 100;
  }

  if (blanking >= 1)
  {

    if (dispDig1 == 0 && dispDig2 == 0)
    {
      dispDig1 = 15;
      dispDig2 = 15;
    }

    if (dispDig2 == 0)
    {
      dispDig2 = 15;
    }
  }

  if (leftJustified >= 1)
  {

    if (dispDig1 == 15 && dispDig2 == 15)
    {
      dispDig2 = dispDig0;
      dispDig0 = 15;
      dispDig1 = 15;
    }

    if (dispDig2 == 15)
    {
      dispDig2 = dispDig1;
      dispDig1 = dispDig0;
      dispDig0 = 15;
    }
  }

  if (centered >= 1 && dispNum < 10)
  {

    dispDig2 = 15;
    dispDig1 = dispDig0;
    dispDig0 = 15;
  }

  // EEPROM.update(0x77, dispDig0);
  // EEPROM.update(0x7A, dispDig1);
  // EEPROM.update(0x7D, dispDig2);
  dispNix();
}

uint8_t int2byte(uint16_t sentInt, char HL)
{

  if (HL == 'H' || HL == 'h')
  {
    uint8_t msb = sentInt >> 8; // shift the higher 8 bits
    msb = msb & 0x00FF;         // make sure we didnt shift in garbage
    return msb;
  }

  else if (HL == 'L' || HL == 'l')
  {

    uint8_t lsb = sentInt & 0x00FF;
    return lsb;
  }
  else
  {
    return 0;
  }
}

void dc2bytes(void)
{

  duckCountH = int2byte(duckCount, 'H');

  duckCountL = int2byte(duckCount, 'L');

  totalCountH = int2byte(totalCount, 'H');

  totalCountL = int2byte(totalCount, 'L');
}

void bytes2dc(void)
{

  if (duckCount < 0)
  {
    duckCount = 0;
  }

  if (totalCount < 0)
  {
    totalCount = 0;
  }

  duckCount = duckCountH << 8;

  duckCount = duckCount | duckCountL;

  totalCount = totalCountH << 8;

  totalCount = totalCount | totalCountL;

  // return duckCount;
}

uint16_t bytes2int(uint8_t H, uint8_t L)
{
  uint16_t integerFromBytes = H << 8;

  integerFromBytes = integerFromBytes | L;

  return integerFromBytes;
}

void writedc(void)
{
  
  if (duckCount <= 0)
  {
    duckCount = 0;
  }

  if (totalCount <= 0)
  {
    totalCount = 0;
  }

  dc2bytes();

  EEPROM.update(COUNTADDRH, duckCountH);
  EEPROM.update(COUNTADDRL, duckCountL);

  EEPROM.update(TOTALCOUNTH, totalCountH);
  EEPROM.update(TOTALCOUNTL, totalCountL);
}

void newDay(void)
{

  dc2bytes();

  shiftHistory();

  duckCount = 0;

  dc2bytes();
  EEPROM.update(COUNTADDRH, duckCountH);
  EEPROM.update(COUNTADDRL, duckCountL);
  if (golfMode == 0)
  {
    if (dayTotal == 1)
    {
      splitBCD(duckCount);
    }
    else
    {
      splitBCD(totalCount);
    }
  }
}

void showGolfHistory(void)
{

  uint8_t tempHole = EEPROM.read(TEMPHOLE);
  golfNumPlayers = EEPROM.read(GOLFNUMPLAYERS);
  golfPlayer = EEPROM.read(GOLFPLAYER);
  uint8_t tempPlayer = EEPROM.read(TEMPPLAYER);
  uint8_t golfHole = EEPROM.read(GOLFHOLE);

  if (tempHole == 0xFF)
  {
    tempPlayer = 0;
    EEPROM.update(TEMPPLAYER, 0x00);
    EEPROM.update(TEMPHOLE, 0x00);
    tempHole = 0;
  }

  uint8_t score2 = 0;

  if (EEPROM.read(TEMPHOLE) == EEPROM.read(GOLFHOLE))
  {

    for (int i = 0; i < golfNumPlayers; i++) // this makes it not show the next hole if nobody has entered a score
    {
      if (EEPROM.read(TOTADDR + (EEPROM.read(GOLFHOLE) * golfNumPlayers) + i) != 0xFF)
      {
        score2++;
      }
    }
  }

  if (score2 == 0)
  {
    golfHole = golfHole - 1;
  }

  if (tempHole > golfHole && tempPlayer >= golfNumPlayers)
  {
    EEPROM.update(SPECIALMENUFLAG, 0xFF);
    EEPROM.update(SHOWHISTORYFLAG, 0xFF);
    EEPROM.update(TEMPHOLE, 0xFF);
    EEPROM.update(GOLFTOTALFLAG, 0xFF);
    EEPROM.update(TEMPPLAYER, 0xFF);
    // return;
    while (1)
      ;
  }
  if (EEPROM.read(GOLFTOTALFLAG) == 0x02)
  {
    EEPROM.update(TEMPPLAYER, tempPlayer + 1);

    if (EEPROM.read(TEMPPLAYER) >= golfNumPlayers)
    {
      EEPROM.update(SPECIALMENUFLAG, 0x06);
      EEPROM.update(SHOWHISTORYFLAG, 0xEE);
      EEPROM.update(TEMPHOLE, 0xFF);
      EEPROM.update(GOLFTOTALFLAG, 0xFF);
      EEPROM.update(TEMPPLAYER, 0xFF);
      EEPROM.update(GOLFHOLE, 0xFF);
      // return;
      // while (1)
      //;
    }

    unsigned long previousMillisgt = 0;

    while (1)
    {

      splitBCD(EEPROM.read(PLAYER1SCORE + tempPlayer));

      unsigned long currentMillisgt = millis();

      if ((currentMillisgt) % 100 >= 30)
      {

        // previousMillisgt = currentMillisgt;

        digitalWriteFast(dp2, LOW);
        digitalWriteFast(dp1, LOW);
        digitalWriteFast(dp0, LOW);
        digitalWriteFast(ind, LOW);
      }
      else
      {
        golfPlayerDP(tempPlayer, 0);
      }
    }
  }

  if (EEPROM.read(TEMPHOLE) > golfHole && tempPlayer < golfNumPlayers)
  {

    // EEPROM.update(GOLFTOTALFLAG, 0x01);
    tempHole = 0;
    EEPROM.update(TEMPHOLE, 0x00);

    // EEPROM.update(GOLFTOTALFLAG, 0xFF);
    //  tempPlayer++;
    EEPROM.update(TEMPPLAYER, tempPlayer + 1);

    unsigned long previousMillisgt = 0;

    while (1)
    {

      splitBCD(EEPROM.read(PLAYER1SCORE + tempPlayer));

      unsigned long currentMillisgt = millis();

      if ((currentMillisgt) % 100 >= 30)
      {

        // previousMillisgt = currentMillisgt;

        digitalWriteFast(dp2, LOW);
        digitalWriteFast(dp1, LOW);
        digitalWriteFast(dp0, LOW);
        digitalWriteFast(ind, LOW);
      }
      else
      {
        golfPlayerDP(tempPlayer, 0);
      }
    }
  }

  else if (EEPROM.read(TEMPPLAYER) >= golfNumPlayers)
  {
    EEPROM.update(SPECIALMENUFLAG, 0xFF);
    EEPROM.update(SHOWHISTORYFLAG, 0xFF);
    EEPROM.update(TEMPHOLE, 0xFF);
    EEPROM.update(GOLFTOTALFLAG, 0xFF);
    EEPROM.update(TEMPPLAYER, 0xFF);
    return;
    while (1)
      ;
  }

  uint8_t score = EEPROM.read(TOTADDR + (tempHole * golfNumPlayers) + tempPlayer);

  if (score > 0xAA)
  {
    score = 0;
  }

  // golfPlayer++;

    if (tempPlayer + 1 < golfNumPlayers && tempHole >= 17)
    {
      leftJustified = 1;
      splitBCD(tempHole + 1);
      leftJustified = 0;
      golfPlayerDP(tempPlayer, 0);

      delay(200);
      splitBCD(score);
      tempHole = 0;
      EEPROM.update(TEMPHOLE, 0x00);
      // EEPROM.update(GOLFTOTALFLAG, 0x02);
      EEPROM.update(TEMPPLAYER, tempPlayer + 1);
    }
    else if ((EEPROM.read(TEMPPLAYER) + 1) >= EEPROM.read(GOLFNUMPLAYERS) && tempHole > 17)
    {
      // splitBCD(999);
      // delay(400);
      EEPROM.update(GOLFHOLE, 0x00);
      EEPROM.update(SPECIALMENUFLAG, 0xFF);

      EEPROM.update(SHOWHISTORYFLAG, 0xFF);
      EEPROM.update(TEMPHOLE, 0x00);
      EEPROM.update(GOLFTOTALFLAG, 0xFF);
      EEPROM.update(GOLFPLAYER, 0xFF);
      digitalWriteFast(ind, HIGH);
      while (1)
        ;
      return;
    }
  
  else
  {
    leftJustified = 1;
    splitBCD(tempHole + 1);
    leftJustified = 0;
    golfPlayerDP(tempPlayer, 0);

    delay(200);
    splitBCD(score);
    EEPROM.update(TEMPHOLE, tempHole + 1);
  }
  while (1)
    ;
}

void showHistory(void)
{
  int dayFlip = EEPROM.read(SHOWHISTORYFLAG); //these variables are poorly named
  int last = EEPROM.read(SPECIALSELECTION);

  loadHistory();

  int days = EEPROM.read(TOTLEN);

 int count = abs(days - dayFlip);

  if (last == 0x06)
  {

    leftJustified = 1;
    splitBCD(count + 1);
    leftJustified = 0;

    delay(200);

    splitBCD(bytes2int(totals[0][(days - count) - 1], totals[1][(days - count) - 1]));

    if (dayFlip == 1)
    {
      EEPROM.update(SPECIALSELECTION, 0x00);
    }
    else
    {
      EEPROM.update(SHOWHISTORYFLAG, dayFlip - 1);
    }
    while (1)
      ;
  }
  else if (last == 0x00)
  {

    leftJustified = 1;
    splitBCD((count + 2));
    leftJustified = 0;

    delay(200);

    splitBCD(bytes2int(EEPROM.read(COUNTADDRH), EEPROM.read(COUNTADDRL)));
    EEPROM.update(SHOWHISTORYFLAG, dayFlip - 1);
    EEPROM.update(SPECIALSELECTION, 0xFF);
    // EEPROM.update(SPECIALMENUFLAG, 0xFF);

    while (1)
      ;
  }
  else
  {
    EEPROM.update(SHOWHISTORYFLAG, 0xFF);
    EEPROM.update(SPECIALMENUFLAG, 0xFF);
    EEPROM.update(SPECIALSELECTION, 0xFF);
/*
    leftJustified = 1;
    splitBCD((count + 2));
    leftJustified = 0;

    delay(200);
    splitBCD(duckCount);
    while (1)
      ;

    return;*/
  }
}

void voltageDisplay(int loaded)
{

  int numSamps = 100;
  long total = 0;
  digitalWriteFast(dp1, HIGH);

  for (int j = 0; j < 1; j++)
  {

    if (loaded > 0)
    {
      digitalWriteFast(PIN_PF0, LOW);
      delay(loaded); // let battery voltage bounce back up a bit
    }
    total = 0;
    for (int i = 0; i < numSamps; i++)
    {
      total += analogRead(PIN_PF1);
      delayMicroseconds(loaded * 2);
    }

    digitalWriteFast(PIN_PF0, HIGH);

    int voltage = (total / numSamps);
    splitBCD(voltage + 11);
    delay(10);
  }
}

void stopwatch(void)
{
  int timeDigits = 0;
  uint8_t precision = EEPROM.read(STOPWATCHFLAG);

  if (precision == 0x01 || precision == 0x05)
  {
    // EEPROM.update(0xF0, 0x01);
    if (precision == 0x01)
    {
      EEPROM.update(STOPWATCHFLAG, 0x00);
    }
    else
    {
      EEPROM.update(STOPWATCHFLAG, 0x01);
    }

    while (1)
    {
      centered = 1;
      if (millis() % 1000 < 500)
      {
        splitBCD(0);
        splitBCD(1);
      }
      else
      {
        centered = 1;
        splitBCD(0);
        splitBCD(7);
      }
    }
  }
  else if (precision == 0x02)
  {
    digitalWrite(dp2, HIGH);
    splitBCD(bytes2int(EEPROM.read(TIMEDIGITSTART + 3), EEPROM.read(TIMEDIGITSTART + 2)));
    EEPROM.update(STOPWATCHFLAG, 0x00);
    digitalWriteFast(ind, HIGH);
    while (1)
    {
      if (digitalRead(pulse1) == 1)
      {
        EEPROM.update(SPECIALMENUFLAG, 0xFF);
        return;
      }
    }
  }
  else if (precision == 0x03)
  {
    digitalWrite(dp1, HIGH);
    splitBCD(bytes2int(EEPROM.read(TIMEDIGITSTART + 1), EEPROM.read(TIMEDIGITSTART)));
    EEPROM.update(STOPWATCHFLAG, 0x00);
    digitalWriteFast(ind, HIGH);
    while (1)
    {
      if (digitalRead(pulse1) == 1)
      {
        EEPROM.update(SPECIALMENUFLAG, 0xFF);
        return;
      }
    }
  }

  while (1)
  {
    if (digitalRead(pulse1) == 1)
    {
      // EEPROM.update(SPECIALSELECTION, 0x00);
      EEPROM.update(SPECIALMENUFLAG, 0xFF);
      return;
    }
    unsigned long currentMillis = millis();

    if (currentMillis - previousMillis >= intervalStopwatch)
    {

      previousMillis = currentMillis;

      if (currentMillis < 60000)
      {
        EEPROM.update(STOPWATCHFLAG, 0x02);
        intervalStopwatch = 100;
        digitalWriteFast(dp1, LOW);
        digitalWriteFast(dp2, HIGH);
        timeDigits = currentMillis / 100;

        if (currentMillis % 1000 < 5 || currentMillis % 1000 > 995) // makes it flash right in the center of the digit flipping
        {
          digitalWriteFast(ind, HIGH);
        }
        else
        {
          digitalWriteFast(ind, LOW);
        }

        EEPROM.update(TIMEDIGITSTART + 2, timeDigits & 0xFF);
        EEPROM.update(TIMEDIGITSTART + 3, timeDigits >> 8 & 0xFF);
      }
      else
      {
        EEPROM.update(STOPWATCHFLAG, 0x03);
        intervalStopwatch = 1000;

        digitalWriteFast(dp0, LOW);
        digitalWriteFast(dp1, HIGH);
        digitalWriteFast(dp2, LOW);

        digitalWriteFast(ind, HIGH);

        timeDigits = currentMillis / 1000;

        int timeMinutes = timeDigits;

        timeDigits = timeDigits % 60;

        timeMinutes = timeMinutes / 60;

        timeDigits = (timeMinutes * 100) + timeDigits;

        EEPROM.update(TIMEDIGITSTART, timeDigits & 0xFF);
        EEPROM.update(TIMEDIGITSTART + 1, timeDigits >> 8 & 0xFF);
        digitalWriteFast(ind, LOW);
      }
      if (currentMillis >= 600000)
      {
        return;
      }

      splitBCD(timeDigits);
    }
  }
  EEPROM.update(STOPWATCHFLAG, 0x01);
}

void enterStartingTotal(void)
{

  subtractFlag = 0;
  blanking = 0;
  leftJustified = 0;
  centered = 0;

  if (EEPROM.read(ENTERTOTALFLAG) == 0x05)
  {
    digitalWriteFast(dp0, HIGH);

    enteredTotal1 = 0;

    splitBCD(000);
    while (1)
    {

      enteredTotal1 = readDial(2);

      if (enteredTotal1 > 0)
      {
        break;
      }
    }
    if (enteredTotal1 == 10)
    {
      enteredTotal1 = 0;
    }

    splitBCD(enteredTotal1 * 100);
    EEPROM.update(0xCA, enteredTotal1);

    digitalWriteFast(dp0, LOW);

    EEPROM.update(ENTERTOTALFLAG, 0x15);
    return;
  }

  else if (EEPROM.read(ENTERTOTALFLAG) == 0x15)
  {

    digitalWriteFast(dp1, HIGH);
    splitBCD((enteredTotal1 * 100));

    enteredTotal2 = 0;

    while (1)
    {
      enteredTotal2 = readDial(3);
      if (enteredTotal2 > 0)
      {
        break;
      }
    }
    if (enteredTotal2 == 10)
    {
      enteredTotal2 = 0;
    }

    splitBCD((enteredTotal1 * 100) + (enteredTotal2 * 10));
    EEPROM.update(0xCB, enteredTotal2);

    digitalWriteFast(dp1, LOW);
    EEPROM.update(ENTERTOTALFLAG, 0x25);
    return;
  }

  else if (EEPROM.read(ENTERTOTALFLAG) == 0x25)
  {
    digitalWriteFast(dp2, HIGH);
    splitBCD((enteredTotal1 * 100) + (enteredTotal2 * 10));
    enteredTotal3 = 0;
    while (1)
    {

      enteredTotal3 = readDial(4);
      if (enteredTotal3 > 0)
      {
        break;
      }
    }
    if (enteredTotal3 == 10)
    {
      enteredTotal3 = 0;
    }

    splitBCD((enteredTotal1 * 100) + (enteredTotal2 * 10) + enteredTotal3);

    EEPROM.update(0xCC, enteredTotal3);

    digitalWriteFast(dp2, LOW);

    duckCount = (int)(EEPROM.read(0xCA) * 100) + (int)(EEPROM.read(0xCB) * 10) + enteredTotal3;
    totalCount = totalCount + duckCount;

    EEPROM.update(ENTERTOTALFLAG, 0x05);

    EEPROM.update(SPECIALMENUFLAG, 0xFF);

    writedc();
    return;
  }
  // EEPROM.update(SPECIALMENUFLAG, 0x05);
  // blanking = 0;
  // splitBCD(duckCount);
  // blanking = 1;
  EEPROM.update(SPECIALSELECTION, 0x05);
}