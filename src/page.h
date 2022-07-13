#ifndef PAGE_H
#define PAGE_H

#include <ESP32-HUB75-MatrixPanel-I2S-DMA.h>
#include "icon.h"
#include "character.h"

class Page {
    public:
        Page(char _train, const char* _stop_id, const char* station, MatrixPanel_I2S_DMA* _m, uint16_t _color, int _nresults);
        void setUptownTime(int i, int time);
        void setDowntownTime(int i, int time);
        void display();
        char get_train();
        char* get_stop_id();
        int get_nresults();

    private:
        void drawIcon();
        void drawText(const char* text, int x, int y);
        int drawNum(int num, int x, int y);
        int drawLetter(char letter, int x, int y);

        char train;
        const char* stop_id;
        const char* station;
        int uptown[];
        int downtown[];
        MatrixPanel_I2S_DMA* m;
        uint16_t color;
        int nresults;
};

#endif