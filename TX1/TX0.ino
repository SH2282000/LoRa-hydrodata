#include <OneWire.h>
#include <DallasTemperature.h>
#include "heltec.h" 
#include "images.h"

// communication
#define BAND    433E6   // same as RX
int localAddress = 0;
int msgCount = 0;

// temp retrieving
#define ONE_WIRE_BUS 32  
OneWire oneWire(ONE_WIRE_BUS); 
DallasTemperature sensors(&oneWire); 

void logo(){
  Heltec.display->clear();
  Heltec.display->drawXbm(0,10,logo_width,logo_height,logo_bits);
  Heltec.display->setFont(ArialMT_Plain_10);
  Heltec.display->drawString(23 , 0 , "Maison du Rocher");
  Heltec.display->display();
}
void disp(String temperature, int msgId) {  
  Heltec.display->clear();
  Heltec.display->drawString(0, 40, "Temp: " + temperature + ", msgId: " + String(msgId));
  Heltec.display->display();
}
void send(int temperature)
{
  String message = String(temperature) + "°";
  LoRa.beginPacket();                   // start packet
  LoRa.write(localAddress);             // add sender address
  LoRa.write(msgCount);                 // add message ID
  LoRa.write(message.length());        // add payload length
  LoRa.write(temperature);             // add temperature data

  disp(message, msgCount);
  LoRa.print(message);                 // add payload
  
  LoRa.endPacket();                     // finish packet and send it
  msgCount++;                           // increment message ID
}

int getTemperature()
{
  sensors.requestTemperatures();  
  return sensors.getTempCByIndex(0);
}

void setup() { 
  Heltec.begin(true /*Display*/, true /*Heltec.Heltec.Heltec.LoRa*/, true /*Serial*/, true /*PABOOST*/, BAND);
  LoRa.setTxPower(20,RF_PACONFIG_PASELECT_PABOOST);
 
  Heltec.display->init();
  Heltec.display->flipScreenVertically();  
  Heltec.display->setFont(ArialMT_Plain_10);
  logo();
  delay(10000);
  Heltec.display->clear();
}

void loop() {
  //if (millis() - lastSendTime > interval) { //send the message at different interval of time
    send(getTemperature());
    delay(2000);
    //lastSendTime = millis();            // timestamp the message
}
