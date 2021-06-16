
#include "KTANECommon.h"
#include "LedControl.h"
#include "MAX6954.h"
#include "SoftwareSerial.h"
#include "DFRobotDFPlayerMini.h"
#include <DFMiniMp3.h>

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

class Mp3Notify
{
public:
  static void OnError(uint16_t errorCode)
  {
    // see DfMp3_Error for code meaning
    Serial.println();
    Serial.print("DFP Com Error ");
    Serial.println(errorCode);
  }
  static void OnPlayFinished(DfMp3_PlaySources source, uint16_t track) {}
  static void OnPlaySourceOnline(DfMp3_PlaySources source) {}
  static void OnPlaySourceInserted(DfMp3_PlaySources source) {}
  static void OnPlaySourceRemoved(DfMp3_PlaySources source) {}
};

//Class init
SoftwareSerial ESPSerial(ESP32_RX, ESP32_TX);
SoftwareSerial DFPlayerSerial(DFPLAYER_RX, DFPLAYER_TX); // RX, TX
DFMiniMp3<SoftwareSerial, Mp3Notify> DFPlayer(DFPlayerSerial);
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

enum SoundTrack
{
  singlebeep = 1, //singlebeep
  doublebeep_125, //doublebeep_1.25
  doublebeep,     //doublebeep
  strike,         //strike
  bombExplosion,  //explosion concrete large
  fanfare,        //GameOver_Fanfare
  bombDefused,    //bomb_defused
  winMelody,      //GameOver_Win
  looseMelody,    //GameOver_Lose
  needyActivated, //needy_activated
  needyWarning,   //needy_warning
  alarmClock      //alarm_clock_beep
};

void youLose()
{
  controller.stopNeedys();

  // Play lose music
  Serial.println("Loose");
  // if (deltaTime < 1000)
  // {
  //   clockDriver.setDigit(0, 0, 0, false);
  //   clockDriver.setDigit(0, 1, 0, false);
  //   clockDriver.setDigit(0, 2, 0, false);
  //   clockDriver.setDigit(0, 3, 0, false);
  // }

  // DFPlayer.playMp3FolderTrack(SoundTrack::bombExplosion);
  // while (DFPlayer.getCurrentTrack() == SoundTrack::bombExplosion)
  // {
  //   controller.interpretData();
  // }

  // DFPlayer.playMp3FolderTrack(SoundTrack::looseMelody);

  // Stop clock
  while (1)
  {
    delayWithUpdates(controller, 10);
  }
}

void youWin()
{

  controller.stopNeedys();

  // Play win music
  Serial.println("Win");

  digitalWrite(GREEN_CLEAR_PIN, HIGH);

  submitTime();

  // DFPlayer.playMp3FolderTrack(SoundTrack::bombDefused);
  // while (DFPlayer.getCurrentTrack() == SoundTrack::bombDefused)
  // {
  //   controller.interpretData();
  // }

  // DFPlayer.playMp3FolderTrack(SoundTrack::fanfare);
  // while (DFPlayer.getCurrentTrack() == SoundTrack::fanfare)
  // {
  //   controller.interpretData();
  // }

  // DFPlayer.playMp3FolderTrack(SoundTrack::winMelody);

  // Stop clock
  while (1)
  {
    delayWithUpdates(controller, 10);
  }
}

void submitTime()
{
  // int seconds = (deltaTime / 1000) % 60;
  // int minutes = deltaTime / 60000;
  // ESPSerial.write('W');
  // ESPSerial.write((char)((minutes / 10) + '0'));
  // ESPSerial.write((char)((minutes % 10) + '0'));
  // ESPSerial.write((char)((seconds / 10) + '0'));
  // ESPSerial.write((char)((seconds % 10) + '0'));
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

void handleBeep()
{
  if (rateModifier > 1.25)
  {
    // DFPlayer.playMp3FolderTrack(SoundTrack::singlebeep);
  }
  else if (rateModifier > 1)
  {
    // DFPlayer.playMp3FolderTrack(SoundTrack::doublebeep_125);
  }
  else
  {
    // DFPlayer.playMp3FolderTrack(SoundTrack::doublebeep);
  }
}

void updateDisplay()
{
  static uint8_t lastSecondDisplayed;
  uint8_t seconds;
  uint8_t leftNumber;
  uint8_t rightNumber;
  bool showDecimalPlace = false;
  seconds = (timeRemaining / 1000L) % 60L;
  if (timeRemaining > (1L * 60L * 1000L)) //1 minute
  {
    leftNumber = timeRemaining / 60000L; // minutes
    rightNumber = seconds;               // seconds
  }
  else
  {
    leftNumber = seconds;                               // seconds
    rightNumber = timeRemaining - (leftNumber * 1000L); // miliseconds
    showDecimalPlace = true;
    digitalWrite(CLOCK_DOT, LOW);
  }

  clockDriver.setDigit(0, 0, (leftNumber / 10L), false);
  clockDriver.setDigit(0, 1, (leftNumber % 10L), showDecimalPlace);
  clockDriver.setDigit(0, 2, (rightNumber / 10L), false);
  clockDriver.setDigit(0, 3, (rightNumber % 10L), false);

  if (lastSecondDisplayed != seconds)
  {
    lastSecondDisplayed = seconds;
    handleBeep();
  }

  controller.setTime(timeRemaining);
}

void updateStrikes()
{
  strikeDriver.write_char(' ', 0); // Star
  strikeDriver.write_char(' ', 1); // Star
  if (!panicModeStatus)
  {
    switch (settings.strikes)
    {
    case 1:
      strikeDriver.write_char(0x2A, 1); // Star
    case 2:
      strikeDriver.write_char(0x2A, 0); // Star
      if (strikes)
      {
        strikeDriver.write_char(0x2A, 1); // Star
      }
      break;
    case 3:
      switch (strikes)
      {
      case 2:
        strikeDriver.write_char(0x2A, 1); // Star
      case 1:
        strikeDriver.write_char(0x2A, 0); // Star
      default:
        break;
      }
      break;

    default:
      if (strikes < 3)
      {
        switch (strikes)
        {
        case 2:
          strikeDriver.write_char(0x2A, 1); // Star
        case 1:
          strikeDriver.write_char(0x2A, 0); // Star
        default:
          break;
        }
      }
      else if (strikes < 10)
      {
        strikeDriver.write_char(strikes + '0', 0);
        strikeDriver.write_char(0x2A, 1); // Star
      }
      else
      {
        strikeDriver.write_char((strikes / 10) + '0', 0);
        strikeDriver.write_char((strikes % 10) + '0', 1);
      }
      break;
    }
  }
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

#pragma region Reset modules
  Serial.println("Resetting modules");
  controller.sendReset();
  delayWithUpdates(controller, 1000);
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

#pragma region Sound setup
  // DFPlayer.begin();
  // DFPlayer.setVolume(24);

#pragma endregion

#pragma region Prepare modules
  Serial.println("Sending config to clients");
  controller.sendConfig(&config);
  while (!controller.clientsAreReady())
  {
    controller.interpretData();
  }

  Serial.println("Clients are ready");
#pragma endregion

#pragma region Clock setup
  timeRemaining = settings.time * 60.0 * 1000.0;
  controller.setTime(timeRemaining);
  previousMillis = millis();

  digitalWrite(CLOCK_DOT, settings.time >= 1 ? HIGH : LOW);
  Serial.println(timeRemaining);
  Serial.println("Done setup");
#pragma endregion
}

void loop()
{
  controller.interpretData();

  // DFPlayer.loop();

  // while (Serial.available() > 0)
  // {
  //   settings.strikes = Serial.parseInt();
  //   strikes = Serial.parseInt();
  //   Serial.print("Max strikes: ");
  //   Serial.print(settings.strikes);
  //   Serial.print("\tStrikes: ");
  //   Serial.println(strikes);
  //   panicModeStatus = false;
  // }

  deltaTime = millis() - previousMillis;
  timeRemaining -= deltaTime * rateModifier;
  previousMillis = millis();

  if (!panicMode && (settings.strikes - strikes == 1))
  {
    panicMode = true;
    panicModeTimer = timeRemaining;
  }

  if (panicMode && panicModeTimer - timeRemaining > 100)
  {
    panicModeStatus = !panicModeStatus;
    panicModeTimer = timeRemaining;
  }

  updateDisplay();

  if (strikes < controller.getStrikes())
  {
    //DFPlayer.playMp3FolderTrack(SoundTrack::strike);
    strikes = controller.getStrikes();
    if (rateModifier < 2)
    {
      rateModifier += 0.25;
    }
    Serial.print("STRIKE!:  ");
    Serial.println(strikes);
  }

  if (solves < controller.getSolves())
  {
    solves = controller.getSolves();
  }

  if (strikes >= settings.strikes || timeRemaining <= 0)
  {
    youLose();
  }

  updateStrikes();

  if (controller.getSolves() >= num_modules)
  {
    youWin();
  }
}
