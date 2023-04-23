#include <LiquidCrystal_I2C.h>
#include <TM1638plus.h>

#define STB_PIN 7
#define CLK_PIN 6
#define DIO_PIN 5
#define buzzerPin 9
const uint16_t SHORT_BEEP_DURATION = 2;
const uint16_t LONG_BEEP_DURATION = 3;
const uint8_t sequenceLength = 4;

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
    void displaySequence(uint8_t *sequence, uint8_t length) {
      for (uint8_t i = 0; i < length; i++) {
        tm.displayASCII(i, '0' + sequence[i]);
      }
    }
    void setLED(uint8_t value, uint8_t pos) {
      tm.setLED(value, pos);
    }
    uint16_t readButtons() {
      return tm.readButtons();
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
  lcd.init();
  tm.init();
  lcd.print("PUT THE NUMBERS");
  pinMode(buzzerPin, OUTPUT);
  randomSeed(analogRead(0));
}

void loop() {
  unsigned long initialTimeout = 5000;
  unsigned long currentTimeout = initialTimeout;
  uint16_t score = 0;

  while (true) {
    playRound(currentTimeout, score);
  }
}

void playRound(unsigned long &currentTimeout, uint16_t &score) {
  uint8_t sequence[sequenceLength];

  generateSequence(sequence, sequenceLength);
  tm.displaySequence(sequence, sequenceLength);
  delay(1200);

  tm.reset();
  lcd.clear();

  bool isCorrect = processInputs(sequence, currentTimeout, score);
  displayResult(isCorrect);

  if (isCorrect) {
    increaseDifficulty(currentTimeout);
  } else {
    resetDifficulty(currentTimeout);
  }

  displayScore(score);
  delay(2000);
  tm.reset();
  lcd.clear();
}

bool processInputs(const uint8_t *sequence, unsigned long currentTimeout, uint16_t &score) {
  uint8_t inputIndex = 0;
  bool isCorrect = true;
  unsigned long startTime = millis();

  while (inputIndex < sequenceLength && isCorrect) {
    uint16_t buttons = tm.readButtons();

    for (uint8_t i = 1; i <= 8; i++) {
      if (buttons & (1 << (i - 1))) {
        if (sequence[inputIndex] == i) {
          handleCorrectInput(inputIndex, i, score);
          inputIndex++;
        } else {
          isCorrect = false;
        }
        delay(200);
      }
    }

    if (isTimedOut(startTime, currentTimeout)) {
      isCorrect = false;
      break;
    }
  }

  return isCorrect;
}

void handleCorrectInput(uint8_t inputIndex, uint8_t inputValue, uint16_t &score) {
  tm.setLED(0xff, inputIndex);
  lcd.print(String(inputValue).c_str(), inputIndex * 4, 0);
  playBuzzer(SHORT_BEEP_DURATION-1);
  score++;
}

void displayResult(bool isCorrect) {
  if (isCorrect) {
    correctAnswer();
  } else {
    wrongAnswer();
  }
}

void increaseDifficulty(unsigned long &currentTimeout) {
  unsigned long decrementFactor = 600;
  currentTimeout -= decrementFactor;
  if (currentTimeout < decrementFactor) {
    currentTimeout = decrementFactor;
  }
}

void resetDifficulty(unsigned long &currentTimeout) {
  unsigned long initialTimeout = 5000;
  currentTimeout = initialTimeout;
}

void displayScore(uint16_t score) {
  lcd.print("Score: ", 0, 1);
  lcd.print(String(score).c_str(), 7, 1);
}

void generateSequence(uint8_t *sequence, uint8_t length) {
  for (uint8_t i = 0; i < length; i++) {
    sequence[i] = random(1, 9);
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

void correctAnswer() {
  lcd.clear();
  tm.setLEDs(0xFF00);
  lcd.print("Correct!");
  playBuzzer(LONG_BEEP_DURATION);
}

void wrongAnswer() {
  blinkLeds();
  playAlarm();
  lcd.print("Game Over!", 0, 0);
}

void blinkLeds() {
  for (int i = 0; i < 3; i++) {
    tm.setLEDs(0xFF00);
    delay(300);
    tm.setLEDs(0x0000);
    delay(300);
  }
}

bool isTimedOut(unsigned long startTime, unsigned long timeout) {
  return (millis() - startTime) > timeout;
}

void playAlarm() {
  for (int i = 0; i < 3; i++) {
    playBuzzer(SHORT_BEEP_DURATION);
    delay(50);
  }
}
