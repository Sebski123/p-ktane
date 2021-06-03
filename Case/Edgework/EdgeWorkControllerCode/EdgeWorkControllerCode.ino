#include "KTANECommon.h"
#include <GxEPD2_3C.h>
#include <U8g2_for_Adafruit_GFX.h>
#include <Adafruit_MCP3008.h>
#include <GxEPD2_MH29EP.h>

#define ENABLE_GxEPD2_GFX 0
#define MAX_DISPLAY_BUFFER_SIZE 400
#define MAX_HEIGHT(EPD) (EPD::HEIGHT <= (MAX_DISPLAY_BUFFER_SIZE / 2) / (EPD::WIDTH / 8) ? EPD::HEIGHT : (MAX_DISPLAY_BUFFER_SIZE / 2) / (EPD::WIDTH / 8))

/*
ADC val     0-20    20-100    100-180   180-260   260-340   340-420   420-500   500-580   580-660   660-740   740-820   820-900   900-1023
Res val     47      620       1k54      2k7       4k12      5k9       8k2       11k      15k       21k5      33k       53k6      open
Indicator   SND     CLR       CAR       IND       FRQ       SIG       NSA       MSA       TRN       BOB       FRK       ---       ---
Equivalent  Batt1   Batt2     P1        P2        P3        P4        P5        P6        P7        P8        P9        P10       ---
Char 2      0       1         2         3         4         5         6         7         8         9         A         B         X

P1  =   Serial
P2  =   Parallel
P3  =   Serial + Parallel
P4  =   DVI
P5  =   PS2
P6  =   RJ45
P7  =   DVI + PS2
P8  =   DVI + RJ45
P9  =   DVI + PS2 + RJ45
P10 =   PS2 + RJ45
*/

// Defines
//  Pins
//RX 0
//TX 1
#define WIDGET_5_LED 2
#define WIDGET_4_LED 3
#define WIDGET_3_LED 4
#define WIDGET_2_LED 5
#define WIDGET_1_LED 6
#define IND_TEST 7
#define EDP_DC 8
#define EDP_RST 9
#define EDP_CS 10
#define SPI_MOSI 11
#define SPI_MISO 12
#define SPI_SCK 13
#define ADC_CS 14
#define EPD_BUSY 15
//I2C SDA 18
//I2C SCL 19

SWireClient client(1); // ID 1
KTANEModule module(client);
GxEPD2_3C<GxEPD2_MH29EP, MAX_HEIGHT(GxEPD2_MH29EP)> display(GxEPD2_MH29EP(EDP_CS, EDP_DC, EDP_RST, EPD_BUSY));
U8G2_FOR_ADAFRUIT_GFX u8g2Fonts;
Adafruit_MCP3008 adc;

const uint16_t colorWhite = GxEPD_WHITE;
const uint16_t colorBlack = GxEPD_BLACK;
const uint16_t colorRed = GxEPD_RED;

int widgetLeds[] = {WIDGET_1_LED, WIDGET_2_LED, WIDGET_3_LED, WIDGET_4_LED, WIDGET_5_LED};
char serialNumber[] = "XXXXXX";
char widgets[5][3] = {"XX", "XX", "XX", "XX", "XX"};
char litLeds = 0;
int numBatteries = 0;
bool litCAR = false;
bool litFRK = false;
bool hasParallelPort = false;

char randomAlpha()
{
  byte randomValue = random(0, 25);
  char symbol = randomValue + 'A';
  if (randomValue == 14)
  {
    symbol = 'E';
  }
  if (randomValue == 24)
  {
    symbol = 'Z';
  }
  return symbol;
}

char randomNumeric()
{
  byte randomValue = random(0, 10);
  char symbol = randomValue + '0';
  return symbol;
}

char randomAlphaNumeric()
{
  byte randomValue = random(0, 35);
  char symbol = randomValue + 'A';
  if (randomValue == 14)
  {
    symbol = 'E';
  }
  if (randomValue == 24)
  {
    symbol = 'Z';
  }
  if (randomValue >= 25)
  {
    symbol = (randomValue - 25) + '0';
  }
  return symbol;
}

void generateSerial()
{
  serialNumber[0] = randomAlphaNumeric();
  serialNumber[1] = randomAlphaNumeric();
  serialNumber[2] = randomNumeric();
  serialNumber[3] = randomAlpha();
  serialNumber[4] = randomAlpha();
  serialNumber[5] = randomNumeric();
}

void initEPD()
{
  display.setRotation(2);
  display.init();
  u8g2Fonts.begin(display);
  u8g2Fonts.setFontMode(1);
  u8g2Fonts.setFontDirection(1);
}

void writeSerialEPD()
{
  display.firstPage();
  do
  {
    display.fillScreen(colorWhite);
    u8g2Fonts.setForegroundColor(colorBlack);
    u8g2Fonts.setBackgroundColor(colorWhite);
    //u8g2Fonts.setFont(u8g2_font_inb53_mf);
    u8g2Fonts.setFont(u8g2_font_AnonPro_tf);
    u8g2Fonts.setCursor(16, 148 - (u8g2Fonts.getUTF8Width(serialNumber) / 2));
    u8g2Fonts.print(serialNumber);
    display.fillRect(76, 0, 52, 296, colorRed);
    u8g2Fonts.setForegroundColor(colorWhite);
    u8g2Fonts.setBackgroundColor(colorRed);
    u8g2Fonts.setFont(u8g2_font_LibSans_tf);
    u8g2Fonts.setCursor(90, 148 - (u8g2Fonts.getUTF8Width("SERIAL #") / 2));
    u8g2Fonts.print(F("SERI"));
    u8g2Fonts.setCursor(90, 154);
    u8g2Fonts.print(F("AL #"));
  } while (display.nextPage());
}

void identifyWidget(char idx, int adcVal, bool ledVal)
{

  char *widget = widgets[idx];

  if (adcVal < 95)
  {
    widget[0] = 'B';
  }
  else
  {
    widget[0] = 'P';
  }

  if (adcVal < 20)
  {
    widget[1] = '0';
    if (!ledVal)
    {
      numBatteries += 1;
    }
  }
  else if (adcVal < 100)
  {
    widget[1] = '1';
    if (!ledVal)
    {
      numBatteries += 2;
    }
  }
  else if (adcVal < 180)
  {
    widget[1] = '2';
  }
  else if (adcVal < 260)
  {
    widget[1] = '3';
    if (!ledVal)
    {
      hasParallelPort = true;
    }
  }
  else if (adcVal < 340)
  {
    widget[1] = '4';
    if (!ledVal)
    {
      hasParallelPort = true;
    }
  }
  else if (adcVal < 420)
  {
    widget[1] = '5';
  }
  else if (adcVal < 500)
  {
    widget[1] = '6';
  }
  else if (adcVal < 580)
  {
    widget[1] = '7';
  }
  else if (adcVal < 660)
  {
    widget[1] = '8';
  }
  else if (adcVal < 740)
  {
    widget[1] = '9';
  }
  else if (adcVal < 820)
  {
    widget[1] = 'A';
  }
  else if (adcVal < 900)
  {
    widget[1] = 'B';
  }
  else
  {
    widget[0] = 'X';
    widget[1] = 'X';
  }
  if (ledVal)
  {
    widget[0] = 'I';
  }
}

void detectWidgets()
{
  pinMode(IND_TEST, OUTPUT);
  digitalWrite(IND_TEST, HIGH);
  for (char i = 0; i < sizeof(widgetLeds) / sizeof(widgetLeds[0]); i++)
  {
    int adcVal = adc.readADC(i);
    bool ledVal = digitalRead(widgetLeds[i]);
    // Serial.println(adcVal);
    // Serial.print("\t");
    // Serial.println(ledVal);

    identifyWidget(i, adcVal, ledVal);
    if (ledVal)
    {
      if (random(10) > 4)
      {
        if (widgets[i][1] == '2')
        {
          litCAR = true;
        }
        else if (widgets[i][1] == 'A')
        {
          litFRK = true;
        }

        pinMode(widgetLeds[i], OUTPUT);
        digitalWrite(widgetLeds[i], HIGH);
        litLeds |= 1 << (7 - i);
      }
    }
  }
  pinMode(IND_TEST, INPUT);
}

void sendConfig()
{
  config_t config;

  config.batteries = 4;                //numBatteries;
  config.indicators = 3;               //litFRK | (litCAR << 1);
  config.ports = 1;                    //hasParallelPort;
  strcpy(config.serial, serialNumber); //serialNumber);

  module.sendConfig(&config);
}

void setup()
{
  Serial.begin(19200);
  while (!Serial)
  {
    ;
  } // wait for serial port to connect. Needed for native USB

  Serial.println(F("Getting Seed"));

  while (!module.getSeed())
  {
    module.interpretData();
  }

  Serial.print(F("Seed is: "));
  Serial.println(module.getSeed());

  randomSeed(module.getSeed());

  Serial.println(F("Generating Serial #"));
  generateSerial();
  Serial.print(F("Serial is "));
  Serial.println(serialNumber);

  adc.begin(ADC_CS);
  Serial.println(F("Detecting widgets"));
  detectWidgets();
  Serial.println(F("Widgets are:"));
  for (auto &&i : widgets)
  {
    Serial.print(i);
    Serial.print("\t");
  }
  Serial.println();

  sendConfig();
  Serial.println(F("Sent config:"));
  Serial.println(F("Displaying Serial #"));
  //initEPD();
  //writeSerialEPD();
  // display.clearScreen();
  // display.hibernate();

  Serial.println(F("Setup done"));

  module.sendReady();
  delay(50);
  module.sendSolve();
}

void loop()
{
  module.interpretData();
}