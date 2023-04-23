#include <LiquidCrystal_I2C.h>
#include <TM1638plus.h>

#define STB_PIN 7
#define CLK_PIN 6
#define DIO_PIN 5
#define buzzerPin 9
#define potentiometerPin 8

const uint16_t SHORT_BEEP_DURATION = 1;
const uint16_t LONG_BEEP_DURATION = 3;
const uint16_t gameTime = 20000; 
const float smoothingFactor = 0.1;
uint8_t targetPrecision = 9;
float smoothedPotValue = 0;
int lockerCode[4];
int correctNumbersCount = 0;


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


void setup() {
  initializeGame();
}

void loop() {
  playGame();
}

void initializeGame() {
  lcd.init();
  tm.init();
  pinMode(buzzerPin, OUTPUT);
  pinMode(potentiometerPin, INPUT);
  randomSeed(analogRead(0));
}

void playGame() {
  while (correctNumbersCount < 4) {
    playRound();
  }
  lockerUnlocked();
}


void playRound() {
  int targetValue = generateTargetValue();
  displayTargetValue(targetValue);
  unsigned long startTime = millis();
  bool isCorrect = false;
  int previousRemainingTime = -1;
  int previousAdjustedValue = -1;

  while (!isCorrect && !isTimeUp(startTime)) {
    delay(200);
    int adjustedValue = readPotentiometer();
    int remainingTime = getRemainingTime(startTime);
    if (remainingTime != previousRemainingTime || adjustedValue != previousAdjustedValue) {
      displayValueAndTime(adjustedValue, remainingTime);
      previousRemainingTime = remainingTime;
      previousAdjustedValue = adjustedValue;
    }
    isCorrect = isValueCorrect(targetValue, adjustedValue);
  }

  if (isCorrect) {
    correctAnswer();
  } else{
    timeIsUp();
  }

  tm.reset();
  lcd.clear();
}

void timeIsUp(){
    lcd.clear();
    tm.reset();
    blinkLeds();
    playAlarm();
    lcd.print("Time is up.", 0, 0);
    lcd.print("Try again.", 0, 1);
    delay(3000);
}
int generateTargetValue() {
  return random(350, 999);
}

void displayTargetValue(int targetValue) {
  tm.displayASCII(0, '0' + (targetValue / 100) % 10);
  tm.displayASCII(1, '0' + (targetValue / 10) % 10);
  tm.displayASCII(2, '0' + targetValue % 10);
}

bool isTimeUp(unsigned long startTime) {
  return millis() - startTime >= gameTime;
}

int readPotentiometer() {
  int potValue = analogRead(potentiometerPin);
  return map(potValue, 350, 1023, 350, 1023);
}
int getRemainingTime(unsigned long startTime) {
  return (gameTime - (millis() - startTime)) / 1000;
}
void displayValueAndTime(int adjustedValue, int remainingTime) {
  lcd.clear();
  lcd.print("Value:", 0, 0);
  lcd.print(adjustedValue, 6, 0);
  tm.displayASCII(4, '0' + (remainingTime / 1000) % 10);
  tm.displayASCII(5, '0' + (remainingTime / 100) % 10);
  tm.displayASCII(6, '0' + (remainingTime / 10) % 10);
  tm.displayASCII(7, '0' + remainingTime % 10);
}

bool isValueCorrect(int targetValue, int adjustedValue) {
  return abs(targetValue - adjustedValue) <= targetPrecision;
}

void displayResult() {
  lcd.clear();
  tm.setLEDs(0xFF00);
  playBuzzer(SHORT_BEEP_DURATION);
  lcd.print("Success!", 0, 0);
  lcd.print("Opened!", 0, 1);
  delay(3000);
  lcd.clear();
  lcd.print("The new game is starting!", 0, 1);
  delay(3000);
}

void resetGame() {
  correctNumbersCount = 0;
  for (int i = 0; i < 4; i++) {
    lockerCode[i] = -1;
  }
}

void lockerUnlocked() {
  lcd.clear();
  tm.setLEDs(0xFF00);
  playBuzzer(LONG_BEEP_DURATION);
  lcd.print("Success!", 0, 0);
  lcd.print("Opened!", 0, 1);
  delay(3000);
  tm.setLEDs(0x00);
  resetGame();
}
//default
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
  playBuzzer(SHORT_BEEP_DURATION);
  lcd.print("Correct!");
  displayLockerCode();
  correctNumbersCount++;
  targetPrecision -= 2;
}

void displayLockerCode(){
  uint8_t randomNumber = random(0, 9);
  lcd.print("Locker Code:",0,1);
  lockerCode[correctNumbersCount] = randomNumber;
  for (int i = 0; i <= correctNumbersCount; i++) {
    lcd.print(lockerCode[i], i + 12, 1);
  }
}

void blinkLeds() {
  for (int i = 0; i < 3; i++) {
    tm.setLEDs(0xFF00);
    delay(500);
    tm.setLEDs(0x00);
    delay(500);
  }
}

void playAlarm() {
  for (int i = 0; i < 5; i++) {
    playBuzzer(SHORT_BEEP_DURATION);
    delay(50);
  }
}