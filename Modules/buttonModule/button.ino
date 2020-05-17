#include "DSerial.h"
#include "KTANECommon.h"
#include "NeoICSerial.h"

// Defines
//Green clear Led 2
//Red strike Led 3
#define BUTTON_PIN 4
//NeoICSerial RX-pin 8
//NeoICSerial TX-pin 9
#define CLOCK_PIN 11
#define LOAD_PIN 12
#define DATA_IN_PIN 13

#define BUTTON_LED_PIN_GREEN 14
#define BUTTON_LED_PIN_BLUE 15
#define BUTTON_LED_PIN_RED 16
#define STRIP_LED_PIN_GREEN 17
#define STRIP_LED_PIN_BLUE 18
#define STRIP_LED_PIN_RED 19

#define DISP_SINGLE(x, y) maxSingle((x), (y), LOAD_PIN, CLOCK_PIN, DATA_IN_PIN)

NeoICSerial serial_port;
DSerialClient client(serial_port, MY_ADDRESS);
KTANEModule module(client, 2, 3);

int buttonColor;
int stripColor;
int text;
bool stripOn = false;
bool wasButtonHeld = false;

int buttonState = 0;     // current state of the button
int lastButtonState = 0; // previous state of the button
int timer;

byte max7219_reg_decodeMode = 0x09;
byte max7219_reg_intensity = 0x0a;
byte max7219_reg_scanLimit = 0x0b;
byte max7219_reg_shutdown = 0x0c;
byte max7219_reg_displayTest = 0x0f;

int colors[5][2] = {
    {0, 4}, //Blue
    {1, 1}, //Red
    {2, 1}, //White
    {3, 5}, //Yellow
    {4, 0}  //Transparent (buttons only, not  coloured strips)
};

int rgbValues[5][3] = {
    {0, 0, 255},        //Blue
    {255, 0, 0},        //Red
    {255, 255, 255},    //White
    {255, 255, 0},      //Yellow
    {0, 0, 0}           //Transparent (buttons only, not  coloured strips)
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
    analogWrite(BUTTON_LED_PIN_RED, rgbValues[buttonColor][0]);
    analogWrite(BUTTON_LED_PIN_GREEN, rgbValues[buttonColor][1]);
    analogWrite(BUTTON_LED_PIN_BLUE, rgbValues[buttonColor][2]);
}

void enableStrip()
{
    analogWrite(STRIP_LED_PIN_RED, rgbValues[stripColor][0]);
    analogWrite(STRIP_LED_PIN_GREEN, rgbValues[stripColor][1]);
    analogWrite(STRIP_LED_PIN_BLUE, rgbValues[stripColor][2]);
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
    serial_port.begin(19200);
    Serial.begin(19200);

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

    DISP_SINGLE(max7219_reg_scanLimit, 0x07);
    DISP_SINGLE(max7219_reg_decodeMode, 0x00);  // using an led matrix (not digits)
    DISP_SINGLE(max7219_reg_shutdown, 0x01);    // not in shutdown mode
    DISP_SINGLE(max7219_reg_displayTest, 0x00); // no display test

    while (!module.getConfig())
    {
        module.interpretData();
    }

    randomSeed(config_to_seed(module.getConfig()));

    // Generate button
    generateButton();

    // Show text
    DISP_SINGLE(0, constants[text]);

    timer = millis();

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