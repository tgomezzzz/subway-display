#ifndef PAGE_H
#define PAGE_H

#include <ESP32-HUB75-MatrixPanel-I2S-DMA.h>
#include <ArduinoJson.h>
#include "icon.h"
#include "character.h"

class Page {
    public:
        Page(char _train, const char* _stop_id, const char* station, int _group, MatrixPanel_I2S_DMA* _m, uint16_t _color, int _nresults);
        ~Page();

        void set_uptown(int i, const char* value);
        void set_downtown(int i, const char* value);

        void display(int prev_group);
        char get_train();
        const char* get_station();
        const char* get_stop_id();
        int get_group();
        int get_nresults();

        char** uptown;
        char** downtown;

        enum arrow_dir {UP, DOWN, LEFT, RIGHT};

    private:
        void draw_icon(int x, int y, int d);
        int draw_text(const char* text, int x, int y, uint16_t color, int d);
        int draw_num(int num, int x, int y, uint16_t color, int d);
        int draw_letter(char letter, int x, int y, uint16_t color, int d);
        void draw_arrow(arrow_dir dir, int x, int y, uint16_t color, int d);

        char train;
        const char* stop_id;
        const char* station;
        int group;
        MatrixPanel_I2S_DMA* m;
        uint16_t color;
        int nresults;
        int n_uptown, n_downtown;

};

#endif