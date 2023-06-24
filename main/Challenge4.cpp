#include "Challenge4.h"
#include "GameManager.h"


Challenge4::Challenge4(CustomTM1638& tmInstance, CustomLCD& lcdInstance, GameManager& gameInstance)
: tm(tmInstance),
  lcd(lcdInstance),
  game(gameInstance)
{}


// The lightRandomLED method randomly lights up an LED.
uint8_t Challenge4::lightRandomLED(){
  // Flash the LEDs four times.
  for (int i = 1; i < 5; i++) {
    flashLeds();
  }
  uint8_t ledNum = random(0,9); // Select a random LED to light up.
  delay(speedOfLED);
  blinkLed(ledNum); 
  tm.reset(); 
  return ledNum; // Return the number of the LED that was lit up.
}

// The flashLeds method makes the LEDs blink in a sequence.
void Challenge4::flashLeds(){
  for (int i = 1; i < 9; i++) {
    blinkLed(i);
  }
  for (int i = 8; i > 0; i--) {
    blinkLed(i);
  }
}

// The checkUserInput method checks the user's input against the correct position.
bool Challenge4::checkUserInput(uint8_t correctPos) {
  unsigned long startTime = millis(); 
  uint8_t currentInput = 0; 
  // Loop until the game times out.
  while (!game.isTimedOut(startTime, timeout)) {
    game.displayRemainingTimeOnLED(timeout, startTime);
    // Check if any button on the TM1638 is pressed.
    if (tm.readButtons() != 0) {
      currentInput = tm.getButtonNumber(tm.readButtons()); // Get the number of the pressed button.
      if (currentInput == correctPos) { // If the button number matches the correct position, the input is correct.
        tm.reset(); 
        return true; 
      } else {
        return false;
      }
    }
  }
  game.timeOutMessage(); 
  return false; 
}

  
// The blinkLed method makes an LED blink.
void Challenge4::blinkLed(uint8_t ledPos) {
  tm.setLED(ledPos, 0x01); 
  delay(speedOfLED); 
  tm.setLEDs(0x00);
  delay(speedOfLED); 
}

// The resetGame method resets the game.
void Challenge4::resetGame() {
  lcd.clear(); 
  tm.reset(); 
  speedOfLED = 30; 
}

void Challenge4::playRounds(){
// Loop over each round.
  for (currentRound = 0; currentRound < maxRounds; currentRound++) {
    uint8_t correctPos = lightRandomLED(); 
    
    // Check if user's input matches the correct LED position.
    if(checkUserInput(correctPos)){
      game.correctAnswer();
      speedOfLED -= 5; // To increase the difficulty we increase the speed of displaying the sequence.
      lcd.clear();
    } else {
      game.wrongAnswer(); // If user's input is incorrect, register it as a wrong answer.
      break;
    }
  }
}


// The setup method initializes the display messages on the LCD.
void Challenge4::setup() {
  lcd.print("Welcome to", 0, 0); 
  lcd.print("Challenge - 4", 0, 1); 
  delay(2000);
  lcd.clear(); 
  lcd.print("Input the sequence", 0, 0); 
  lcd.print("u see on LED", 0, 1);
  delay(2000);
}

// The loop method checks the user's input against the correct position.
void Challenge4::loop() {
  while(game.getScore() < 400){
    playRounds();
  }
  game.handleChallenge5();
}


