#ifndef Challenge2_h
#define Challenge2_h

class GameManager;
#include "CustomTM1638.h"
#include "CustomLCD.h"

class Challenge2 {
  private:
    GameManager& game;
    CustomTM1638& tm;
    CustomLCD& lcd;
    const uint16_t gameTime = 20000;
    const float smoothingFactor = 0.1;
    uint8_t targetPrecision = 9;
    float smoothedPotValue = 0;
    int lockerCode[4];
    int correctNumbersCount = 0;
    void playRound();
    void displayTargetValue(int targetValue);
    bool isTimeUp(unsigned long startTime);
    int readPotentiometer();
    int getRemainingTime(unsigned long startTime);
    void displayValueAndTime(int adjustedValue, int remainingTime);
    bool isValueCorrect(int targetValue, int adjustedValue);
    void resetGame();
    void lockerUnlocked();
    void displayLockerCode();

  public:
    Challenge2(CustomTM1638& tm, CustomLCD& lcd, GameManager& game);
    void setup();
    void loop();
};

#endif
