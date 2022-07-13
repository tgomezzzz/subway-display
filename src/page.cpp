/*****
 * 
 * Class that organizes and displays each train page.
 * 
*****/

#include "page.h"

bool symbolPixelIsSet(char train, int i, int j) {
    switch (train) {
    case 'A':
        return ICON_A[i][j];
    case 'C':
        return ICON_C[i][j];
    case 'E':
        return ICON_E[i][j];
    case '1':
        return ICON_1[i][j];
    case 'Q':
        return ICON_Q[i][j];
    }
    return false;
}

bool numPixelIsSet(int num, int i, int j) {
    return num > -1 && num < 10 ? NUMS[num][i][j] : false;
}

bool letterPixelIsSet(char letter, int i, int j) {
    switch (letter) {
    case 's':
        return S[i][j];
    case 't':
        return T[i][j];
    case 'r':
        return R[i][j];
    case 'e':
        return E[i][j];
    case 'a':
        return A[i][j];
    case 'v':
        return V[i][j];
    }
    return false;
}

int letterWidth(char letter) {
    switch (letter) {
    case 's':
        return S_WIDTH;
    case 't':
        return T_WIDTH;
    case 'r':
        return R_WIDTH;
    case 'e':
        return E_WIDTH;
    case 'a':
        return A_WIDTH;
    case 'v':
        return V_WIDTH;
    }
    return 1;
}

Page::Page(char _train, const char* _stop_id, const char* _station, MatrixPanel_I2S_DMA* _m, uint16_t _color, int _nresults) :
        train(_train), stop_id(_stop_id), station(_station), m(_m), color(_color), nresults(_nresults) {
}

void Page::display() {
    drawIcon();
    m->fillRect(13, 1, m->width() - 13, 9, 0xFFFF);
    drawText(station, 13, 1);
}

void Page::drawIcon() {
    for (int i = 0; i < ICON_SIZE; i++) {
        for (int j = 0; j < ICON_SIZE; j++) {
            if (ICON_BACKGROUND[i][j]) {
                delay(3);
                m->drawPixel(j, i, color);
            }
        }
    }
    for (int i = 0; i < ICON_LETTER_HEIGHT; i++) {
        for (int j = 0; j < ICON_LETTER_WIDTH; j++) {
            if (symbolPixelIsSet(train, i, j)) {
                delay(25);
                m->drawPixel(3 + j, 2 + i, train == 'Q' ? 0x000 : 0xFFFF);
            }
        }
    }
}

void Page::drawText(const char* text, int x, int y) {
    int cursor = x;
    for (int i = 0; i < strlen(text); i++) {
        char c = text[i];
        if (c >= '0' && c <= '9') {
            cursor += drawNum(c - '0', cursor, y) + 1;
        } else {
            cursor += drawLetter(c, cursor, y + 1) + 1;
        }
    }
}

int Page::drawNum(int num, int x, int y) {
    for (int i = 0; i < NUM_HEIGHT; i++) {
        for (int j = 0; j < NUM_WIDTH; j++) {
            if (numPixelIsSet(num, i, j)) {
                delay(5);
                m->drawPixel(x + j, y + i, 0x0000);
            }
        }
    }
    return NUM_WIDTH;
}

int Page::drawLetter(char letter, int x, int y) {
    int width = letterWidth(letter);
    for (int i = 0; i < LETTER_HEIGHT; i++) {
        for (int j = 0; j < width; j++) {
            if (letterPixelIsSet(letter, i, j)) {
                delay(5);
                m->drawPixel(x + j, y + i, 0x0000);
            }
        }
    }
    return width;
}