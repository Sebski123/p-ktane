/** @file KTANECommon.h
 *  @brief Headers and definitions for common KTANE functionality
 *
 *  @author Dillon Lareau (dlareau)
 */

#pragma once
#include "Arduino.h"
#include "SWire.h"

// Serial number tools:
#define IS_ODD(x) ((x)&1)
#define IS_EVEN(x) (!((x)&1))
#define IS_VOWEL(x) ((x) == 'A' || (x) == 'E' || (x) == 'I' || (x) == 'O' || (x) == 'U')
#define IS_NUMBER(x) ((x) >= '0' && (x) <= '9')
#define IS_LETTER(x) ((x) >= 'A' && (x) <= 'Z')

// Data prefix codes:
#define STRIKE (char)0xC0
#define SOLVE (char)0xC1
#define CONFIG (char)0xC2
#define READY (char)0xC3
#define RESET (char)0xC4
#define NUM_STRIKES (char)0xC5
#define NUM_SOLVES (char)0xC6
#define TIME (char)0xC7
#define STOP (char)0xC8

typedef struct raw_config_st
{
  // Byte 0
  unsigned int ports : 1;
  unsigned int batteries : 5;
  unsigned int indicators : 2;

  // Bytes 1-4
  char serial[4];

  // Byte 5
  unsigned int serial3 : 4;
  unsigned int serial6 : 4;
} raw_config_t;

typedef struct config_st
{
  unsigned int ports : 1;
  unsigned int batteries : 5;
  unsigned int indicators : 2;
  char serial[7];
} config_t;

void config_to_raw(config_t *config, raw_config_t *raw_config_t);
void raw_to_config(raw_config_t *raw_config, config_t *config_t);

void saveTimeLeft(char *new_time, char *stored_time);

unsigned long config_to_seed(config_t *config);

void putByte(byte data, int clock_pin, int data_in_pin);
void maxSingle(byte reg, byte col, int load_pin, int data_pin, int clock_pin);

class KTANEModule
{
public:
  KTANEModule(SWireClient &swire, int green_led_pin = 100, int red_led_pin = 100);
  void interpretData();
  void interpretData(Stream &s);
  config_t *getConfig();
  int strike();
  int win();
  int sendReady();
  int getNumStrikes();
  int getNumSolves();
  int is_solved;
  bool needyStop;
  int sendDebugMsg(char *msg);

  // Helper functions for strike() and win()
  int sendSolve();
  int sendStrike();
  int sendTime();

  // Various config getters
  char *getTime();
  void resetTime();
  int getLitFRK();
  int getLitCAR();
  int getNumBatteries();
  int getParallelPort();
  int getRCAPort();
  int getRJ45Port();
  char getSerialDigit(int index);
  int getLastDigitEven();
  int serialContains(char c);
  int serialContainsVowel();

  // currently useless because of hard reset
  int getReset();

private:
  SWireClient &_swire;
  config_t _config;
  char _timeLeft[5];
  int _green_led_pin;
  int _red_led_pin;
  int _got_config;
  int _got_time;
  int _num_strikes;
  int _num_solves;
  int _got_reset;
};

class KTANEController
{
public:
  KTANEController(SWireMaster &swire);
  void setTime(unsigned long timeLeft);
  void interpretData();
  int sendConfig(config_t *config);
  int getStrikes();
  int getSolves();
  int clientsAreReady();
  int sendReset();
  int sendStrikes();
  int sendSolves();
  int stopNeedys();

private:
  SWireMaster &_swire;
  uint8_t _strikes[MAX_CLIENTS];
  uint8_t _solves[MAX_CLIENTS];
  uint8_t _readies[MAX_CLIENTS];
  unsigned long timeLeftOnTimer;
  int _needy_modules;
};

void delayWithUpdates(KTANEModule &module, unsigned int length);
void delayWithUpdates(KTANEController &controller, unsigned int length);
