#ifndef CustomLCD_h
#define CustomLCD_h

#include <LiquidCrystal_I2C.h>

class CustomLCD {
private:
  LiquidCrystal_I2C lcd;

public:
  CustomLCD(uint8_t addr, uint8_t cols, uint8_t rows);
  void init();
  void clear();
  void print(const char *str, uint8_t col = 0, uint8_t row = 0);
  void setCursor(uint8_t col, uint8_t row);
  void print(int value, uint8_t col = 0, uint8_t row = 0);
};

#endif
