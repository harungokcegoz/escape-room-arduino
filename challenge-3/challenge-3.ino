#include <LiquidCrystal_I2C.h>
#include <TM1638plus.h>

#define STB_PIN 7
#define CLK_PIN 6
#define DIO_PIN 5
#define buzzerPin 9

const uint16_t SHORT_BEEP_DURATION = 2;
const uint16_t LONG_BEEP_DURATION = 5;
const uint8_t sequenceMaxLength = 5;
uint8_t score = 0;

class CustomLCD {
  public:
    CustomLCD(uint8_t addr, uint8_t cols, uint8_t rows) : lcd(addr, cols, rows) {}
    void init() {
      lcd.init();
      lcd.backlight();
    }
    void clear() {
      lcd.clear();
    }
    void print(const char *str, uint8_t col = 0, uint8_t row = 0) {
      lcd.setCursor(col, row);
      lcd.print(str);
    }
    void setCursor(uint8_t col, uint8_t row) {
      lcd.setCursor(col, row);
    }
    void print(int value, uint8_t col = 0, uint8_t row = 0) {
      lcd.setCursor(col, row);
      lcd.print(value);
    }
  private:
    LiquidCrystal_I2C lcd;
};


class CustomTM1638 {
  public:
    CustomTM1638(uint8_t stb, uint8_t clk, uint8_t dio) : tm(stb, clk, dio) {}
    void init() {
      tm.displayBegin();
      tm.brightness(7);
    }
    void reset() {
      tm.reset();
    }
    void displayASCII(uint8_t pos, uint8_t ascii) {
      tm.displayASCII(pos, ascii);
    }
    void setLED(uint8_t pos, uint8_t value) {
      tm.setLED(pos-1, value);
    }
    uint16_t readButtons() {
      return tm.readButtons();
    }
    int8_t getButtonNumber(uint16_t buttons) {
      uint8_t buttonIndex = 0;
      uint8_t buttonValue = tm.readButtons();
      for (int i = 0; i < 8; i++) {
        if ((buttons >> i) & 0x01) {
            if (buttonValue == 1) {
                buttonIndex = 1;
            } else if (buttonValue == 2) {
                buttonIndex = 2;
            } else if (buttonValue == 4) {
                buttonIndex = 3;
            } else if (buttonValue == 8) {
                buttonIndex = 4;
            } else if (buttonValue == 16) {
                buttonIndex = 5;
            } else if (buttonValue == 32) {
                buttonIndex = 6;
            } else if (buttonValue == 64) {
                buttonIndex = 7;
            } else if (buttonValue == 128) {
                buttonIndex = 8;
            }
            break;
        }
      }
      return buttonIndex;
    }
    void setLEDs(uint16_t value) {
      tm.setLEDs(value);
    }
  private:
    TM1638plus tm;
};

 
CustomLCD lcd(0x27, 16, 2);
CustomTM1638 tm(STB_PIN, CLK_PIN, DIO_PIN);


void setup() {
  initializeGame();
}

void loop() {
  playGame();
}
void initializeGame() {
  pinMode(buzzerPin, OUTPUT);
  lcd.init();
  tm.init();
}
void playGame() {
    uint8_t currentSequence[sequenceMaxLength];
  uint8_t sequenceLength = 3;

  for (uint8_t round = 1; round <= sequenceMaxLength; round++) {
    displayRoundNumber(round);
    generateRandomSequence(currentSequence, sequenceLength);
    displaySequence(currentSequence, sequenceLength);

    if (checkUserInput(currentSequence, sequenceLength, 5000)) {
      correctAnswer();
      sequenceLength++;
      delay(1000);
    } else {
      wrongAnswer();
      break;
    }
  }
  displayGameOver();
}
void displayRoundNumber(uint8_t round) {
  lcd.clear();
  lcd.print("Round: ", 0, 0);
  lcd.print(round, 7, 0);
  lcd.print("Score:", 0, 1);
  lcd.print(score, 7, 1);
}

void displayGameOver() {
  lcd.clear();
  lcd.print("Game Over", 0, 0);
  delay(3000);
}
void generateRandomSequence(uint8_t sequence[], uint8_t length) {
  for (uint8_t i = 0; i < length; i++) {
    sequence[i] = random(1, 9);
  }
}

void displaySequence(uint8_t sequence[], uint8_t length) {
  for (uint8_t i = 0; i < length; i++) {
    showLEDAndPlayBuzzer(sequence[i]);
    delay(200);
  }
}
void showLEDAndPlayBuzzer(uint8_t ledNumber) {
  tm.setLED(ledNumber, 0x01);
  delay(400);
  playBuzzer(SHORT_BEEP_DURATION - 1);
  tm.setLEDs(0x00);
}
// bool checkUserInput(uint8_t sequence[], uint8_t length, unsigned long timeout) {
//   uint16_t buttonsArray[length];
//   uint8_t currentIndex = 0;
//   unsigned long startTime = millis();

//   while (currentIndex < length) {
//     while (tm.readButtons() != 0) {
//       currentIndex = handleButtonPress(currentIndex, buttonsArray);
//     }
//     if (isTimeout(startTime, timeout)) {
//       return false;
//     }
//   }
//   return isUserInputCorrect(sequence, buttonsArray, length);
// }
bool checkUserInput(uint8_t sequence[], uint8_t length, unsigned long timeout) {
  uint16_t buttonsArray[length];
  uint8_t currentIndex = 0;
  unsigned long startTime = millis();

  while (currentIndex < length) {
    displayRemainingTimeOnLED(timeout, startTime);
    while (tm.readButtons() != 0) {
      currentIndex = handleButtonPress(currentIndex, buttonsArray);
    }
    if (isTimeout(startTime, timeout)) {
      return false;
    }
  }
  return isUserInputCorrect(sequence, buttonsArray, length);
}

void displayRemainingTimeOnLED(unsigned long timeout, unsigned long startTime) {
  int remainingTime = getRemainingTime(startTime, timeout);
  tm.displayASCII(4, '0' + (remainingTime / 1000) % 10);
  tm.displayASCII(5, '0' + (remainingTime / 100) % 10);
  tm.displayASCII(6, '0' + (remainingTime / 10) % 10);
  tm.displayASCII(7, '0' + remainingTime % 10);
}

int getRemainingTime(unsigned long startTime, unsigned long timeout) {
  unsigned long elapsedTime = millis() - startTime;
  return (timeout - elapsedTime) / 1000;
}
uint8_t handleButtonPress(uint8_t currentIndex, uint16_t buttonsArray[]) {
  uint16_t button = tm.readButtons();
  uint8_t buttonNumber = tm.getButtonNumber(button);
  showLEDAndPlayBuzzer(buttonNumber);
  buttonsArray[currentIndex] = buttonNumber;
  return ++currentIndex;
}

bool isTimeout(unsigned long startTime, unsigned long timeout) {
  return millis() - startTime > timeout;
}

bool isUserInputCorrect(uint8_t sequence[], uint16_t buttonsArray[], uint8_t length) {
  for (int i = 0; i < length; i++) {
    if (sequence[i] != buttonsArray[i]) {
      return false;
    }
  }
  return true;
}

//// default methods
void correctAnswer() {
  lcd.clear();
  lcd.print("Correct!");
  tm.setLEDs(0xFF00);
  playBuzzer(SHORT_BEEP_DURATION);
  tm.setLEDs(0x0000);
  score += 25;
}

void wrongAnswer() {
  lcd.clear();
  lcd.print("Incorrect!", 0, 0);
  blinkLeds();
  playAlarm();
  score = 0;
}

void blinkLeds() {
  for (int i = 0; i < 3; i++) {
    tm.setLEDs(0xFF00);
    delay(500);
    tm.setLEDs(0x0000);
    delay(500);
  }
}

void playBuzzer(uint16_t duration) {
  for (unsigned long i = 0; i < duration * 1000; i += 4) {
    digitalWrite(buzzerPin, HIGH);
    delayMicroseconds(500);
    digitalWrite(buzzerPin, LOW);
    delayMicroseconds(500);
  }
}
void playAlarm() {
  for (int i = 0; i < 3; i++) {
    playBuzzer(SHORT_BEEP_DURATION);
    delay(50);
  }
}