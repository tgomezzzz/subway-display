#include <stdint.h>

class Drawable {
    public:
        Drawable(int _x, int _y, int _w, int _h, uint8_t** _pixels);
        void draw();

    private:
        int x, y, w, h;
        uint8_t** pixels;
};

class Icon : public Drawable {
    public:
        Icon(int _x, int _y, uint8_t** _pixels, uint16_t _primary, uint16_t _background);

    private:
        uint8_t colors[2];
};

class Letter : public Drawable {
    public:
        Letter(int _x, int _y, uint8_t** _pixels, uint16_t _color);

    private:
        uint8_t color;

};