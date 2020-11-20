// nrf24_reliable_datagram_server.pde
// -*- mode: C++ -*-
// Example sketch showing how to create a simple addressed, reliable messaging server
// with the RHReliableDatagram class, using the RH_NRF24 driver to control a NRF24 radio.
// It is designed to work with the other example nrf24_reliable_datagram_client
// Tested on Uno with Sparkfun WRL-00691 NRF24L01 module
// Tested on Teensy with Sparkfun WRL-00691 NRF24L01 module
// Tested on Anarduino Mini (http://www.anarduino.com/mini/) with RFM73 module
// Tested on Arduino Mega with Sparkfun WRL-00691 NRF25L01 module

#include <RHReliableDatagram.h>
#include <RH_NRF24.h>
#include <SPI.h>

#define CLIENT_ADDRESS 1
#define SERVER_ADDRESS 2
#define CLIENT1_ADDRESS 3

// Singleton instance of the radio driver
//RH_NRF24 driver;
RH_NRF24 driver(9, 10);   // For RFM73 on Anarduino Mini

// Class to manage message delivery and receipt, using the driver declared above
RHReliableDatagram manager(driver, SERVER_ADDRESS);

void setup()
{
  pinMode(LED_BUILTIN, OUTPUT);

  Serial.begin(9600);
  if (!manager.init())
    Serial.println("init failed");
  // Defaults after init are 2.402 GHz (channel 2), 2Mbps, 0dBm
  
}
char* he[6];
uint8_t data[] = "And hello back to you";
// Dont put this on the stack:
uint8_t buf[RH_NRF24_MAX_MESSAGE_LEN];

void loop()
{

  if (manager.available())
  {
    for (byte i = 0; i < 24; i++) {
      buf[i] = " ";
    }
    // turn the LED off by making the voltage LOW
    //delay(10);
    // Wait for a message addressed to us from the client
    uint8_t len = sizeof(buf);
    uint8_t from;
    if (manager.recvfromAck(buf, &len, &from))
    {
      
      if (buf[0] != '~') {
         

            Serial.println((char*)buf);
            delay(10);


      } else {
        
         Serial.print((char)buf[0]);
         Serial.print((char)buf[1]);
        for (byte i = 2; i < 6; i++) {
          Serial.print(buf[i], HEX);
          //he[i-1] = (buf[i], HEX);
          delay(100);
        }
        he[5]='\0';
        Serial.println("");
        //Serial.println(DEC,*he);
      }



      // Send a reply back to the originator client

    }
  }
}
