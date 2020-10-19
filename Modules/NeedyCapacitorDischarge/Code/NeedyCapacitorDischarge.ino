#include "KTANECommon.h"
#include "LedControl.h"
#include "Adafruit_LiquidCrystal.h"

// Defines
//  Pins
//RX 0
//TX 1
#define LEVER_BUTTON 2
#define GREEN_CLEAR_LED 6
#define RED_STRIKE_LED 7
#define DATA_IN_PIN 8
#define CLOCK_PIN 9
#define LOAD_PIN 10
//I2C SDA 18
//I2C SCL 19

SWireClient client(0x07);
KTANEModule module(client, GREEN_CLEAR_LED, RED_STRIKE_LED);
LedControl lc = LedControl(DATA_IN_PIN, CLOCK_PIN, LOAD_PIN);

unsigned long timer = 0;
unsigned long forcedTimer = 0;
unsigned long updateTimer = 0;
int timeLeft;
bool moduleActive = false;
int solves = 0;
int strikes = 0;
int numEvents = 0;
bool btnState;

void setup()
{
    Serial.begin(19200);

    Serial.println("Starting setup");
    pinMode(LEVER_BUTTON, INPUT_PULLUP);

    Serial.println("Preparing 7-segment");
    lc.shutdown(0, false);
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
        btnState = digitalRead(LEVER_BUTTON);

        if (!moduleActive)
        {
            if (millis() >= forcedTimer)
            {
                timer = millis() + (40 * 1000); //Add 40 seconds for countdown
                moduleActive = true;
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
            if (millis() >= timer)
            {
                module.strike();
                lc.shutdown(0, true);
                delayWithUpdates(module, random(10, 40));
                strikes = module.getNumStrikes();
                solves = module.getNumSolves();
                timer = millis() + (90 * 1000); //Add 90 seconds for forced start
                moduleActive = false;
            }

            if (millis() - updateTimer > 500)
            {
                updateTimer = millis();

                if (btnState)
                {
                    timer += 1000; //should this be 500?
                    if(timer - millis() > (40 * 1000))
                    {
                        timer = millis() + (40 * 1000);
                    }
                }
                else
                {
                    updateCountdown();
                }
            }
        }
    }
}

void setGraph(int timeLeft)
{
    int howManyToLight = 40 - timeLeft;

    for (int i = 2; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            lc.setLed(0, i, j, howManyToLight-- > 0);
        }
    }
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
            timer = millis() + (40 * 1000); //Add 40 seconds for countdown
            moduleActive = true;
        }
    }
    else
    {
        numEvents = 0;
        timer = millis() + (40 * 1000); //Add 40 seconds for countdown
        moduleActive = true;
    }
}

void updateCountdown()
{
    timeLeft = (timer - millis()) / 1000;
    setGraph(timeLeft);
    lc.setDigit(0, 0, timeLeft / 10, false);
    lc.setDigit(0, 1, timeLeft % 10, false);
}