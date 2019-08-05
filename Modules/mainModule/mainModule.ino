#include <Arduino.h>

#include "DSerial.h"
#include "KTANECommon.h"
#include <NeoICSerial.h>
#include <string.h>
#include <Wire.h>
#include <SPI.h>
#include "LedControl.h"

// Defines
#define DATA_PIN 12
#define CLOCK_PIN 11
#define LOAD_PIN 10
#define STRIKE_1_PIN 9
#define STRIKE_2_PIN 8
#define CLOCK_DOT 7
#define SPEAKER_PIN 5

//Function prototypes
void toggleClockBlink();
void playMelody(int *melody, int* durations, int melody_len);
void youLose();
void youWin();
void getConfigESP();
void getConfigManual();

//Variables
int brightness = 4;
int win_melody[] = {262, 330, 294, 370, 392};
int win_melody_durations[] = {8, 8, 8, 8, 2};
int win_melody_len = 5;
int lose_melody[] = {659, 622, 587, 554};
int lose_melody_durations[] = {8,8,8,1};
int lose_melody_len = 4;
unsigned long num_minutes;
unsigned long currentMillis;
unsigned long previousMillis = 0;


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
config_t config;
NeoICSerial serial_port;
DSerialMaster master(serial_port);
KTANEController controller(master);

// Globals
int strikes = 0;
int solves = 0;
unsigned long dest_time;
int num_modules;



void toggleClockBlink() {
  digitalWrite(CLOCK_DOT, !digitalRead(CLOCK_DOT));
}

void playMelody(int *melody, int* durations, int melody_len) {
  for (int thisNote = 0; thisNote < melody_len; thisNote++) {

    int noteDuration = 1000 / durations[thisNote];
    tone(SPEAKER_PIN, melody[thisNote], noteDuration);

    // to distinguish the notes, set a minimum time between them.
    int pauseBetweenNotes = noteDuration * 1.30;
    delayWithUpdates(controller, pauseBetweenNotes);

    // stop the tone playing:
    noTone(SPEAKER_PIN);
  }
}

void youLose() {
  // Play lose music
  Serial.println("Loose");
  lc.clearDisplay(0);
  lc.setChar(0, 4, 'd', false);
  lc.setChar(0, 5, 'e', false);
  lc.setChar(0, 6, 'a', false);
  lc.setChar(0, 7, 'd', false);
  playMelody(lose_melody, lose_melody_durations, lose_melody_len);

  // Stop clock
  while(1){
    delayWithUpdates(controller, 10);
  }
}

void youWin() {
  // Play win music
  Serial.println("Win");

  lc.clearDisplay(0);
  lc.setDigit(0, 4, 5, false);
  lc.setChar(0, 5, 'u', false);
  lc.setChar(0, 6, 'c', false);
  lc.setChar(0, 7, 'c', false);
  playMelody(win_melody, win_melody_durations, win_melody_len);

  // Stop clock
  while(1){
    delayWithUpdates(controller, 10);
  }
}

void getConfigESP(){
  raw_config_t recv_config;

  Serial.write(1);
  while (Serial.available() <= 0) {
    delay(10);
  }
  for(int i = 0; i < 7; i++) {
    ((char *)(&recv_config))[i] = Serial.read();
  }
  num_minutes = Serial.read();
  raw_to_config(&recv_config, &config);
}

void getConfigManual(){
  config.ports = 3;
  config.batteries = 1;
  config.indicators = 0;
  strncpy(config.serial, "KTANE1", 6);
  config.serial[6] = '\0';
  num_minutes = 1;
}

void setup() {
  // Serial setup
  serial_port.begin(19200);
  Serial.begin(19200);

  delay(1000);

  Serial.println("Getting config");
  //getConfigESP();
  getConfigManual();
  Serial.println("Got config");
  Serial.write((uint8_t *)(&config), 7);
  Serial.write(num_minutes);
  Serial.println();

  Serial.println("Setting up I/O-pins");
  // LED/Speaker setup
  pinMode(STRIKE_1_PIN,  OUTPUT);
  pinMode(STRIKE_2_PIN,  OUTPUT);
  pinMode(CLOCK_DOT,  OUTPUT);
  pinMode(SPEAKER_PIN,   OUTPUT);

  // 7-segment setup
  pinMode(DATA_PIN, OUTPUT);
  pinMode(CLOCK_PIN,  OUTPUT);
  pinMode(LOAD_PIN,   OUTPUT);


  Serial.println("Initializing display");
  /*
   The MAX72XX is in power-saving mode on startup,
   we have to do a wakeup call
   */
  lc.shutdown(0,false);
  /* Set the brightness to a medium values */
  lc.setIntensity(0,8);
  /* and clear the display */
  lc.clearDisplay(0);


  Serial.println("Writing serial-number");
  // Serial alphanumeric setup
  lc.setChar(0, 4, "A"/*config.serial[0]*/, false);
  lc.setChar(0, 5, "C"/*config.serial[1]*/, false);
  lc.setChar(0, 6, "E"/*config.serial[2]*/, false);
  lc.setChar(0, 7, "F"/*config.serial[3]*/, false);


  delay(1000);

  Serial.println("Get modules");
  num_modules = 2;//master.identifyClients();

  controller.sendReset();
  delayWithUpdates(controller, 500);
  Serial.println("Sending config to clients");
  controller.sendConfig(&config);
  while(!controller.clientsAreReady()) {
    controller.interpretData();
  }

  dest_time = millis() + num_minutes*60*1000;

  Serial.println("Done setup");
}

void loop() {
  //Serial.println("Getting data");
  controller.interpretData();

  currentMillis = millis();

  if (currentMillis - previousMillis >= 1000) {
    // save the last time you blinked the LED
    previousMillis = currentMillis;
    Serial.println("blink");
    // if the LED is off turn it on and vice-versa:
    toggleClockBlink();
  }


  if(millis() > dest_time) {
    youLose();
  } else {
    // Update clock
    unsigned long diff_time = dest_time - millis();
    int seconds = (diff_time / 1000)%60;
    int minutes = diff_time / 60000;
    lc.setDigit(0, 0, minutes / 10, false);
    lc.setDigit(0, 1, minutes % 10, false);
    lc.setDigit(0, 2, seconds / 10, false);
    lc.setDigit(0, 3, seconds % 10, false);
  }

  if(strikes < controller.getStrikes()){
    tone(5, 340, 150);
    delayWithUpdates(controller, 200);
    tone(5, 140, 150);
    delayWithUpdates(controller, 150);
    noTone(5);
    strikes = controller.getStrikes();
    Serial.println("STRIKE!");
    Serial.println(strikes);
  }

  if(solves < controller.getSolves()){
    tone(5, 140, 150);
    delayWithUpdates(controller, 200);
    tone(5, 340, 150);
    delayWithUpdates(controller, 150);
    noTone(5);
    solves = controller.getSolves();
  }

  digitalWrite(STRIKE_1_PIN, strikes >= 1);
  digitalWrite(STRIKE_2_PIN, strikes >= 2);

  if(strikes >= 3){
    youLose();
  }

  if(controller.getSolves() >= num_modules) {
    youWin();
  }
}
