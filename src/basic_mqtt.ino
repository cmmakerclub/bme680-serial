#include <Arduino.h>

#ifdef ESP8266 
  #include <ESP8266WiFi.h>
#else 
  #include <WiFi.h>
#endif
#include <HardwareSerial.h>

#define tx (16)
#define rx (17)

uint16_t temp1 = 0;
int16_t temp2 = 0;

unsigned char Re_buf[30], counter = 0;
unsigned char sign = 0;

HardwareSerial mySerial(2);
#include <ArduinoJson.h>
#include <MqttConnector.h>
#include <Wire.h>
#include <SPI.h>

#include "init_mqtt.h"
#include "_publish.h"
#include "_receive.h"
#include "_config.h"

MqttConnector *mqtt; 

int relayPin = 15; 
int relayPinState = HIGH;
int LED_PIN = 2;


char myName[40];

void init_hardware()
{
  pinMode(relayPin, OUTPUT);
  pinMode(LED_PIN, OUTPUT);

  digitalWrite(relayPin, relayPinState);;
  // serial port initialization
  Serial.begin(115200);
  delay(10);
  Serial.println();
  Serial.println("Starting...");
   mySerial.begin(9600 , SERIAL_8N1 , tx , rx , false);
  mySerial.write(0XA5);
  mySerial.write(0X55);
  mySerial.write(0X3F);
  mySerial.write(0X39);
  delay(10);

  mySerial.write(0XA5);
  mySerial.write(0X56);
  mySerial.write(0X02);
  mySerial.write(0XFD);
  delay(10);
  Serial.println("Configured");
}

void init_wifi() {
  WiFi.disconnect();
  delay(20);
  WiFi.mode(WIFI_STA);
  delay(50);
  const char* ssid =  WIFI_SSID.c_str();
  const char* pass =  WIFI_PASSWORD.c_str();
  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.printf ("Connecting to %s:%s\r\n", ssid, pass);
    delay(300);
  }
  Serial.println("WiFi Connected.");
  digitalWrite(2, HIGH);
}

void setup()
{
  init_hardware();
  init_wifi();
  init_mqtt();
}

void loop()
{
  mqtt->loop();
}
