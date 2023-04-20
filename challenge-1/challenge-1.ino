#include <LiquidCrystal_I2C.h>
#include <TM1638plus.h>

#define STB_PIN 7
#define CLK_PIN 6
#define DIO_PIN 5
const uint8_t buzzerPin = 9;
const uint16_t SHORT_BEEP_DURATION = 1;
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

//i added this because without this block lcd doesnt display properly
void generateSequence(uint8_t *sequence, uint8_t length);
void playBuzzer(uint16_t duration);
void correctAnswer();
void wrongAnswer();
void blinkLeds();
void playAlarm();

void setup() {
  // Initialize LCD and TM1638
  lcd.init();
  tm.init();

  // Show message on LCD
  lcd.print("PUT THE NUMBERS");

  // Initialize buzzer pin and random seed
  pinMode(buzzerPin, OUTPUT);
  randomSeed(analogRead(0));
}
void loop() {
  uint8_t sequence[sequenceLength];

  // Generate and display the sequence
  generateSequence(sequence, sequenceLength);
  tm.displaySequence(sequence, sequenceLength);
  delay(3000);

  tm.reset();
  lcd.clear();

  uint8_t inputIndex = 0;
  bool isCorrect = true;

  while (inputIndex < sequenceLength && isCorrect) {
    uint16_t buttons = tm.readButtons();

    for (uint8_t i = 1; i <= 8; i++) {
      if (buttons & (1 << (i - 1))) {
        if (sequence[inputIndex] == i) {
          tm.setLED(0xff, inputIndex);
          lcd.print(String(i).c_str(), inputIndex * 4, 0);
          playBuzzer(SHORT_BEEP_DURATION);
          inputIndex++;
        } else {
          isCorrect = false;
        }
        delay(200);
      }
    }
  }

  if (isCorrect) {
    correctAnswer();
  } else {
    wrongAnswer();
  }

  delay(3000);
  tm.reset();
  lcd.clear();
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
  tm.setLEDs(0xFF00);
  lcd.print("Correct!");
}

void wrongAnswer() {
  blinkLeds();
  playAlarm();
  lcd.print("Incorrect!", 0, 0);
  lcd.print("TRY AGAIN", 0, 1);
}

void blinkLeds() {
  for (int i = 0; i < 3; i++) {
    tm.setLEDs(0xFF00);
    delay(500);
    tm.setLEDs(0x0000);
    delay(500);
  }
}

void playAlarm() {
  for (int i = 0; i < 5; i++) {
    playBuzzer(SHORT_BEEP_DURATION);
    delay(50);
  }
}
