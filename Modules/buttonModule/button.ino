#include "SWire.h"
#include "KTANECommon.h"
#include "NeoICSerial.h"
#include "LedControl.h"

// Defines
//  Pins
#define GREEN_CLEAR_LED 2
#define BUTTON_LED_PIN_GREEN 3
#define BUTTON_PIN 4
#define BUTTON_LED_PIN_BLUE 5
#define BUTTON_LED_PIN_RED 6
#define RED_STRIKE_LED 7
#define CLOCK_PIN 8
#define STRIP_LED_PIN_GREEN 9
#define STRIP_LED_PIN_BLUE 10
#define STRIP_LED_PIN_RED 11
#define LOAD_PIN 12
#define DATA_IN_PIN 13
//I2C SDA 18
//I2C SCL 19

//Class inits
SWireClient client(0x04);
KTANEModule module(client, GREEN_CLEAR_LED, RED_STRIKE_LED);

//Variables
int buttonColor;
int stripColor;
int text;
bool stripOn = false;
bool wasButtonHeld = false;

int buttonState = 0;     // current state of the button
int lastButtonState = 0; // previous state of the button
int timer;

int colors[5][2] = {
    {0, 4}, //Blue
    {1, 1}, //Red
    {2, 1}, //White
    {3, 5}, //Yellow
    {4, 0}  //Transparent (buttons only, not  coloured strips)
};

int rgbValues[5][3] = {
    {0, 0, 1}, //Blue
    {1, 0, 0}, //Red
    {1, 1, 1}, //White
    {1, 1, 0}, //Yellow
    {1, 0, 1}  //Purple (buttons only, not  coloured strips)
};

int constants[4] = {
    0b10111110, // 0    Abort
    0b00010010, // 1    Detonate
    0b11011100, // 2    Hold
    0b11011010  // 3    Press
};

char solutionCheckTime[5];

void generateButton()
{
    buttonColor = colors[random(5)][0];
    stripColor = colors[random(4)][0];
    text = random(4);
    digitalWrite(BUTTON_LED_PIN_RED, rgbValues[buttonColor][0]);
    digitalWrite(BUTTON_LED_PIN_GREEN, rgbValues[buttonColor][1]);
    digitalWrite(BUTTON_LED_PIN_BLUE, rgbValues[buttonColor][2]);
}

void enableStrip()
{
    digitalWrite(STRIP_LED_PIN_RED, rgbValues[stripColor][0]);
    digitalWrite(STRIP_LED_PIN_GREEN, rgbValues[stripColor][1]);
    digitalWrite(STRIP_LED_PIN_BLUE, rgbValues[stripColor][2]);
    stripOn = true;
}

void disableStrip()
{
    digitalWrite(STRIP_LED_PIN_GREEN, LOW);
    digitalWrite(STRIP_LED_PIN_BLUE, LOW);
    digitalWrite(STRIP_LED_PIN_RED, LOW);
    stripOn = false;
}

void checkSolution(bool held)
{
    if (!held)
    {
        if (text == 1 || (text == 2 && buttonColor == 1))
        {
            module.win();
            return;
        }
        else
        {
            module.strike();
            return;
        }
    }
    else
    {
        if (!(text == 1 || (text == 2 && buttonColor == 1)))
        {
            module.sendTime();
            delay(100);
            memcpy(solutionCheckTime, module.getTime(), 5);
            if (!!((int)strchr(solutionCheckTime, colors[stripColor][1] + '0')))
            {
                module.win();
                return;
            }
        }
        module.strike();
        wasButtonHeld = false;
        stripColor = colors[random(4)][0];
        disableStrip();
        return;
    }
}

void setup()
{
    Serial.begin(19200);

    Serial.println("Starting setup");

    pinMode(DATA_IN_PIN, OUTPUT);
    pinMode(LOAD_PIN, OUTPUT);
    pinMode(CLOCK_PIN, OUTPUT);
    pinMode(BUTTON_PIN, INPUT);
    pinMode(BUTTON_LED_PIN_GREEN, OUTPUT);
    pinMode(BUTTON_LED_PIN_BLUE, OUTPUT);
    pinMode(BUTTON_LED_PIN_RED, OUTPUT);
    pinMode(STRIP_LED_PIN_GREEN, OUTPUT);
    pinMode(STRIP_LED_PIN_BLUE, OUTPUT);
    pinMode(STRIP_LED_PIN_RED, OUTPUT);

    Serial.println("Preparing 7-segment");
    lc.shutdown(0, false);
    /* Set the brightness to a medium values */
    lc.setIntensity(0, 8);
    /* and clear the display */
    lc.clearDisplay(0);

    Serial.println("Getting config");
    /*while (!module.getConfig())
    {
        module.interpretData();
    }*/

    Serial.println("Got config");

    randomSeed(1234); //config_to_seed(module.getConfig()));

    Serial.println("Generating button");
    // Generate button
    generateButton();

    Serial.print("Button color ");
    Serial.println(buttonColor);
    Serial.print("Strip color ");
    Serial.println(stripColor);

    Serial.print("Showing text ");
    Serial.println(text);
    // Show text
    lc.setDigit(0, 0, text, false);

    timer = millis();

    Serial.println("Finished setup");

    module.sendReady();
}

void loop()
{
    module.interpretData();

    if (!module.is_solved)
    {
        // read the pushbutton input pin:
        buttonState = digitalRead(BUTTON_PIN);

        if (buttonState && !stripOn && millis() - timer > 1000)
        {
            enableStrip();
            wasButtonHeld = true;
        }

        // compare the buttonState to its previous state
        if (buttonState != lastButtonState)
        {
            timer = millis();

            if (buttonState == LOW)
            {
                // if the current state is LOW then the button went from on to off:
                checkSolution(wasButtonHeld);
            }
            // Delay a little bit to avoid bouncing
            delay(50);
        }

        // save the current state as the last state, for next time through the loop
        lastButtonState = buttonState;
    }
}