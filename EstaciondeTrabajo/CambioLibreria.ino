#include"rfid1.h"
//#include <dtostrf.h>
RFID1 rfid;
uchar serNum[5];
uchar serNum1[4];

#include <RHReliableDatagram.h>
#include <RH_NRF24.h>
#include <SPI.h>


float tiempoIn = 0;
float tiempoOut = 0;
float tiempoTotal = 0;
unsigned char het[16] = {'0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0'};
unsigned char *writeData = het;
bool sw = true;
uchar key[] = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
//uchar *key[6]= {'FF', 'FF', 'FF', 'FF', 'FF', 'FF'} ;
//String *key = "FF FF FF FF FF FF";
uint8_t buffer3[24];
uchar buffer1[16] = {'0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0'};
uchar *str;
uchar *str1;
uchar buffer2[16] = {'0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0'};
uint8_t id[6];
uint8_t buf[RH_NRF24_MAX_MESSAGE_LEN];
#define CLIENT1_ADDRESS 3
#define SERVER_ADDRESS 2
RH_NRF24 driver(9, 10);
RHReliableDatagram manager(driver, CLIENT1_ADDRESS);

void setup() {
  id[0] = '~'; //identificador tiempo
  id[1] = '2'; //numero banco
  buffer3[0] = '*'; //identificador tiempo
  buffer3[1] = '2'; //numero banco
  Serial.begin(9600);
  for (byte i = 2; i < 24; i++) {
    buffer3[i] = ' ';
  }

}

void checkRFID(int i) {
  if (i == 0) {
    rfid.begin(2, 4, 5, 7, 3, 6); //rfid.begin(IRQ_PIN,SCK_PIN,MOSI_PIN,MISO_PIN,NSS_PIN,RST_PIN)
    sw = true;
    //Serial.println("Entra");
  }
  if (i == 1) {
    rfid.begin(2, 4, 5, 8, 3, 6);
    sw = false;
  }

  uchar status;

  delay(100);
  rfid.init();
  uchar str[MAX_LEN];
  // Search card, return card types
  status = rfid.request(PICC_REQIDL, str);
  
  if (status != MI_OK)
  {
    return;
  }
  rfid.showCardType(str);
  status = rfid.anticoll(str);
  if (status == MI_OK)
  {
   //pitar();
    
    if (sw) {
      Serial.println("Entra");
      for (byte i = 0; i < 16; i++) {
        buffer1[i] = '0';
      }

      tiempoIn = millis();
      Serial.print("tiempo entrada: ");
      
      Serial.println(tiempoIn);
      rfid.write(2, buffer1);
     
    } else {
      tiempoOut = millis();
      tiempoTotal = tiempoOut - tiempoIn ;
      tiempoTotal = tiempoTotal / 1000;
      Serial.print("tiempo salida: ");
      
      Serial.println(tiempoOut);
      Serial.print("tiempo total: ");
      Serial.println(tiempoTotal);
      int ptInt = tiempoTotal;
      int prueba;
      int total = 0;
      prueba = ptInt / 10;
      if (prueba == 0) {
        total = 1;
      } else {
        prueba = ptInt / 100;
        if (prueba == 0) {
          total = 2;
        } else {
          prueba = ptInt / 1000;
          if (prueba == 0) {
            total = 3;
          } else {
            total = 4;
          }
        }
      }
     
      uchar u[total + 2];
      dtostrf(tiempoTotal, total, 2, u);

      for (uint8_t i = 0; i < total + 3; i++) {
        buffer1[i] = u[i];
      }
      //Serial.println("buffer");

      
      rfid.write(2, buffer1);
      
      memcpy(serNum, str, 5);
      
      String me = "";
      for (byte i = 2; i < 6; i++) {
        id[i] = serNum[i-2];
        
      }
      Serial.println(me);
      Enviar();

      

    }

    memcpy(serNum, str, 5);

    rfid.showCardID(serNum);//show the card ID
    Serial.println();
    Serial.println();

    delay(100);
    rfid.halt(); //command the card into sleep mode

  }
}
void Enviar() {
  Serial.println("empieza envio");
  String temp = (String)tiempoTotal;
  char te[temp.length() + 2];
  temp.toCharArray(te, temp.length());
  for ( byte i = 0; i < temp.length(); i++) {
    buffer3[i+2] = uint8_t (te[i]);
  }
  manager.init();
  delay(10);
  manager.sendtoWait(id, sizeof(id), SERVER_ADDRESS);
  manager.sendtoWait(buffer3, sizeof(buffer3), SERVER_ADDRESS);
  driver.sleep();

}
void loop() {
  
  checkRFID(0);
  delay(10);
  checkRFID(1);

}
