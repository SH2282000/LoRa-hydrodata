#include "heltec.h" 
#include "images.h"

#define BAND    433E6  //868E6,915E6
String rssi = "RSSI --";
String packSize = "--";
String packet ;

int i = 0;
long lastSendTime = 0;        // last send time
int interval = 2000;          // interval between sends

void logo(){
  Heltec.display->clear();
  Heltec.display->drawXbm(0,10,logo_width,logo_height,logo_bits);
  Heltec.display->setFont(ArialMT_Plain_10);
  Heltec.display->drawString(23 , 0 , "Maison du Rocher");
  Heltec.display->display();
}

void LoRaData(){
  Serial.println("Received "+ packSize + " bytes. Content: " + packet + " with " + rssi);
  Heltec.display->clear();
  Heltec.display->setTextAlignment(TEXT_ALIGN_LEFT);
  Heltec.display->setFont(ArialMT_Plain_10);
  Heltec.display->drawString(0 , 15 , "Received "+ packSize + " bytes");
  Heltec.display->drawStringMaxWidth(0 , 26 , 128, packet);
  Heltec.display->drawString(0, 0, rssi);  
  Heltec.display->display();
}

void onReceive(int packetSize) {
  if (packetSize == 0) return;          // if there's no packet, return
  packet = "";
  packSize = String(packetSize,DEC);
  for (int i = 0; i < packetSize; i++) { packet += (char) LoRa.read(); }
  rssi = "RSSI " + String(LoRa.packetRssi(), DEC);
  LoRaData();
}

void setup() { 
   //WIFI Kit series V1 not support Vext control
  Heltec.begin(true /*DisplayEnable Enable*/, true /*Heltec.Heltec.Heltec.LoRa Disable*/, true /*Serial Enable*/, true /*PABOOST Enable*/, BAND /*long BAND*/);
 
  Heltec.display->init();
  Heltec.display->flipScreenVertically();  
  Heltec.display->setFont(ArialMT_Plain_10);
  logo();
  delay(10000);
  Heltec.display->clear();
  
  Heltec.display->drawString(0, 10, "Wait for incoming data...");
  Heltec.display->display();
  delay(1000);
  LoRa.receive();
}

void send(int counter) {
  Serial.print("Sending packet : ");
  Serial.println(counter);
    
  LoRa.beginPacket();
  LoRa.setTxPower(20,RF_PACONFIG_PASELECT_PABOOST);
  LoRa.print("Temperature : ");
  LoRa.print(counter);
  LoRa.print("°");
  LoRa.endPacket();
}
void loop() {
  send(i);
  if (millis() - lastSendTime > interval) {
    send(i++);
    lastSendTime = millis();            // timestamp the message
    interval = random(2000) + 1000;    // 2-3 seconds
  }
  int packetSize = LoRa.parsePacket();
  if (packetSize) { 
    onReceive(packetSize);  
  }
}
