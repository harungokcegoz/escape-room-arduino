#ifndef Challenge4_h
#define Challenge4_h

class GameManager;
#include "CustomTM1638.h"
#include "CustomLCD.h"

class Challenge4 {
  private:
    GameManager& game;
    CustomTM1638& tm;
    CustomLCD& lcd;
    const uint8_t maxRounds = 5;
    uint8_t currentRound;
    uint8_t speedOfLED = 40;
    unsigned long timeout = 5000;
    uint8_t lightRandomLED();
    void flashLeds();
    bool checkUserInput(uint8_t correctPos);
    void blinkLed(uint8_t ledPos);
    void resetGame();
    void playRounds();

  public:
    Challenge4(CustomTM1638& tm, CustomLCD& lcd, GameManager& game);
    void setup();
    void loop();
};

#endif
