#ifndef GAMEMANAGER_H
#define GAMEMANAGER_H

#include "CustomTM1638.h"
#include "CustomLCD.h"
#include "Challenge1.h"
#include "Challenge2.h"
#include "Challenge3.h"
#include "Challenge4.h"
#include "Challenge5.h"

#define STB_PIN 7
#define CLK_PIN 6
#define DIO_PIN 5
#define potentiometerPin 8
#define buzzerPin 9


class GameManager {
public:
  GameManager();
  void manageChallenges();
  void displayScore();
  void generateSequence(uint8_t *sequence, uint8_t length);
  void playBuzzer(uint16_t duration);
  void playAlarm();
  void blinkLeds();
  bool isTimedOut(unsigned long startTime, unsigned long timeout);
  void correctAnswer();
  void wrongAnswer();
  void timeOutMessage();
  void displayRemainingTimeOnLED(unsigned long timeout, unsigned long startTime);
  int getRemainingTime(unsigned long startTime, unsigned long timeout);
  int getScore();
  int getShortBeepDuration();
  int getLongBeepDuration();
  int getPotentiometerPin();
  void handleChallenge1();
  void handleChallenge2();
  void handleChallenge3();
  void handleChallenge4();
  void handleChallenge5();
  void winnerMessage();

private:
  unsigned long startTime = millis();
  uint16_t score = 0;
  uint8_t currentChallenge;
  uint16_t SHORT_BEEP_DURATION = 2;
  uint16_t LONG_BEEP_DURATION = 3;
};

#endif 