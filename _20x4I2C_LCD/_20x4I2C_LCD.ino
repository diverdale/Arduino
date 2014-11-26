/*
**Testing SainSmart LCD2004 I2C 20x4 LCD
**The I2C address is 0x3F
**Dale Wright - 2014
*/
#include <Wire.h>
#include <LCD.h>
#include <LiquidCrystal_I2C.h>

#define I2C_ADDR    0x3F  // Define I2C Address where the PCF8574A is
#define BACKLIGHT_PIN     3
#define BACKLIGHT_POL POSITIVE
#define En_pin  2
#define Rw_pin  1
#define Rs_pin  0
#define D4_pin  4
#define D5_pin  5
#define D6_pin  6
#define D7_pin  7

int n = 1;

LiquidCrystal_I2C	lcd(I2C_ADDR,En_pin,Rw_pin,Rs_pin,D4_pin,D5_pin,D6_pin,D7_pin, BACKLIGHT_PIN, BACKLIGHT_POL);

void setup()
{
  lcd.begin (20,4);
  lcd.home ();                   // go home

  lcd.print("Testing the LCD");  
  lcd.setCursor ( 0, 1 );        // go to the 2nd line
  lcd.print("I2C address is 0x3f");
  lcd.setCursor ( 0, 2 );        // go to the third line
  lcd.print("Lot's of room");
  lcd.setCursor ( 0, 3 );        // go to the fourth line
  lcd.print("Thanks! ");
}

void loop()
{
  // Backlight on/off every 3 seconds
  lcd.setCursor (14,3);        // go col 14 of line 3
  lcd.backlight();     // Backlight on

}


