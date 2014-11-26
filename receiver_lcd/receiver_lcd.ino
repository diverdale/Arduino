#include <VirtualWire.h>
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

#undef int
#undef abs
#undef double
#undef float
#undef round

LiquidCrystal_I2C	lcd(I2C_ADDR,En_pin,Rw_pin,Rs_pin,D4_pin,D5_pin,D6_pin,D7_pin, BACKLIGHT_PIN, BACKLIGHT_POL);

void setup()
{
  lcd.begin(20,4);
  lcd.home();
  lcd.setCursor(0,2);
  lcd.print("Initializing System");
  Serial.begin(115200);    

  // Initialise the IO and ISR
  vw_set_ptt_inverted(true);    // Required for RX Link Module
  vw_setup(2000);               // Bits per sec
  vw_set_rx_pin(4);             // We will be receiving on pin 4 i.e the RX pin from the module connects to this pin. 
  vw_rx_start();                // Start the receiver 
}

void loop()
{
    uint8_t buf[VW_MAX_MESSAGE_LEN];
    uint8_t buflen = VW_MAX_MESSAGE_LEN;

    if (vw_get_message(buf, &buflen)) // check to see if anything has been received
    {
    int i;
     // Message with a good checksum received.
        
    for (i = 0; i < buflen; i++)
    {
        Serial.write(buf[i]);                     // the received data is stored in buffer
        lcd.print(buf[i]);
    }
    
    Serial.write("");
     }
}
