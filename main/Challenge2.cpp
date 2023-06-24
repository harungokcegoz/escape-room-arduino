#include "Challenge2.h"
#include "GameManager.h"

Challenge2::Challenge2(CustomTM1638& tmInstance, CustomLCD& lcdInstance, GameManager& gameInstance) 
  : tm(tmInstance), 
    lcd(lcdInstance), 
    game(gameInstance) 
{}

  void Challenge2::playRound() {
    // target value for the potentiometer in this round
    int targetValue = random(400, 999);

    // display target value on the led display
    displayTargetValue(targetValue);

    // start time of this round
    unsigned long startTime = millis();

    bool isCorrect = false;
    int previousRemainingTime = -1;
    int previousAdjustedValue = -1;

    // keep checking if the user input is correct or if time has run out
    while (!isCorrect && !game.isTimedOut(startTime, gameTime)) {
      delay(200);

      // read value from the potentiometer
      int adjustedValue = readPotentiometer();

      // calculate the remaining time
      int remainingTime = game.getRemainingTime(startTime, gameTime);
      // if the remaining time or the adjusted value has changed, update the display
      if (remainingTime != previousRemainingTime || adjustedValue != previousAdjustedValue) {
        displayValueAndTime(adjustedValue, remainingTime);
        previousRemainingTime = remainingTime;
        previousAdjustedValue = adjustedValue;
      }

      // check if the user input is within the acceptable range of the target value
      isCorrect = isValueCorrect(targetValue, adjustedValue);
    }

    // handle the end of the round
    if (isCorrect) {
      game.correctAnswer();
      correctNumbersCount++;
    } else{
      game.timeOutMessage();
    }

    lcd.clear();
    tm.reset();
  }


  void Challenge2::displayTargetValue(int targetValue) {
    tm.displayASCII(0, '0' + (targetValue / 100) % 10);
    tm.displayASCII(1, '0' + (targetValue / 10) % 10);
    tm.displayASCII(2, '0' + targetValue % 10);
  }

  int Challenge2::readPotentiometer() {
    int potValue = analogRead(game.getPotentiometerPin());
    return map(potValue, 350, 1023, 350, 1023);
  }

  void Challenge2::displayValueAndTime(int adjustedValue, int remainingTime) {
    lcd.clear();
    lcd.print("Value:", 0, 0);
    lcd.print(adjustedValue, 6, 0);
    tm.displayASCII(4, '0' + (remainingTime / 1000) % 10);
    tm.displayASCII(5, '0' + (remainingTime / 100) % 10);
    tm.displayASCII(6, '0' + (remainingTime / 10) % 10);
    tm.displayASCII(7, '0' + remainingTime % 10);
  }

  bool Challenge2::isValueCorrect(int targetValue, int adjustedValue) {
    return abs(targetValue - adjustedValue) <= targetPrecision;
  }


  void Challenge2::resetGame() {
    correctNumbersCount = 0;
    for (int i = 0; i < 4; i++) {
      lockerCode[i] = -1;
    }
  }

  void Challenge2::lockerUnlocked() {
    lcd.clear();
    tm.setLEDs(0xFF00);
    game.playBuzzer(game.getLongBeepDuration());
    lcd.print("Success!", 0, 0);
    lcd.print("Opened!", 0, 1);
    delay(3000);
    tm.setLEDs(0x00);
    resetGame();
  }

  void Challenge2::displayLockerCode(){
    uint8_t randomNumber = random(0, 9);
    lcd.print("Locker Code:",0,1);
    lockerCode[correctNumbersCount] = randomNumber;
    for (int i = 0; i <= correctNumbersCount; i++) {
      lcd.print(lockerCode[i], i + 12, 1);
    }
  }

  void Challenge2::setup() {
  lcd.print("Welcome to", 0, 0);
  lcd.print("Challenge - 2", 0, 1);
  delay(2000);
  lcd.clear();
  lcd.print("Adjust pot to", 0, 0);
  lcd.print("match target.", 0, 1);
  delay(2000);
  lcd.clear();
}

void Challenge2::loop() {
  while(game.getScore() < 200){
    while (correctNumbersCount < 4) {
      playRound();
    }
    lockerUnlocked();
  }
  game.handleChallenge3();
}
