#include "Challenge5.h"
#include "GameManager.h"


Challenge5::Challenge5(CustomTM1638& tmInstance, CustomLCD& lcdInstance, GameManager& gameInstance) 
    : tm(tmInstance), lcd(lcdInstance), game(gameInstance) {}

// Display the current round on LCD
void Challenge5::displayRound(uint8_t round) {
    lcd.clear();
    lcd.print("Round ", 0, 0);
    lcd.print(String(round).c_str(), 6, 0);
}

// Display Morse Code by manipulating LEDs and buzzer
void Challenge5::playMorseCode(const String& morseCode) {
    for (uint8_t i = 0; i < morseCode.length(); i++) {
        if (morseCode.charAt(i) == '.') {
            game.playBuzzer(game.getShortBeepDuration() - 1);  // short beep for '.'
        } else if (morseCode.charAt(i) == '-') {
            game.playBuzzer(game.getLongBeepDuration() + 1);  // long beep for '-'
        }
        tm.setLEDs(0x0000);
        delay(1000); // Delay to help distinguish between beeps
    }
}

// Get user input within the timeout period
uint8_t Challenge5::getUserInput(unsigned long startTime) {
    while (!game.isTimedOut(startTime, timeout)) {
        game.displayRemainingTimeOnLED(timeout, startTime);
        uint16_t buttons = tm.readButtons();
        for (uint8_t i = 0; i <= 8; i++) {
            if (buttons & (1 << i)) {
                return i;    // return the number of the pressed button
            }
        }
    }
    return 0; // return 0 if no input was received within the timeout period
}

void Challenge5::playRounds(){
  for (uint8_t round = 1; round <= maxRounds; round++) {
          displayRound(round);
          uint8_t randomNumber = random(1, 8); 
          playMorseCode(morseCodes[randomNumber]);  // Display morse code for the random number

          unsigned long startTime = millis(); 
          uint8_t userInput = getUserInput(startTime); 
        
          if (randomNumber == userInput) {
              game.correctAnswer();
          } else {
              game.wrongAnswer();
              break; 
          }
          game.displayScore(); 
          delay(2000);
  }
}

// Setup function for the challenge - Displays welcome messages on LCD
void Challenge5::setup() {
    lcd.print("Welcome to", 0, 0);
    lcd.print("Challenge - 5", 0, 1);
    delay(2000);
    lcd.clear();
    lcd.print("Guess the Morse", 0, 0);
    lcd.print("Code for numbers", 0, 1);
    delay(2000);
    lcd.print("Hints:", 0, 0);
    lcd.print("----- => 0", 0, 1);
    delay(1000);
    lcd.print(".---- => 1", 0, 0);
    lcd.print("-.... => 6", 0, 1);
}


void Challenge5::loop() {
  while(game.getScore() < 500){
    playRounds();
  }
  game.winnerMessage();
}
