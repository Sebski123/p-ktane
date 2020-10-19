#include "KTANECommon.h"
#include "LedControl.h"
#include "Adafruit_MCP3008.h"

//    Needs testing
// Resistor values  =   ?,    ?,    ?,     ?
// Wire colors      = White, Blue, Red, Blue+Red
// Wire int         =   1     2     3      4
// 0 indicates no wire

// Defines
#define WHITE 1
#define BLUE 2
#define RED 3
#define BLUExRED 4

//  Pins
//RX 0
//TX 1
#define ADC_CLK 2
#define ADC_DOUT 3
#define ADC_DIN 4
#define ADC_CS 5
#define GREEN_CLEAR_LED 6
#define RED_STRIKE_LED 7
#define DATA_IN_PIN 8
#define CLOCK_PIN 9
#define LOAD_PIN 10
//I2C SDA 18
//I2C SCL 19

//Class inits
Adafruit_MCP3008 adc;
SWireClient client(0x03);
KTANEModule module(client, GREEN_CLEAR_LED, RED_STRIKE_LED);
LedControl lc = LedControl(DATA_IN_PIN, CLOCK_PIN, LOAD_PIN);

//Variables
char *debug_color[] = {"NULL", "White", "Blue", "Red", "BluexRed"};
int wires[6] = {0, 0, 0, 0, 0, 0};
bool leds[6] = {0, 0, 0, 0, 0, 0};
bool stars[6] = {0, 0, 0, 0, 0, 0};
bool wires_to_cut[6] = {0, 0, 0, 0, 0, 0};
int numOfWiresToCut = 0;
int wiresCut = 0;
int numOfBatteries;
bool hasParallelPort;
bool lastDigitIsEven;

// Adjust this
int voltageToWire(int voltage)
{
    if (voltage < 10)
    {
        return 0;
    }
    else if (voltage < 138)
    {
        return WHITE;
    }
    else if (voltage < 384)
    {
        return BLUE;
    }
    else if (voltage < 700)
    {
        return RED;
    }
    else
    {
        return BLUExRED;
    }
}

void setup()
{
    Serial.begin(19200);

    Serial.println("Begin setup");

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
    Serial.println(module.getConfig()->serial);
#pragma region Detect wires:
    adc.begin(ADC_CLK, ADC_DIN, ADC_DOUT, ADC_CS);

    Serial.print("Raw:\t");

    for (int i = 0; i < 6; i++)
    {
        int reading = adc.readADC(i);
        wires[i] = voltageToWire(reading);
        Serial.print(reading);
        Serial.print("\t");
        delay(10);
    }
#pragma endregion
#pragma region Init LEDs &Stars:
    for (int i = 0; i < 6; i++)
    {
        if (wires[i] > 0)
        {
            leds[i] = random(0, 2);
            stars[i] = random(0, 2);
            lc.setLed(0, 0, i, leds[i]);
            lc.setLed(0, 1, i, stars[i]);
        }
    }
#pragma endregion
    Serial.println();

    Serial.print("wires:\t");
    for (int i = 0; i < 6; i++)
    {
        Serial.print(wires[i]);
        Serial.print("\t");
    }
    Serial.println();

    Serial.print("Color:\t");
    for (int i = 0; i < 6; i++)
    {
        Serial.print(debug_color[wires[i]]);
        Serial.print("\t");
    }
    Serial.println();

    // Detect Solution:
    numOfBatteries = module.getNumBatteries();
    hasParallelPort = module.getParallelPort();
    lastDigitIsEven = module.getLastDigitEven();

    for (int i = 0; i < 6; i++)
    {
        if (wires[i] > 0)
        {
            if (leds[i])
            {
                if (stars[i])
                {
                    switch (wires[i])
                    {
                    case WHITE:
                    case RED:
                        wires_to_cut[i] = numOfBatteries >= 2;
                        break;
                    case BLUE:
                        wires_to_cut[i] = hasParallelPort;
                        break;
                    case BLUExRED:
                        break;
                    default:
                        //something went wrong
                        Serial.print("something went wrong: 1, 1, ");
                        Serial.print(wires[i]);
                        Serial.println(i);
                        break;
                    }
                }
                else
                {
                    switch (wires[i])
                    {
                    case WHITE:
                        break;
                    case RED:
                        wires_to_cut[i] = numOfBatteries >= 2;
                        break;
                    case BLUE:
                        wires_to_cut[i] = hasParallelPort;
                        break;
                    case BLUExRED:
                        wires_to_cut[i] = lastDigitIsEven;
                        break;
                    default:
                        //something went wrong
                        Serial.print("something went wrong: 1, 0, ");
                        Serial.print(wires[i]);
                        Serial.println(i);
                        break;
                    }
                }
            }
            else
            {
                if (stars[i])
                {
                    switch (wires[i])
                    {
                    case WHITE:
                    case RED:
                        wires_to_cut[i] = true;
                        break;
                    case BLUE:
                        break;
                    case BLUExRED:
                        wires_to_cut[i] = hasParallelPort;
                        break;
                    default:
                        //something went wrong
                        Serial.print("something went wrong: 0, 1, ");
                        Serial.print(wires[i]);
                        Serial.println(i);
                        break;
                    }
                }
                else
                {
                    switch (wires[i])
                    {
                    case WHITE:
                        wires_to_cut[i] = true;
                        break;
                    case RED:
                    case BLUE:
                    case BLUExRED:
                        wires_to_cut[i] = lastDigitIsEven;
                        break;
                    default:
                        //something went wrong
                        Serial.print("something went wrong: 0, 0, ");
                        Serial.print(wires[i]);
                        Serial.println(i);
                        break;
                    }
                }
            }
        }
    }

    for (int i = 0; i < 6; i++)
    {
        numOfWiresToCut += wires_to_cut[i];
    }
    Serial.println("Done with setup");
    Serial.println(module.sendReady());
}

void loop()
{
    module.interpretData();

    if (!module.is_solved)
    {
        for (int i = 0; i < 6; i++)
        {
            if (wires[i] != 0)
            {
                if ((wires[i] != voltageToWire(adc.readADC(i)) && (adc.readADC(i) < 10)))
                {
                    delayWithUpdates(module, 100);
                    if ((wires[i] != voltageToWire(adc.readADC(i)) && (adc.readADC(i) < 10)))
                    { // Check again for debouncing reasons
                        if (!wires_to_cut[i])
                        {
                            module.strike();
                            Serial.println("Strike");
                            wires[i] = voltageToWire(adc.readADC(i));
                        }
                        else
                        {
                            wiresCut++;
                            if (wiresCut == numOfWiresToCut)
                            {
                                module.win();
                                Serial.println("WIN");
                            }
                        }
                    }
                }
                delayWithUpdates(module, 10);
            }
        }
    }
}