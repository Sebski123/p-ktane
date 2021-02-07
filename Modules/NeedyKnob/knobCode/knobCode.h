#include "Arduino.h"

byte positions[4][2][2] = {
    { //UP
        { //Solution 1
            B00101100, //UPPER
            B11110100  //LOWER
        },
        { //Solution 2
            B10101000, //UPPER
            B01101100  //LOWER
        }
    },
    { //DOWN
        { //Solution 1
            B01100100, //UPPER
            B11110100  //LOWER
        },
        { //Solution 2
            B10101000, //UPPER
            B01000100  //LOWER
        }    
    },
    { //LEFT
        { //Solution 1
            B00001000, //UPPER
            B10011100  //LOWER
        },
        { //Solution 2
            B00001000, //UPPER
            B00011000  //LOWER
        }    
    },
    { //RIGHT
        { //Solution 1
            B10111100, //UPPER
            B11101000  //LOWER
        },
        { //Solution 2
            B10110000, //UPPER
            B11101000  //LOWER
        }    
    }
};