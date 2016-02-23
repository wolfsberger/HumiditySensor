#ifndef LINEGRAPH_H_
#define LINEGRAPH_H_

#include "SPI_TFT_ILI9341.h"
#include <cmath>

template <size_t itemCount>
class LineGraph
{
public:
    LineGraph(SPI_TFT_ILI9341 * tft, int x, int y, int width, int height, int min, int max)
        : tft_(tft), x_(x), y_(y), width_(width), height_(height), min_(min), max_(max)
    {
        for(size_t i = 0; i<itemCount; i++)
        {
            dataOld_[i] = NAN;
            dataNew_[i] = NAN;
        }
    }

    void addItem(float value)
    {
        if (!isInRange(value))
        {
            value = NAN;
        }

        memcpy(dataOld_,dataNew_,(itemCount)*sizeof(float));
        memmove(dataNew_,dataNew_+1,(itemCount-1)*sizeof(float));
        dataNew_[itemCount-1] = value;
    }

    void draw(int color)
    {
        float pointStepSize = static_cast<float>(width_) / static_cast<float>(itemCount-1);

        for (size_t index = 1; index < itemCount; index++)
        {
            size_t indexItem1 = index - 1;
            size_t indexItem2 = index;

            float x1 = 1 + pointStepSize * static_cast<float>(indexItem1);
            float x2 = 1 + pointStepSize * static_cast<float>(indexItem2);

            float y1Old = scaleY(dataOld_[indexItem1]);
            float y2Old = scaleY(dataOld_[indexItem2]);

            tft_->line(x1, y1Old, x2, y2Old, Black);

            if (isnan(dataNew_[indexItem1]) || isnan(dataNew_[indexItem2]))
            {
                continue;
            }

            float y1New = scaleY(dataNew_[indexItem1]);
            float y2New = scaleY(dataNew_[indexItem2]);

            tft_->line(x1, y1New, x2, y2New, color);
        }
    }
private:
    float scaleY(float ypos)
    {
        static float valueRange = static_cast<float>(max_-min_);
        static float buttomOfGraph = static_cast<float>(y_+height_);

        if (isnan(ypos))
        {
            return buttomOfGraph;
        }

        if (ypos < min_)
            ypos = min_;
        if (ypos > max_)
            ypos = max_;

        float scaled = buttomOfGraph - (height_ * ((ypos-min_) / valueRange));

        return scaled;
    }
    bool isInRange(float value)
    {
        return  value > min_ &&
                value < max_;
    }

    float dataOld_[itemCount];
    float dataNew_[itemCount];
    SPI_TFT_ILI9341 * tft_;
    int x_;
    int y_;
    int width_;
    int height_;
    int min_;
    int max_;
};

#endif
