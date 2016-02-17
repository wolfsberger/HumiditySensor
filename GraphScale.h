#ifndef GRAPHSCALE_H_
#define GRAPHSCALE_H_

#include "SPI_TFT_ILI9341.h"

class GraphScale
{
public:
    GraphScale(SPI_TFT_ILI9341 * tft, int x, int y, int width, int height, int min, int max, int scaleSteps);
    void draw(int color); 
private:
    float scaleY(float value);
    SPI_TFT_ILI9341 * tft_;
    int x_;
    int y_;
    int width_;
    int height_;
    int min_;
    int max_;
    int scaleSteps_;
};

#endif