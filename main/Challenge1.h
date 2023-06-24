#ifndef Challenge1_h
#define Challenge1_h

class GameManager;
#include "CustomTM1638.h"
#include "CustomLCD.h"



class Challenge1 {
  private:
    GameManager& game;
    CustomTM1638& tm;
    CustomLCD& lcd;
    const uint8_t sequenceLength = 4;
    unsigned long currentTimeout = 5000;
    void playRound(unsigned long& currentTimeout);
    bool processInputs(const uint8_t *sequence, unsigned long currentTimeout);
    void adjustDifficulty(bool isCorrect, unsigned long &currentTimeout);
    void handleCorrectInput(uint8_t inputIndex, uint8_t inputValue);
    void displayResult(bool isCorrect);
    void increaseDifficulty(unsigned long &currentTimeout);
    void resetDifficulty(unsigned long &currentTimeout);
    
  public:
    Challenge1(CustomTM1638& tm, CustomLCD& lcd, GameManager& game);
    void setup();
    void loop();
};

#endif
