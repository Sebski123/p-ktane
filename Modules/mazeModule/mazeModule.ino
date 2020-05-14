
const char string_0[] PROGMEM = "Getting config";
const char string_1[] PROGMEM = "Got config";
const char string_2[] PROGMEM = "Setting random seed";
const char string_3[] PROGMEM = "Setting up I/O-pins";
const char string_4[] PROGMEM = "Choosing maze";
const char string_5[] PROGMEM = "Choose maze nr: ";
const char string_6[] PROGMEM = "Getting marker locations";
const char string_7[] PROGMEM = "Choosing player location";
const char string_8[] PROGMEM = "Player location: ";
const char string_9[] PROGMEM = "Choosing goal location";
const char string_10[] PROGMEM = "Goal location: ";
const char string_11[] PROGMEM = "Showing goal location";
const char string_12[] PROGMEM = "Setup done";

const char *const string_table[] PROGMEM = {string_0, string_1, string_2, string_3, string_4, string_5, string_6, string_7, string_8, string_9, string_10, string_11, string_12};

#include "DSerial.h"
#include "KTANECommon.h"
#include "NeoICSerial.h"
#include "LedControl.h"
#include "mazeModule.h"

// Defines
//Green clear Led 2
//Red strike Led 3
#define RIGHT_BTN 4
#define LEFT_BTN 5
#define DOWN_BTN 6
#define UP_BTN 7
//NeoICSerial RX-pin 8
//NeoICSerial TX-pin 9
#define CLOCK_PIN 10
#define LOAD_PIN 11
#define DATA_PIN 12

//Function prototypes
int getBtnDir();
void move(int direction);

//Variables
int playerLocation[2];
int goalLocation[2];
byte markerLocations[2][2];
int mazeNum;
bool markerBlinkState = true;
bool playerBlinkState = true;
unsigned long currentMillis;
unsigned long previousMarkerMillis = 0;
unsigned long previousPlayerMillis = 0;
int dir;
int markerBlinkCount = 0;
unsigned long lastDebounceTime = 0; // the last time the output pin was toggled

//Class inits
/*
 Now we need a LedControl to work with.
 ***** These pin numb
 ers will probably not work with your hardware *****
 pin 12 is connected to the DataIn 
 pin 11 is connected to the CLK 
 pin 10 is connected to LOAD 
 We have only a single MAX72XX.
 */
LedControl lc = LedControl(DATA_PIN,CLOCK_PIN,LOAD_PIN,1);
NeoICSerial serial_port;
DSerialClient client(serial_port, 0x02);
KTANEModule module(client, 2, 3);

int getBtnDir()
{
  if (digitalRead(UP_BTN) == 0)
  {
    return UP;
  }
  else if (digitalRead(DOWN_BTN) == 0)
  {
    return DOWN;
  }
  else if (digitalRead(LEFT_BTN) == 0)
  {
    return LEFT;
  }
  else if (digitalRead(RIGHT_BTN) == 0)
  {
    return RIGHT;
  }
  else
  {
    return -1;
  }
}

void move(int direction)
{
  Serial.print("Trying to move ");

  switch (direction)
  {

    case UP:
    Serial.println("up");
    if (mazeHorizontalWalls[mazeNum][playerLocation[0]][playerLocation[1]])
    {
        module.strike();
      Serial.println("Strike");
    }
    else
    {
      lc.setLed(0, playerLocation[0], playerLocation[1], false);
        playerLocation[1] -= 1;
      lc.setLed(0, playerLocation[0], playerLocation[1], playerBlinkState);
      }
      break;
    
    case DOWN:
    Serial.println("down");
    if (mazeHorizontalWalls[mazeNum][playerLocation[0]][playerLocation[1] + 1])
    {
        module.strike();
      Serial.println("Strike");
    }
    else
    {
      lc.setLed(0, playerLocation[0], playerLocation[1], false);
        playerLocation[1] += 1;
      lc.setLed(0, playerLocation[0], playerLocation[1], playerBlinkState);
      }
      break;
    
    case LEFT:
    Serial.println("left");
    if (mazeVerticalWalls[mazeNum][playerLocation[1]][playerLocation[0]])
    {
        module.strike();
      Serial.println("Strike");
    }
    else
    {
      lc.setLed(0, playerLocation[0], playerLocation[1], false);
        playerLocation[0] -= 1;
      lc.setLed(0, playerLocation[0], playerLocation[1], playerBlinkState);
      }
      break;
    
    case RIGHT:
    Serial.println("right");
    if (mazeVerticalWalls[mazeNum][playerLocation[1]][playerLocation[0] + 1])
    {
        module.strike();
      Serial.println("Strike");
    }
    else
    {
      lc.setLed(0, playerLocation[0], playerLocation[1], false);
        playerLocation[0] += 1;
      lc.setLed(0, playerLocation[0], playerLocation[1], playerBlinkState);
      }
      break;
    
    default:
      break;
  }
  return;
}

void setup() {
  serial_port.begin(19200);
  Serial.begin(19200);

  Serial.println((__FlashStringHelper *)pgm_read_word(string_table + 0));
  while (!module.getConfig())
    module.interpretData();
  }
  Serial.println((__FlashStringHelper *)pgm_read_word(string_table + 1));

  Serial.println((__FlashStringHelper *)pgm_read_word(string_table + 2));
  randomSeed(config_to_seed(module.getConfig()));

  Serial.println((__FlashStringHelper *)pgm_read_word(string_table + 3));
  // 8x8 Led-matrix setup
  pinMode(DATA_PIN, OUTPUT);
  pinMode(CLOCK_PIN,  OUTPUT);
  pinMode(LOAD_PIN,   OUTPUT);
  
  //Directional-buttons setup
  pinMode(UP_BTN, INPUT_PULLUP);
  pinMode(DOWN_BTN, INPUT_PULLUP);
  pinMode(LEFT_BTN, INPUT_PULLUP);
  pinMode(RIGHT_BTN, INPUT_PULLUP);

  /*
   The MAX72XX is in power-saving mode on startup,
   we have to do a wakeup call
   */
  lc.shutdown(0,false);
  /* Set the brightness to a medium values */
  lc.setIntensity(0,8);
  /* and clear the display */
  lc.clearDisplay(0);

  Serial.println((__FlashStringHelper *)pgm_read_word(string_table + 4));
  mazeNum = random(0, 9);
  Serial.println((__FlashStringHelper *)pgm_read_word(string_table + 5));
  Serial.println(mazeNum);

  Serial.println((__FlashStringHelper *)pgm_read_word(string_table + 6));
  memcpy(markerLocations, markers[mazeNum], sizeof(markerLocations));
  Serial.print("Marker 1 location: ");
  Serial.print(markerLocations[0][0]);
  Serial.print("\t");
  Serial.println(markerLocations[0][1]);

  Serial.print("Marker 2 location: ");
  Serial.print(markerLocations[1][0]);
  Serial.print("\t");
  Serial.println(markerLocations[1][1]);

  Serial.println((__FlashStringHelper *)pgm_read_word(string_table + 7));
  playerLocation[0] = random(0, 6);
  playerLocation[1] = random(0, 6);
  Serial.println((__FlashStringHelper *)pgm_read_word(string_table + 8));
  Serial.print(playerLocation[0]);
  Serial.print("\t");
  Serial.println(playerLocation[1]);

  Serial.println((__FlashStringHelper *)pgm_read_word(string_table + 9));
  while ((goalLocation[0] == playerLocation[0]) && (goalLocation[1] == playerLocation[1]))
  {
    goalLocation[0] = random(0,6);
    goalLocation[1] = random(0,6);
  }

  Serial.println((__FlashStringHelper *)pgm_read_word(string_table + 10));
  Serial.print(goalLocation[0]);
  Serial.print("\t");
  Serial.println(goalLocation[1]);

  Serial.println((__FlashStringHelper *)pgm_read_word(string_table + 11));
  lc.setLed(0, goalLocation[0], goalLocation[1], true);

  Serial.println((__FlashStringHelper *)pgm_read_word(string_table + 12));
  module.sendReady();
}

void loop() {
  module.interpretData();

  if (!module.is_solved)
  {
  //Blink maze markers and player location with different intervals
  currentMillis = millis();
  
    if (currentMillis - previousPlayerMillis >= 100)
    {
      // save the last time you blinked the player
      if (markerBlinkCount >= 5)
      {
    markerBlinkState = !markerBlinkState;
        markerBlinkCount = 0;
      }
      else
      {
        markerBlinkCount++;
  }

    previousPlayerMillis = currentMillis;
      playerBlinkState = !playerBlinkState;

  lc.setLed(0, markerLocations[0][0], markerLocations[0][1], markerBlinkState);
  lc.setLed(0, markerLocations[1][0], markerLocations[1][1], markerBlinkState);
  lc.setLed(0, playerLocation[0], playerLocation[1], playerBlinkState);
    }


    if (millis() - lastDebounceTime >= 200)
    {
    
    //Read button press
    dir = getBtnDir();
      if (dir >= 0)
      {
        lastDebounceTime = millis();
        Serial.print("Pressed: ");
        Serial.println(dir);

    //Try to move the player, gave a strike if a wall is hit
    move(dir);

        Serial.println((__FlashStringHelper *)pgm_read_word(string_table + 8));
        Serial.print(playerLocation[0]);
        Serial.print("\t");
        Serial.println(playerLocation[1]);
      }
    }
    //Check if player reached the goal
    if ((playerLocation[0] == goalLocation[0]) && (playerLocation[1] == goalLocation[1]))
    {
      module.win();
      Serial.println("Win");
      while (1)
      {
        delayWithUpdates(module, 10);
      }
    } 
  }
}