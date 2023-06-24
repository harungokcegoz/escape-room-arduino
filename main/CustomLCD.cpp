#include "CustomLCD.h"

CustomLCD::CustomLCD(uint8_t addr, uint8_t cols, uint8_t rows) : lcd(addr, cols, rows) {}

void CustomLCD::init() {
  lcd.init();
  lcd.backlight();
}

void CustomLCD::clear() {
  lcd.clear();
}

void CustomLCD::print(const char *str, uint8_t col, uint8_t row) {
  lcd.setCursor(col, row);
  lcd.print(str);
}

void CustomLCD::setCursor(uint8_t col, uint8_t row) {
  lcd.setCursor(col, row);
}

void CustomLCD::print(int value, uint8_t col, uint8_t row) {
  lcd.setCursor(col, row);
  lcd.print(value);
}
