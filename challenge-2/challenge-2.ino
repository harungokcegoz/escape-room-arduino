#include <LiquidCrystal_I2C.h>
#include <TM1638plus.h>

#define STB_PIN 7
#define CLK_PIN 6
#define DIO_PIN 5
const uint8_t buzzerPin = 9;
const uint16_t SHORT_BEEP_DURATION = 1;
const uint16_t LONG_BEEP_DURATION = 3;
const uint8_t sequenceLength = 4;
const uint8_t potentiometerPin = 8;
const uint8_t targetPrecision = 5;
const uint16_t gameTime = 20000; 
const uint8_t maxRounds = 5;
const float smoothingFactor = 0.1;
float smoothedPotValue = 0;


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


void generateSequence(uint8_t *sequence, uint8_t length);
void playBuzzer(uint16_t duration);
void correctAnswer();
void wrongAnswer();
void blinkLeds();
void playAlarm();

int roundResults[maxRounds];
int currentRound = 0;
int currentScore = 0;


void setup() {
  lcd.init();
  tm.init();

  pinMode(buzzerPin, OUTPUT);
  pinMode(potentiometerPin, INPUT);

  randomSeed(analogRead(0));
}


void loop() {
  if(currentRound < maxRounds) {
    int targetValue = random(350, 999);
    tm.displayASCII(0, '0' + (targetValue / 100) % 10);
    tm.displayASCII(1, '0' + (targetValue / 10) % 10);
    tm.displayASCII(2, '0' + targetValue % 10);

    unsigned long startTime = millis();
    unsigned long elapsedTime = 0;
    bool isCorrect = false;

    while (elapsedTime < gameTime && !isCorrect) {
      int potValue = analogRead(potentiometerPin);
      int adjustedValue = map(potValue, 350, 1023, 0, 2000);

      lcd.clear();
      lcd.print("Value:", 0, 0);
      lcd.print(adjustedValue, 6, 0);

      if (abs(targetValue - adjustedValue) <= targetPrecision) {
        isCorrect = true;
        break;
      }
      elapsedTime = millis() - startTime;
      delay(100);
    }

    if (isCorrect) {
      correctAnswer();
    } else {
      wrongAnswer();
    }
    currentRound++;
    delay(3000);
    tm.reset();
    lcd.clear();
  } else {
    lcd.clear();
    displayResults();
    delay(5000);
    resetGame();
  }
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
  roundResults[currentRound] = 1;
  currentScore++;
  lcd.clear();
  lcd.print("Correct!");
  tm.setLEDs(0xFF00);
  playBuzzer(SHORT_BEEP_DURATION);
}

void wrongAnswer() {
  roundResults[currentRound] = 0;
  lcd.clear();
  lcd.print("Incorrect!", 0, 0);
  blinkLeds();
  playAlarm();
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

void displayResults() {
  lcd.clear();
  lcd.print("RESULT", 0, 0);
  int score = (currentScore * 100) / maxRounds;
  lcd.print("Score:", 0, 1);
  lcd.print(score, 7, 1);
}

void resetGame() {
  currentRound = 0;
  for (int i = 0; i < maxRounds; i++) {
    roundResults[i] = 0;
  }
}

int getScore() {
  int correctAnswers = 0;
  for (int i = 0; i < maxRounds; i++) {
    if (roundResults[i] == 1) {
      correctAnswers++;
    }
  }
  return (correctAnswers * 100) / maxRounds;
}
