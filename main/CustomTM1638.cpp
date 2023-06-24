#include "CustomTM1638.h"

CustomTM1638::CustomTM1638(uint8_t stb, uint8_t clk, uint8_t dio) : tm(stb, clk, dio) {}

void CustomTM1638::init() {
    tm.displayBegin();
    tm.brightness(7);
}

void CustomTM1638::reset() {
    tm.reset();
}

void CustomTM1638::displayASCII(uint8_t pos, uint8_t ascii) {
    tm.displayASCII(pos, ascii);
}

void CustomTM1638::setLED(uint8_t pos, uint8_t value) {
    tm.setLED(pos-1, value);
}

uint16_t CustomTM1638::readButtons() {
    return tm.readButtons();
}

int8_t CustomTM1638::getButtonNumber(uint16_t buttons) {
    uint8_t buttonIndex = 0;
    uint8_t buttonValue = tm.readButtons();
    for (int i = 0; i < 8; i++) {
        if ((buttons >> i) & 0x01) {
            buttonIndex = i+1;
            break;
        }
    }
    return buttonIndex;
}

void CustomTM1638::displaySequence(uint8_t *sequence, uint8_t length) {
    for (uint8_t i = 0; i < length; i++) {
        tm.displayASCII(i, '0' + sequence[i]);
    }
}

void CustomTM1638::setLEDs(uint16_t value) {
    tm.setLEDs(value);
}
