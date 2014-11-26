#include <VirtualWire.h>
#include <avr/interrupt.h>
#include <avr/power.h>
#include <avr/sleep.h>
#include <avr/io.h>

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
}

void sleepNow(void)
{
	//Set pin 2 as interrupt and attach handler:
	attachInterrupt(0, pinInterrupt, LOW);
	delay(100);
	
	//Set preferred sleep mode
	// SLEEP_MODE_IDLE   - least power saving
	// SLEEP_MODE_ADC
	//SLEEP_MODE_PWR_SAVE
	//SLEEP_MODE_STANDBY
	//SLEEP_MODE_PWR_DOWN  - most power savings
	set_sleep_mode(SLEEP_MODE_IDLE);
	
	//Set sleep enable (SE) bit
	sleep_enable();
	
	//Put arduino to sleep
	digitalWrite(13, LOW);
	sleep_mode();
	
	//Upon waking up, sketch continues
	sleep_disable();
	digitalWrite(13, HIGH);
}

void pinInterrupt(void)
{
	detachInterrupt(0);
}

void loop()
{
    const char *msg = "This is a test";       // this is your message to send

   vw_send((uint8_t *)msg, strlen(msg));
   vw_wait_tx();                                          // Wait for message to finish
   delay(200);
}
