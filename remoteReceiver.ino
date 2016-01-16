#include <RF24_config.h>
#include <printf.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <SPI.h>

/****************** User Config ***************************/
/***      Set this radio as radio number 0 or 1         ***/
bool radioNumber = 1;
unsigned long loopCounter = 0;
bool flash = 0;

/* Hardware configuration: Set up nRF24L01 radio on SPI bus plus pins 7 & 8 */
RF24 radio(7,8);
/**********************************************************/

byte addresses[][6] = {"1Node","2Node"};

void setup() {

  pinMode(4, OUTPUT);
  digitalWrite(4, 0);
  Serial.begin(9600);
  radio.begin();

  //RF24_PA_MAX is default.
  radio.setPALevel(RF24_PA_LOW);
  
  // Open a writing and reading pipe on each radio, with opposite addresses
  radio.openWritingPipe(addresses[1]);
  radio.openReadingPipe(1,addresses[0]);
  
  // Start the radio listening for data
  radio.startListening();

}

void loop() {

  byte axes[2];

  if( radio.available())
  {                                                                    // Variable for the received timestamp
      while (radio.available())                                       // While there is data ready
      {                                   
        radio.read( &axes, (2*sizeof(byte)) );             // Get the payload
      }
      Serial.print("X");
      Serial.print(axes[0]);
      Serial.print("Y");
      Serial.println(axes[1]);
     
   radio.stopListening();                                        // First, stop listening so we can talk   
   radio.write( &axes, (2*sizeof(byte)) );                       // Send the final one back.      
   radio.startListening();                                       // Now, resume listening so we catch the next packets.     
   //Serial.print(F("Sent response "));
   //Serial.println(got_time);  
   }
  
}
