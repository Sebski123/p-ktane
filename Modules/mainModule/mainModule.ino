
#include "SWire.h"
#include "KTANECommon.h"
#include "LedControl.h"
#include "MAX6954.h"
#include <SoftwareSerial.h>

// Defines
//  Pins
#define SPEAKER_PIN 3
#define STRIKE_1_PIN 4
#define STRIKE_2_PIN 5
#define CLEAR_PIN 6
#define CLOCK_DOT 7
#define CLOCK_DATA 10
#define CLOCK_LOAD 11
#define CLOCK_CLK 12
#define SERIAL_CS 14      
#define SERIAL_DATAOUT 15 
#define SERIAL_CLK 16     
//I2C SDA 18
//I2C SCL 19

#define DBG_RX 14
#define DBG_TX 15
#define SERIAL_CS 16      // Chip Select
#define SERIAL_DATAOUT 17 // MOSI
#define SERIAL_CLK 18     // SCK

//Function prototypes
void toggleClockBlink();
void playMelody(int *melody, int *durations, int melody_len);
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
int lose_melody_durations[] = {8, 8, 8, 1};
int lose_melody_len = 4;
int num_minutes;
unsigned long currentMillis;
unsigned long previousMillis = 0;

unsigned long count = 0;

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
MAX6954 serialnr = MAX6954(SERIAL_DATAOUT, SERIAL_CLK, SERIAL_CS);
LedControl clock = LedControl(CLOCK_DATA, CLOCK_CLK, CLOCK_LOAD, 1);
config_t config;
SWireMaster master;
KTANEController controller(master);

// Globals
int strikes = 0;
int solves = 0;
unsigned long dest_time;
int num_modules;
uint8_t clients[16];

void toggleClockBlink()
{
  digitalWrite(CLOCK_DOT, !digitalRead(CLOCK_DOT));
}

void playMelody(int *melody, int *durations, int melody_len)
{
  for (int thisNote = 0; thisNote < melody_len; thisNote++)
  {

    int noteDuration = 1000 / durations[thisNote];
    tone(SPEAKER_PIN, melody[thisNote], noteDuration);

    // to distinguish the notes, set a minimum time between them.
    int pauseBetweenNotes = noteDuration * 1.30;
    delayWithUpdates(controller, pauseBetweenNotes);

    // stop the tone playing:
    noTone(SPEAKER_PIN);
  }
}

void youLose()
{
  // Play lose music
  Serial.println("Loose");
  serialnr.write_string(" boom ");
  playMelody(lose_melody, lose_melody_durations, lose_melody_len);

  // Stop clock
  while (1)
  {
    delayWithUpdates(controller, 10);
  }
}

void youWin()
{
  // Play win music
  Serial.println("Win");

  digitalWrite(CLEAR_PIN, HIGH);

  serialnr.write_string("winner");
  playMelody(win_melody, win_melody_durations, win_melody_len);

  // Stop clock
  while (1)
  {
    delayWithUpdates(controller, 10);
  }
}

void getConfigESP()
{
  raw_config_t recv_config;

  Serial.write("yo");
  while (Serial.available() <= 0)
  {
    delay(10);
  }
  for (int i = 0; i < 7; i++)
  {
    ((char *)(&recv_config))[i] = Serial.read();
  }
  num_minutes = Serial.read();
  raw_to_config(&recv_config, &config);
}

void getConfigManual()
{
  config.ports = 3;
  config.batteries = 1;
  config.indicators = 0;
  strncpy(config.serial, "123456", 6);
  config.serial[6] = '\0';
  num_minutes = 6;
}

void setup()
{
#pragma region Serial setup
  Serial.begin(19200);

  delay(1000);
#pragma endregion

#pragma region Get config
  Serial.println("Getting config");
  //getConfigESP();
  getConfigManual();
  Serial.println("Got config");
  Serial.write((uint8_t *)(&config), 7);
  Serial.println();
  Serial.println(num_minutes);

  delay(100);
#pragma endregion

#pragma region I / O setup
  Serial.println("Setting up I/O-pins");
  // LED/Speaker setup
  pinMode(STRIKE_1_PIN, OUTPUT);
  pinMode(STRIKE_2_PIN, OUTPUT);
  pinMode(CLEAR_PIN, OUTPUT);
  pinMode(CLOCK_DOT, OUTPUT);
  pinMode(SPEAKER_PIN, OUTPUT);

  digitalWrite(STRIKE_1_PIN, LOW);
  digitalWrite(STRIKE_2_PIN, LOW);
  digitalWrite(CLEAR_PIN, LOW);

#pragma endregion

#pragma region Display setup
  Serial.println("Initializing display");
  /*
   The MAX72XX is in power-saving mode on startup,
   we have to do a wakeup call
   */
  serialnr.begin();
  clock.shutdown(0, false);
  /* Set the brightness to a medium values */
  serialnr.set_global_brightness(8);
  clock.setIntensity(0, 8);
  /* and clear the display */
  clock.clearDisplay(0);
  serialnr.clear();

  delay(500);
#pragma endregion

#pragma region Serial setup
  Serial.println("Writing serial-number");
  // Serial alphanumeric setup
  serialnr.write_string(config.serial);

  delay(1000);
#pragma endregion

  Serial.println("Get modules");
  num_modules = master.identifyClients();

  Serial.print("Number of modules: ");
  Serial.println(num_modules);

  Serial.print("Client id's: ");
  master.getClients(clients);
  for (size_t i = 0; i < num_modules; i++)
  {
    Serial.print(clients[i]);
    Serial.print("\t");
  }
  Serial.println();

#pragma region Prepare modules
  controller.sendReset();
  delayWithUpdates(controller, 1000);
  Serial.println("Sending config to clients");
  int test = controller.sendConfig(&config);
  Serial.println(test);
  while (!controller.clientsAreReady())
  {
    controller.interpretData();
  }

  Serial.println("Clients are ready");
#pragma endregion

  dest_time = millis() + (long)num_minutes * 60 * 1000;
  controller.setTime(dest_time);

  Serial.println("Done setup");
}

void loop()
{
  //Serial.println("Getting data");
  controller.interpretData();

  currentMillis = millis();

  if (currentMillis - previousMillis >= 500)
  {
    // save the last time you blinked the LED
    previousMillis = currentMillis;
    //Serial.println("blink");
    // if the LED is off turn it on and vice-versa:
    toggleClockBlink();

    // Update clock
    unsigned long diff_time = dest_time - millis();
    controller.setTime(diff_time);
    int seconds = (diff_time / 1000) % 60;
    int minutes = diff_time / 60000;
    clock.setDigit(0, 0, (minutes / 10), false);
    clock.setDigit(0, 1, (minutes % 10), false);
    clock.setDigit(0, 2, (seconds / 10), false);
    clock.setDigit(0, 3, (seconds % 10), false);
  }

  if (millis() > dest_time)
  {
    youLose();
  }

  if (strikes < controller.getStrikes())
  {
    tone(5, 340, 150);
    delayWithUpdates(controller, 200);
    tone(5, 140, 150);
    delayWithUpdates(controller, 150);
    noTone(5);
    strikes = controller.getStrikes();
    Serial.println("STRIKE!");
    Serial.println(strikes);
  }

  if (solves < controller.getSolves())
  {
    tone(5, 140, 150);
    delayWithUpdates(controller, 200);
    tone(5, 340, 150);
    delayWithUpdates(controller, 150);
    noTone(5);
    solves = controller.getSolves();
  }

  digitalWrite(STRIKE_1_PIN, strikes >= 1);
  digitalWrite(STRIKE_2_PIN, strikes >= 2);

  if (strikes >= 3)
  {
    youLose();
  }

  if (controller.getSolves() >= num_modules)
  {
    youWin();
  }
}
