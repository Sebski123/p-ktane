#include "Arduino.h"

int markers[9][2][2] = {
    {
        {0,1},
        {5,2}
    },
    {
        {4,1},
        {1,3}
    },
    {
        {3,3},
        {5,3}
    },
    {
        {0,0},
        {0,3}
    },
    {
        {4,2},
        {3,5}
    },
    {
        {4,0},
        {2,4}
    },
    {
        {1,0},
        {1,5}
    },
    {
        {3,0},
        {2,3}
    },
    {
        {2,1},
        {0,4}
    }
};

int mazeVerticalWalls[9][6][5] = {
    {
        {0,0,1,0,0},
        {1,0,1,0,0},
        {1,0,1,0,0},
        {1,0,0,1,0},
        {0,0,1,0,1},
        {0,1,0,1,0}
    },
    {
        {0,0,1,0,0},
        {0,1,0,1,0},
        {1,0,1,0,0},
        {0,1,0,1,1},
        {1,1,1,0,1},
        {1,0,1,0,0}
    },
    {
        {0,0,1,1,0},
        {1,1,1,0,1},
        {0,1,1,0,1},
        {1,1,1,1,1},
        {1,0,1,1,1},
        {0,0,0,1,0}
    },
    {
        {0,1,0,0,0},
        {1,1,0,0,0},
        {1,0,1,0,1},
        {1,0,0,0,0},
        {0,0,0,0,1},
        {0,0,1,0,1}
    },
    {
        {0,0,0,0,0},
        {0,0,0,0,1},
        {0,1,0,1,0},
        {1,0,0,1,1},
        {1,0,0,0,1},
        {1,0,0,0,0}
    },
    {
        {1,0,1,0,0},
        {1,1,1,0,1},
        {0,1,1,1,0},
        {0,1,0,1,1},
        {0,1,1,1,0},
        {0,0,0,1,0}
    },
    {
        {0,0,0,1,0},
        {1,0,1,0,1},
        {0,1,0,1,0},
        {0,1,0,0,1},
        {1,1,0,0,1},
        {0,0,0,0,0}
    },
    {
        {1,0,0,1,0},
        {0,0,1,0,1},
        {1,0,0,0,1},
        {1,0,1,0,0},
        {1,1,0,0,0},
        {0,0,0,0,0}
    },
    {
        {1,0,0,0,0},
        {1,1,0,1,1},
        {0,0,1,0,1},
        {1,1,0,1,0},
        {1,1,1,0,1},
        {0,1,0,1,0}
    }
};