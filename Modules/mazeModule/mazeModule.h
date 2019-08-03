#include "Arduino.h"

#define UP 0
#define DOWN 1
#define LEFT 2
#define RIGHT 3

int markers[9][2][2] = {
    {
        {0, 1},
        {5, 2}
    },
    {
        {4, 1},
        {1, 3}
    },
    {
        {3, 3},
        {5, 3}
    },
    {
        {0, 0},
        {0, 3}
    },
    {
        {4, 2},
        {3, 5}
    },
    {
        {4, 0},
        {2, 4}
    },
    {
        {1, 0},
        {1, 5}
    },
    {
        {3, 0},
        {2, 3}
    },
    {
        {2, 1},
        {0, 4}
    }
};

int mazeVerticalWalls[9][6][7] = {
    {
        {1,0,0,1,0,0,1},
        {1,1,0,1,0,0,1},
        {1,1,0,1,0,0,1},
        {1,1,0,0,1,0,1},
        {1,0,0,1,0,1,1},
        {1,0,1,0,1,0,1}
    },
    {
        {1,0,0,1,0,0,1},
        {1,0,1,0,1,0,1},
        {1,1,0,1,0,0,1},
        {1,0,1,0,1,1,1},
        {1,1,1,1,0,1,1},
        {1,1,0,1,0,0,1}
    },
    {
        {1,0,0,1,1,0,1},
        {1,1,1,1,0,1,1},
        {1,0,1,1,0,1,1},
        {1,1,1,1,1,1,1},
        {1,1,0,1,1,1,1},
        {1,0,0,0,1,0,1}
    },
    {
        {1,0,1,0,0,0,1},
        {1,1,1,0,0,0,1},
        {1,1,0,1,0,1,1},
        {1,1,0,0,0,0,1},
        {1,0,0,0,0,1,1},
        {1,0,0,1,0,1,1}
    },
    {
        {1,0,0,0,0,0,1},
        {1,0,0,0,0,1,1},
        {1,0,1,0,1,0,1},
        {1,1,0,0,1,1,1},
        {1,1,0,0,0,1,1},
        {1,1,0,0,0,0,1}
    },
    {
        {1,1,0,1,0,0,1},
        {1,1,1,1,0,1,1},
        {1,0,1,1,1,0,1},
        {1,0,1,0,1,1,1},
        {1,0,1,1,1,0,1},
        {1,0,0,0,1,0,1}
    },
    {
        {1,0,0,0,1,0,1},
        {1,1,0,1,0,1,1},
        {1,0,1,0,1,0,1},
        {1,0,1,0,0,1,1},
        {1,1,1,0,0,1,1},
        {1,0,0,0,0,0,1}
    },
    {
        {1,1,0,0,1,0,1},
        {1,0,0,1,0,1,1},
        {1,1,0,0,0,1,1},
        {1,1,0,1,0,0,1},
        {1,1,1,0,0,0,1},
        {1,0,0,0,0,0,1}
    },
    {
        {1,1,0,0,0,0,1},
        {1,1,1,0,1,1,1},
        {1,0,0,1,0,1,1},
        {1,1,1,0,1,0,1},
        {1,1,1,1,0,1,1},
        {1,0,1,0,1,0,1}
    }
};

int mazeHorizontalWalls[9][6][7] = {
    {
        {1,0,0,0,0,0,1},
        {1,1,0,1,1,1,1},
        {1,0,1,0,1,0,1},
        {1,0,1,0,1,0,1},
        {1,1,1,1,1,1,1},
        {1,1,0,0,0,0,1}
    },
    {
        {1,1,0,0,0,0,1},
        {1,0,1,0,1,0,1},
        {1,1,0,1,0,0,1},
        {1,0,1,0,1,0,1},
        {1,0,1,1,0,1,1},
        {1,1,0,0,0,0,1}
    },
    {
        {1,0,1,0,0,0,1},
        {1,1,0,0,0,1,1},
        {1,0,0,0,0,1,1},
        {1,0,1,0,0,0,1},
        {1,0,1,0,0,0,1},
        {1,0,0,0,0,0,1}
    },
    {
        {1,0,0,0,0,0,1},
        {1,0,0,1,1,1,1},
        {1,1,0,1,1,1,1},
        {1,1,1,0,1,1,1},
        {1,1,1,1,1,0,1},
        {1,0,0,0,0,0,1}
    },
    {
        {1,1,0,0,0,0,1},
        {1,1,1,0,1,0,1},
        {1,1,1,1,1,1,1},
        {1,1,0,1,0,1,1},
        {1,0,1,0,1,1,1},
        {1,0,1,0,0,0,1}
    },
    {
        {1,0,0,0,1,0,1},
        {1,0,0,1,0,1,1},
        {1,0,0,1,0,1,1},
        {1,1,0,0,0,0,1},
        {0,0,1,0,0,1,1},
        {1,0,0,1,0,0,1}
    },
    {
        {1,0,0,1,0,0,1},
        {1,1,0,1,0,1,1},
        {1,1,1,0,0,1,1},
        {1,0,1,1,1,1,1},
        {1,0,1,0,1,0,1},
        {1,0,0,1,0,0,1}
    },
    {
        {1,0,0,0,0,0,1},
        {1,0,1,0,1,0,1},
        {1,1,1,1,0,1,1},
        {1,0,1,1,1,1,1},
        {1,0,1,0,1,1,1},
        {1,0,0,0,1,1,1}
    },
    {
        {1,0,0,0,0,0,1},
        {1,0,0,1,0,0,1},
        {1,1,0,1,0,0,1},
        {1,1,1,0,1,0,1},
        {1,0,0,1,1,0,1},
        {1,0,0,0,0,1,1}
    }
};