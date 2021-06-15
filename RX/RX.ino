#include "heltec.h" 
#include "images.h"                             // image imported in an hex array
#include <SPI.h>
#include <WiFi.h>

// WiFi
char ssid[] = "Livebox-86B0"; //  your network SSID (name)
char pass[] = "aJTnxm6TPUWRvLWczV";    // your network password (use for WPA, or use as key for WEP)

int status = WL_IDLE_STATUS;
char server[] = "www.teamrotor.com";
WiFiClient client;

//LoRa
#define BAND    433E6                           // other available frequncies: 868E6,915E6
#define TXNB    4                               // number of TX
int TXData[TXNB][2] = {0};                     // 0: RSSI, 1: temperature

void logo(){
  Heltec.display->clear();
  Heltec.display->drawXbm(0,10,logo_width,logo_height,logo_bits);
  Heltec.display->setFont(ArialMT_Plain_10);
  Heltec.display->drawString(23 , 0 , "Maison du Rocher");
  Heltec.display->display();
}

void LoRaData(){
  //cli
  for(int i = 0; i < TXNB; i++)
    Serial.println("Bain n" + String(i+1) + " Temp: " + String(TXData[i][1]) + " RSSI:" + String(TXData[i][0]));
  Serial.println();

  //integrated display
  Heltec.display->clear();
  Heltec.display->setTextAlignment(TEXT_ALIGN_LEFT);
  Heltec.display->setFont(ArialMT_Plain_10);

  for(int i = 0; i < TXNB; i++)
    Heltec.display->drawString(0, 15*i, "Bain n" + String(i+1) + " Temp: " + String(TXData[i][1]) + " RSSI:" + String(TXData[i][0]));
  Heltec.display->display();

  //send by WiFi
  if (client.connect(server, 80)) {
    Serial.println("connected to server");
  
    // Make a HTTP request:
    client.println("GET /temperature?"+String(abs(TXData[0][1]), DEC)+"&"+String(abs(TXData[1][1]), DEC)+"&"+String(abs(TXData[2][1]), DEC)+"&"+String(abs(TXData[3][1]), DEC)+" HTTP/1.1");
    Serial.println("GET /temperature?"+String(TXData[0][1], DEC)+"&"+String(TXData[1][1], DEC)+"&"+String(TXData[2][1], DEC)+"&"+String(TXData[3][1], DEC)+" HTTP/1.1");
    client.println("Host: www.teamrotor.com");
    client.println("Connection: close");
    client.println();
  }
}
void onReceive(int packetSize)
{
  if (packetSize == 0) return;          // if there's no packet, return

  // read packet header bytes:
  int sender = LoRa.read();            // sender address
  int incomingMsgId = LoRa.read();     // incoming msg ID
  byte incomingLength = LoRa.read();    // incoming msg length
  int temperature = LoRa.read();    // incoming temperature
 
  // attribute temperature and RSSI from the sender in the array
  TXData[sender][0] = LoRa.packetRssi(); // RSSI
  TXData[sender][1] = temperature; // temp

  String rssi = "RSSI " + String(TXData[sender][0], DEC);

  String incoming = "";

  while (LoRa.available())
    incoming += (char)LoRa.read();
 

  if (incomingLength != incoming.length())
  {   // check length for error
    Serial.println("error: message length does not match length");
    return;                             // skip rest of function
  }

  LoRaData();
  // if message is correct, print details:
  Serial.println("Received from: " + String(sender, DEC));
  Serial.println("Message ID: " + String(incomingMsgId));
  Serial.println("Message length: " + String(incomingLength));
  Serial.println("Tmp data: " + String(temperature));
  Serial.println("Temperature: " + incoming);
  Serial.println("RSSI: " + String(LoRa.packetRssi()));
  Serial.println("Snr: " + String(LoRa.packetSnr()));
  Serial.println();
}

void setup() { 
  Heltec.begin(true /*Display*/, true /*Heltec.Heltec.Heltec.LoRa*/, true /*Serial*/, true /*PABOOST*/, BAND);
 
  Heltec.display->init();
  Heltec.display->flipScreenVertically();  
  Heltec.display->setFont(ArialMT_Plain_10);
  logo();
  delay(10000);
  Heltec.display->clear();
  
  Heltec.display->drawString(0, 10, "Wait for incoming data...");
  Heltec.display->display();
  delay(5000);
  LoRa.receive();

  // WiFi
  while (status != WL_CONNECTED) {
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
    status = WiFi.begin(ssid, pass);
    // wait 10 seconds for connection:
    delay(2000);
  }
  Serial.println("Connected !");
}
void loop() {
  onReceive(LoRa.parsePacket()); 
  delay(2000); 
}
