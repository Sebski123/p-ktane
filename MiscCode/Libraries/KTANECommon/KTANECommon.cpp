/** @file KTANECommon.cpp
 *  @brief Headers and definitions for common KTANE functionality
 *
 *  Possibly restart microcontroller on reset?
 *
 *  @author Dillon Lareau (dlareau)
 */

#include "Arduino.h"
#include "KTANECommon.h"
#include <string.h>

const int needys[] = {0x7, 0x8, 0x9};

void config_to_raw(config_t *config, raw_config_t *raw_config)
{
  raw_config->ports = config->ports;
  raw_config->batteries = config->batteries;
  raw_config->indicators = config->indicators;
  memcpy(raw_config->serial, config->serial, 5);
  raw_config->serial6 = config->serial[5] - '0';
  raw_config->spacer1 = 1;
  raw_config->spacer2 = 1;
}

void raw_to_config(raw_config_t *raw_config, config_t *config)
{
  config->ports = raw_config->ports;
  config->batteries = raw_config->batteries;
  config->indicators = raw_config->indicators;
  memcpy(config->serial, raw_config->serial, 5);
  config->serial[5] = raw_config->serial6 + '0';
  config->serial[6] = '\0';
}

unsigned long config_to_seed(config_t *config)
{
  unsigned long retval = 0;
  int i;

  for (i = 5; i >= 0; i--)
  {
    retval = (retval << 6) + (int)(config->serial[i] - ' ');
  }
  retval = retval * (3 + config->ports);
  retval = retval * (5 + config->batteries);
  retval = retval * (7 + config->indicators);
  return retval;
}

void saveTimeLeft(char *new_time, char *stored_time)
{
  memcpy(stored_time, new_time, 4);
  stored_time[4] = '\0';
}

void delayWithUpdates(KTANEModule &module, unsigned int length)
{
  unsigned long start_millis = millis();
  while (millis() - start_millis < length)
  {
    module.interpretData();
  }
}

void delayWithUpdates(KTANEController &controller, unsigned int length)
{
  unsigned long start_millis = millis();
  while (millis() - start_millis < length)
  {
    controller.interpretData();
  }
}

void (*softwareReset)(void) = 0;

KTANEModule::KTANEModule(SWireClient &swire, int green_led_pin, int red_led_pin) : _swire(swire)
{
  memset(&_config, 0, sizeof(config_t));
  memset(&_timeLeft, 0, sizeof(_timeLeft));
  _red_led_pin = red_led_pin;
  _green_led_pin = green_led_pin;
  pinMode(_green_led_pin, OUTPUT);
  pinMode(_red_led_pin, OUTPUT);
  _got_config = 0;
  _got_time = 0;
  _num_strikes = 0;
  _num_solves = 0;
  _got_reset = 0;
  is_solved = 0;
  needyStop = false;
}

void KTANEModule::interpretData()
{
  char out_message[MAX_MSG_LEN];
  unsigned long start_millis;

  if (_swire.getData(out_message))
  {
    Serial.print("Got data");
    Serial.println(out_message);
    if (out_message[0] == CONFIG && strlen(out_message) == 8)
    {
      _got_config = 1;
      raw_to_config((raw_config_t *)(out_message + 1), &_config);
    }
    else if (out_message[0] == RESET)
    {
      // All of the stuff before softwareReset() is currently useless
      //  but is kept in case the hard-reset call is removed.
      is_solved = 0;
      _num_strikes = 0;
      _num_solves = 0;
      _got_config = 0;
      memset(&_config, 0, sizeof(config_t));
      _got_reset = 1;
      digitalWrite(_green_led_pin, LOW);

      // Delay for a small bit to allow client to ACK the reset.
      start_millis = millis();
      while (millis() - start_millis < 300)
      {
      }
      softwareReset();
    }
    else if (out_message[0] == NUM_STRIKES)
    {
      _num_strikes = out_message[1];
    }
    else if (out_message[0] == NUM_SOLVES)
    {
      _num_solves = out_message[1];
    }
    else if (out_message[0] == TIME)
    {
      _got_time = 1;
      saveTimeLeft((out_message + 1), _timeLeft);
    }
    else if (out_message[0] == STOP)
    {
      needyStop = true;
    }
  }
}

int KTANEModule::strike()
{
  int result = sendStrike();
  digitalWrite(_red_led_pin, HIGH);
  delayWithUpdates(*this, 500);
  digitalWrite(_red_led_pin, LOW);
  return result;
}

int KTANEModule::sendStrike()
{
  char str[2] = {STRIKE, '\0'};
  return _swire.sendData(str);
}

int KTANEModule::win()
{
  digitalWrite(_green_led_pin, HIGH);
  return sendSolve();
}

int KTANEModule::sendSolve()
{
  char str[2] = {SOLVE, '\0'};
  is_solved = 1;
  return _swire.sendData(str);
}

int KTANEModule::sendReady()
{
  char str[2] = {READY, '\0'};
  int result = _swire.sendData(str);
  if (result)
  {
    digitalWrite(_red_led_pin, LOW);
    digitalWrite(_green_led_pin, HIGH);
    delayWithUpdates(*this, 300);
    digitalWrite(_green_led_pin, LOW);
  }
  return result;
}

int KTANEModule::sendDebugMsg(char *msg)
{
  return _swire.sendData(msg);
}

config_t *KTANEModule::getConfig()
{
  if (_got_config)
  {
    return &_config;
  }
  else
  {
    return NULL;
  }
}

int KTANEModule::sendTime()
{
  char str[2] = {TIME, '\0'};
  return _swire.sendData(str);
}

void KTANEModule::resetTime()
{
  _got_time = 0;
}

char *KTANEModule::getTime()
{
  if (_got_time)
  {
    return _timeLeft;
  }
  else
  {
    return NULL;
  }
}

int KTANEModule::getLitFRK()
{
  return _config.indicators & 1;
}

int KTANEModule::getLitCAR()
{
  return _config.indicators & 2;
}

int KTANEModule::getNumBatteries()
{
  return _config.batteries;
}

int KTANEModule::getParallelPort()
{
  return _config.ports & 1;
}

int KTANEModule::getRJ45Port()
{
  return _config.ports & 2;
}

int KTANEModule::getRCAPort()
{
  return _config.ports & 4;
}

char KTANEModule::getSerialDigit(int index)
{
  if (index < 0 or index > 5)
  {
    return (char)0;
  }
  return _config.serial[index];
}

int KTANEModule::getLastDigitEven()
{
  return IS_EVEN(getSerialDigit(5) - '0');
}

int KTANEModule::serialContains(char c)
{
  return !!((int)strchr(_config.serial, c));
}

int KTANEModule::serialContainsVowel()
{
  int serial_length = strlen(_config.serial);
  char temp;
  for (int i = 0; i < serial_length; i++)
  {
    temp = _config.serial[i];
    if (temp == 'A' || temp == 'E' || temp == 'I' || temp == 'O' || temp == 'U')
    {
      return 1;
    }
  }
  return 0;
}

int KTANEModule::getNumStrikes()
{
  return _num_strikes;
}

int KTANEModule::getNumSolves()
{
  return _num_solves;
}

int KTANEModule::getReset()
{
  if (_got_reset)
  {
    _got_reset = 0;
    return 1;
  }
  return 0;
}

KTANEController::KTANEController(SWireMaster &swire) : _swire(swire)
{
  memset(_strikes, 0, MAX_CLIENTS);
  memset(_solves, 0, MAX_CLIENTS);
  memset(_readies, 0, MAX_CLIENTS);
  _needy_modules = 0;
}

void KTANEController::setTime(unsigned long timeLeft)
{
  timeLeftOnTimer = timeLeft;
}

void KTANEController::interpretData()
{
  char out_message[MAX_MSG_LEN];
  int client_id = _swire.getData(out_message);
  if (client_id)
  {
    Serial.print("Got ");
    Serial.print(out_message);
    Serial.print(" from ");
    Serial.println((char)client_id);
    if (out_message[0] == STRIKE)
    {
      _strikes[client_id] = _strikes[client_id] + 1;
      sendStrikes();
    }
    else if (out_message[0] == SOLVE)
    {
      _solves[client_id] = 1;
      if (!(client_id == 0x7 || client_id == 0x8 || client_id == 0x9))
      {
        sendSolves();
      }
    }
    else if (out_message[0] == READY)
    {
      _readies[client_id] = 1;
      if (client_id == 0x7 || client_id == 0x8 || client_id == 0x9)
      {
        _needy_modules++;
      }
    }
    else if (out_message[0] == TIME)
    {
      char msg[6];
      msg[0] = TIME;
      int seconds = (timeLeftOnTimer / 1000) % 60;
      int minutes = timeLeftOnTimer / 60000;
      msg[1] = (minutes / 10) + '0';
      msg[2] = (minutes % 10) + '0';
      msg[3] = (seconds / 10) + '0';
      msg[4] = (seconds % 10) + '0';
      msg[5] = '\0';
      _swire.sendData(client_id, msg);
    }
  }
}

int KTANEController::sendConfig(config_t *config)
{
  char msg[9];
  int err = 0;
  uint8_t clients[MAX_CLIENTS];
  int num_clients = 0;
  num_clients = _swire.getClients(clients);

  msg[0] = CONFIG;
  config_to_raw(config, (raw_config_t *)(msg + 1));
  msg[8] = '\0';

  for (int i = 0; i < num_clients; i++)
  {
    if (!_swire.sendData(clients[i], msg))
    {
      err++;
    }
  }
  return (err == 0);
}

int KTANEController::getStrikes()
{
  int num_strikes = 0;
  for (int i = 0; i < MAX_CLIENTS; i++)
  {
    num_strikes += _strikes[i];
  }
  return num_strikes;
}

int KTANEController::getSolves()
{
  int num_solves = 0;
  for (int i = 0; i < MAX_CLIENTS; i++)
  {
    num_solves += _solves[i];
  }
  return num_solves;
}

int KTANEController::clientsAreReady()
{
  int num_readies = 0;
  for (int i = 0; i < MAX_CLIENTS; i++)
  {
    num_readies += _readies[i];
  }
  return num_readies >= _swire.getClients(NULL);
}

int KTANEController::sendReset()
{
  char msg[2] = {RESET, '\0'};
  int err = 0;
  uint8_t clients[MAX_CLIENTS];
  int num_clients = 0;
  num_clients = _swire.getClients(clients);

  for (int i = 0; i < num_clients; i++)
  {
    if (!_swire.sendData(clients[i], msg))
    {
      err++;
    }
  }
  return (err == 0);
}

int KTANEController::sendStrikes()
{
  int num_strikes = getStrikes();
  char msg[3] = {NUM_STRIKES, (char)num_strikes, '\0'};
  int err = 0;
  uint8_t clients[MAX_CLIENTS];
  int num_clients = 0;

  if (num_strikes > 0)
  {
    num_clients = _swire.getClients(clients);

    for (int i = 0; i < num_clients; i++)
    {
      if (!_swire.sendData(clients[i], msg))
      {
        err++;
      }
    }
    return (err == 0);
  }
  return 0;
}

int KTANEController::sendSolves()
{
  int num_solves = getSolves() - _needy_modules;
  char msg[3] = {NUM_SOLVES, (char)num_solves, '\0'};
  int err = 0;
  uint8_t clients[MAX_CLIENTS];
  int num_clients = 0;

  if (num_solves > 0)
  {
    num_clients = _swire.getClients(clients);

    for (int i = 0; i < num_clients; i++)
    {
      if (!_swire.sendData(clients[i], msg))
      {
        err++;
      }
    }
    return (err == 0);
  }
  return 0;
}

int KTANEController::stopNeedys()
{
  char msg[2] = {STOP, '\0'};
  int err = 0;
  uint8_t clients[MAX_CLIENTS];
  int num_clients = 0;
  num_clients = _swire.getClients(clients);

  for (int i = 0; i < num_clients; i++)
  {
    for (int j = 0; j < 3; j++)
    {
      if (clients[i] == needys[j])
      {
        if (!_swire.sendData(clients[i], msg))
        {
          err++;
        }
      }
    }
    return (err == 0);
  }
  return 0;
}