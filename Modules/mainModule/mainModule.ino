
#include "SWire.h"
#include "KTANECommon.h"
#include "LedControl.h"
#include "MAX6954.h"
#include "Adafruit_MCP23017.h"
#include <SoftwareSerial.h>

// Defines
//  Pins
#define SPEAKER_PIN 3
#define STRIKE_1_PIN 4
#define STRIKE_2_PIN 5
#define CLEAR_PIN 6
#define CLOCK_DOT 7
#define CONF_RX 8
#define CONF_TX 9
#define CLOCK_DATA 10
#define CLOCK_LOAD 11
#define CLOCK_CLK 12
#define SERIAL_CS 14
#define SERIAL_DATAOUT 15
#define SERIAL_CLK 16
//I2C SDA 18
//I2C SCL 19
//  I/O Expander pins
#define BATT1 0
#define BATT2 1
#define BATT3 2
#define BATT4 3
#define FRK_LED 8
#define CAR_LED 9
#define FRQ_LED 10
#define IND_LED 11
#define BOB_LED 12
#define NSA_LED 13

//Class init
Adafruit_MCP23017 mcp;
SoftwareSerial configSerial = SoftwareSerial(CONF_RX, CONF_TX);
MAX6954 serialnr = MAX6954(SERIAL_DATAOUT, SERIAL_CLK, SERIAL_CS);
LedControl clock = LedControl(CLOCK_DATA, CLOCK_CLK, CLOCK_LOAD, 1);
config_t config;
SWireMaster master;
KTANEController controller(master);

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
unsigned long diff_time = 0;
byte msClockCount = 0;

//  Globals
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
  if (diff_time < 1000)
  {
    clock.setDigit(0, 0, 0, false);
    clock.setDigit(0, 1, 0, false);
    clock.setDigit(0, 2, 0, false);
    clock.setDigit(0, 3, 0, false);
  }

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
  int seconds = (diff_time / 1000) % 60;
  int minutes = diff_time / 60000;
  // Play win music
  Serial.println("Win");

  digitalWrite(CLEAR_PIN, HIGH);

  serialnr.write_string("winner");
  playMelody(win_melody, win_melody_durations, win_melody_len);

  configSerial.write('W');
  configSerial.write((char)((minutes / 10) + '0'));
  configSerial.write((char)((minutes % 10) + '0'));
  configSerial.write((char)((seconds / 10) + '0'));
  configSerial.write((char)((seconds % 10) + '0'));

  // Stop clock
  while (1)
  {
    delayWithUpdates(controller, 10);
  }
}

void getConfigESP()
{
  raw_config_t recv_config;

  configSerial.write("1");
  while (configSerial.available() <= 0)
  {
    delay(10);
  }
  for (int i = 0; i < 7; i++)
  {
    ((char *)(&recv_config))[i] = configSerial.read();
  }
  num_minutes = configSerial.read();
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
  configSerial.begin(19200);

  while (!Serial)
  {
    ;
  } // wait for serial port to connect. Needed for native USB
  while (!configSerial)
  {
    ;
  } // wait for serial port to connect. Needed for native USB

#pragma endregion

#pragma region Get config
  Serial.println("Getting config");
  getConfigESP();
  //getConfigManual();
#pragma endregion

#pragma region I / O Expander setup
  mcp.begin(); // use default address 0x20

  //Setup battery pins and enable 100K internal pullup 
  mcp.pinMode(BATT1, INPUT);
  mcp.pinMode(BATT2, INPUT);
  mcp.pinMode(BATT3, INPUT);
  mcp.pinMode(BATT4, INPUT);
  mcp.pullUp(BATT1, HIGH);
  mcp.pullUp(BATT2, HIGH);
  mcp.pullUp(BATT3, HIGH);
  mcp.pullUp(BATT4, HIGH);

  //Setup indicator pins
  mcp.pinMode(FRK_LED, OUTPUT);
  mcp.pinMode(CAR_LED, OUTPUT);
  mcp.pinMode(FRQ_LED, OUTPUT);
  mcp.pinMode(IND_LED, OUTPUT);
  mcp.pinMode(BOB_LED, OUTPUT);
  mcp.pinMode(NSA_LED, OUTPUT);
#pragma endregion

#pragma region SET / GET I / O Expander values
  //Read number of batteries
  config.batteries = 0;
  for(int i = 0; i < 4; i++){
    config.batteries += !mcp.digitalRead(i);
  }
  
  config.indicators = 0;
  randomSeed(config_to_seed(&config));

  // Turn on indicators randomly
  for(int i = 8; i < 14; i++){
    mcp.digitalWrite(i, random(2));
  }

  //Read FRK and CAR indicators
  config.indicators |= mcp.digitalRead(FRK_LED);
  config.indicators |= (mcp.digitalRead(CAR_LED) << 1);
#pragma endregion

#pragma region Show config
  Serial.println("Got config");
  Serial.print("Serial nr. : ");
  Serial.println(config.serial);
  Serial.print("No. of Batteries: ");
  Serial.println(config.batteries);
  Serial.print("Lit indicators: ");
  if (config.indicators & 1)
  {
    Serial.print("FRK\t");
  }
  if (config.indicators & 2)
  {
    Serial.print("CAR");
  }
  Serial.println();
  Serial.print("Ports: ");
  if (config.ports & 1)
  {
    Serial.print("Parallel\t");
  }
  if (config.ports & 2)
  {
    Serial.print("RJ45\t");
  }
  if (config.ports & 4)
  {
    Serial.print("RCA");
  }
  Serial.println();
  Serial.print("Minutes to beat: ");
  Serial.println(num_minutes);
  Serial.println();

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

  diff_time = dest_time - millis();

  if (diff_time > 60000)
  {
    if (currentMillis - previousMillis >= 500)
    {
      // save the last time you blinked the LED
      previousMillis = currentMillis;
      //Serial.println("blink");
      // if the LED is off turn it on and vice-versa:
      toggleClockBlink();

      // Update clock

      controller.setTime(diff_time);
      int seconds = (diff_time / 1000) % 60;
      int minutes = diff_time / 60000;
      clock.setDigit(0, 0, (minutes / 10), false);
      clock.setDigit(0, 1, (minutes % 10), false);
      clock.setDigit(0, 2, (seconds / 10), false);
      clock.setDigit(0, 3, (seconds % 10), false);
    }
  }
  else
  {
    if (currentMillis - previousMillis >= 10)
    {
      // save the last time you blinked the LED
      previousMillis = currentMillis;
      //Serial.println("blink");
      // if the LED is off turn it on and vice-versa:
      if (msClockCount > 25)
      {
        toggleClockBlink();
        msClockCount = 0;
      }
      else
      {
        msClockCount++;
      }
      // Update clock

      controller.setTime(diff_time);
      int seconds = (diff_time / 1000) % 60;
      int millisecs = diff_time - (seconds * 1000);
      clock.setDigit(0, 0, (seconds / 10), false);
      clock.setDigit(0, 1, (seconds % 10), false);
      clock.setDigit(0, 2, (millisecs / 10), false);
      clock.setDigit(0, 3, (millisecs % 10), false);
    }
  }
  if (millis() > dest_time)
  {
    youLose();
  }

  if (strikes < controller.getStrikes())
  {
    tone(SPEAKER_PIN, 340, 150);
    delayWithUpdates(controller, 200);
    tone(SPEAKER_PIN, 140, 150);
    delayWithUpdates(controller, 150);
    noTone(SPEAKER_PIN);
    strikes = controller.getStrikes();
    Serial.println("STRIKE!");
    Serial.println(strikes);
  }

  if (solves < controller.getSolves())
  {
    tone(SPEAKER_PIN, 140, 150);
    delayWithUpdates(controller, 200);
    tone(SPEAKER_PIN, 340, 150);
    delayWithUpdates(controller, 150);
    noTone(SPEAKER_PIN);
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
