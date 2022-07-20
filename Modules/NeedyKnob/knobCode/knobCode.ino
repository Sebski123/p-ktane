#include "KTANECommon.h"
#include "needyknob.h"
#include "LedControl.h"

//  Pins
//RX 0
//TX 1
#define KNOB_UP 2
#define KNOB_RIGHT 3
#define KNOB_DOWN 4
#define KNOB_LEFT 5
#define GREEN_CLEAR_LED 6
#define RED_STRIKE_LED 7
#define DATA_IN_PIN 8
#define CLOCK_PIN 9
#define LOAD_PIN 10
//I2C SDA 18
//I2C SCL 19

SWireClient client(0x08);
KTANEModule module(client, GREEN_CLEAR_LED, RED_STRIKE_LED);
LedControl lc = LedControl(DATA_IN_PIN, CLOCK_PIN, LOAD_PIN);

int currentSolution;
unsigned long timer = 0;
unsigned long forcedTimer = 0;
unsigned long updateTimer = 0;
int timeLeft;
bool moduleActive = false;
int solves = 0;
int strikes = 0;
int numEvents = 0;
bool blinkState = false;

void setup()
{
    Serial.begin(19200);

    Serial.println("### Knob Module ###");
    Serial.println("Starting setup");

    pinMode(KNOB_UP, INPUT_PULLUP);
    pinMode(KNOB_RIGHT, INPUT_PULLUP);
    pinMode(KNOB_DOWN, INPUT_PULLUP);
    pinMode(KNOB_LEFT, INPUT_PULLUP);

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

    Serial.println("Initializing leds");
    generateLeds();

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
            if (millis() >= timer)
            {
                if (!(
                        (!digitalRead(KNOB_UP) && currentSolution == 0) ||
                        (!digitalRead(KNOB_DOWN) && currentSolution == 1) ||
                        (!digitalRead(KNOB_LEFT) && currentSolution == 2) ||
                        (!digitalRead(KNOB_RIGHT) && currentSolution == 3)))
                {
                    module.strike();
                }
                lc.shutdown(0, true);
                generateLeds();
                delayWithUpdates(module, random(10, 40));
                strikes = module.getNumStrikes();
                solves = module.getNumSolves();
                forcedTimer = millis() + (90 * 1000); //Add 90 seconds for forced start
                moduleActive = false;
            }

            if ((millis() - timer < 5000) && (millis() % 200 == 0))
            {
                Serial.println("Fast blink");
                lc.shutdown(0, blinkState);
                blinkState = !blinkState;
            }

            if (millis() - updateTimer > 500)
            {
                updateTimer = millis();
                updateCountdown();
            }
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
    lc.shutdown(0, false);
}

void updateCountdown()
{
    timeLeft = (timer - millis()) / 1000;
    lc.setDigit(0, 0, timeLeft / 10, false);
    lc.setDigit(0, 1, timeLeft % 10, false);
}

void generateLeds()
{
    int solutionIdx = random(8);
    currentSolution = solutionIdx / 2;
    lc.setRow(0, 2, positions[solutionIdx / 2][solutionIdx % 2][0]);
    lc.setRow(0, 3, positions[solutionIdx / 2][solutionIdx % 2][1]);
}