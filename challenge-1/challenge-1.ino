#include <LiquidCrystal_I2C.h>
#include <TM1638plus.h>


#define STB_PIN 7
#define CLK_PIN 6
#define DIO_PIN 5

LiquidCrystal_I2C lcd(0x27, 16, 2);

const uint8_t buzzerPin = 9; 
const uint16_t SHORT_BEEP_DURATION = 1; 
const uint16_t LONG_BEEP_DURATION = 3; 

TM1638plus tm(STB_PIN, CLK_PIN, DIO_PIN);


void generateSequence(uint8_t *sequence, uint8_t length);
void displaySequence(uint8_t *sequence, uint8_t length);
void playBuzzer(uint16_t duration);

void setup() {
  // Initialize LCD
  lcd.init();
  lcd.backlight();


  tm.displayBegin();
  tm.brightness(7);
  lcd.print("PUT THE NUMBERS");

  pinMode(buzzerPin, OUTPUT);



  randomSeed(analogRead(0));
}

void loop() {
  const uint8_t sequenceLength = 4;
  uint8_t sequence[sequenceLength];

  // Generate and display the sequence
  generateSequence(sequence, sequenceLength);
  displaySequence(sequence, sequenceLength);
  delay(3000);

  tm.reset(); 

  uint8_t inputIndex = 0;
  bool isCorrect = true;

  lcd.clear(); 

  while (inputIndex < sequenceLength && isCorrect) {
    uint16_t buttons = tm.readButtons();

    for (uint8_t i = 1; i <= 8; i++) {
      if (buttons & (1 << (i - 1))) {
        if (sequence[inputIndex] == i) {
          // Correct input
          tm.setLED(0xff, inputIndex); 
          lcd.setCursor(inputIndex * 4, 0);
          lcd.print(i); 
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

void displaySequence(uint8_t *sequence, uint8_t length) {
  for (uint8_t i = 0; i < length; i++) {
    tm.displayASCII(i, '0' + sequence[i]);
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
void correctAnswer() {
    tm.setLEDs(0xFF00);
    lcd.setCursor(0, 0);
    lcd.print("Correct!");
}
void wrongAnswer() {
    blinkLeds();
    playAlarm();
    lcd.setCursor(0, 0);
    lcd.print("Incorrect!");
    lcd.setCursor(0, 1);
    lcd.print("TRY AGAIN");
}