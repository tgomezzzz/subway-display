/*****
 * 
 * Constant bitmaps and macros used for drawing train symbols.
 * 
*****/

#ifndef ICON_H
#define ICON_H

#define ICON_SIZE 11
#define ICON_LETTER_HEIGHT 7
#define ICON_LETTER_WIDTH 5

const uint8_t ICON_BACKGROUND[ICON_SIZE][ICON_SIZE] = { 
    {0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0},
    {0, 0, 1, 1, 1, 1, 1, 1, 1, 0, 0},
    {0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0},
    {0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0},
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    {0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0},
    {0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0},
    {0, 0, 1, 1, 1, 1, 1, 1, 1, 0, 0},
    {0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0},
};

const uint8_t ICON_A[ICON_LETTER_HEIGHT][ICON_LETTER_WIDTH] = {
    {0, 1, 1, 1, 0},
    {1, 0, 0, 0, 1},
    {1, 0, 0, 0, 1},
    {1, 1, 1, 1, 1},
    {1, 0, 0, 0, 1},
    {1, 0, 0, 0, 1},
    {1, 0, 0, 0, 1}
};

const uint8_t ICON_C[ICON_LETTER_HEIGHT][ICON_LETTER_WIDTH] = {
    {0, 1, 1, 1, 0},
    {1, 0, 0, 0, 1},
    {1, 0, 0, 0, 0},
    {1, 0, 0, 0, 0},
    {1, 0, 0, 0, 0},
    {1, 0, 0, 0, 1},
    {0, 1, 1, 1, 0}
};

const uint8_t ICON_E[ICON_LETTER_HEIGHT][ICON_LETTER_WIDTH] = {
    {1, 1, 1, 1, 1},
    {1, 0, 0, 0, 0},
    {1, 0, 0, 0, 0},
    {1, 1, 1, 1, 0},
    {1, 0, 0, 0, 0},
    {1, 0, 0, 0, 0},
    {1, 1, 1, 1, 1}
};

const uint8_t ICON_1[ICON_LETTER_HEIGHT][ICON_LETTER_WIDTH] = {
    {0, 0, 1, 0, 0},
    {0, 1, 1, 0, 0},
    {0, 0, 1, 0, 0},
    {0, 0, 1, 0, 0},
    {0, 0, 1, 0, 0},
    {0, 0, 1, 0, 0},
    {0, 1, 1, 1, 0}
};

#endif