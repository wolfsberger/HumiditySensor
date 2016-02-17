#include "GraphScale.h"

GraphScale::GraphScale(SPI_TFT_ILI9341 * tft, int x, int y, int width, int height, int min, int max, int scaleSteps)
    : tft_(tft), x_(x), y_(y), width_(width), height_(height), min_(min), max_(max), scaleSteps_(scaleSteps)
{   
}

void GraphScale::draw(int color)
{
    int x1 = x_-1;
    int x2 = x_+width_+1;
    int y1 = y_-1;
    int y2 = y_+height_+1;
    
    tft_->rect(x1, y1, x2, y2, color);
    
    int stepsize = height_ / ((max_-min_) / scaleSteps_);
    int stepposition = min_;
      
    while (stepposition <= max_)
    {
        float yPos = scaleY(stepposition);
        tft_->line(x2, yPos, x2+5, yPos, color);
        tft_->locate(x2+10,yPos-6);
        tft_->printf("%d",stepposition);
        stepposition += scaleSteps_;
    }
}

float GraphScale::scaleY(float ypos)
{
    static float valueRange = static_cast<float>(max_-min_);
    static float buttomOfGraph = static_cast<float>(y_+height_);
    
    if (ypos < min_)
        ypos = min_;
    if (ypos > max_)
        ypos = max_;
    
    float scaled = buttomOfGraph - (height_ * ((ypos-min_) / valueRange));
    
    return scaled;
}