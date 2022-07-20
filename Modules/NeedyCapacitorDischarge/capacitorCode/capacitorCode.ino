#include "KTANECommon.h"
#include "LedControl.h"

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

unsigned long deltaTime = 0;
unsigned long previousMillis = 0;
long timeRemaining = 0;
long forcedTimer = 0;
int secondsLeft;
bool moduleActive = false;
int solves = 0;
int strikes = 0;
int numEvents = 0;
bool btnState;

void setup()
{
    Serial.begin(19200);

    while (!Serial)
    {
        ; // wait for serial port to connect. Needed for native USB
    }
    Serial.println("### Capacitor Discharge Module ###");
    Serial.println("Starting setup");
    pinMode(LEVER_BUTTON, INPUT_PULLUP);

    Serial.println("Preparing 7-segment");
    lc.shutdown(0, false);
    /* Set the brightness to a medium values */
    lc.setIntensity(0, 8);
    /* and clear the display */
    lc.clearDisplay(0);
    lc.shutdown(0, true); //put MAX7219 in shutdown mode untill module is activated

    Serial.println("Getting config");
    while (!module.getConfig())
    {
        module.interpretData();
    }

    Serial.println("Got config");
    randomSeed(config_to_seed(module.getConfig()));

    forcedTimer = (long)90 * 1000; //Add 90 seconds for forced start
    previousMillis = millis();

    Serial.println("Finished setup");
    module.sendReady();

    module.sendSolve(); //Send solve immediately as it is a needy module
}

void loop()
{
    module.interpretData();

    if (!module.needyStop)
    {

        deltaTime = millis() - previousMillis;
        previousMillis = millis();

        btnState = !digitalRead(LEVER_BUTTON);

        if (!moduleActive)
        {

            forcedTimer -= deltaTime;

            if (forcedTimer <= 0)
            {
                timeRemaining = 40L * 1000; //Add 40 seconds for countdown
                moduleActive = true;
                lc.shutdown(0, false);
                Serial.println("Activating by forced timer");
            }

            if (module.getNumStrikes() > strikes || module.getNumSolves() > solves)
            {
                eventHappened();
            }
        }
        else
        {
            timeRemaining -= deltaTime;

            if (btnState)
            {
                timeRemaining += (long)deltaTime * 5L; //time since last check + 5 times increase time
                if (timeRemaining > 40L * 1000)
                {
                    timeRemaining = 40L * 1000;
                }
            }

            if (timeRemaining <= 0)
            {
                Serial.println("strike");
                lc.shutdown(0, true);
                forcedTimer = (long)(90 * 1000); //Add 90 seconds for forced start
                moduleActive = false;
                module.strike();
                delayWithUpdates(module, 40); //delay as to not get triggered by it's own strike
                strikes = module.getNumStrikes();
                solves = module.getNumSolves();
            }

            updateCountdown();
        }
    }
}

void setGraph()
{
    int howManyToLight = 40 - secondsLeft;

    for (int digit = 6; digit > 1; digit--)
    {
        lc.setLed(0, digit, 0, howManyToLight-- > 0);
        for (int segment = 7; segment > 0; segment--)
        {
            lc.setLed(0, digit, segment, howManyToLight-- > 0);
        }
    }
}

void eventHappened()
{
    Serial.println("Event happened");

    strikes = module.getNumStrikes();
    solves = module.getNumSolves();

    numEvents++;

    if (numEvents == 1)
    {
        if (random(0, 101) > 85)
        {
            numEvents = 0;
            lc.shutdown(0, false);
            timeRemaining = 40L * 1000; //Add 40 seconds for countdown
            moduleActive = true;
            Serial.println("Activated by 1nd event");
        }
    }
    else
    {
        numEvents = 0;
        lc.shutdown(0, false);
        timeRemaining = 40L * 1000; //Add 40 seconds for countdown
        moduleActive = true;
        Serial.println("Activated by 2nd event");
    }
}

void updateCountdown()
{
    secondsLeft = timeRemaining / 1000;
    setGraph();
    lc.setDigit(0, 0, secondsLeft / 10, false);
    lc.setDigit(0, 1, secondsLeft % 10, false);
}