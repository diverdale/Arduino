#include <VirtualWire.h>
#include <SoftwareSerial.h>
#include <avr/interrupt.h>
#include <avr/power.h>
#include <avr/sleep.h>
#include <avr/io.h>
#include <math.h>

#undef int
#undef abs
#undef double
#undef float
#undef round

void setup()
{
    DDRD &= B00000011;       // set Arduino pins 2 to 7 as inputs, leaves 0 & 1 (RX & TX) as is
    DDRB = B00000000;        // set pins 8 to 13 as inputs
    PORTD |= B11111100;      // enable pullups on pins 2 to 7, leave pins 0 and 1 alone
    PORTB |= B11111111;      // enable pullups on pins 8 to 13
    pinMode(13,OUTPUT);  
    digitalWrite(13, HIGH);
    // Initialise the IO and ISR
    //vw_set_ptt_inverted(true);    // Required for RF Link module
    vw_setup(2000);                 // Bits per sec
    vw_set_tx_pin(3);               // pin 3 is used as the transmit data out into the TX Link module, change this as per your needs  
    Serial.begin(9600);
}

double Thermister(int RawADC) {
  double Temp;
  Temp = log(10000*((1024.0/RawADC-1)));
  Temp = 1 / (0.001129148 + (0.000234125 + (0.0000000876741 * Temp * Temp ))* Temp );
  Temp = Temp - 273.15;           // Convert Kelvin to Celcius
  return Temp;
    
}


double getTemp() 
{
  double fTemp;
  double temp = Thermister(analogRead(0));
  fTemp = (temp * 9.0)/5.0 + 32.0;
  return -fTemp;      
}

void loop()
{
   double tmp = getTemp();
   Serial.println(tmp);
   char temp[6];
   dtostrf(tmp, 6, 3, temp);
   const char *msg = temp ;   // this is your message to send
   vw_send((uint8_t *)msg, strlen(msg));
   vw_wait_tx();                                          // Wait for message to finish
   delay(200);
}
