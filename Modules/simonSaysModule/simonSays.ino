#include "SWire.h"
#include "KTANECommon.h"

// Defines
#define MAX_NUM_STAGES 5
#define RED 1
#define YELLOW 2
#define GREEN 3
#define BLUE 4

//  Pins
#define GREEN_CLEAR_LED 2
#define RED_STRIKE_LED 3
#define BUZZER 5
int led_pins[4] = {10, 11, 12, 13};
int button_pins[4] = {14, 15, 16, 17};
//I2C SDA 18
//I2C SCL 19

//Class inits
SWireClient client(0x05);
KTANEModule module(client, GREEN_CLEAR_LED, RED_STRIKE_LED);

//Variables
bool resetLightStage = false;
unsigned long last_button_action = 0;
int last_action_multiplier;
int button_state = 0;
int old_button_state = 0;
int button_stage = 0;
int stage;
int num_stages;
int stage_colors[MAX_NUM_STAGES];
int colorFrequency[4] = {
    880, //Red
    400, //Yellow
    450, //Green
    305  //Blue
};
int mapping[2][3][4] = {
    {
        // No Vowel
        {BLUE, RED, GREEN, YELLOW}, // No Strikes
        {RED, GREEN, YELLOW, BLUE}, // One Strike
        {YELLOW, RED, BLUE, GREEN}, // Two Strikes
    },
    {
        // Vowel
        {BLUE, GREEN, YELLOW, RED}, // No Strikes
        {YELLOW, RED, BLUE, GREEN}, // One Strike
        {GREEN, BLUE, YELLOW, RED}, // Two Strikes
    },
};

void stopLights(){
  noTone(BUZZER);
  for(int i = 0; i < 4; i++){
    digitalWrite(led_pins[i], HIGH);
  }
}

void update_lights()
{
  static unsigned long old_millis = 0;
  static int light_stage = 0;

  if(resetLightStage) {
    old_millis = 0;
    light_stage = 0;
    resetLightStage = false;
  }

  if (light_stage >= ((stage + 1) * 2))
  {
    if (millis() - old_millis > 700)
    {
      old_millis = millis();
      light_stage = 0;
    }
  }
  else
  {
    if ((light_stage % 2 == 0) && (millis() - old_millis > 300))
    {
      digitalWrite(led_pins[stage_colors[light_stage / 2]], LOW);
      if (stage > 0)
      {
        tone(BUZZER, colorFrequency[stage_colors[light_stage / 2]], 700);
      }
      old_millis = millis();
      light_stage++;
    }
    else if ((light_stage % 2 == 1) && (millis() - old_millis > 700))
    {
      digitalWrite(led_pins[stage_colors[light_stage / 2]], HIGH);
      old_millis = millis();
      light_stage++;
    }
  }
}

int get_button()
{
  int button_pressed = 0;
  if (!digitalRead(button_pins[0]))
  {
    button_pressed = RED;
    stopLights();
    tone(BUZZER, colorFrequency[RED - 1], 700);
  }
  else if (!digitalRead(button_pins[1]))
  {
    button_pressed = YELLOW;
    stopLights();
    tone(BUZZER, colorFrequency[YELLOW - 1], 700);
  }
  else if (!digitalRead(button_pins[2]))
  {
    button_pressed = GREEN;
    stopLights();
    tone(BUZZER, colorFrequency[GREEN - 1], 700);
  }
  else if (!digitalRead(button_pins[3]))
  {
    button_pressed = BLUE;
    stopLights();
    tone(BUZZER, colorFrequency[BLUE - 1], 700);
  }
  return button_pressed;
}

void setup()
{
  Serial.begin(19200);

  Serial.println("Begin setup");

  pinMode(button_pins[0], INPUT);
  pinMode(button_pins[1], INPUT);
  pinMode(button_pins[2], INPUT);
  pinMode(button_pins[3], INPUT);
  pinMode(led_pins[0], OUTPUT);
  pinMode(led_pins[1], OUTPUT);
  pinMode(led_pins[2], OUTPUT);
  pinMode(led_pins[3], OUTPUT);
  digitalWrite(led_pins[0], HIGH);
  digitalWrite(led_pins[1], HIGH);
  digitalWrite(led_pins[2], HIGH);
  digitalWrite(led_pins[3], HIGH);

  Serial.println("Getting config");

  while(!module.getConfig()){
    module.interpretData();
  }

  Serial.println("Got config");

  randomSeed(config_to_seed(module.getConfig()));
  num_stages = random(3, MAX_NUM_STAGES + 1);

  for (int i = 0; i < num_stages; i++)
  {
    stage_colors[i] = random(0, 4);
  }
  stage = 0;

  Serial.print("Num stages: ");
  Serial.println(num_stages);

  for (int i = 0; i < num_stages; i++)
  {
    Serial.print(stage_colors[i]);
    Serial.print("\t");
  }
  Serial.println();

  Serial.print("Vowel? ");
  Serial.println(module.serialContainsVowel());

  Serial.println("Done with setup");
  module.sendReady();
}

void loop()
{
  module.interpretData();

  if (!module.is_solved)
  {
    int vowel = module.serialContainsVowel();
    int strikes = module.getNumStrikes();
    update_lights();
    if (millis() - last_button_action > (10 * last_action_multiplier))
    {
      button_state = get_button();
      last_button_action = millis();
      last_action_multiplier = 1;
    }
    if (button_state != old_button_state)
    {
      old_button_state = button_state;

      if (button_state != 0)
      {
        resetLightStage = true;
        if (button_state == mapping[vowel][strikes][stage_colors[button_stage]])
        {
          if (button_stage == stage)
          {
            stage++;
            button_stage = 0;
            last_action_multiplier = 100;
          }
          else
          {
            button_stage++;
          }
          if (stage == num_stages)
          {
            module.win();
            Serial.println("win!");
          }
        }
        else
        {
          button_stage = 0;
          module.strike();
          Serial.println("strike");
        }
      }
    }
  }
}