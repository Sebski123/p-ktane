#include "SWire.h"
#include "KTANECommon.h"
#include "LedControl.h"
#include "Bounce2.h"
#include "FastLED.h"
#include "maze.h"

/*                                                                                                                            
                                                    dddddddd                                                                 
UUUUUUUU     UUUUUUUU                               d::::::d                          tttt                                   
U::::::U     U::::::U                               d::::::d                       ttt:::t                                   
U::::::U     U::::::U                               d::::::d                       t:::::t                                   
UU:::::U     U:::::UU                               d:::::d                        t:::::t                                   
 U:::::U     U:::::Uppppp   ppppppppp       ddddddddd:::::d   aaaaaaaaaaaaa  ttttttt:::::ttttttt        eeeeeeeeeeee         
 U:::::D     D:::::Up::::ppp:::::::::p    dd::::::::::::::d   a::::::::::::a t:::::::::::::::::t      ee::::::::::::ee       
 U:::::D     D:::::Up:::::::::::::::::p  d::::::::::::::::d   aaaaaaaaa:::::at:::::::::::::::::t     e::::::eeeee:::::ee     
 U:::::D     D:::::Upp::::::ppppp::::::pd:::::::ddddd:::::d            a::::atttttt:::::::tttttt    e::::::e     e:::::e     
 U:::::D     D:::::U p:::::p     p:::::pd::::::d    d:::::d     aaaaaaa:::::a      t:::::t          e:::::::eeeee::::::e     
 U:::::D     D:::::U p:::::p     p:::::pd:::::d     d:::::d   aa::::::::::::a      t:::::t          e:::::::::::::::::e      
 U:::::D     D:::::U p:::::p     p:::::pd:::::d     d:::::d  a::::aaaa::::::a      t:::::t          e::::::eeeeeeeeeee       
 U::::::U   U::::::U p:::::p    p::::::pd:::::d     d:::::d a::::a    a:::::a      t:::::t    tttttte:::::::e                
 U:::::::UUU:::::::U p:::::ppppp:::::::pd::::::ddddd::::::dda::::a    a:::::a      t::::::tttt:::::te::::::::e               
  UU:::::::::::::UU  p::::::::::::::::p  d:::::::::::::::::da:::::aaaa::::::a      tt::::::::::::::t e::::::::eeeeeeee       
    UU:::::::::UU    p::::::::::::::pp    d:::::::::ddd::::d a::::::::::aa:::a       tt:::::::::::tt  ee:::::::::::::e       
      UUUUUUUUU      p::::::pppppppp       ddddddddd   ddddd  aaaaaaaaaa  aaaa         ttttttttttt      eeeeeeeeeeeeee       
                     p:::::p                                                                                                 
                     p:::::p                                                                                                 
                    p:::::::p                                                                                                
                    p:::::::p                                                                                                
                    p:::::::p                                                                                                
                    ppppppppp      
		See https://github.com/SyberxSpace/KTANE_IRL/tree/master/Modules/01_Vanilla/10_Mazes/MazeTest
*/

// Defines
//  Pins
#define RIGHT_BTN 2
#define UP_BTN 3
#define DOWN_BTN 4
#define LEFT_BTN 5
#define GREEN_CLEAR_LED 6
#define RED_STRIKE_LED 7
#define CLOCK_PIN 8
#define DATA_PIN 9
//I2C SDA 18
//I2C SCL 19

#define NUM_GRID_LEDS 144
#define debounceTime 20

//Class inits
SWireClient client(0x07);
KTANEModule module(client, GREEN_CLEAR_LED, RED_STRIKE_LED);
Bounce up_db = Bounce();
Bounce left_db = Bounce();
Bounce down_db = Bounce();
Bounce right_db = Bounce();
CRGB dark = CRGB(4, 4, 8);        //color of unoccupied spaces
CRGB white = CRGB(255, 255, 255); //color of player position
CRGB red = CRGB(255, 0, 0);       //color of walls
CRGB darkred = CRGB(25, 0, 0);    //color of debug walls
CRGB circle = CRGB(0, 255, 0);    //color of circle positions during setup
CRGB pulse = CRGB(0, 0, 0);       //color of goal position (placeholder)

//Variables
CRGB leds[NUM_GRID_LEDS];
bool upState = false;
bool upStateLast = false;
bool leftState = false;
bool leftStateLast = false;
bool downState = false;
bool downStateLast = false;
bool rightState = false;
bool rightStateLast = false;
byte maze[11][11];
byte mazeNum;
byte playerLocation[2];
byte goalLocation[2];

const int maxPulse = 255; //max brightness of goal pulse
const int minPulse = 100; //min brightness of goal pulse

void updateButtons()
{
    //update debouncing
    up_db.update();
    left_db.update();
    down_db.update();
    right_db.update();

    //update button states
    upStateLast = upState;
    upState = !up_db.read();
    leftStateLast = leftState;
    leftState = !left_db.read();
    downStateLast = downState;
    downState = !down_db.read();
    rightStateLast = rightState;
    rightState = !right_db.read();
}

/*
bool: 0 = x-axis, 1 = y-axis
dir:  0 = down, 1 = up 
*/
void move()
{
    if (upState > upStateLast)
    {
        //check if move is in bounds
        if (playerLocation[1] > 0)
        {
            //check if move is valid
            if (maze[playerLocation[1] * 2 - 1][playerLocation[0] * 2] == 1)
            {
                //strike if it isn't
                module.strike();
                leds[mazeMap[playerLocation[1] * 2 - 1][playerLocation[0] * 2]] = red;
            }
            else
            {
                leds[mainDotsMap[playerLocation[1]][playerLocation[0]]] = dark;
                playerLocation[1] = playerLocation[1] - 1; //move if it is
            }
        }
    }

    if (leftState > leftStateLast)
    {
        //check if move is in bounds
        if (playerLocation[0] > 0)
        {
            //check if move is valid
            if (maze[playerLocation[1] * 2][playerLocation[0] * 2 - 1] == 1)
            { //strike if it isn't
                module.strike();
                leds[mazeMap[playerLocation[1] * 2][playerLocation[0] * 2 - 1]] = red;
            }
            else
            {
                leds[mainDotsMap[playerLocation[1]][playerLocation[0]]] = dark;
                playerLocation[0] = playerLocation[0] - 1; //move if it is
            }
        }
    }

    if (downState > downStateLast)
    {
        //check if move is in bounds
        if (playerLocation[1] < 5)
        {
            //check if move is valid
            if (maze[playerLocation[1] * 2 + 1][playerLocation[0] * 2] == 1)
            { //strike if it isn't
                module.strike();
                leds[mazeMap[playerLocation[1] * 2 + 1][playerLocation[0] * 2]] = red;
            }
            else
            {
                leds[mainDotsMap[playerLocation[1]][playerLocation[0]]] = dark;
                playerLocation[1] = playerLocation[1] + 1; //move if it is
            }
        }
    }

    if (rightState > rightStateLast)
    {
        //check if move is in bounds
        if (playerLocation[0] < 5)
        {
            //check if move is valid
            if (maze[playerLocation[1] * 2][playerLocation[0] * 2 + 1] == 1)
            { //strike if it isn't
                module.strike();
                leds[mazeMap[playerLocation[1] * 2][playerLocation[0] * 2 + 1]] = red;
            }
            else
            {
                leds[mainDotsMap[playerLocation[1]][playerLocation[0]]] = dark;
                playerLocation[0] = playerLocation[0] + 1; //move if it is
            }
        }
    }

    //update the lights
    leds[mainDotsMap[playerLocation[1]][playerLocation[0]]] = white;
    leds[mainDotsMap[goalLocation[0]][goalLocation[1]]] = pulse;

    FastLED.show();
}

void setup()
{
    Serial.begin(19200);

    Serial.println("### Maze Module ###");

    Serial.println(F("Getting config"));
    while (!module.getConfig())
    {
        module.interpretData();
    }
    Serial.println(F("Got config"));

    Serial.println(F("Setting random seed"));
    randomSeed(config_to_seed(module.getConfig()));

    Serial.println(F("Setting up I/O-pins"));
    // 8x8 Led-matrix setup
    pinMode(DATA_PIN, OUTPUT);
    pinMode(CLOCK_PIN, OUTPUT);

    //Directional-buttons setup
    pinMode(UP_BTN, INPUT_PULLUP);
    pinMode(DOWN_BTN, INPUT_PULLUP);
    pinMode(LEFT_BTN, INPUT_PULLUP);
    pinMode(RIGHT_BTN, INPUT_PULLUP);

    //Attatch debounce objects
    right_db.attach(RIGHT_BTN);
    right_db.interval(debounceTime);
    up_db.attach(UP_BTN);
    up_db.interval(debounceTime);
    down_db.attach(DOWN_BTN);
    down_db.interval(debounceTime);
    left_db.attach(LEFT_BTN);
    left_db.interval(debounceTime);

    FastLED.addLeds<APA102, DATA_PIN, CLOCK_PIN, GRB>(leds, NUM_GRID_LEDS);

    Serial.println(F("Choosing maze"));
    mazeNum = random(0, 9);
    Serial.print(F("Choose maze nr: "));
    Serial.println(mazeNum);

    Serial.println(F("Choosing player location"));
    playerLocation[0] = random(0, 6);
    playerLocation[1] = random(0, 6);
    Serial.print(F("Player location: "));
    Serial.print(playerLocation[0]);
    Serial.print("\t");
    Serial.println(playerLocation[1]);

    Serial.println(F("Choosing goal location"));
    do
    {
        goalLocation[0] = random(0, 6);
        goalLocation[1] = random(0, 6);
    } while (
        ((goalLocation[0] == playerLocation[0]) && (goalLocation[1] == playerLocation[1])) ||
        (abs((goalLocation[0] - playerLocation[0]) + (goalLocation[1] - playerLocation[1])) < 5));

    Serial.print(F("Goal location: "));
    Serial.print(goalLocation[0]);
    Serial.print("\t");
    Serial.println(goalLocation[1]);

    //fill in the maze matrix from storage
    for (int i = 0; i < 11; i++)
    {
        for (int j = 0; j < 11; j++)
        {
            maze[i][j] = mazes[mazeNum][i][j];
        }
    }

    //clear matrix
    for (int i = 0; i < NUM_GRID_LEDS; i++)
    {
        leds[i] = CRGB(0, 0, 0);
    }

    //display circle positions and maze (if in DEBUG)
    for (int i = 0; i < 11; i++)
    {
        for (int j = 0; j < 11; j++)
        {
            if (maze[i][j] == 2)
            {
                leds[mazeMap[i][j]] = dark;
            }
            if (maze[i][j] == 4)
            {
                leds[mazeMap[i][j]] = circle;
            }
#ifdef DEBUG
            if (maze[i][j] == 1)
            {
                leds[mazeMap[i][j]] = darkred;
            }
#endif
        }
    }
    FastLED.show();

    Serial.println(F("Setup done"));
    module.sendReady();
}

void loop()
{
    module.interpretData();

    if (!module.is_solved)
    {
        updateButtons();

        //update pulse animation
        pulse = CRGB(map(quadwave8(millis() / (20) * PI), 0, 255, minPulse, maxPulse), 0, 0);

        move();

        //Check if player reached the goal
        if ((playerLocation[0] == goalLocation[0]) && (playerLocation[1] == goalLocation[1]))
        {
            module.win();
            Serial.println(F("Win"));
        }
    }
}