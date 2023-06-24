#ifndef CustomTM1638_h
#define CustomTM1638_h

#include <TM1638plus.h>

class CustomTM1638 {
private:
    TM1638plus tm;

public:
    CustomTM1638(uint8_t stb, uint8_t clk, uint8_t dio);
    void init();
    void reset();
    void displayASCII(uint8_t pos, uint8_t ascii);
    void setLED(uint8_t pos, uint8_t value);
    uint16_t readButtons();
    int8_t getButtonNumber(uint16_t buttons);
    void displaySequence(uint8_t *sequence, uint8_t length);
    void setLEDs(uint16_t value);
};

#endif
