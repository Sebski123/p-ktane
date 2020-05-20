#include "DSerial.h"
#include "KTANECommon.h"
#include "NeoICSerial.h"
#include "LedControl.h"


// Defines
#define BUTTON1_PIN 2
#define BUTTON2_PIN 3
#define BUTTON3_PIN 4
#define BUTTON4_PIN 5
//Green clear Led 6
//Red strike Led 7
//NeoICSerial RX-pin 8
//NeoICSerial TX-pin 9
#define CLOCK_PIN 10
#define LOAD_PIN 11
#define DATA_IN_PIN 12

#define LED1_PIN 14
#define LED2_PIN 15
#define LED3_PIN 16
#define LED4_PIN 17
#define LED5_PIN 18

LedControl lc=LedControl(DATA_IN_PIN, CLOCK_PIN, LOAD_PIN);

NeoICSerial serial_port;
DSerialClient client(serial_port, 0x04);
KTANEModule module(client, 6, 7);

uint8_t bottom_nums[5][4];
uint8_t top_nums[5];
uint8_t buttons_to_press[5];
int stage = 0;

// Has 6 elements to stop overflow when you win
int led_pins[6] = {LED1_PIN, LED2_PIN, LED3_PIN, LED4_PIN, LED5_PIN, LED5_PIN};

int digits[5] = {0, 1, 2, 3, 4};

uint8_t getIndexFromNumber(uint8_t *buttons, uint8_t num){
  for(int i = 0; i < 4; i++) {
    if(buttons[i] == num) {
      return i;
    }
  }
  return 255;
}

void updateDisplays() {
  lc.setDigit(0, 0, bottom_nums[stage][0], false);
  lc.setDigit(0, 1, bottom_nums[stage][1], false);
  lc.setDigit(0, 2, bottom_nums[stage][2], false);
  lc.setDigit(0, 3, bottom_nums[stage][3], false);
  lc.setDigit(0, 4, top_nums[stage], false);
  for(int i = 0; i < 5; i++) {
    digitalWrite(led_pins[i], LOW);
  }
  for(int i = 0; i <= stage; i++) {
    digitalWrite(led_pins[i], HIGH);
  }
}

void displayWaitingScreen() {

  // needs to delay for a total of 2500
  for(int i = 0; i < 5; i++) {
    lc.setDigit(0, digits[i], 0, false);
    delayWithUpdates(module, 100);
  }
  for(int i = 0; i < 15; i++){
    for(int j = 0; j < 5; j++) {
      lc.setDigit(0, digits[j], 1 << ((i+j)%7), false);
    }
    delayWithUpdates(module, 100);
  }
  for(int i = 0; i < 5; i++) {
    lc.setDigit(0, digits[i], 0, false);
  }
  delayWithUpdates(module, 500);
}

void generateRandomNumbers() {
  int r1, r2;
  uint8_t temp;
  for(int i = 0; i < 5; i++){
    //Insert 1-4 in each row
    bottom_nums[i][0] = 1;
    bottom_nums[i][1] = 2;
    bottom_nums[i][2] = 3;
    bottom_nums[i][3] = 4;

    //Swap random numbers in row 20 times to scramble them
    for(int j = 0; j < 20; j++){
      r1 = random(0, 4);
      r2 = random(0, 4);
      temp = bottom_nums[i][r1];
      bottom_nums[i][r1] = bottom_nums[i][r2];
      bottom_nums[i][r2] = temp;
    }

    top_nums[i] = random(1,5);
  }

  switch(top_nums[0]) {
    case(1):
      buttons_to_press[0] = 1; // Second Position
      break;
    case(2):
      buttons_to_press[0] = 1; // Second Position
      break;
    case(3):
      buttons_to_press[0] = 2; // Third Position
      break;
    case(4):
      buttons_to_press[0] = 3; // Fourth Position
      break;
  }
  switch(top_nums[1]) {
    case(1):
      // Button labeled 4
      buttons_to_press[1] = getIndexFromNumber(bottom_nums[1], 4);
      break;
    case(2):
      buttons_to_press[1] = buttons_to_press[0]; // Same position as stage 1
      break;
    case(3):
      buttons_to_press[1] = 0; // First Position
      break;
    case(4):
      buttons_to_press[1] = buttons_to_press[0]; // Same position as stage 1
      break;
  }
  switch(top_nums[2]) {
    case(1):
      // Same label as stage 2
      buttons_to_press[2] = getIndexFromNumber(bottom_nums[2], bottom_nums[1][buttons_to_press[1]]);
      break;
    case(2):
      // Same label as stage 1
      buttons_to_press[2] = getIndexFromNumber(bottom_nums[2], bottom_nums[0][buttons_to_press[0]]);
      break;
    case(3):
      buttons_to_press[2] = 2;  // Third Position
      break;
    case(4):
      // Button labeled 4
      buttons_to_press[2] = getIndexFromNumber(bottom_nums[2], 4) ;
      break;
  }
  switch(top_nums[3]) {
    case(1):
      buttons_to_press[3] = buttons_to_press[0]; // Same position as stage 1
      break;
    case(2):
      buttons_to_press[3] = 0;  // First Position
      break;
    case(3):
      buttons_to_press[3] = buttons_to_press[1]; // Same position as stage 2
      break;
    case(4):
      buttons_to_press[3] = buttons_to_press[1]; // Same position as stage 2
      break;
  }
  switch(top_nums[4]) {
    case(1):
      // Same label as stage 1
      buttons_to_press[4] = getIndexFromNumber(bottom_nums[4], bottom_nums[0][buttons_to_press[0]]);
      break;
    case(2):
      // Same label as stage 2
      buttons_to_press[4] = getIndexFromNumber(bottom_nums[4], bottom_nums[1][buttons_to_press[1]]);
      break;
    case(3):
      // Same label as stage 3
      buttons_to_press[4] = getIndexFromNumber(bottom_nums[4], bottom_nums[2][buttons_to_press[3]]);
      break;
    case(4):
      // Same label as stage 4
      buttons_to_press[4] = getIndexFromNumber(bottom_nums[4], bottom_nums[3][buttons_to_press[2]]);
      break;
  }
}

void setup() {
  serial_port.begin(19200);
  Serial.begin(19200);

  pinMode(DATA_IN_PIN, OUTPUT);
  pinMode(LOAD_PIN, OUTPUT);
  pinMode(CLOCK_PIN, OUTPUT);
  pinMode(BUTTON1_PIN, INPUT);
  pinMode(BUTTON2_PIN, INPUT);
  pinMode(BUTTON3_PIN, INPUT);
  pinMode(BUTTON4_PIN, INPUT);
  pinMode(LED1_PIN, OUTPUT);
  pinMode(LED2_PIN, OUTPUT);
  pinMode(LED3_PIN, OUTPUT);
  pinMode(LED4_PIN, OUTPUT);
  pinMode(LED5_PIN, OUTPUT);

  Serial.println("Setting up displays");

  lc.shutdown(0,false);    // not in shutdown mode
  lc.clearDisplay(0);
  lc.setIntensity(0,8); 

  Serial.println("Getting config");

  while(!module.getConfig()){
    module.interpretData();
  }

  Serial.println("Got config");

  randomSeed(config_to_seed(module.getConfig()));

  Serial.println("Generateing numbers");

  // Generate numbers
  generateRandomNumbers();
  updateDisplays();


  for(int i = 0; i < 5; i++){
    Serial.print("Stage");
    Serial.print(i);
    Serial.print(": ");
    for(int j = 0; j < 4; j++){
      Serial.print(bottom_nums[i][j]);
      Serial.print("\t");
    }
    Serial.print("\t");
    Serial.print(top_nums[i]);
    Serial.println();
  }

  Serial.print("Buttons to press: ");
  for(int i = 0; i < 5; i++){
    Serial.print(buttons_to_press[i]);
    Serial.print(" ");
  }
  Serial.println();

  Serial.println("Done setup");

  module.sendReady();
}

void loop() {
  int button_pressed = -1;

  module.interpretData();

  if(!module.is_solved) {
    if(!digitalRead(BUTTON1_PIN)) {
      button_pressed = 0;
    } else if(!digitalRead(BUTTON2_PIN)) {
      button_pressed = 1;
    } else if(!digitalRead(BUTTON3_PIN)) {
      button_pressed = 2;
    } else if(!digitalRead(BUTTON4_PIN)) {
      button_pressed = 3;
    }

    if(button_pressed != -1) {
      if(button_pressed == buttons_to_press[stage]) {
        stage++;
      } else {
        stage = 0;
        generateRandomNumbers();
        module.strike();
      }
      if(stage == 5){
        module.win();
      } else {
      displayWaitingScreen();
      updateDisplays();
      }
    }
  }

}