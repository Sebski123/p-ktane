#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>
#include <BLE2902.h>
#include <Preferences.h>
#include "KTANECommon.h"

Preferences preferences;

// https://www.uuidgenerator.net/

#define DEVICENAME "P-Ktane ESP32"

#define SEND "46a3c400-8dd9-4bdc-9898-a954947c5802"
#define SEND_SEED "ce7082a7-c061-477d-a04c-4d6e60126f3c"
#define SEND_TIME "d4e642e4-9830-4c86-9965-c13eed553e6a"
#define SEND_STRIKES "a8fac7f4-0a71-445e-92fc-3b53a4136447"
#define SEND_TIMELEFT "069cd0b7-d8fc-465f-820f-097ada211ed8"
#define SEND_SUBMIT "7381bae4-aa40-4d1c-ba82-6977250574cf"

#define RECIVE "8ea3ef8d-db01-4328-be0b-0f403dd5ac8d"
#define RECIVE_SEED "d1f387bf-fbe4-4f0b-ab79-3e545de4488f"
#define RECIVE_TIME "89b753e7-ff02-4ce9-95f7-221e24596d48"
#define RECIVE_STRIKES "607e6006-eb85-4738-8d59-a1ce1defd7fc"

union
{
  float f;
  char a[4];
} u;

bool deviceConnected = false;

BLEServer *btServer;

BLEService *sRecive;
BLEService *sSend;

BLECharacteristic *sReciveSeed;
BLECharacteristic *sReciveTime;
BLECharacteristic *sReciveStrikes;

BLECharacteristic *sSendSeed;
BLECharacteristic *sSendTime;
BLECharacteristic *sSendStrikes;
BLECharacteristic *sSendTimeLeft;
BLECharacteristic *sSendSubmit;

uint32_t cwrite = BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_WRITE;
uint32_t cnotify = BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_WRITE | BLECharacteristic::PROPERTY_NOTIFY | BLECharacteristic::PROPERTY_INDICATE;

BLEAdvertising *pAdvertising;

settings_t stored_settings;
int num_minutes;
float timeLeft;

float strToFloat(std::string str)
{
  const char *encoded = str.c_str();
  //memcpy(u.a, encoded, 4);
  u.a[0] = encoded[3];
  u.a[1] = encoded[2];
  u.a[2] = encoded[1];
  u.a[3] = encoded[0];

  return u.f;
}

int strToInt(std::string str)
{
  const char *encoded = str.c_str();
  return (int(encoded[1]) << 8) + int(encoded[0]);
}

class ConnectionServerCallbacks : public BLEServerCallbacks
{
  void onConnect(BLEServer *pServer)
  {
    deviceConnected = true;
    Serial.println("BLE Connected");
  };

  void onDisconnect(BLEServer *pServer)
  {
    deviceConnected = false;
    Serial.println("BLE Disconnected");
  }
};

class ReciveSeed : public BLECharacteristicCallbacks
{
  void onWrite(BLECharacteristic *pCharacteristic)
  {
    int rint = strToInt(pCharacteristic->getValue());
    stored_settings.seed = rint;
    preferences.begin("pktane");
    preferences.putInt("seed", rint);
    preferences.end();
    sSendSeed->setValue(rint);
    Serial.print("Recived seed: ");
    Serial.println(stored_settings.seed);
  }
};

class ReciveTime : public BLECharacteristicCallbacks
{
  void onWrite(BLECharacteristic *pCharacteristic)
  {
    float rfloat = strToFloat(pCharacteristic->getValue());
    stored_settings.time = rfloat;
    preferences.begin("pktane");
    preferences.putFloat("time", rfloat);
    preferences.end();
    sSendTime->setValue(rfloat);
    Serial.print("Recived time: ");
    Serial.println(stored_settings.time);
  }
};

class ReciveStrikes : public BLECharacteristicCallbacks
{
  void onWrite(BLECharacteristic *pCharacteristic)
  {
    int rint = strToInt(pCharacteristic->getValue());
    stored_settings.strikes = rint;
    preferences.begin("pktane");
    preferences.putInt("strikes", rint);
    preferences.end();
    sSendStrikes->setValue(rint);
    Serial.print("Recived strikes: ");
    Serial.println(stored_settings.strikes);
  }
};

void submitTime(bool won)
{
  Serial.print("Seed: ");
  Serial.println(stored_settings.seed);
  sSendSeed->setValue(stored_settings.seed);
  sSendSeed->notify();

  Serial.print("Time: ");
  Serial.println(stored_settings.time, 5);
  sSendTime->setValue(stored_settings.time);
  sSendTime->notify();

  Serial.print("Strikes: ");
  Serial.println(stored_settings.strikes);
  uint16_t s = (uint16_t)stored_settings.strikes; //need to cast as setValue doesn't accept uint8_t
  sSendStrikes->setValue(s);
  sSendStrikes->notify();

  Serial.print("TimeLeft: ");
  Serial.println(timeLeft, 5);
  sSendTimeLeft->setValue(timeLeft);
  sSendTimeLeft->notify();

  Serial.print("Result: ");
  Serial.println(won ? "winner" : (timeLeft <= 0.0 ? "timedOut" : "strikedOut"));
  sSendSubmit->setValue(won ? "winner" : (timeLeft <= 0.0 ? "timedOut" : "strikedOut"));
  sSendSubmit->notify();
}

void setup(void)
{
  Serial.begin(19200);
  Serial2.begin(19200); //To communicate with timer

  Serial.println("Loading settings from flash");

  preferences.begin("pktane");

  stored_settings.seed = preferences.getInt("seed", 0);
  stored_settings.time = preferences.getFloat("time", 5);
  stored_settings.strikes = preferences.getInt("strikes", 3);
  preferences.end();

  Serial.println("Setting up BLE");
  Serial.print("Device Name:");
  Serial.println(DEVICENAME);

  BLEDevice::init(DEVICENAME);
  btServer = BLEDevice::createServer();
  btServer->setCallbacks(new ConnectionServerCallbacks());

  sRecive = btServer->createService(RECIVE);

  sReciveSeed = sRecive->createCharacteristic(RECIVE_SEED, cwrite);
  sReciveSeed->setCallbacks(new ReciveSeed());

  sReciveTime = sRecive->createCharacteristic(RECIVE_TIME, cwrite);
  sReciveTime->setCallbacks(new ReciveTime());

  sReciveStrikes = sRecive->createCharacteristic(RECIVE_STRIKES, cwrite);
  sReciveStrikes->setCallbacks(new ReciveStrikes());

  sSend = btServer->createService(SEND);

  sSendSeed = sSend->createCharacteristic(SEND_SEED, cnotify);
  sSendSeed->addDescriptor(new BLE2902());
  sSendSeed->setValue(stored_settings.seed);

  sSendTime = sSend->createCharacteristic(SEND_TIME, cnotify);
  sSendTime->addDescriptor(new BLE2902());
  sSendTime->setValue(stored_settings.time);

  sSendStrikes = sSend->createCharacteristic(SEND_STRIKES, cnotify);
  sSendStrikes->addDescriptor(new BLE2902());
  uint16_t s = (uint16_t)stored_settings.strikes; //need to cast as setValue doesn't accept uint8_t
  sSendStrikes->setValue(s);

  sSendTimeLeft = sSend->createCharacteristic(SEND_TIMELEFT, cnotify);
  sSendTimeLeft->addDescriptor(new BLE2902());
  float tl = 0;
  sSendTimeLeft->setValue(tl);

  sSendSubmit = sSend->createCharacteristic(SEND_SUBMIT, cnotify);
  sSendSubmit->addDescriptor(new BLE2902());
  sSendSubmit->setValue("none");

  sRecive->start();
  sSend->start();

  pAdvertising = btServer->getAdvertising();
  pAdvertising->start();

  Serial.println("Setup done");
}

void loop(void)
{

  if (Serial2.available() > 0)
  {
    while (Serial2.available() > 0)
    {
      if (Serial2.read() == 'W')
      {
        while (Serial2.available() <= 0)
        {
          delay(1);
        }
        u.f = 0;
        Serial2.readBytes(u.a, 4);
        delay(5);
        bool won = (Serial2.read() == '1');
        timeLeft = u.f;

        submitTime(won);
      }
      // Throw away data
      Serial2.read();
    }
    u.f = stored_settings.time;
    Serial2.write(stored_settings.seed >> 8);
    Serial2.write(stored_settings.seed);
    Serial2.write(u.a, 4);
    Serial2.write(stored_settings.strikes);
  }
}
