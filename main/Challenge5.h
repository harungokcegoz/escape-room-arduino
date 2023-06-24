#ifndef Challenge5_h
#define Challenge5_h

#include "GameManager.h"
#include "CustomTM1638.h"
#include "CustomLCD.h"

class Challenge5 {
  private:
    GameManager& game;
    CustomTM1638& tm;
    CustomLCD& lcd;
    static constexpr uint8_t morseCodesCount = 8;
    const String morseCodes[morseCodesCount] = {".----", "..---", "...--", "....-", ".....", "-....", "--...", "---.."};
    const uint8_t maxRounds = 4;
    const unsigned long timeout = 5000;
    void displayRound(uint8_t round);
    void playMorseCode(const String& morseCode);
    uint8_t getUserInput(unsigned long startTime);
    void playRounds();

  public:
    Challenge5(CustomTM1638& tm, CustomLCD& lcd, GameManager& game);
    void setup();
    void loop();
};

#endif
