typedef struct config_st
{
    unsigned int ports : 3;
    unsigned int batteries : 3;
    unsigned int indicators : 2;
    char serial[7];
} config_t;

config_t config;

#define W_WHITE 1
#define W_BLUE 2
#define W_YELLOW 3
#define W_BLACK 4
#define W_RED 5

char serial[7] = "TESTI1";
int numOfBatts = 0;
bool Parallel = false;
bool RJ45 = false;
bool Stereo = false;

int wireOrder[6] = {W_WHITE, W_BLUE, W_YELLOW, W_BLACK, W_RED, 0};

void getMazesSolution();
void getMemorySolution();
void getSimonSolution();
void getButtonSolution();
void getWiresSolution();
int relLastColorIndex(int color);
int firstWireIndex();
int lastWireIndex();
unsigned long config_to_seed(config_t *config);
int serialContainsVowel();
uint8_t getIndexFromNumber(uint8_t *buttons, uint8_t num);

void setup()
{
    Serial.begin(19200);
    strncpy(config.serial, serial, 6);
    config.serial[6] = '\0';
    config.batteries = 0;
    config.ports = (Parallel | (RJ45 << 1) | (Stereo << 2));

    randomSeed(config_to_seed(&config));

    config.indicators |= random(2);
    config.indicators |= (random(2) << 1);

    randomSeed(config_to_seed(&config));
    getWiresSolution();

    randomSeed(config_to_seed(&config));
    getButtonSolution();

    randomSeed(config_to_seed(&config));
    getSimonSolution();

    randomSeed(config_to_seed(&config));
    getMemorySolution();

    randomSeed(config_to_seed(&config));
    getMazesSolution();
}

void loop()
{
}

unsigned long config_to_seed(config_t *config)
{
    unsigned long retval = 0;
    int i;

    for (i = 5; i >= 0; i--)
    {
      
        retval = (retval << 6) + (config->serial[i] - ' ');
    }
    retval = retval * (3 + config->ports);
    retval = retval * (5 + config->batteries);
    retval = retval * (7 + config->indicators);
    return retval;
}

int lastWireIndex()
{
    for (int i = 5; i >= 0; i++)
    {
        if (wireOrder[i] != 0)
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
        if (wireOrder[i] != 0)
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
        if (wireOrder[i] != 0)
        {
            if (wireOrder[i] == color)
            {
                retIndex = index;
            }
            index++;
        }
    }
    return retIndex;
}

void getWiresSolution()
{

    Serial.print("To solve Wires module you need to ");

#define IS_ODD(x) ((x)&1)
    int num_wires = 0;
    int wire_to_cut;
    int color_count[6] = {0, 0, 0, 0, 0, 0};
    for (int i = 0; i < 6; i++)
    {
        if (wireOrder[i])
        {
            num_wires++;
            color_count[wireOrder[i]]++;
        }
    }

    // Detect Solution:
    switch (num_wires)
    {

    case (3):
        if (color_count[W_RED] == 0)
        {
            wire_to_cut = 2; // Second wire
        }
        else if (wireOrder[lastWireIndex()] == W_WHITE)
        {
            wire_to_cut = num_wires; // Last wire
        }
        else if (color_count[W_BLUE] > 1)
        {
            wire_to_cut = relLastColorIndex(W_BLUE) + 1; // Last blue wire
        }
        else
        {
            wire_to_cut = num_wires; // Last wire
        }
        break;

    case (4):
        if (color_count[W_RED] > 1 && IS_ODD(config.serial[5]))
        {
            wire_to_cut = relLastColorIndex(W_RED) + 1; // Last red wire
        }
        else if (wireOrder[lastWireIndex()] == W_YELLOW && color_count[W_RED] == 0)
        {
            wire_to_cut = 1; // First wire
        }
        else if (color_count[W_BLUE] == 1)
        {
            wire_to_cut = 1; // First wire
        }
        else if (color_count[W_YELLOW] > 1)
        {
            wire_to_cut = num_wires; // Last wire
        }
        else
        {
            wire_to_cut = 2; // Second wire
        }
        break;

    case (5):
        if (wireOrder[lastWireIndex()] == W_BLACK && IS_ODD(config.serial[5]))
        {
            wire_to_cut = 4; // Fourth wire
        }
        else if (color_count[W_RED] == 1 && color_count[W_YELLOW] > 1)
        {
            wire_to_cut = 1; // First wire
        }
        else if (color_count[W_BLACK] == 0)
        {
            wire_to_cut = 2; // Second wire
        }
        else
        {
            wire_to_cut = 1; // First wire
        }
        break;

    case (6):
        if (color_count[W_YELLOW] == 0 && IS_ODD(config.serial[5]))
        {
            wire_to_cut = 3; // Third wire
        }
        else if (color_count[W_YELLOW] == 1 && color_count[W_WHITE] > 1)
        {
            wire_to_cut = 4; // Fourth wire
        }
        else if (color_count[W_RED] == 0)
        {
            wire_to_cut = num_wires; // Last wire
        }
        else
        {
            wire_to_cut = 4; // Fourth wire
        }
        break;

    default:
        break;
    }

    int temp = wire_to_cut;

    Serial.print("cut wire no. ");
    Serial.println(wire_to_cut);
}

void getButtonSolution()
{
    const int ABORT = 0;
    const int DETONATE = 1;
    const int HOLD = 2;
    const int PRESS = 3;

    const int B_BLUE = 0;
    const int B_RED = 1;
    const int B_WHITE = 2;
    const int B_YELLOW = 3;

    Serial.print("To solve Button module you need to ");

    int colors[5][2] = {
        {0, 4}, //Blue
        {1, 1}, //Red
        {2, 1}, //White
        {3, 5}, //Yellow
        {4, 0}  //Transparent (button only, not  coloured strips)
    };
  
    int buttonColor = colors[random(5)][0];
    int stripColor = colors[random(4)][0];
    int text = random(4);


    if (text == DETONATE || (text == HOLD && buttonColor == B_RED))
    {
        Serial.println("press and immediately release the button");
    }
    else
    {
        Serial.print("hold the button and release it when the countdown timer has a ");
        switch (stripColor)
        {
        case B_BLUE:
            Serial.print(4);
            break;
        case B_YELLOW:
            Serial.print(5);
            break;
        default:
            Serial.print(1);
            break;
        }
        Serial.println(" in any position");
    }
}

int serialContainsVowel()
{
    int serial_length = strlen(config.serial);
    char temp;
    for (int i = 0; i < serial_length; i++)
    {
        temp = config.serial[i];
        if (temp == 'A' || temp == 'E' || temp == 'I' || temp == 'O' || temp == 'U')
        {
            return 1;
        }
    }
    return 0;
}

void getSimonSolution()
{
    Serial.println("To solve Simon Says module you need to:");

    const int MAX_NUM_STAGES = 5;
    const int RED = 1;
    const int YELLOW = 2;
    const int GREEN = 3;
    const int BLUE = 4;
    char *dbgColors[5] = {"Error", "Red", "Yellow", "Green", "Blue"};
    int num_stages = random(3, MAX_NUM_STAGES + 1);
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
    int stage_colors[MAX_NUM_STAGES];
    for (int i = 0; i < num_stages; i++)
    {
        stage_colors[i] = random(0, 4);
    }
    int vowel = serialContainsVowel();
    for (int strike = 0; strike < 3; strike++)
    {
        Serial.print("\tFor ");
        Serial.print(strike);
        Serial.print(" strike(s): ");
        for (int stage = 0; stage < num_stages; stage++)
        {
            Serial.print(dbgColors[mapping[vowel][strike][stage_colors[stage]]]);
            Serial.print("\t");
        }
        Serial.println();
    }
}

uint8_t getIndexFromNumber(uint8_t *buttons, uint8_t num)
{
    for (int i = 0; i < 4; i++)
    {
        if (buttons[i] == num)
        {
            return i;
        }
    }
    return 255;
}

void getMemorySolution()
{
    Serial.print("To solve Memory module you need to ");

    uint8_t bottom_nums[5][4];
    uint8_t top_nums[5];
    uint8_t buttons_to_press[5];
    int r1, r2;
    uint8_t temp;
  	int count = 0;
    for (int i = 0; i < 5; i++)
    {
        //Insert 1-4 in each row
        bottom_nums[i][0] = 1;
        bottom_nums[i][1] = 2;
        bottom_nums[i][2] = 3;
        bottom_nums[i][3] = 4;

        //Swap random numbers in row 20 times to scramble them
        for (int j = 0; j < 20; j++)
        {	count+=2;
            r1 = random(0, 4);
            r2 = random(0, 4);
            temp = bottom_nums[i][r1];
            bottom_nums[i][r1] = bottom_nums[i][r2];
            bottom_nums[i][r2] = temp;
        }
		count++;
        top_nums[i] = random(1, 5);
    }
    switch (top_nums[0])
    {
    case (1):
        buttons_to_press[0] = 1; // Second Position
        break;
    case (2):
        buttons_to_press[0] = 1; // Second Position
        break;
    case (3):
        buttons_to_press[0] = 2; // Third Position
        break;
    case (4):
        buttons_to_press[0] = 3; // Fourth Position
        break;
    }
    switch (top_nums[1])
    {
    case (1):
        // Button labeled 4
        buttons_to_press[1] = getIndexFromNumber(bottom_nums[1], 4);
        break;
    case (2):
        buttons_to_press[1] = buttons_to_press[0]; // Same position as stage 1
        break;
    case (3):
        buttons_to_press[1] = 0; // First Position
        break;
    case (4):
        buttons_to_press[1] = buttons_to_press[0]; // Same position as stage 1
        break;
    }
    switch (top_nums[2])
    {
    case (1):
        // Same label as stage 2
        buttons_to_press[2] = getIndexFromNumber(bottom_nums[2], bottom_nums[1][buttons_to_press[1]]);
        break;
    case (2):
        // Same label as stage 1
        buttons_to_press[2] = getIndexFromNumber(bottom_nums[2], bottom_nums[0][buttons_to_press[0]]);
        break;
    case (3):
        buttons_to_press[2] = 2; // Third Position
        break;
    case (4):
        // Button labeled 4
        buttons_to_press[2] = getIndexFromNumber(bottom_nums[2], 4);
        break;
    }
    switch (top_nums[3])
    {
    case (1):
        buttons_to_press[3] = buttons_to_press[0]; // Same position as stage 1
        break;
    case (2):
        buttons_to_press[3] = 0; // First Position
        break;
    case (3):
        buttons_to_press[3] = buttons_to_press[1]; // Same position as stage 2
        break;
    case (4):
        buttons_to_press[3] = buttons_to_press[1]; // Same position as stage 2
        break;
    }
    switch (top_nums[4])
    {
    case (1):
        // Same label as stage 1
        buttons_to_press[4] = getIndexFromNumber(bottom_nums[4], bottom_nums[0][buttons_to_press[0]]);
        break;
    case (2):
        // Same label as stage 2
        buttons_to_press[4] = getIndexFromNumber(bottom_nums[4], bottom_nums[1][buttons_to_press[1]]);
        break;
    case (3):
        // Same label as stage 4
        buttons_to_press[4] = getIndexFromNumber(bottom_nums[4], bottom_nums[3][buttons_to_press[3]]);
        break;
    case (4):
        // Same label as stage 3
        buttons_to_press[4] = getIndexFromNumber(bottom_nums[4], bottom_nums[2][buttons_to_press[2]]);
        break;
    }
  
    Serial.print("press: ");
    for (int i = 0; i < 5; i++)
    {
        Serial.print(buttons_to_press[i] + 1);
        Serial.print(" ");
    }
    Serial.println();
}

void getMazesSolution()
{
    /*
    Not implemented yet
    Could use https://github.com/SMFSW/Queue and then copy python solver
    */
}