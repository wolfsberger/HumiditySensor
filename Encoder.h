#ifndef ENCODER_H_INCLUDED
#define ENCODER_H_INCLUDED

class RotaryEncoder {
public:
    RotaryEncoder(PinName outa, PinName outb)
        : position_(0), delta_(0),
          inta_(outa), intb_(outb), outa_(outa), outb_(outb) {
        outa_.mode(PullUp);
        outb_.mode(PullUp);
        inta_.rise(this, &RotaryEncoder::handleIntA);
        inta_.fall(this, &RotaryEncoder::handleIntA);
        intb_.rise(this, &RotaryEncoder::handleIntB);
        intb_.fall(this, &RotaryEncoder::handleIntB);
    }

    /** Returns the overall position of the encoder
     */
    int position() {
        return position_;
    }

    /** Returns the encoders change since the last call to this function
     */
    int delta() {
        int d = delta_;
        delta_ = 0;
        return d;
    }

private:

    // A  = B --> Clockwise
    // A /= B --> Counter clockwise
    void handleIntA() {
        if (outa_ == outb_) {
            increment(1);
        } else {
            increment(-1);
        }
    }

    // A  = B --> Counter clockwise
    // A /= B --> Clockwise
    void handleIntB() {
        if (outa_ == outb_) {
            increment(-1);
        } else {
            increment(1);
        }
    }

    void increment(int i) {
        position_ += i;
        delta_ += i;
    }

    int position_;
    int delta_;

    InterruptIn inta_;
    InterruptIn intb_;
    DigitalIn outa_;
    DigitalIn outb_;
};

#endif /* ENCODER_H_INCLUDED */
