#include "Challenge1.h"
#include "GameManager.h"


Challenge1::Challenge1(CustomTM1638& tmInstance, CustomLCD& lcdInstance, GameManager& gameInstance) 
  : tm(tmInstance), 
    lcd(lcdInstance), 
    game(gameInstance) 
{}

void Challenge1::playRound(unsigned long &currentTimeout) {
  // Declare and generate the sequence
  uint8_t sequence[sequenceLength];
  game.generateSequence(sequence, sequenceLength);
  
  // Display the generated sequence to the user
  tm.displaySequence(sequence, sequenceLength);
  delay(1200);

  tm.reset();
  lcd.clear();

  // Get and process the user input, checking if it's correct
  bool isCorrect = processInputs(sequence, currentTimeout);

  // Display the result of the round
  displayResult(isCorrect);
  
  // Adjust the difficulty based on the result
  adjustDifficulty(isCorrect, currentTimeout);

  game.displayScore();
  delay(2000);
  tm.reset();
  lcd.clear();
}

bool Challenge1::processInputs(const uint8_t *sequence, unsigned long currentTimeout) {
  uint8_t inputIndex = 0;
  bool isCorrect = true;
  unsigned long startTime = millis();

  // Loop to process each input, stopping if the sequence is wrong
  while (inputIndex < sequenceLength && isCorrect) {
    uint16_t buttons = tm.readButtons();
    for (uint8_t i = 1; i <= 8; i++) {
      if (buttons & (1 << (i - 1))) {
        if (sequence[inputIndex] == i) {
          handleCorrectInput(inputIndex, i);
          inputIndex++;
        } else {
          isCorrect = false;
        }
        delay(200);
      }
    }
    game.displayRemainingTimeOnLED(currentTimeout, startTime);
    // Check for timeout
    if (game.isTimedOut(startTime, currentTimeout)) {
      isCorrect = false;
      break;
    }
  }

  return isCorrect;
}

void Challenge1::adjustDifficulty(bool isCorrect, unsigned long &currentTimeout) {
  // Adjust the difficulty based on the correctness of the user input
  if (isCorrect) {
    // Decrease the time limit to increase difficulty
    unsigned long decrementFactor = 500;
    currentTimeout -= decrementFactor;
  } else {
    //Reset the timeout(difficulty)
    currentTimeout = 5000;
  }
}

void Challenge1::handleCorrectInput(uint8_t inputIndex, uint8_t inputValue) {
  // Light up the correct LED and play a short beep
  tm.setLED(0xff, inputIndex);
  game.playBuzzer(game.getShortBeepDuration() - 1);
}

void Challenge1::displayResult(bool isCorrect) {
  if (isCorrect) {
    game.correctAnswer();
  } else {
    game.wrongAnswer();
  }
}


void Challenge1::setup() {
  lcd.print("Welcome to", 0, 0);
  lcd.print("Challenge - 1", 0, 1);
  delay(2000);

  lcd.clear();
  lcd.print("Put numbers u");
  lcd.print("see on the LED",0,1);
  delay(2000);
  lcd.clear();
}

void Challenge1::loop() {
  // Continue playing rounds until the score reaches 100
  while (game.getScore() < 100) {
    playRound(currentTimeout);
  }
  game.handleChallenge2();
}
