
#include "DSerial.h"
#include "KTANECommon.h"
#include "NeoICSerial.h"
#include "LedControl.h"
#include "mazeModule.h"

// Defines
#define DATA_PIN 12
#define CLOCK_PIN 11
#define LOAD_PIN 10
#define UP_BTN 7
#define DOWN_BTN 6
#define LEFT_BTN 5
#define RIGHT_BTN 4

//Function prototypes
int getBtnDir();
void move(int direction);

//Variables
int playerLocation[2];
int goalLocation[2];
int markerLocations[2][2];
int mazeNum;
bool markerBlinkState = true;
bool playerBlinkState = true;
unsigned long currentMillis;
unsigned long previousMarkerMillis = 0;
unsigned long previousPlayerMillis = 0;
int dir;

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
DSerialClient client(serial_port, 1);
KTANEModule module(client, 3, 4);

int getBtnDir(){
  if (analogRead(UP_BTN) == 0){
    return UP;
  } else if (analogRead(DOWN_BTN) == 0){
    return DOWN;
  } else if (analogRead(LEFT_BTN) == 0){
    return LEFT;
  } else if (analogRead(RIGHT_BTN) == 0){
    return RIGHT;
  } else {
    return -1;
  }
}

void move(int direction){
  switch (direction)
  {
    case UP:
      if(mazeHorizontalWalls[mazeNum][playerLocation[0]][playerLocation[1]]){
        module.strike();
      } else {
        playerLocation[1] -= 1;
      }
      break;
    
    case DOWN:
      if(mazeHorizontalWalls[mazeNum][playerLocation[0]][playerLocation[1]+1]){
        module.strike();
      } else {
        playerLocation[1] += 1;
      }
      break;
    
    case LEFT:
      if(mazeVerticalWalls[mazeNum][playerLocation[1]][playerLocation[0]]){
        module.strike();
      } else {
        playerLocation[0] -= 1;
      }
      break;
    
    case RIGHT:
      if(mazeVerticalWalls[mazeNum][playerLocation[1]][playerLocation[0]+1]){
        module.strike();
      } else {
        playerLocation[0] += 1;
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

  Serial.println("Getting config");
  while(!module.getConfig()){
    module.interpretData();
  }
  Serial.println("Got config");

  Serial.println("Setting random seed");
  randomSeed(config_to_seed(module.getConfig()));

  Serial.println("Setting up I/O-pins");
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

  Serial.println("Choosing maze");
  mazeNum = random(0,9);

  Serial.println("Getting marker locations");
  memcpy(markerLocations,markers[mazeNum],sizeof(markerLocations));

  Serial.println("Choosing player location");
  playerLocation[0] = random(0,6);
  playerLocation[1] = random(0,6);

  Serial.println("Choosing goal location");
  while (goalLocation != playerLocation)
  {
    goalLocation[0] = random(0,6);
    goalLocation[1] = random(0,6);
  }

  Serial.println("Showing goal location");
  lc.setLed(0, goalLocation[0], goalLocation[1], true);

  module.sendReady();
}

void loop() {
  module.interpretData();

  //Blink maze markers and player location with different intervals
  currentMillis = millis();
  
  if (currentMillis - previousMarkerMillis >= 500) {
    // save the last time you blinked the markers
    previousMarkerMillis = currentMillis;
    markerBlinkState = !markerBlinkState;
  }

  if (currentMillis - previousPlayerMillis >= 100) {
    // save the last time you blinked the player
    previousPlayerMillis = currentMillis;
    markerBlinkState = !markerBlinkState;
  }

  lc.setLed(0, goalLocation[0], goalLocation[1], true);
  lc.setLed(0, markerLocations[0][0], markerLocations[0][1], markerBlinkState);
  lc.setLed(0, markerLocations[1][0], markerLocations[1][1], markerBlinkState);
  lc.setLed(0, playerLocation[0], playerLocation[1], playerBlinkState);



  if(!module.is_solved){
    
    //Read button press
    dir = getBtnDir();

    //Try to move the player, gave a strike if a wall is hit
    move(dir);

    //Check if player reached the goal
    if(playerLocation == goalLocation) {
      module.win();
      while (1){
        delayWithUpdates(module, 10);
      }
    } 
  }

  lc.clearDisplay(0);
}