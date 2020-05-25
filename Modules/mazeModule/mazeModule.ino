#include "SWire.h"
#include "KTANECommon.h"
#include "LedControl.h"
#include "mazeModule.h"

// Defines
//  Pins
#define GREEN_CLEAR_LED 2
#define RED_STRIKE_LED 3
#define RIGHT_BTN 4
#define UP_BTN 5
#define DOWN_BTN 6
#define LEFT_BTN 7
#define CLOCK_PIN 10
#define LOAD_PIN 11
#define DATA_PIN 12
//I2C SDA 18
//I2C SCL 19

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
LedControl lc = LedControl(DATA_PIN, CLOCK_PIN, LOAD_PIN, 1);
SWireClient client(0x02);
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
    Serial.println(F("up"));
    if (mazeHorizontalWalls[mazeNum][playerLocation[0]][playerLocation[1]])
    {
      module.strike();
      Serial.println(F("Strike"));
    }
    else
    {
      lc.setLed(0, playerLocation[0], playerLocation[1], false);
      playerLocation[1] -= 1;
      lc.setLed(0, playerLocation[0], playerLocation[1], playerBlinkState);
    }
    break;

  case DOWN:
    Serial.println(F("down"));
    if (mazeHorizontalWalls[mazeNum][playerLocation[0]][playerLocation[1] + 1])
    {
      module.strike();
      Serial.println(F("Strike"));
    }
    else
    {
      lc.setLed(0, playerLocation[0], playerLocation[1], false);
      playerLocation[1] += 1;
      lc.setLed(0, playerLocation[0], playerLocation[1], playerBlinkState);
    }
    break;

  case LEFT:
    Serial.println(F("left"));
    if (mazeVerticalWalls[mazeNum][playerLocation[1]][playerLocation[0]])
    {
      module.strike();
      Serial.println(F("Strike"));
    }
    else
    {
      lc.setLed(0, playerLocation[0], playerLocation[1], false);
      playerLocation[0] -= 1;
      lc.setLed(0, playerLocation[0], playerLocation[1], playerBlinkState);
    }
    break;

  case RIGHT:
    Serial.println(F("right"));
    if (mazeVerticalWalls[mazeNum][playerLocation[1]][playerLocation[0] + 1])
    {
      module.strike();
      Serial.println(F("Strike"));
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

void setup()
{
  Serial.begin(19200);

  Serial.println(F("Getting config"));
  while (!module.getConfig())
  {
    module.interpretData();
  }
  Serial.println(F("Got config"));

  Serial.println(F("Setting random seed"));
  randomSeed(config_to_seed(module.getConfig()));

  Serial.println(F("Setting up I/O-pins"));
  // 8x8 Led-matrix setup
  pinMode(DATA_PIN, OUTPUT);
  pinMode(CLOCK_PIN, OUTPUT);
  pinMode(LOAD_PIN, OUTPUT);

  //Directional-buttons setup
  pinMode(UP_BTN, INPUT_PULLUP);
  pinMode(DOWN_BTN, INPUT_PULLUP);
  pinMode(LEFT_BTN, INPUT_PULLUP);
  pinMode(RIGHT_BTN, INPUT_PULLUP);

  /*
   The MAX72XX is in power-saving mode on startup,
   we have to do a wakeup call
   */
  lc.shutdown(0, false);
  /* Set the brightness to a medium values */
  lc.setIntensity(0, 8);
  /* and clear the display */
  lc.clearDisplay(0);

  Serial.println(F("Choosing maze"));
  mazeNum = random(0, 9);
  Serial.println(F("Choose maze nr: "));
  Serial.println(mazeNum);

  Serial.println(F("Getting marker locations"));
  memcpy(markerLocations, markers[mazeNum], sizeof(markerLocations));
  Serial.print(F("Marker 1 location: "));
  Serial.print(markerLocations[0][0]);
  Serial.print("\t");
  Serial.println(markerLocations[0][1]);

  Serial.print(F("Marker 2 location: "));
  Serial.print(markerLocations[1][0]);
  Serial.print("\t");
  Serial.println(markerLocations[1][1]);

  Serial.println(F("Choosing player location"));
  playerLocation[0] = random(0, 6);
  playerLocation[1] = random(0, 6);
  Serial.println(F("Player location: "));
  Serial.print(playerLocation[0]);
  Serial.print("\t");
  Serial.println(playerLocation[1]);

  Serial.println(F("Choosing goal location"));
  while ((goalLocation[0] == playerLocation[0]) && (goalLocation[1] == playerLocation[1]))
  {
    goalLocation[0] = random(0, 6);
    goalLocation[1] = random(0, 6);
  }

  Serial.println(F("Goal location: "));
  Serial.print(goalLocation[0]);
  Serial.print("\t");
  Serial.println(goalLocation[1]);

  Serial.println(F("Showing goal location"));
  lc.setLed(0, goalLocation[0], goalLocation[1], true);

  Serial.println(F("Setup done"));
  module.sendReady();
}

void loop()
{
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
        Serial.print(F("Pressed: "));
        Serial.println(dir);

        //Try to move the player, gave a strike if a wall is hit
        move(dir);

        Serial.println(F("Player location: "));
        Serial.print(playerLocation[0]);
        Serial.print("\t");
        Serial.println(playerLocation[1]);
      }
    }
    //Check if player reached the goal
    if ((playerLocation[0] == goalLocation[0]) && (playerLocation[1] == goalLocation[1]))
    {
      module.win();
      Serial.println(F("Win"));
    }
  }
}