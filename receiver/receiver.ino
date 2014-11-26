#include <VirtualWire.h>
#undef int
#undef abs
#undef double
#undef float
#undef round

void setup()
{
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
        }
    Serial.write("");
     }
}
