/*

        WIP 


 */



#include <Arduino.h>

//We always have to include the library
#include "LedControl.h"


#define DATA_PIN 12
#define CLOCK_PIN 11
#define LOAD_PIN 10
#define STRIKE_1_PIN 9
#define STRIKE_2_PIN 8
#define CLOCK_DOT 7
#define SPEAKER_PIN 5


// Globals
int strikes = 0;
int solves = 0;
unsigned long dest_time = 300000;
int num_modules = 1;

/*
 Now we need a LedControl to work with.
 ***** These pin numb
 ers will probably not work with your hardware *****
 pin 12 is connected to the DataIn 
 pin 11 is connected to the CLK 
 pin 10 is connected to LOAD 
 We have only a single MAX72XX.
 */
LedControl lc=LedControl(12,11,10,1);

/* we always wait a bit between updates of the display */
unsigned long delaytime=500;

unsigned long previousMillis = 0;

int win_melody[] = {262, 330, 294, 370, 392};
int win_melody_durations[] = {8, 8, 8, 8, 2};
int win_melody_len = 5;
int lose_melody[] = {659, 622, 587, 554};
int lose_melody_durations[] = {8,8,8,1};
int lose_melody_len = 4;


void toggleClockBlink();
void writeArduinoOn7Segment();
void scrollDigits();
void showOrder();
void playMelody(int *melody, int* durations, int melody_len);


void toggleClockBlink() {
  digitalWrite(CLOCK_DOT, !digitalRead(CLOCK_DOT));
}

void playMelody(int *melody, int* durations, int melody_len) {
  for (int thisNote = 0; thisNote < melody_len; thisNote++) {

    int noteDuration = 1000 / durations[thisNote];
    tone(SPEAKER_PIN, melody[thisNote], noteDuration);

    // to distinguish the notes, set a minimum time between them.
    int pauseBetweenNotes = noteDuration * 1.30;
    
    delay(pauseBetweenNotes);
    //delayWithUpdates(controller, pauseBetweenNotes);

    // stop the tone playing:
    noTone(SPEAKER_PIN);
  }
}

void youLose() {
  // Play lose music
  lc.clearDisplay(0);
  lc.setChar(0, 4, 'd', false);
  lc.setChar(0, 5, 'e', false);
  lc.setChar(0, 6, 'a', false);
  lc.setChar(0, 7, 'd', false);
  playMelody(lose_melody, lose_melody_durations, lose_melody_len);

  // Stop clock
  while(1){
    delay(10);
    //delayWithUpdates(controller, 10);
  }
}

void youWin() {
  // Play win music
  lc.clearDisplay(0);
  lc.setChar(0, 4, 'd', false);
  lc.setChar(0, 5, 'e', false);
  lc.setChar(0, 6, 'a', false);
  lc.setChar(0, 7, 'd', false);
  playMelody(win_melody, win_melody_durations, win_melody_len);

  // Stop clock
  while(1){
    delay(10);
    //delayWithUpdates(controller, 10);
  }
}

void setup() {
  Serial.begin(9600);

  pinMode(CLOCK_DOT, OUTPUT);
  pinMode(STRIKE_1_PIN, OUTPUT);
  pinMode(STRIKE_2_PIN, OUTPUT);

  /*
   The MAX72XX is in power-saving mode on startup,
   we have to do a wakeup call
   */
  lc.shutdown(0,false);
  /* Set the brightness to a medium values */
  lc.setIntensity(0,8);
  /* and clear the display */
  lc.clearDisplay(0);
}


/*
 This method will display the characters for the
 word "Arduino" one after the other on digit 0. 
 */
void writeArduinoOn7Segment() {
  lc.setChar(0,0,'a',false);
  Serial.print("a");
  delay(delaytime);
  lc.setRow(0,1,0x05);
  Serial.print("r");
  delay(delaytime);
  lc.setChar(0,2,'d',false);
  Serial.print("d");
  delay(delaytime);
  lc.setRow(0,3,0x1c);
  Serial.print("u");
  delay(delaytime);
  lc.setRow(0,4,B00010000);
  Serial.print("i");
  delay(delaytime);
  lc.setRow(0,5,0x15);
  Serial.print("n");
  delay(delaytime);
  lc.setRow(0,6,0x1D);
  Serial.println("o");
  delay(delaytime);
  lc.clearDisplay(0);
  delay(delaytime);
} 

/*
  This method will scroll all the hexa-decimal
 numbers and letters on the display. You will need at least
 four 7-Segment digits. otherwise it won't really look that good.
 */
void scrollDigits() {
  for(int i=0;i<16;i++) {
    Serial.println(i);
    lc.setDigit(0,7,i,false);
    lc.setDigit(0,6,i+1,false);
    lc.setDigit(0,5,i+2,false);
    lc.setDigit(0,4,i+3,false);
    lc.setDigit(0,3,i+4,false);
    lc.setDigit(0,2,i+5,false);
    lc.setDigit(0,1,i+6,false);
    lc.setDigit(0,0,i+7,false);
    delay(delaytime);
  }
  lc.clearDisplay(0);
  delay(delaytime);
}

void showOrder() {
  for(int i=0;i<8;i++) {
    Serial.println(i);
    lc.setDigit(0,i,i,false);
  }
  delay(delaytime*4);
  lc.clearDisplay(0);
}

void loop() { 

  /* 
  //Old stuff for testing
  Serial.println("Ready");
  writeArduinoOn7Segment();
  scrollDigits();
  //showOrder();
  Serial.println("done");
  */

  //controller.interpretData();

  if(millis() > dest_time) {
    youLose();
  } else {
    // Update clock
    unsigned long diff_time = dest_time - millis();
    int seconds = (diff_time / 1000) % 60;
    int minutes = diff_time / 60000;
    lc.setDigit(0, 0, minutes / 10, false);
    lc.setDigit(0, 1, minutes % 10, false);
    lc.setDigit(0, 2, seconds / 10, false);
    lc.setDigit(0, 3, seconds % 10, false);
  }

  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= 1000) {
    // save the last time you blinked the LED
    previousMillis = currentMillis;

    // if the LED is off turn it on and vice-versa:
    toggleClockBlink();
  }

/* 
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
  */
  digitalWrite(STRIKE_1_PIN, strikes >= 1);
  digitalWrite(STRIKE_2_PIN, strikes >= 2);


  if(strikes >= 3){
    youLose();
  }
  /*
  if(controller.getSolves() >= num_modules) {
    youWin();
  }*/
}
