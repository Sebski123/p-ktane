
#include "KTANECommon.h"
#include "LedControl.h"
#include "MAX6954.h"
#include "SoftwareSerial.h"
#include "DFRobotDFPlayerMini.h"

// Defines
//  Pins
//RX 0
//TX 1
#define CLOCK_DOT 2
#define ESP32_RX 3
#define ESP32_TX 4
#define GREEN_CLEAR_PIN 7
#define SPI_MOSI 8
#define SPI_SCK 9
#define MAX7219_CS 10
#define MAX6954_CS 11
#define DFPLAYER_RX 12
#define DFPLAYER_TX 13
//I2C SDA 18
//I2C SCL 19

//Class init
SoftwareSerial ESPSerial(ESP32_RX, ESP32_TX);
SoftwareSerial DFPlayerSerial(DFPLAYER_RX, DFPLAYER_TX); // RX, TX
MAX6954 strikeDriver(SPI_MOSI, SPI_SCK, MAX6954_CS);
LedControl clockDriver(SPI_MOSI, SPI_SCK, MAX7219_CS, 1);
SWireMaster master;
KTANEController controller(master);

//Variables
unsigned long previousMillis = 0;
unsigned long deltaTime = 0;
unsigned long panicModeTimer = 0;
long timeRemaining = 0;
float rateModifier = 1;
bool panicModeStatus = false;
bool panicMode = false;
byte msClockCount = 0;
config_t config;
settings_t settings;

//  Globals
int strikes = 0;
int solves = 0;

int num_modules;
uint8_t clients[16];

void playMelody(int *melody, int *durations, int melody_len)
{
  // for (int thisNote = 0; thisNote < melody_len; thisNote++)
  // {

  //   int noteDuration = 1000 / durations[thisNote];
  //   tone(SPEAKER_PIN, melody[thisNote], noteDuration);

  //   // to distinguish the notes, set a minimum time between them.
  //   int pauseBetweenNotes = noteDuration * 1.30;
  //   delayWithUpdates(controller, pauseBetweenNotes);

  //   // stop the tone playing:
  //   noTone(SPEAKER_PIN);
  // }
}

void youLose()
{
  controller.stopNeedys();

  // Play lose music
  Serial.println("Loose");
  if (deltaTime < 1000)
  {
    clockDriver.setDigit(0, 0, 0, false);
    clockDriver.setDigit(0, 1, 0, false);
    clockDriver.setDigit(0, 2, 0, false);
    clockDriver.setDigit(0, 3, 0, false);
  }

  //playMelody(lose_melody, lose_melody_durations, lose_melody_len);

  // Stop clock
  while (1)
  {
    delayWithUpdates(controller, 10);
  }
}

void youWin()
{

  controller.stopNeedys();
  int seconds = (deltaTime / 1000) % 60;
  int minutes = deltaTime / 60000;
  // Play win music
  Serial.println("Win");

  digitalWrite(GREEN_CLEAR_PIN, HIGH);

  //playMelody(win_melody, win_melody_durations, win_melody_len);

  ESPSerial.write('W');
  ESPSerial.write((char)((minutes / 10) + '0'));
  ESPSerial.write((char)((minutes % 10) + '0'));
  ESPSerial.write((char)((seconds / 10) + '0'));
  ESPSerial.write((char)((seconds % 10) + '0'));

  // Stop clock
  while (1)
  {
    delayWithUpdates(controller, 10);
  }
}

// void getConfigESP()
// {
//   raw_config_t recv_config;

//   configSerial.write("1");
//   while (configSerial.available() <= 0)
//   {
//     delay(10);
//   }
//   for (int i = 0; i < 7; i++)
//   {
//     ((char *)(&recv_config))[i] = configSerial.read();
//   }
//   num_minutes = configSerial.read();
//   raw_to_config(&recv_config, &config);
// }

// void getConfigManual()
// {
//   config.ports = 3;
//   config.batteries = 1;
//   config.indicators = 0;
//   strncpy(config.serial, "123456", 6);
//   config.serial[6] = '\0';
//   num_minutes = 6;
// }

void getSettingsESP()
{
  ESPSerial.write(".");
  while (ESPSerial.available() <= 0)
  {
    delay(1);
  }
  settings.seed = ESPSerial.read() << 8;
  settings.seed |= ESPSerial.read();
  settings.time = ESPSerial.read();
}

void getSettingsManual()
{
  settings.seed = 12345;
  settings.time = 10;
  settings.strikes = 3;
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
  ESPSerial.begin(19200);
  DFPlayerSerial.begin(9600);

  while (!Serial)
  {
    ;
  } // wait for serial port to connect. Needed for native USB

#pragma endregion

#pragma region Get settings
  Serial.println("Getting settings");
  //getSettingsESP();
  getSettingsManual();
  Serial.println("Got settings:");
  Serial.print("Seed: ");
  Serial.println(settings.seed);
  Serial.print("Time: ");
  Serial.println(settings.time);
  Serial.print("Max Strikes: ");
  Serial.println(settings.strikes);
  Serial.println();
#pragma endregion

#pragma region Identify connected modules
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
  Serial.println();
#pragma endregion

#pragma region Get config
  Serial.println("Get config");
  Serial.println("Sending seed");
  controller.sendSeed(settings);
  while (!controller.getConfig())
  {
    controller.interpretData();
  }
  Serial.println("Got config");
  memcpy(&config, controller.getConfig(), sizeof(config_t));
  //config = controller.getConfig();
#pragma endregion

#pragma region Show config
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
  Serial.println();
  Serial.print("Minutes to beat: ");
  Serial.println(settings.time);

  delay(100);
#pragma endregion

#pragma region I / O setup
  Serial.println("Setting up I/O-pins");
  pinMode(CLOCK_DOT, OUTPUT);
  pinMode(GREEN_CLEAR_PIN, OUTPUT);

  digitalWrite(GREEN_CLEAR_PIN, LOW);
#pragma endregion

#pragma region Display setup
  Serial.println("Initializing display");
  /*
     The MAX72XX is in power-saving mode on startup,
     we have to do a wakeup call
     */
  strikeDriver.begin();
  clockDriver.shutdown(0, false);
  /* Set the brightness to a medium values */
  strikeDriver.set_global_brightness(2);
  clockDriver.setIntensity(0, 8);
  /* and clear the display */
  clockDriver.clearDisplay(0);
  strikeDriver.clear();

  delay(500);
#pragma endregion

#pragma region Prepare modules
  controller.sendReset();
  delayWithUpdates(controller, 1000);
  Serial.println("Sending config to clients");
  controller.sendConfig(&config);
  while (!controller.clientsAreReady())
  {
    controller.interpretData();
  }

  Serial.println("Clients are ready");
#pragma endregion

  timeRemaining = settings.time * 60.0 * 1000.0;
  controller.setTime(timeRemaining);
  previousMillis = millis();

  digitalWrite(CLOCK_DOT, HIGH);
  Serial.println(timeRemaining);
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

  deltaTime = millis() - previousMillis;
  timeRemaining -= deltaTime * rateModifier;
  previousMillis = millis();

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
    //tone(SPEAKER_PIN, 140, 150);
    delayWithUpdates(controller, 200);
    //tone(SPEAKER_PIN, 340, 150);
    delayWithUpdates(controller, 150);
    //noTone(SPEAKER_PIN);
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
