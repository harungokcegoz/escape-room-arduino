#include "GameManager.h"


CustomLCD lcd(0x27, 16, 2);
CustomTM1638 tm(STB_PIN, CLK_PIN, DIO_PIN);


GameManager::GameManager() : currentChallenge(1) {}

void GameManager::manageChallenges() {

  lcd.init();
  tm.init();
  pinMode(buzzerPin, OUTPUT);
  randomSeed(analogRead(0));
  lcd.print("U have 10 min", 0, 0);
  lcd.print("to open case!", 0, 1);
  delay(2000);

  
  //15 min timer for whole process.
  while (!isTimedOut(startTime, 900000)) {
    handleChallenge1();
  }
  timeOutMessage();
} 

void GameManager::displayScore() {
  lcd.print("Score: ", 0, 1);
  lcd.print(String(score).c_str(), 7, 1);
  delay(3000);
  lcd.clear();
}

void GameManager::generateSequence(uint8_t *sequence, uint8_t length) {
  for (uint8_t i = 0; i < length; i++) {
    sequence[i] = random(1, 9);
  }
}

void GameManager::playBuzzer(uint16_t duration) {
  for (unsigned long i = 0; i < duration * 1000; i += 4) {
    digitalWrite(buzzerPin, HIGH);
    delayMicroseconds(500);
    digitalWrite(buzzerPin, LOW);
    delayMicroseconds(500);
  }
}

void GameManager::playAlarm() {
  for (int i = 0; i < 3; i++) {
    playBuzzer(SHORT_BEEP_DURATION);
    delay(50);
  }
}

void GameManager::blinkLeds() {
  for (int i = 0; i < 3; i++) {
    tm.setLEDs(0xFF00);
    delay(300);
    tm.setLEDs(0x0000);
    delay(300);
  }
}

bool GameManager::isTimedOut(unsigned long startTime, unsigned long timeout) {
  return (millis() - startTime) > timeout;
}

void GameManager::correctAnswer() {
  score += 25;
  lcd.clear();
  tm.setLEDs(0xFF00);
  lcd.print("Correct!");
  playBuzzer(LONG_BEEP_DURATION);
  delay(300);
  lcd.clear();
  tm.setLEDs(0x0000);
}

void GameManager::wrongAnswer() {
  blinkLeds();
  playAlarm();
  lcd.print("Game Over!", 0, 0);
  displayScore();
  score = 0;
}

void GameManager::timeOutMessage(){
  lcd.clear();
  tm.reset();
  lcd.print("Timeout!",0,0);
  playAlarm();
  lcd.clear();
}

void GameManager::displayRemainingTimeOnLED(unsigned long timeout, unsigned long startTime) {
  int remainingTime = getRemainingTime(startTime, timeout);
  tm.displayASCII(4, '0' + (remainingTime / 1000) % 10);
  tm.displayASCII(5, '0' + (remainingTime / 100) % 10);
  tm.displayASCII(6, '0' + (remainingTime / 10) % 10);
  tm.displayASCII(7, '0' + remainingTime % 10);
  tm.reset();
}

int GameManager::getRemainingTime(unsigned long startTime, unsigned long timeout) {
  unsigned long elapsedTime = millis() - startTime;
  return (timeout - elapsedTime) / 1000;
}

void GameManager::winnerMessage(){
  lcd.print("Congrats! ", 0, 1);
  lcd.print("You won the game! ", 0, 2);
  delay(2000);
  lcd.clear();
  displayScore();
}

void GameManager::handleChallenge1() {
  Challenge1 ch1(tm, lcd, *this);
  ch1.setup();
  ch1.loop();
}

void GameManager::handleChallenge2() {
  Challenge2 ch2(tm, lcd, *this);
  ch2.setup();
  ch2.loop();
}

void GameManager::handleChallenge3() {
  Challenge3 ch3(tm, lcd, *this);
  ch3.setup();
  ch3.loop();
}

void GameManager::handleChallenge4() {
  Challenge4 ch4(tm, lcd, *this);
  ch4.setup();
  ch4.loop();
}

void GameManager::handleChallenge5() {
  Challenge5 ch5(tm, lcd, *this);
  ch5.setup();
  ch5.loop();
}

int GameManager::getScore() {
  return score;
}

int GameManager::getShortBeepDuration() {
  return SHORT_BEEP_DURATION;
}

int GameManager::getLongBeepDuration() {
  return LONG_BEEP_DURATION;
}

int GameManager::getPotentiometerPin() {
  return potentiometerPin;
}