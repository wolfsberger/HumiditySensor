#include "HygroClip2.h"
#include <cmath>

HygroClip2::HygroClip2(PinName tx, PinName rx)
    : uart_(tx, rx), temperature_(0.0f), humidity_(0.0f), dataReadyToPars_(false)
{
    buffer_[HYGROCLIP_BUFFER_SIZE] = 0;
    uart_.baud(19200);
    uart_.attach(this, &HygroClip2::uartCallback);
}

void HygroClip2::uartCallback()
{
    static size_t index = 0;
    uint8_t data = uart_.getc();

    if (dataReadyToPars_) {
        return;
    }

    if (data == '{') {
        index = 0;
    } else if (data == '\r') {
        dataReadyToPars_ = true;
    } else {
        buffer_[index] = data;
        index++;
    }

    if (index >= HYGROCLIP_BUFFER_SIZE) {
        index = 0;
    }
}

void HygroClip2::update()
{
    if (dataReadyToPars_)
    {
        dataReadyToPars_ = false;
        humidity_ = atof(reinterpret_cast<char *>(&buffer_[11]));
        temperature_ = atof(reinterpret_cast<char *>(&buffer_[29]));
        isDataValid_ = true;
    }
    else
    {
        humidity_ = NAN;
        temperature_ = NAN;
        isDataValid_ = false;
    }

    uart_.printf("{F00RDD}\r");
}

float HygroClip2::getHumidity()
{
    return humidity_;
}
float HygroClip2::getTemperature()
{
    return temperature_;
}
float HygroClip2::getDewPoint()
{
    return 243.04f*(log(getHumidity()/100)+((17.625*getTemperature())/(243.04+getTemperature())))/(17.625-log(getHumidity()/100)-((17.625*getTemperature())/(243.04+getTemperature())));
}
float HygroClip2::getAbsolutHumidity()
{
    float saturationPressure = 611.2f * exp((17.62f * getTemperature()) / (243.12f + getTemperature()));
    float partialPressure = (getHumidity() * saturationPressure) / 100.0f;
    float absoluteHumidity = partialPressure / (461.51f * (getTemperature() + 273.15f));
    return absoluteHumidity*1000;
}
bool HygroClip2::isDataValid()
{
    return isDataValid_;
}
