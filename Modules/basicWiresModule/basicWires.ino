#include "SWire.h"
#include "KTANECommon.h"

SWireClient client(0x03);
KTANEModule module(client, 2, 3);

// Resistor values  = 22000,  3300,  1000,   330,  33 
// Wire colors      = White, Blue, Yellow, Black, Red
// Wire int         =   1     2       3      4     5
// 0 indicates no wire
#define WHITE 1
#define BLUE 2
#define YELLOW 3
#define BLACK 4
#define RED 5

char* debug_color[] = {"NULL", "White", "Blue", "Yellow", "Black", "Red"};
// Defines
//Pin header A0 
//Pin header A1     
//Pin header A2
//Pin header A3
//Pin header A4
//Pin header A5
//Green clear Led 2
//Red strike Led 3
//NeoICSerial RX-pin 8
//NeoICSerial TX-pin 9

int wires[6] = {0, 0, 0, 0, 0, 0};
int color_count[6] = {0, 0, 0, 0, 0, 0};
int wire_to_cut; // One indexed and relative
int cut_index;   // wire_to_cut but zero indexed and absolute
int num_wires = 0;

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
  else if (voltage < 640)
  {
    return YELLOW;
  }
  else if (voltage < 896)
  {
    return BLACK;
  }
  else
  {
    return RED;
  }
  return 0;
}

int lastWireIndex()
{
  for (int i = 5; i >= 0; i++)
  {
    if (wires[i] != 0)
    {
      return i;
    }
  }
  return 0;
}

int firstWireIndex()
{
  for (int i = 0; i < 6; i++)
  {
    if (wires[i] != 0)
    {
      return i;
    }
  }
  return 0;
}

int relLastColorIndex(int color)
{
  int index = 0;
  int retIndex = 0;
  for (int i = 0; i < 6; i++)
  {
    if (wires[i] != 0)
    {
      if (wires[i] == color)
      {
        retIndex = index;
      }
      index++;
    }
  }
  return retIndex;
}

void setup()
{
  Serial.begin(19200);

  Serial.println("Begin setup");

  #pragma region Detect wires:
  pinMode(14, INPUT);   //A0
  pinMode(15, INPUT);   //A1
  pinMode(16, INPUT);   //A2
  pinMode(17, INPUT);   //A3
  pinMode(18, INPUT);   //A4
  pinMode(19, INPUT);   //A5


  Serial.print("Raw:\t"); 

  for (int i = 0; i < 6; i++)
  {
    int reading = analogRead(i);
    wires[i] = voltageToWire(reading);
    Serial.print(reading);
    Serial.print("\t");
    color_count[wires[i]] = color_count[wires[i]] + 1;
    if (wires[i] != 0)
    {
      num_wires++;
    }
    delay(10);
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

  Serial.print("num_wires: ");
  Serial.println(num_wires);


  Serial.println("Getting config");

  while (!module.getConfig())
  {
    module.interpretData();
  }

  Serial.println("Got config");
  Serial.println(module.getConfig()->serial);

  // Detect Solution:
  switch (num_wires)
  {

  case (3):
    if (color_count[RED] == 0)
    {
      wire_to_cut = 2; // Second wire
    }
    else if (wires[lastWireIndex()] == WHITE)
    {
      wire_to_cut = num_wires; // Last wire
    }
    else if (color_count[BLUE] > 1)
    {
      wire_to_cut = relLastColorIndex(BLUE) + 1; // Last blue wire
    }
    else
    {
      wire_to_cut = num_wires; // Last wire
    }
    break;

  case (4):
    if (color_count[RED] > 1 && IS_ODD(module.getSerialDigit(5)))
    {
      wire_to_cut = relLastColorIndex(RED) + 1; // Last red wire
    }
    else if (wires[lastWireIndex()] == YELLOW && color_count[RED] == 0)
    {
      wire_to_cut = 1; // First wire
    }
    else if (color_count[BLUE] == 1)
    {
      wire_to_cut = 1; // First wire
    }
    else if (color_count[YELLOW] > 1)
    {
      wire_to_cut = num_wires; // Last wire
    }
    else
    {
      wire_to_cut = 2; // Second wire
    }
    break;

  case (5):
    if (wires[lastWireIndex()] == BLACK && IS_ODD(module.getSerialDigit(5)))
    {
      wire_to_cut = 4; // Fourth wire
    }
    else if (color_count[RED] == 1 && color_count[YELLOW] > 1)
    {
      wire_to_cut = 1; // First wire
    }
    else if (color_count[BLACK] == 0)
    {
      wire_to_cut = 2; // Second wire
    }
    else
    {
      wire_to_cut = 1; // First wire
    }
    break;

  case (6):
    if (color_count[YELLOW] == 0 && IS_ODD(module.getSerialDigit(5)))
    {
      wire_to_cut = 3; // Third wire
    }
    else if (color_count[YELLOW] == 1 && color_count[WHITE] > 1)
    {
      wire_to_cut = 4; // Fourth wire
    }
    else if (color_count[RED] == 0)
    {
      wire_to_cut = num_wires; // Last wire
    }
    else
    {
      wire_to_cut = 4; // Fourth wire
    }
    break;

  default:
    module.strike();
  }

  int temp = wire_to_cut;
  for (int i = 0; i < 6; i++)
  {
    if (wires[i] != 0)
    {
      temp--;
      if (temp == 0)
      {
        cut_index = i;
        break;
      }
    }
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
        if ((wires[i] != voltageToWire(analogRead(i)) && (analogRead(i) < 10)))
        {
          delayWithUpdates(module, 100);
          if ((wires[i] != voltageToWire(analogRead(i)) && (analogRead(i) < 10)))
          { // Check again for debouncing reasons
            if (i == cut_index)
            {
              module.win();
              Serial.println("WIN");
            }
            else
            {
              module.strike();
              Serial.println("Strike");
              wires[i] = voltageToWire(analogRead(i));
            }
          }
        }
        delayWithUpdates(module, 10);
      }
    }
  }
}