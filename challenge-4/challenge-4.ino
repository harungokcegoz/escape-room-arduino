#include <LiquidCrystal_I2C.h>
#include <TM1638plus.h>

#define STB_PIN 7
#define CLK_PIN 6
#define DIO_PIN 5
#define buzzerPin 9
const uint16_t SHORT_BEEP_DURATION = 2;
const uint16_t LONG_BEEP_DURATION = 5;
const uint8_t maxRounds = 5;
uint8_t currentRound;
uint8_t difficulty = 40;
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
  initialize();
}

void loop() {
  playReactionGame();
  delay(1000);
}

void initialize() {
  lcd.init();
  tm.init();
  pinMode(buzzerPin, OUTPUT);
  randomSeed(analogRead(0));
  displayRoundMessage();
}

void playReactionGame() {
  lcd.clear();
  if(currentRound == 5){
    lcd.clear();
    lcd.print("YOU WON!",0,0);
    delay(3000);
    resetGame();
  }
  for (currentRound = 0; currentRound < maxRounds; currentRound++) {
    displayRoundMessage();
    uint8_t correctPos = lightRandomLED();
    if(checkUserInput(correctPos)){
      correctAnswer();
      lcd.clear();
    } else {
      gameOver();
      break;
    }
  }
}

uint8_t lightRandomLED(){
  for (int i = 1; i < 5; i++) {
    flashLeds();
  }
  uint8_t ledNum = random(0,9);
  delay(difficulty);
  blinkLed(ledNum);
  tm.reset();
  return ledNum;
}
void flashLeds(){
  for (int i = 1; i < 9; i++) {
    blinkLed(i);
  }
  for (int i = 8; i > 0; i--) {
    blinkLed(i);
  }
}
void blinkLed(uint8_t ledPos) {
  tm.setLED(ledPos, 0x01);
  delay(difficulty);
  tm.setLEDs(0x00);
  delay(difficulty);
}

bool checkUserInput(uint8_t correctPos) {
  unsigned long startTime = millis();
  unsigned long lastUpdateTime = 0;
  uint8_t currentInput = 0;
  int remainingTime = 5;
  tm.displayASCII(0, '5');
  tm.displayASCII(5, 's');

  while (millis() - startTime < 5000) {
    unsigned long currentTime = millis();

    if (tm.readButtons() != 0) {
      currentInput = tm.getButtonNumber(tm.readButtons());
      if (handleButtonPress(currentInput, correctPos)) {
        tm.reset(); // Clear the display when the input is received
        return true;
      } else {
        return false;
      }
    }

    if (currentTime - startTime > (5000 - remainingTime * 1000)) {
      remainingTime--;
      tm.displayASCII(0, '0' + remainingTime);
      lastUpdateTime = currentTime;
    }

    if (currentTime - lastUpdateTime >= 50) {
      lastUpdateTime = currentTime;
    }
  }

  timeOutMessage();
  return false;
}

void timeOutMessage(){
  lcd.clear();
  tm.reset();
  lcd.print("Timeout!",0,0);
  delay(3000);
  playAlarm();
  gameOver();
}
bool handleButtonPress(uint8_t currentInput, uint8_t correctPos){
  if(currentInput == correctPos){
    return true;
  } else {
    return false;
  }
}
void wrongAnswer() {
  lcd.print("Incorrect Input!", 0, 0);
  blinkLeds();
  playAlarm();
  displayResults();
  resetGame();
}
void correctAnswer() {
  lcd.clear();
  lcd.print("Correct!");
  tm.setLEDs(0xFF00);
  playBuzzer(SHORT_BEEP_DURATION);
  tm.setLEDs(0x0000);
  score += 20;
  difficulty -= 5;
}

void gameOver() {
  lcd.clear();
  lcd.print("Game Over!", 0, 0);
  blinkLeds();
  playAlarm();
  displayResults();
}
void displayRoundMessage() {
  lcd.print("Round: ", 0, 0);
  lcd.print(currentRound + 1, 6, 0);
}

void displayResults() {
  lcd.clear();
  lcd.print("Results:", 0, 0);
  lcd.print("Score:", 0, 1);
  lcd.print(score, 7, 1);
}

void resetGame() {
  lcd.clear();
  tm.reset();
  difficulty = 30;
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
