/*Generic Sensor Node Sketch
//Jan 2015 -- Dale Wright
//Jan 7 2015 - added power saving code
*/

/* Start Includes */
#include <RFM69.h>    
#include <SPI.h>
#include <SPIFlash.h>
#include <LowPower.h>
#include <avr/sleep.h>
#include <avr/delay.h>
#include <DHT.h>

/* End Includes */

/* Start Definitions */

#define DHTPIN        7
#define DHTTYPE       DHT11
#define NODEID        2    //unique for each node on same network
#define SENSORID      10   //unique for each sensor
#define NETWORKID     100  //the same on all nodes that talk to each other
#define GATEWAYID     1    //RFM Gateway node id
#define FREQUENCY     RF69_915MHZ
#define ENCRYPTKEY    "xxxxxxxxxxxxxxxx" //exactly the same 16 characters/bytes on all nodes!
#define IS_RFM69HW    //uncomment only for RFM69HW! Leave out if you have RFM69W!
#define ACK_TIME      30 // max # of ms to wait for an ack
#define LED           9 // Moteinos have LEDs on D9
#define FLASH_SS      8 // and FLASH SS on D8
#define SERIAL_BAUD   115200

/* End Definitions */

/* Start Variables */

int TRANSMITPERIOD = 30000; //transmit a packet to gateway so often (in ms)
boolean requestACK = true;
SPIFlash flash(FLASH_SS, 0xEF30); //EF30 for 4mbit  Windbond chip (W25X40CL)
DHT dht(DHTPIN, DHTTYPE);
RFM69 radio;

typedef struct {
  int            nodeId; //store this nodeId
  int            sensorID;
  unsigned long  var1_usl; //uptime in ms
  float          var2_float; //temp from DHT11
  float          var3_float; //humidity from DHT11
} Payload;
Payload theData;

/* End Variables */

/* Start Setup */

void setup() {
  Serial.begin(SERIAL_BAUD);
  dht.begin();
  radio.initialize(FREQUENCY,NODEID,NETWORKID);
  #ifdef IS_RFM69HW
    radio.setHighPower(); //uncomment only for RFM69HW!
  #endif
  radio.encrypt(ENCRYPTKEY);
  radio.sleep();
  char buff[50];
  sprintf(buff, "\nTransmitting at %d Mhz...", FREQUENCY==RF69_433MHZ ? 433 : FREQUENCY==RF69_868MHZ ? 868 : 915);
  Serial.println(buff);
  
  if (flash.initialize())
  {
    Serial.print("SPI Flash Init OK ... UniqueID (MAC): ");
    flash.readUniqueId();
    for (byte i=0;i<8;i++)
    {
      Serial.print(flash.UNIQUEID[i], HEX);
      Serial.print(' ');
    }
    flash.sleep();
  }
  else
    Serial.println("SPI Flash Init FAIL! (is chip present?)");
}
long lastPeriod = -1;

/* End Setup */

/* Start Main Loop */

void loop() {   

    LowPower.powerDown(SLEEP_8S, ADC_OFF, BOD_OFF);
  
  //int currPeriod = millis()/TRANSMITPERIOD;
  //if (currPeriod != lastPeriod)
  //{
    //fill in the struct with new values
    theData.nodeId = NODEID;
    theData.sensorID = SENSORID;
    theData.var1_usl = millis();
    theData.var2_float = dht.readTemperature(true); // send Farenheit
    theData.var3_float = dht.readHumidity();
    Serial.print("Sending struct (");
    Serial.print(sizeof(theData));
    Serial.print(" bytes) ... ");
    if (radio.sendWithRetry(GATEWAYID, (const void*)(&theData), sizeof(theData)))
    {
      Serial.print(" ok!");
      radio.sleep();
    }
    else Serial.print(" nothing...");
    Serial.println();
    Blink(LED,3);
    //lastPeriod=currPeriod;
  //}
} 

/* End Main Loop */

/* Start Blink Method */
// Blinks LED

void Blink(byte PIN, int DELAY_MS)
{
  pinMode(PIN, OUTPUT);
  digitalWrite(PIN,HIGH);
  delay(DELAY_MS);
  digitalWrite(PIN,LOW);
}

/* End Blink Method */

