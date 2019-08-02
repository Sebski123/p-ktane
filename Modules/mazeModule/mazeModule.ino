#include <Arduino.h>

#include "DSerial.h"
#include "KTANECommon.h"
#include <NeoICSerial.h>
#include "LedControl.h"
#include "mazeModule.h"

// Defines
#define DATA_PIN 12
#define CLOCK_PIN 11
#define LOAD_PIN 10
#define UP_BTN 9
#define DOWN_BTN 8
#define LEFT_BTN 7
#define RIGHT_BTN 6

//Variables
int playerLocation[2];
int goalLocation[2];
int (*markerLocations)[2][2];
int mazeNum;

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
DSerialClient client(serial_port, MY_ADDRESS);
KTANEModule module(client, 3, 4);

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


  /*
    Get player location
    Get goal location
    show markers
  */

  module.sendReady();
}

void loop() {
  module.interpretData();

  if(!module.is_solved){
    /*
    checkInputs();
    if(they_solved_it) {
      module.win();
    }
    if(they_messed_up) {
      module.strike();
    }
    updateOutputs();
    */
  }
}