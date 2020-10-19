#include "KTANECommon.h"
#include "LedControl.h"
#include "Adafruit_LiquidCrystal.h"

// Defines
//  Pins
//RX 0
//TX 1
#define YES_BTN 2
#define NO_BTN 3
#define GREEN_CLEAR_LED 6
#define RED_STRIKE_LED 7
#define DATA_IN_PIN 8
#define CLOCK_PIN 9
#define LOAD_PIN 10
#define LCD_ENABLE 11
#define LCD_RS 12
#define LCD_D4 14
#define LCD_D5 15
#define LCD_D6 16
#define LCD_D7 17
//I2C SDA 18
//I2C SCL 19

SWireClient client(0x09);
KTANEModule module(client, GREEN_CLEAR_LED, RED_STRIKE_LED);
Adafruit_LiquidCrystal lcd(LCD_RS, LCD_ENABLE, LCD_D4, LCD_D5, LCD_D6, LCD_D7);
LedControl lc = LedControl(DATA_IN_PIN, CLOCK_PIN, LOAD_PIN);

unsigned long timer = 0;
unsigned long forcedTimer = 0;
unsigned long updateTimer = 0;
int timeLeft;
bool moduleActive = false;
int solves = 0;
int strikes = 0;
int numEvents = 0;
bool yesBtnState;
bool noBtnState;
bool textState; //0 = DETONATE?, 1 = VENT GAS?

void setup()
{
    Serial.begin(19200);

    Serial.println("Starting setup");

    Serial.println("Initializing display");
    lcd.begin(16, 2);
    lcd.noDisplay();

    Serial.println("Preparing 7-segment");
    lc.shutdown(0, true);
    /* Set the brightness to a medium values */
    lc.setIntensity(0, 8);
    /* and clear the display */
    lc.clearDisplay(0);

    Serial.println("Getting config");
    while (!module.getConfig())
    {
        module.interpretData();
    }

    Serial.println("Got config");
    randomSeed(config_to_seed(module.getConfig()));

    Serial.println("Initializing LCD");
    generateLCD();

    forcedTimer = millis() + (90 * 1000); //Add 90 seconds for forced start

    Serial.println("Finished setup");
    module.sendReady();

    module.sendSolve(); //Send solve immediately as it is a needy module
}

void loop()
{
    module.interpretData();

    if (!module.needyStop)
    {
        yesBtnState = !digitalRead(YES_BTN);
        noBtnState = !digitalRead(NO_BTN);

        if (!moduleActive)
        {
            if (millis() >= forcedTimer)
            {
                resetAndStart();
            }
            if (millis() - updateTimer > 100)
            {
                if (module.getNumStrikes() > strikes || module.getNumSolves() > solves)
                {
                    eventHappened();
                }
            }
        }

        if (moduleActive)
        {
            if (yesBtnState)
            {
                if (textState) //if text is 'VENT GAS?'
                {
                    slowPrintAnswer(1);
                    wait(false);
                }
                else //if text is 'DETONATE?'
                {
                    slowPrintAnswer(1);
                    wait(true);
                }
            }
            else if (noBtnState)
            {
                if (textState) //if text is 'VENT GAS?'
                {
                    slowPrintAnswer(0);
                    printVentWarning();
                }
                else //if text is 'DETONATE?'
                {
                    slowPrintAnswer(0);
                    wait(false);
                }
            }

            if (millis() >= timer)
            {
                wait(true);
            }

            if (millis() - updateTimer > 500)
            {
                updateTimer = millis();
                updateCountdown();
            }
        }
    }
}

void printVentWarning()
{
    lcd.setCursor(0, 0);
    lcd.print("VENT TO PREVENT");
    lcd.setCursor(0, 1);
    lcd.print("   EXPLOSIONS  ");
    for (size_t i = 0; i < 5; i++)
    {
        delayWithUpdates(module, 200);
        lcd.noDisplay();
        delayWithUpdates(module, 200);
        lcd.display();
    }
    lcd.print("   VENT GAS?   ");
    lcd.setCursor(0, 1);
    lcd.print("      Y/N      ");
}

void slowPrintAnswer(bool answer)
{
    lcd.setCursor(0, 1);
    lcd.print("               ");
    if (answer)
    {
        lcd.print("      Y        ");
        delayWithUpdates(module, 500);
        lcd.print("      YE       ");
        delayWithUpdates(module, 500);
        lcd.print("      YES      ");
        delayWithUpdates(module, 500);
    }
    else
    {
        lcd.print("       N       ");
        delayWithUpdates(module, 500);
        lcd.print("       NO      ");
        delayWithUpdates(module, 500);
    }
}

void wait(bool strike)
{
    if (strike)
    {
        module.strike();
    }
    lcd.noDisplay();
    lc.shutdown(0, true);
    delayWithUpdates(module, random(10, 40));
    strikes = module.getNumStrikes();
    solves = module.getNumSolves();
    forcedTimer = millis() + (90 * 1000); //Add 90 seconds for forced start
    moduleActive = false;
}

void eventHappened()
{
    strikes = module.getNumStrikes();
    solves = module.getNumSolves();

    numEvents++;

    if (numEvents == 1)
    {
        if (random(0, 101) > 85)
        {
            numEvents = 0;
            resetAndStart();
        }
    }
    else
    {
        numEvents = 0;
        resetAndStart();
    }
}

void resetAndStart()
{
    timer = millis() + (40 * 1000); //Add 40 seconds for countdown
    moduleActive = true;
    lcd.display();
    lc.shutdown(0, false);
}

void updateCountdown()
{
    timeLeft = (timer - millis()) / 1000;
    lc.setDigit(0, 0, timeLeft / 10, false);
    lc.setDigit(0, 1, timeLeft % 10, false);
}

void generateLCD()
{
    lcd.setCursor(0, 0);
    if (random(0, 10) == 0)
    {
        textState = false;
        lcd.print("   DETONATE?   ");
    }
    else
    {
        textState = true;
        lcd.print("   VENT GAS?   ");
    }
    lcd.setCursor(0, 1);
    lcd.print("      Y/N      ");
}