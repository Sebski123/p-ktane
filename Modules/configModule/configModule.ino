#include "FirebaseESP8266.h"
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <EEPROM.h>
#include "KTANECommon.h"
#include "configModule.h"

extern const char INDEX_HTML[];

// Fill in your WiFi router SSID and password
#define FIREBASE_HOST "p-ktane.firebaseio.com"
#define FIREBASE_AUTH "IOSc03Pc3bdDasJCHNqLeZueGVEVHrJgSFLerxui"
#define WIFI_SSID "KatjaKaj"
#define WIFI_PASSWORD "Sebastianqaz"
MDNSResponder mdns;

ESP8266WebServer server(80);
FirebaseData firebaseData;

raw_config_t stored_config;
int num_minutes;
char timeLeft[5] = {0, 0, 0, 0, '\0'};

void returnFail(String msg)
{
  server.sendHeader("Connection", "close");
  server.sendHeader("Access-Control-Allow-Origin", "*");
  server.send(500, "text/plain", msg + "\r\n");
}

void handleSubmit()
{
  config_t config;
  int addr = 0;

  if (!server.hasArg("serial_num") || !server.hasArg("num_minutes"))
  {
    return returnFail("BAD ARGS");
  }
  server.arg("serial_num").toCharArray(config.serial, 7);
  num_minutes = server.arg("num_minutes").toInt();
  config.batteries = server.arg("num_batteries").toInt();
  config.indicators = ((!!server.hasArg("port1")) | ((!!server.hasArg("port2")) << 1));
  config.ports = (!!server.hasArg("port3") | ((!!server.hasArg("port4")) << 1) | ((!!server.hasArg("port5")) << 2));
  config_to_raw(&config, &stored_config);

  for (int i = 0; i < 7; i++)
  {
    byte b = ((byte *)(&stored_config))[i];
    EEPROM.write(addr++, b);
  }
  // Write time
  EEPROM.write(addr++, (byte)(num_minutes));
  EEPROM.commit();

  server.send(200, "text/html", INDEX_HTML);
}

void handleRoot()
{
  if (server.hasArg("serial_num"))
  {
    handleSubmit();
  }
  else
  {
    server.send(200, "text/html", INDEX_HTML);
  }
}

void handleNotFound()
{
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i = 0; i < server.args(); i++)
  {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
}

void submitTime(char *time)
{
  char path[14];
  strcpy(path, "/Score/");
  memcpy(path + 7, stored_config.serial, 5);
  path[12] = stored_config.serial6 + '0';
  path[13] = '\0';

  float fTimeLeft = 0;
  fTimeLeft += (time[0] - '0') * 10;
  fTimeLeft += time[1] - '0';
  fTimeLeft += (time[2] - '0') / 10.0;
  fTimeLeft += (time[3] - '0') / 100.0;

  if (!Firebase.pushFloat(firebaseData, path, fTimeLeft))
  {
    Serial.print("Error in push, ");
    Serial.println(firebaseData.errorReason());
  }
}

void setup(void)
{
  Serial.begin(19200);

  EEPROM.begin(512);
  int addr = 0;
  for (int i = 0; i < 7; i++)
  {
    byte b = EEPROM.read(addr++);
    ((byte *)(&stored_config))[i] = b;
  }
  // Read time
  num_minutes = EEPROM.read(addr++);

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  //Serial.println("");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(WIFI_SSID);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  Firebase.reconnectWiFi(true);

  if (mdns.begin("ktane-setup", WiFi.localIP()))
  {
    Serial.println("MDNS responder started");
  }

  server.on("/", handleRoot);
  server.onNotFound(handleNotFound);

  server.begin();
  Serial.print("Connect to http://ktane-setup.local or http://");
  Serial.println(WiFi.localIP());

  // Add service to MDNS-SD
  mdns.addService("http", "tcp", 80);
}

void loop(void)
{
  mdns.update();

  server.handleClient();
  if (Serial.available() > 0)
  {
    while (Serial.available() > 0)
    {
      if (Serial.read() == 'W')
      {
        char c;
        for(int i = 0; i < 4; i++){
          while(!Serial.available() > 0){}
          timeLeft[i] = Serial.read();;
        }
        submitTime(timeLeft);
      }
      // Throw away data
      Serial.read();
    }
    Serial.write((uint8_t *)(&stored_config), 7);
    Serial.write(num_minutes);
  }
}
