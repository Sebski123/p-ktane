#include "SWire.h"
#include "KTANECommon.h"
#include "morseCode.h"
#include "LedControl.h"
/*
#define DOT_TIME 300

char freqs[16][4] = {
    "505",
    "515",
    "522",
    "532",
    "535",
    "542",
    "545",
    "552",
    "555",
    "565",
    "572",
    "575",
    "582",
    "592",
    "595",
    "600"};

char words[16][7] = {
    "shell",
    "halls",
    "slick",
    "trick",
    "boxes",
    "leaks",
    "strobe",
    "bistro",
    "flick",
    "bombs",
    "break",
    "brick",
    "steak",
    "sting",
    "vector",
    "beats"};

char morse[26][5] = {
    ".-",   // a
    "-...", // b
    "-.-.", // c
    "-..",  // d (not used)
    ".",    // e
    "..-.", // f
    "--.",  // g
    "....", // h
    "..",   // i
    ".---", // j (not used)
    "-.-",  // k
    ".-..", // l
    "--",   // m
    "-.",   // n
    "---",  // o
    ".--.", // p (not used)
    "--.-", // q (not used)
    ".-.",  // r
    "...",  // s
    "-",    // t
    "..-",  // u (not used)
    "...-", // v
    ".--",  // w (not used)
    "-..-", // x
    "-.--", // y (not used)
    "--.."  // z (not used)
};
*/
// Defines
//  Pins
//RX 0
//TX 1
#define BUTTON_L_PIN 2
#define BUTTON_R_PIN 3
#define BUTTON_TX_PIN 4
#define GREEN_CLEAR_LED 6
#define RED_STRIKE_LED 7
#define DATA_IN_PIN 8
#define CLOCK_PIN 9
#define LOAD_PIN 10
#define MORSE_LED_PIN 11
#define MHZ_LIGHT_PIN 12
//I2C SDA 18
//I2C SCL 19

SWireClient client(0x06);
KTANEModule module(client, GREEN_CLEAR_LED, RED_STRIKE_LED);
LedControl lc = LedControl(DATA_IN_PIN, CLOCK_PIN, LOAD_PIN);

int goal_freq;
int selected_freq = 0;
uint8_t morse_bits[8];
int morse_index;
int morse_length;

unsigned long last_char_time = 0;

unsigned long last_button_time = 0;

//Sets single bit in byte array at given index
void setMorseBit(uint8_t *bits, int index, int val)
{
  uint8_t and_mask, or_mask;
  and_mask = ~(1 << (index % 8));       
  or_mask = ((!!val) << (index % 8));
  bits[index / 8] = (bits[index / 8] & and_mask) | or_mask;
}

//Gets single bit in byte array at given index
int getMorseBit(uint8_t *bits, int index)
{
  uint8_t mask;
  mask = (1 << (index % 8));
  return bits[index / 8] & mask;
}

void setup()
{
  Serial.begin(19200);

  while (!Serial)
  {
    ; // wait for serial port to connect. Needed for native USB
  }

  Serial.println("Beginning Setup");
  Serial.println("Setup 7-segments");
  /*
   The MAX72XX is in power-saving mode on startup,
   we have to do a wakeup call
   */
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
  randomSeed(config_to_seed(module.getConfig()));

  Serial.println("Initializing I/O");

  pinMode(BUTTON_L_PIN, INPUT_PULLUP);
  pinMode(BUTTON_R_PIN, INPUT_PULLUP);
  pinMode(BUTTON_TX_PIN, INPUT_PULLUP);
  pinMode(MORSE_LED_PIN, OUTPUT);
  pinMode(MHZ_LIGHT_PIN, OUTPUT);

  digitalWrite(MHZ_LIGHT_PIN, HIGH);

  Serial.println("Generating morse");

  morse_index = 0;
  goal_freq = random(0, 16);
  Serial.print("Choosen freq: ");
  Serial.println(freqs[goal_freq]);
  Serial.print("Choosen word: ");
  Serial.println(words[goal_freq]);

  for (unsigned int i = 0; i < strlen(words[goal_freq]); i++) //For each letter in choosen word
  {
    char *morse_desc = morse[words[goal_freq][i] - 'a'];  //Get morse representation
    for (unsigned int j = 0; j < strlen(morse_desc); j++) //For each char in the morse representation
    {
      if (morse_desc[j] == '.') //If it's a dot, add a single 'on' unit and one 'off' unit
      {
        setMorseBit(morse_bits, morse_index++, 1);
        setMorseBit(morse_bits, morse_index++, 0);
      }
      else if (morse_desc[j] == '-') //If it's a dash, add three 'on' units and one 'off' unit
      {
        setMorseBit(morse_bits, morse_index++, 1);
        setMorseBit(morse_bits, morse_index++, 1);
        setMorseBit(morse_bits, morse_index++, 1);
        setMorseBit(morse_bits, morse_index++, 0);
      }
    }
    setMorseBit(morse_bits, morse_index++, 0); //After each letter, add two additional 'off' units for a total of three
    setMorseBit(morse_bits, morse_index++, 0);
  }
  setMorseBit(morse_bits, morse_index++, 0);//After each word, add four additional 'off' units for a total of seven
  setMorseBit(morse_bits, morse_index++, 0);
  setMorseBit(morse_bits, morse_index++, 0);
  setMorseBit(morse_bits, morse_index++, 0);
  morse_length = morse_index;
  morse_index = 0;

  Serial.println("Setup done");

  module.sendReady();
}

void doMorse()
{
  if (millis() - last_char_time >= DOT_TIME)
  {
    last_char_time = millis();
    digitalWrite(MORSE_LED_PIN, getMorseBit(morse_bits, morse_index));
    morse_index = (morse_index + 1) % morse_length;
  }
}

void loop()
{
  module.interpretData();

  if (!module.is_solved && !module.needyStop)
  {
    doMorse();

    if (!digitalRead(BUTTON_L_PIN) && (millis() - last_button_time > 250)) //Detect left button press
    {
      last_button_time = millis();
      selected_freq--;
      if (selected_freq < 0)
      {
        selected_freq = 0;
      }
    }

    if (!digitalRead(BUTTON_R_PIN) && (millis() - last_button_time > 250))  //Detect right button press
    {
      last_button_time = millis();
      selected_freq++;
      if (selected_freq > 15)
      {
        selected_freq = 15;
      }
    }

    //Display current selected frequency
    lc.setDigit(0, 0, 3, true);
    lc.setDigit(0, 1, freqs[selected_freq][0] - '0', false);
    lc.setDigit(0, 2, freqs[selected_freq][1] - '0', false);
    lc.setDigit(0, 3, freqs[selected_freq][2] - '0', false);

    //Detect submit button and process result
    if (!digitalRead(BUTTON_TX_PIN))
    {
      if (selected_freq == goal_freq)
      {
        module.win();
      }
      else
      {
        digitalWrite(MORSE_LED_PIN, LOW);
        module.strike();
      }
    }
  }
}