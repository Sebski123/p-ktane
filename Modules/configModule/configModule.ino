/*
  TODO:
    - Get time for disarm
    - Send config/time to controller module
    - Reset controller module?
    - Communication options:
      - I2C
      - Normal Serial
      - DSerial
      - Shared memory?
*/

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include "KTANECommon.h"
#include <EEPROM.h>
#include "configModule.h"

extern const char INDEX_HTML[];

int led_pin = LED_BUILTIN;

// Fill in your WiFi router SSID and password
const char* ssid = "Sebastian";
const char* password = "12341234";
MDNSResponder mdns;

ESP8266WebServer server(80);

raw_config_t stored_config;
int num_minutes;



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

  if (!server.hasArg("serial_num") || !server.hasArg("num_batteries")) {
    return returnFail("BAD ARGS");
  }
  server.arg("serial_num").toCharArray(config.serial, 7);
  num_minutes = server.arg("num_minutes").toInt();
  config.batteries = server.arg("num_batteries").toInt();
  config.indicators = ((!!server.hasArg("port1")) || 
                       ((!!server.hasArg("port2")) << 1)
                      );
  config.ports = (!!server.hasArg("port3") || 
                  ((!!server.hasArg("port4")) << 1) ||
                  ((!!server.hasArg("port5")) << 2)
                 );
  config_to_raw(&config, &stored_config);

  for(int i = 0; i < 7; i++){
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
  if (server.hasArg("serial_num")) {
    handleSubmit();
  }
  else {
    server.send(200, "text/html", INDEX_HTML);
  }
}

void handleNotFound()
{
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET)?"GET":"POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i=0; i<server.args(); i++){
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
}

void setup(void)
{
  Serial.begin(19200);

  EEPROM.begin(512);
  int addr = 0;
  for(int i = 0; i < 7; i++){
    byte b = EEPROM.read(addr++);
    ((byte *)(&stored_config))[i] = b;
  }
  // Read time
  num_minutes = EEPROM.read(addr++);

  pinMode(led_pin,  OUTPUT);

  WiFi.begin(ssid, password);
  //Serial.println("");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  if (mdns.begin("ktane-setup", WiFi.localIP())) {
    //Serial.println("MDNS responder started");
  }

  server.on("/", handleRoot);
  server.onNotFound(handleNotFound);

  server.begin();
  //Serial.print("Connect to http://ktane-setup.local or http://");
  //Serial.println(WiFi.localIP());

  // Add service to MDNS-SD
  mdns.addService("http", "tcp", 80);
  
}

void loop(void)
{
  mdns.update();
  
  server.handleClient();
  if(Serial.available() > 0) {
    while(Serial.available() > 0){
      // Throw away data
      Serial.read();
    }
    Serial.write((uint8_t *)(&stored_config), 7);
    Serial.write(num_minutes);
  }
}
