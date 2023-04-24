# ARDUINO CHALLENGES
 
## System Build
### 1
![IMG_5433](https://user-images.githubusercontent.com/94520965/234025297-c9233c41-9de6-4624-b4cd-96605150c159.png)

### 2
![IMG_5437](https://user-images.githubusercontent.com/94520965/234024840-2e6d20ca-4552-4b55-b738-2fdc8317ef8f.png)
### 3
![IMG_5438](https://user-images.githubusercontent.com/94520965/234024855-86357711-0f4f-4f73-9863-8840af33b2ed.png)
### 4
![IMG_5439](https://user-images.githubusercontent.com/94520965/234024868-bcafd8b0-6080-480c-999b-54bbf6f82009.png)


## Challenge-1
### Title: Fast Number Challenge
The goal of the Fast Number Challenge is to quickly enter a sequence of numbers displayed on the LED display within the given time limit. As user progress through the game, the time limit will decrease, making the game more challenging. The highest score will be recorded and displayed on the LCD screen.
### How to play:
1. Power on the device and wait for the game to initialize.
2. Observe the sequence of numbers displayed on the LED display.
3. Within the given time limit, press the buttons on the TM1638 module that correspond
to the sequence of numbers displayed on the LED display.
4. If you correctly enter the sequence, you will hear a long beep and see "Correct!" on
the LCD screen. Your score will be updated.
5. If you fail to enter the sequence within the time limit or press an incorrect button, the
game will end, and you will hear a series of short beeps. "Game Over!" will be
displayed on the LCD screen.
6. The game will restart automatically after a brief pause.

### Sequence:
1. The game initializes the LCD and TM1638 modules and sets up the buzzer.
2. In each round, the game generates a random sequence of numbers and displays it on
the TM1638 module.
3. The player must press the buttons corresponding to the displayed sequence within the
given time limit.
4. The game checks if the inputs are correct and updates the score accordingly.
5. The game increases the difficulty by decreasing the time limit and moves to the next
round if the player successfully enters the sequence.
6. If the player fails to enter the correct sequence, the game ends and restarts after a
brief pause.

## Challenge - 2
### Title: LockerCrack
The goal is to unlock the locker by finding the locker code by potentiometer within a limited amount of time.
### How to play
1. The game consists of a series of rounds, with each round presenting a target value on the TM1638 display.
2. Use the potentiometer to adjust a value on the LCD screen to match the target value within the given time limit.
3. The closer the adjusted value is to the target value, the higher your chances of success. You must achieve the required precision to move on to the next round.
4. As you progress, the required precision will decrease, making the game more challenging.
5. Successfully completing a round will reveal a digit of the locker code on the LCD screen.
6. Unlock the locker by completing all four rounds and revealing the entire locker code.

### Sequence of the challenge:
1. The game is initialized with the initializeGame() function.
2. The main game loop is entered, consisting of multiple rounds with the playGame()
function.
3. Each round begins by generating a target value and displaying it on the TM1638.
4. The player adjusts the potentiometer value, which is displayed on the LCD screen,
aiming to match the target value.
5. The remaining time is displayed on the TM1638, and the game checks if the adjusted
value is within the required precision.
6. If the player is successful, a part of the locker code is revealed, and the next round
begins with increased difficulty.
7. If the player fails, they must restart the round.
8. After all four rounds are completed, the locker is unlocked, and the game resets.

## Challenge - 3
### Title: Memory Challenge
Complete as many rounds as possible by correctly recalling the sequence of LED flashes.
### How to play:
1. Pay attention to the sequence of LED flashes displayed on the TM1638 module.
2. Use the buttons below each LED to repeat the sequence.
3. Keep an eye on the countdown timer, as you only have limited time to input the
sequence.
4. For each correct sequence, you will advance to the next round with an increased
sequence length to make it more challenging.
5. Your score will increase by 25 points for each correct sequence.
6. The game will end if you input an incorrect sequence or run out of time.


### Sequence of the challenge:
1. Initialize the game by setting up the LCD, TM1638 module, and buzzer.
2. Display the current round number and score on the LCD.
3. Generate a random sequence of LED flashes.
4. Display the sequence on the TM1638 module.
5. Check the user's input against the generated sequence.
6. If the input is correct, increase the score and move to the next round with a longer
sequence.
7. If the input is incorrect or time runs out, end the game and display "Game Over" on
the LCD.

## Challenge - 4:
### Title: Reaction Challenge
This game is designed to test the reaction speed by lighting up a random LED, and user must press the corresponding button as fast as you can. The game consists of 5 rounds, with the difficulty increasing with each round.
### How to play:
1. Power on the hardware setup.
2. Wait for the "Round: 1" message to appear on the LCD screen.
3. Focus on the LEDs. When one of them lights up, press the corresponding button as
quickly as possible.
4. The LCD screen will display whether your input was correct or incorrect.
5. If your input is correct, the game continues to the next round.
6. It is increased the sequence of lights speed in next rounds to make the game more
challenging.
7. If your input is incorrect or you run out of time, the game ends, and your score will be
displayed on the LCD screen.
To restart the game, simply power off the hardware setup and power it back on or click reset button on the board.

### Sequence of the challenge:


1. The hardware initializes and displays the current round number.
2. A random LED is lit up.
3. The player needs to press the corresponding button within 5 seconds.
4. If the player's input is correct, they advance to the next round.
5. If the player's input is incorrect or they run out of time, the game ends.
6. The game consists of 5 rounds. If the player successfully completes all rounds, they win
the game.
