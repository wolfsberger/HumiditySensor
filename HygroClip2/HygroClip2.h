#ifndef HYGROCLIP2_H_
#define HYGROCLIP2_H_

#include "mbed.h"

const size_t HYGROCLIP_BUFFER_SIZE = 120;

class HygroClip2
{
public:
    HygroClip2(PinName tx, PinName rx);
    void update();
    float getTemperature();
    float getHumidity();
    float getDewPoint();
    float getAbsolutHumidity();
    bool isDataValid();
private:
    // Private functions
    void uartCallback();

    // Variables
    Serial uart_;
    float temperature_;
    float humidity_;
    bool dataReadyToPars_;
    int8_t buffer_[HYGROCLIP_BUFFER_SIZE + 1];
    bool isDataValid_;
};

#endif
