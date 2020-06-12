from ArduinoRand import randomSeed, random, random2
from dataclasses import dataclass


@dataclass
class config_t:
    ports: int
    batteries: int
    indicators: float
    serial: str


config = config_t(0, 0, 0, "")

WHITE = 1
BLUE = 2
YELLOW = 3
BLACK = 4
RED = 5

serial = "TEXGS9" #"KS1TL2"
numOfBatts = 3
Parallel = False
RJ45 = False
Stereo = False

wireOrder = [WHITE, BLUE, YELLOW, BLACK, RED, 0]


def config_to_seed(config: config_t):
    retval = 0
    for i in range(5, -1, -1):
        retval = (retval << 6) + (ord(config.serial[i]) - ord(' '))
    retval = retval * (3 + config.ports)
    retval = retval * (5 + config.batteries)
    retval = retval * (7 + config.indicators)
    retval &= 0xffffffff
    return retval


def lastWireIndex():
    for i in range(5, -1, -1):
        if (wireOrder[i] != 0):
            return i
    return 0


def firstWireIndex():
    for i in range(6):
        if (wireOrder[i] != 0):
            return i
    return 0


def relLastColorIndex(color):
    index = 0
    retIndex = 0
    for i in range(6):
        if (wireOrder[i] != 0):
            if (wireOrder[i] == color):
                retIndex = index
            index += 1
    return retIndex


def IS_ODD(x):
    return (x % 2) == 1


def getWiresSolution():
    print("To solve Wires module you need to ", end="")

    num_wires = 0
    wire_to_cut = 0
    color_count = [0, 0, 0, 0, 0, 0]
    for i in range(6):
        if (wireOrder[i]):
            num_wires += 1
            color_count[wireOrder[i]] += 1

    if num_wires == 3:
        if (color_count[RED] == 0):
            wire_to_cut = 2  # Second wire
        elif (wireOrder[lastWireIndex()] == WHITE):
            wire_to_cut = num_wires  # Last wire
        elif (color_count[BLUE] > 1):
            wire_to_cut = relLastColorIndex(BLUE) + 1  # Last blue wire
        else:
            wire_to_cut = num_wires  # Last wire
    elif num_wires == 4:
        if (color_count[RED] > 1 and IS_ODD(config.serial[5])):
            wire_to_cut = relLastColorIndex(RED) + 1  # Last red wire
        elif (wireOrder[lastWireIndex()] == YELLOW and color_count[RED] == 0):
            wire_to_cut = 1  # First wire
        elif (color_count[BLUE] == 1):
            wire_to_cut = 1  # First wire
        elif (color_count[YELLOW] > 1):
            wire_to_cut = num_wires  # Last wire
        else:
            wire_to_cut = 2  # Second wire
    elif num_wires == 5:
        if (wireOrder[lastWireIndex()] == BLACK and IS_ODD(config.serial[5])):
            wire_to_cut = 4  # Fourth wire
        elif (color_count[RED] == 1 and color_count[YELLOW] > 1):
            wire_to_cut = 1  # First wire
        elif (color_count[BLACK] == 0):
            wire_to_cut = 2  # Second wire
        else:
            wire_to_cut = 1  # First wire
    elif num_wires == 6:
        if (color_count[YELLOW] == 0 and IS_ODD(config.serial[5])):
            wire_to_cut = 3  # Third wire
        elif (color_count[YELLOW] == 1 and color_count[WHITE] > 1):
            wire_to_cut = 4  # Fourth wire
        elif (color_count[RED] == 0):
            wire_to_cut = num_wires  # Last wire
        else:
            wire_to_cut = 4  # Fourth wire

    print("cut wire no. ", wire_to_cut)


def getButtonSolution():
    ABORT = 0
    DETONATE = 1
    HOLD = 2
    PRESS = 3

    B_BLUE = 0
    B_RED = 1
    B_WHITE = 2
    B_YELLOW = 3

    print("To solve Button module you need to ", end="")

    colors = [[0, 4],  # Blue
              [1, 1],  # Red
              [2, 1],  # White
              [3, 5],  # Yellow
              [4, 0]]  # Transparent (button only, not  coloured strips)

    buttonColor = colors[random(5)][0]
    stripColor = colors[random(4)][0]
    text = random(4)

    if (text == DETONATE or (text == HOLD and buttonColor == B_RED)):
        print("press and immediately release the button")
    else:
        print("hold the button and release it when the countdown timer has a ", end="")
        if stripColor == B_BLUE:
            print(4, end="")
        elif stripColor == B_YELLOW:
            print(5, end="")
        else:
            print(1, end="")

        print(" in any position")


def serialContainsVowel():
    temp = 0
    for i in range(len(config.serial)):
        temp = config.serial[i]
        if (temp == 'A' or temp == 'E' or temp == 'I' or temp == 'O' or temp == 'U'):
            return 1
    return 0


def getSimonSolution():

    print("To solve Simon Says module you need to:")

    MAX_NUM_STAGES = 5
    RED = 1
    YELLOW = 2
    GREEN = 3
    BLUE = 4
    dbgColors = ["Error", "Red", "Yellow", "Green", "Blue"]
    num_stages = random2(3, MAX_NUM_STAGES + 1)
    mapping = [
        # No Vowel
        [BLUE, RED, GREEN, YELLOW],  # No Strikes
        [RED, GREEN, YELLOW, BLUE],  # One Strike
        [YELLOW, RED, BLUE, GREEN]  # Two Strikes
    ], [
        # Vowel
        [BLUE, GREEN, YELLOW, RED],  # No Strikes
        [YELLOW, RED, BLUE, GREEN],  # One Strike
        [GREEN, BLUE, YELLOW, RED]  # Two Strikes
    ]

    stage_colors = [0, 0, 0, 0, 0]
    for i in range(num_stages):
        stage_colors[i] = random2(0, 4)

    vowel = serialContainsVowel()
    for strike in range(3):
        print("\tFor", strike, "strike(s):", end="")
        stage = 0
        while stage < num_stages:  # ( stage = 0 stage < num_stages stage++)
            print(dbgColors[mapping[vowel][strike]
                            [stage_colors[stage]]], "\t", end="")
            stage += 1

        print()


def getIndexFromNumber(buttons, num):
    for i in range(4):
        if buttons[i] == num:
            return i
    return 255


def getMemorySolution():
    print("To solve Memory module you need to ", end="")

    bottom_nums = [[0, 0, 0, 0], [0, 0, 0, 0], [
        0, 0, 0, 0], [0, 0, 0, 0], [0, 0, 0, 0]]
    top_nums = [0, 0, 0, 0, 0]
    buttons_to_press = [0, 0, 0, 0, 0]
    r1, r2 = 0, 0
    temp = 0
    count = 0
    for i in range(5):
        # Insert 1-4 in each row
        bottom_nums[i][0] = 1
        bottom_nums[i][1] = 2
        bottom_nums[i][2] = 3
        bottom_nums[i][3] = 4
        # Swap random numbers in row 20 times to scramble them
        for j in range(20):
            count += 2
            r1 = random2(0, 4)
            r2 = random2(0, 4)
            temp = bottom_nums[i][r1]
            bottom_nums[i][r1] = bottom_nums[i][r2]
            bottom_nums[i][r2] = temp

        count += 1
        top_nums[i] = random2(1, 5)

    if top_nums[0] == 1:
        buttons_to_press[0] = 1  # Second Position
    elif top_nums[0] == 2:
        buttons_to_press[0] = 1  # Second Position
    elif top_nums[0] == 3:
        buttons_to_press[0] = 2  # Third Position
    elif top_nums[0] == 4:
        buttons_to_press[0] = 3  # Fourth Position

    if top_nums[1] == 1:
        # Button labeled 4
        buttons_to_press[1] = getIndexFromNumber(bottom_nums[1], 4)
    elif top_nums[1] == 2:
        buttons_to_press[1] = buttons_to_press[0]  # Same position as stage 1
    elif top_nums[1] == 3:
        buttons_to_press[1] = 0  # First Position
    elif top_nums[1] == 4:
        buttons_to_press[1] = buttons_to_press[0]  # Same position as stage 1

    if top_nums[2] == 1:
        # Same label as stage 2
        buttons_to_press[2] = getIndexFromNumber(
            bottom_nums[2], bottom_nums[1][buttons_to_press[1]])
    elif top_nums[2] == 2:
        # Same label as stage 1
        buttons_to_press[2] = getIndexFromNumber(
            bottom_nums[2], bottom_nums[0][buttons_to_press[0]])
    elif top_nums[2] == 3:
        buttons_to_press[2] = 2  # Third Position
    elif top_nums[2] == 4:
        # Button labeled 4
        buttons_to_press[2] = getIndexFromNumber(bottom_nums[2], 4)

    if top_nums[3] == 1:
        buttons_to_press[3] = buttons_to_press[0]  # Same position as stage 1
    elif top_nums[3] == 2:
        buttons_to_press[3] = 0  # First Position
    elif top_nums[3] == 3:
        buttons_to_press[3] = buttons_to_press[1]  # Same position as stage 2
    elif top_nums[3] == 4:
        buttons_to_press[3] = buttons_to_press[1]  # Same position as stage 2

    if top_nums[4] == 1:
        # Same label as stage 1
        buttons_to_press[4] = getIndexFromNumber(
            bottom_nums[4], bottom_nums[0][buttons_to_press[0]])
    elif top_nums[4] == 2:
        # Same label as stage 2
        buttons_to_press[4] = getIndexFromNumber(
            bottom_nums[4], bottom_nums[1][buttons_to_press[1]])
    elif top_nums[4] == 3:
        # Same label as stage 4
        buttons_to_press[4] = getIndexFromNumber(
            bottom_nums[4], bottom_nums[3][buttons_to_press[3]])
    elif top_nums[4] == 4:
        # Same label as stage 3
        buttons_to_press[4] = getIndexFromNumber(
            bottom_nums[4], bottom_nums[2][buttons_to_press[2]])

    print("press: ", end="")
    for i in range(5):
        print(buttons_to_press[i] + 1, " ", end="")

    print()


markers = [
    [
        [0, 1],
        [5, 2]
    ],
    [
        [4, 1],
        [1, 3]
    ],
    [
        [3, 3],
        [5, 3]
    ],
    [
        [0, 0],
        [0, 3]
    ],
    [
        [4, 2],
        [3, 5]
    ],
    [
        [4, 0],
        [2, 4]
    ],
    [
        [1, 0],
        [1, 5]
    ],
    [
        [3, 0],
        [2, 3]
    ],
    [
        [2, 1],
        [0, 4]
    ]
]

mazeVerticalWalls = [
    [
        [1, 0, 0, 1, 0, 0, 1],
        [1, 1, 0, 1, 0, 0, 1],
        [1, 1, 0, 1, 0, 0, 1],
        [1, 1, 0, 0, 1, 0, 1],
        [1, 0, 0, 1, 0, 1, 1],
        [1, 0, 1, 0, 1, 0, 1]
    ],
    [
        [1, 0, 0, 1, 0, 0, 1],
        [1, 0, 1, 0, 1, 0, 1],
        [1, 1, 0, 1, 0, 0, 1],
        [1, 0, 1, 0, 1, 1, 1],
        [1, 1, 1, 1, 0, 1, 1],
        [1, 1, 0, 1, 0, 0, 1]
    ],
    [
        [1, 0, 0, 1, 1, 0, 1],
        [1, 1, 1, 1, 0, 1, 1],
        [1, 0, 1, 1, 0, 1, 1],
        [1, 1, 1, 1, 1, 1, 1],
        [1, 1, 0, 1, 1, 1, 1],
        [1, 0, 0, 0, 1, 0, 1]
    ],
    [
        [1, 0, 1, 0, 0, 0, 1],
        [1, 1, 1, 0, 0, 0, 1],
        [1, 1, 0, 1, 0, 1, 1],
        [1, 1, 0, 0, 0, 0, 1],
        [1, 0, 0, 0, 0, 1, 1],
        [1, 0, 0, 1, 0, 1, 1]
    ],
    [
        [1, 0, 0, 0, 0, 0, 1],
        [1, 0, 0, 0, 0, 1, 1],
        [1, 0, 1, 0, 1, 0, 1],
        [1, 1, 0, 0, 1, 1, 1],
        [1, 1, 0, 0, 0, 1, 1],
        [1, 1, 0, 0, 0, 0, 1]
    ],
    [
        [1, 1, 0, 1, 0, 0, 1],
        [1, 1, 1, 1, 0, 1, 1],
        [1, 0, 1, 1, 1, 0, 1],
        [1, 0, 1, 0, 1, 1, 1],
        [1, 0, 1, 1, 1, 0, 1],
        [1, 0, 0, 0, 1, 0, 1]
    ],
    [
        [1, 0, 0, 0, 1, 0, 1],
        [1, 1, 0, 1, 0, 1, 1],
        [1, 0, 1, 0, 1, 0, 1],
        [1, 0, 1, 0, 0, 1, 1],
        [1, 1, 1, 0, 0, 1, 1],
        [1, 0, 0, 0, 0, 0, 1]
    ],
    [
        [1, 1, 0, 0, 1, 0, 1],
        [1, 0, 0, 1, 0, 1, 1],
        [1, 1, 0, 0, 0, 1, 1],
        [1, 1, 0, 1, 0, 0, 1],
        [1, 1, 1, 0, 0, 0, 1],
        [1, 0, 0, 0, 0, 0, 1]
    ],
    [
        [1, 1, 0, 0, 0, 0, 1],
        [1, 1, 1, 0, 1, 1, 1],
        [1, 0, 0, 1, 0, 1, 1],
        [1, 1, 1, 0, 1, 0, 1],
        [1, 1, 1, 1, 0, 1, 1],
        [1, 0, 1, 0, 1, 0, 1]
    ]
]

mazeHorizontalWalls = [
    [
        [1, 0, 0, 0, 0, 0, 1],
        [1, 1, 0, 1, 1, 1, 1],
        [1, 0, 1, 0, 1, 0, 1],
        [1, 0, 1, 0, 1, 0, 1],
        [1, 1, 1, 1, 1, 1, 1],
        [1, 1, 0, 0, 0, 0, 1]
    ],
    [
        [1, 1, 0, 0, 0, 0, 1],
        [1, 0, 1, 0, 1, 0, 1],
        [1, 1, 0, 1, 0, 0, 1],
        [1, 0, 1, 0, 1, 0, 1],
        [1, 0, 1, 1, 0, 1, 1],
        [1, 1, 0, 0, 0, 0, 1]
    ],
    [
        [1, 0, 1, 0, 0, 0, 1],
        [1, 1, 0, 0, 0, 1, 1],
        [1, 0, 0, 0, 0, 1, 1],
        [1, 0, 1, 0, 0, 0, 1],
        [1, 0, 1, 0, 0, 0, 1],
        [1, 0, 0, 0, 0, 0, 1]
    ],
    [
        [1, 0, 0, 0, 0, 0, 1],
        [1, 0, 0, 1, 1, 1, 1],
        [1, 1, 0, 1, 1, 1, 1],
        [1, 1, 1, 0, 1, 1, 1],
        [1, 1, 1, 1, 1, 0, 1],
        [1, 0, 0, 0, 0, 0, 1]
    ],
    [
        [1, 1, 0, 0, 0, 0, 1],
        [1, 1, 1, 0, 1, 0, 1],
        [1, 1, 1, 1, 1, 1, 1],
        [1, 1, 0, 1, 0, 1, 1],
        [1, 0, 1, 0, 1, 1, 1],
        [1, 0, 1, 0, 0, 0, 1]
    ],
    [
        [1, 0, 0, 0, 1, 0, 1],
        [1, 0, 0, 1, 0, 1, 1],
        [1, 0, 0, 1, 0, 1, 1],
        [1, 1, 0, 0, 0, 0, 1],
        [0, 0, 1, 0, 0, 1, 1],
        [1, 0, 0, 1, 0, 0, 1]
    ],
    [
        [1, 0, 0, 1, 0, 0, 1],
        [1, 1, 0, 1, 0, 1, 1],
        [1, 1, 1, 0, 0, 1, 1],
        [1, 0, 1, 1, 1, 1, 1],
        [1, 0, 1, 0, 1, 0, 1],
        [1, 0, 0, 1, 0, 0, 1]
    ],
    [
        [1, 0, 0, 0, 0, 0, 1],
        [1, 0, 1, 0, 1, 0, 1],
        [1, 1, 1, 1, 0, 1, 1],
        [1, 0, 1, 1, 1, 1, 1],
        [1, 0, 1, 0, 1, 1, 1],
        [1, 0, 0, 0, 1, 1, 1]
    ],
    [
        [1, 0, 0, 0, 0, 0, 1],
        [1, 0, 0, 1, 0, 0, 1],
        [1, 1, 0, 1, 0, 0, 1],
        [1, 1, 1, 0, 1, 0, 1],
        [1, 0, 0, 1, 1, 0, 1],
        [1, 0, 0, 0, 0, 1, 1]
    ]
]


def moveIsValid(mazeNum, pos, dir):

    if dir == "U":
        return not mazeHorizontalWalls[mazeNum][pos[0]][pos[1]]
    elif dir == "D":
        return not mazeHorizontalWalls[mazeNum][pos[0]][pos[1] + 1]
    elif dir == "L":
        return not mazeVerticalWalls[mazeNum][pos[1]][pos[0]]
    elif dir == "R":
        return not mazeVerticalWalls[mazeNum][pos[1]][pos[0] + 1]

    ValueError(dir)


def getsToGoal(start, goal, moves):
    x = start[0]
    y = start[1]

    for move in moves:
        if move == "L":
            x -= 1

        elif move == "R":
            x += 1

        elif move == "U":
            y -= 1

        elif move == "D":
            y += 1

    if x == goal[0] and y == goal[1]:
        print(moves)
        return True

    return False



def isValid(start, moves, mazeNum):
    x = start[0]
    y = start[1]

    for move in moves:
        if moveIsValid(mazeNum, [x, y], move):
            if move == "L":
                x -= 1
            elif move == "R":
                x += 1
            elif move == "U":
                y -= 1
            elif move == "D":
                y += 1
        else:
            return False

    return True



def getMazesSolution():
    import queue

    print("To solve Maze module you need to press, in order: ", end="")

    mazeNum = random2(0, 9)
    playerLocation = [0, 0]
    goalLocation = [0, 0]
    markerLocations = markers[mazeNum]
    playerLocation[0] = random2(0, 6)
    playerLocation[1] = random2(0, 6)

    goalLocation[0] = random2(0, 6)
    goalLocation[1] = random2(0, 6)
    while (((goalLocation[0] == playerLocation[0]) and (goalLocation[1] == playerLocation[1])) or ((goalLocation[0] == markerLocations[0][0]) and (goalLocation[1] == markerLocations[0][1])) or ((goalLocation[0] == markerLocations[1][0]) and (goalLocation[1] == markerLocations[1][1]))):
        goalLocation[0] = random2(0, 6)
        goalLocation[1] = random2(0, 6)


    Q = queue.Queue()
    pattern = ""
    Q.put(pattern)
    while not getsToGoal(playerLocation, goalLocation, pattern):
        pattern = Q.get()
        for dir in ["L", "R", "U", "D"]:
            newPattern = pattern + dir
            if isValid(playerLocation, newPattern, mazeNum):
                Q.put(newPattern)

if(__name__ == "__main__"):
    config.serial = serial
    config.batteries = 0
    config.ports = (Parallel | (RJ45 << 1) | (Stereo << 2))

    randomSeed(config_to_seed(config))

    config.indicators = 0

    #config.indicators |= random(2)
    #config.indicators |= (random(2) << 1)

    randomSeed(config_to_seed(config))
    getWiresSolution()

    randomSeed(config_to_seed(config))
    getButtonSolution()

    randomSeed(config_to_seed(config))
    getSimonSolution()

    randomSeed(config_to_seed(config))
    getMemorySolution()

    randomSeed(config_to_seed(config))
    getMazesSolution()
