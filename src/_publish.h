#include <MqttConnector.h>

extern int relayPinState;
extern MqttConnector* mqtt;

extern int relayPin;
extern int LED_PIN;

extern char myName[];

static void readSensor(); 

extern String DEVICE_NAME;
extern int PUBLISH_EVERY;

  float Temperature , Humidity;
  uint32_t Gas;
  uint32_t Pressure;
  uint16_t IAQ;
  int16_t  Altitude;
  uint8_t IAQ_accuracy;
void register_publish_hooks() {
  strcpy(myName, DEVICE_NAME.c_str());
  mqtt->on_prepare_data_once([&](void) {
    Serial.println("initializing sensor...");
  });

  mqtt->on_before_prepare_data([&](void) {
    readSensor();
  });

  mqtt->on_prepare_data([&](JsonObject *root) {
    JsonObject& data = (*root)["d"];
    JsonObject& info = (*root)["info"];
    data["myName"] = myName;
    data["millis"] = millis();
    data["relayState"] = relayPinState;
    data["updateInterval"] = PUBLISH_EVERY;
    data["Temperature"] = Temperature;
    data["Humidity"] = Humidity;
    data["Gas"] = Gas;
    data["Pressure"] = Pressure;
    data["IAQ"] = IAQ;
    data["Altitude"] = Altitude;
    data["IAQ_accuracy"] = IAQ_accuracy;
  }, PUBLISH_EVERY);
  mqtt->on_after_prepare_data([&](JsonObject * root) {
    /**************
      JsonObject& data = (*root)["d"];
      data.remove("version");
      data.remove("subscription");
    **************/
  });

  mqtt->on_published([&](const MQTT::Publish & pub) {
      Serial.println("Published.");
  });
}

static void readSensor() {
  // perform reading sensor 
  Serial.println("Perform reading sensor...");

  unsigned char i = 0, sum = 0;

//  Serial.println("HELLO");
  while (mySerial.available()) {
    Re_buf[counter] = (unsigned char)mySerial.read();

    if (counter == 0 && Re_buf[0] != 0x5A) return;
    if (counter == 1 && Re_buf[1] != 0x5A)
    {
      counter = 0;
      return;
    };
    counter++;
    if (counter == 20)
    {
      counter = 0;
      sign = 1;
    }
  }
  if (sign)
  {
    sign = 0;

    if (Re_buf[0] == 0x5A && Re_buf[1] == 0x5A )
    {

      for (i = 0; i < 19; i++)
        sum += Re_buf[i];
      if (sum == Re_buf[i] )
      {
        temp2 = (Re_buf[4] << 8 | Re_buf[5]);
        Temperature = (float)temp2 / 100;
        temp1 = (Re_buf[6] << 8 | Re_buf[7]);
        Humidity = (float)temp1 / 100;
        Pressure = ((uint32_t)Re_buf[8] << 16) | ((uint16_t)Re_buf[9] << 8) | Re_buf[10];
        IAQ_accuracy = (Re_buf[11] & 0xf0) >> 4;
        IAQ = ((Re_buf[11] & 0x0F) << 8) | Re_buf[12];
        Gas = ((uint32_t)Re_buf[13] << 24) | ((uint32_t)Re_buf[14] << 16) | ((uint16_t)Re_buf[15] << 8) | Re_buf[16];
        Altitude = (Re_buf[17] << 8) | Re_buf[18];
        Serial.print("Temperature:");
        Serial.print(Temperature);
        Serial.print(" ,Humidity:");
        Serial.print(Humidity);
        Serial.print(" ,Pressure:");
        Serial.print(Pressure);
        Serial.print("  ,IAQ:");
        Serial.print(IAQ);
        Serial.print(" ,Gas:");
        Serial.print(Gas );
        Serial.print("  ,Altitude:");
        Serial.print(Altitude);
        Serial.print("  ,IAQ_accuracy:");
        Serial.println(IAQ_accuracy);
      }
    }
  }
}
