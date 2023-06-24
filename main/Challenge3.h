#ifndef Challenge3_h
#define Challenge3_h

class GameManager;
#include "CustomTM1638.h"
#include "CustomLCD.h"

class Challenge3 {
private:
    GameManager& game;
    CustomTM1638& tm;
    CustomLCD& lcd;
    const uint8_t sequenceMaxLength = 4;
    unsigned long timeout = 5000;
    uint8_t sequenceLength = 3;
    void displayRoundNumber(uint8_t round);
    void displaySequence(uint8_t sequence[], uint8_t length);
    void showLEDAndPlayBuzzer(uint8_t ledNumber);
    bool checkUserInput(uint8_t sequence[], uint8_t length, unsigned long timeout);
    void displayRemainingTimeOnLED(unsigned long timeout, unsigned long startTime);
    int getRemainingTime(unsigned long startTime, unsigned long timeout);
    uint8_t handleButtonPress(uint8_t currentIndex, uint16_t buttonsArray[]);
    bool isUserInputCorrect(uint8_t sequence[], uint16_t buttonsArray[], uint8_t length);
    void playRounds();

public:
    Challenge3(CustomTM1638& tm, CustomLCD& lcd, GameManager& game);
    void setup();
    void loop();
};

#endif
