#include "Challenge3.h"
#include "GameManager.h"


Challenge3::Challenge3(CustomTM1638& tmInstance, CustomLCD& lcdInstance, GameManager& gameInstance) 
  : tm(tmInstance), 
    lcd(lcdInstance), 
    game(gameInstance) 
{}


// displays the round number and current score on the lcd
void Challenge3::displayRoundNumber(uint8_t round) {
    lcd.clear();
    lcd.print("Round: ", 0, 0);
    lcd.print(round, 7, 0);
    lcd.print("Score:", 0, 1);
    lcd.print(game.getScore(), 7, 1);
}

// displays the given sequence on the LEDs
void Challenge3::displaySequence(uint8_t sequence[], uint8_t length) {
  for (uint8_t i = 0; i < length; i++) {
    // showing LED and playing buzzer for each sequence element
    showLEDAndPlayBuzzer(sequence[i]);
    delay(200);
  }
}

// shows the given LED number and plays the buzzer
void Challenge3::showLEDAndPlayBuzzer(uint8_t ledNumber) {
  tm.setLED(ledNumber, 0x01);
  delay(400);
  game.playBuzzer(game.getShortBeepDuration() - 1);
  tm.setLEDs(0x00);
}

// checks the user's input against the given sequence
bool Challenge3::checkUserInput(uint8_t sequence[], uint8_t length, unsigned long timeout) {
  // array to hold the user's button presses
  uint16_t buttonsArray[length];
  uint8_t currentIndex = 0;
  unsigned long startTime = millis();

  // wait for the user to press all buttons in the sequence
  while (currentIndex < length) {
    // display the remaining time on the LEDs
    game.displayRemainingTimeOnLED(timeout, startTime);
    // read and handle button presses
    while (tm.readButtons() != 0) {
      currentIndex = handleButtonPress(currentIndex, buttonsArray);
    }
    // check if the user ran out of time
    if (game.isTimedOut(startTime, timeout)) {
      return false;
    }
  }
  // compare the user's input to the sequence
  return isUserInputCorrect(sequence, buttonsArray, length);
}

// handles a button press and updates the button array
uint8_t Challenge3::handleButtonPress(uint8_t currentIndex, uint16_t buttonsArray[]) {
  uint16_t button = tm.readButtons();
  uint8_t buttonNumber = tm.getButtonNumber(button);
  showLEDAndPlayBuzzer(buttonNumber);
  buttonsArray[currentIndex] = buttonNumber;
  return ++currentIndex;
}

// checks if the user's input matches the given sequence
bool Challenge3::isUserInputCorrect(uint8_t sequence[], uint16_t buttonsArray[], uint8_t length) {
  for (int i = 0; i < length; i++) {
    if (sequence[i] != buttonsArray[i]) {
      return false;
    }
  }
  return true;
}

void Challenge3::playRounds(){
  uint8_t currentSequence[sequenceMaxLength];
  for (uint8_t round = 1; round <= sequenceMaxLength; round++) {
    displayRoundNumber(round);
    // generate and display a new sequence for the current round
    game.generateSequence(currentSequence, sequenceLength);
    displaySequence(currentSequence, sequenceLength);
    // check user input and react accordingly
    if (checkUserInput(currentSequence, sequenceLength, timeout)) {
        game.correctAnswer();
        // Increasing the difficulty of the challenge.
        sequenceLength++;
        delay(1000);
    } else {
        game.wrongAnswer();
        game.timeOutMessage();
        break;
    }
  }
}

// setup function - presents the challenge instructions to the user
void Challenge3::setup() {
    lcd.print("Welcome to", 0, 0);
    lcd.print("Challenge - 3", 0, 1);
    delay(2000);
    lcd.clear();
    lcd.print("Input the sequence", 0, 0);
    lcd.print("u see on LED", 0, 1);
    delay(2000);
}

// loop function - iterates over the challenge rounds and handles gameplay
void Challenge3::loop() {
    while(game.getScore() < 300){
      playRounds();
    }
    game.handleChallenge4();
}


