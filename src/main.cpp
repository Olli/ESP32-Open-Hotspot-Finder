
#include <Arduino.h>
#include <WiFi.h>
#include <SPI.h>
// https://github.com/pbecchi/ESP32_ping is needed (beware you have to fix this lib)
#include "../lib/ESP32_ping/ping.h"

#define  ENABLE_DEBUG_PING true

#define MAX_CON_COUNT 50

int LED_BUILTIN = 2;


const IPAddress testIP(1,1,1,1);



void testForInternet(String);

void printMacAddress() {
  // the MAC address of your Wifi shield
  byte mac[6];

  // print your MAC address:
  WiFi.macAddress(mac);
  Serial.print("MAC: ");
  Serial.print(mac[5], HEX);
  Serial.print(":");
  Serial.print(mac[4], HEX);
  Serial.print(":");
  Serial.print(mac[3], HEX);
  Serial.print(":");
  Serial.print(mac[2], HEX);
  Serial.print(":");
  Serial.print(mac[1], HEX);
  Serial.print(":");
  Serial.println(mac[0], HEX);
}

void blinkLed() {
  int counter = 10;
  for(int i = 0; i <= counter; i++) {
    digitalWrite(LED_BUILTIN,HIGH);
    delay(1000);
    digitalWrite(LED_BUILTIN,LOW);
    delay(1000);
  }
}


void printEncryptionType(int thisType) {
  // read the encryption type and print out the name:
  switch (thisType) {
    case WIFI_AUTH_WEP:
      Serial.println("WEP");
      break;
    case WIFI_AUTH_WPA_PSK:
      Serial.println("WPA PSK");
      break;
    case WIFI_AUTH_WPA2_PSK:
      Serial.println("WPA2 PSK");
      break;
    case WIFI_AUTH_WPA_WPA2_PSK:
      Serial.println("WPA/WPA2 PSK");
      break;
    case WIFI_AUTH_WPA2_ENTERPRISE:
      Serial.println("WPA2 Enterprise");
      break;
    case WIFI_AUTH_OPEN:
      Serial.println("None");
      break;
    case WIFI_AUTH_MAX:
      Serial.println("Auto");
      break;
  }
}

void listNetworks() {
  // scan for nearby networks:
  Serial.println("** Scan Networks **");
  int numSsid = WiFi.scanNetworks();
  if (numSsid == -1) {
    Serial.println("Couldn't get a wifi connection");
    while (true);
  }

  // print the list of networks seen:
  Serial.print("number of available networks: ");
  Serial.println(numSsid);

  // print the network number and name for each network found:
  for (int thisNet = 0; thisNet < numSsid; thisNet++) {
    Serial.print(thisNet);
    Serial.print(") ");
    Serial.print(WiFi.SSID(thisNet));
    Serial.print("\tSignal: ");
    Serial.print(WiFi.RSSI(thisNet));
    Serial.print(" dBm");
    Serial.print("\tEncryption: ");
    printEncryptionType(WiFi.encryptionType(thisNet));

    String ssid_scan = WiFi.SSID(thisNet);
    int maybeAdHoc = (String(ssid_scan.charAt(2)) == String(":"));
    if((WiFi.encryptionType(thisNet) == WIFI_AUTH_OPEN && !maybeAdHoc )) {
      testForInternet(WiFi.SSID(thisNet));
    }
    Serial.print("\n ");
  }
}

void testForInternet(String ssid) {

  int counter = 0;

  WiFi.begin(ssid.c_str());

  while ((WiFi.status() != WL_CONNECTED) && counter < MAX_CON_COUNT) {
    delay(500);
    Serial.print("Try to connect to to WiFi: ");
    Serial.println(ssid.c_str());
    counter++;
  }
  if(WiFi.isConnected()) {
    Serial.print("Connected to the WiFi network: ");
    Serial.println(ssid.c_str());

    if(ping_start(testIP, 4, 0, 0, 5)) {
      Serial.println("Open HotSpot Found!");
      blinkLed();
    }
  }
}

void setup() {
  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);


}

void loop() {
  listNetworks();
  delay(10000);
}
