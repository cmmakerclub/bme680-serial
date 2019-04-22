#include <Arduino.h> 

/* BOARD INFO */
String DEVICE_NAME      = "AZUR-003"; 

/* WIFI INFO */ 
String WIFI_SSID        = "CMMC_Sinet_2.4G";
String WIFI_PASSWORD    = "zxc12345";

/* MQTT INFO */ 
String MQTT_HOST        = "mqtt.cmmc.io";
String MQTT_USERNAME    = "";
String MQTT_PASSWORD    = "";
String MQTT_CLIENT_ID   = "";
String MQTT_PREFIX      = "WORKAZUR/";

int    MQTT_PORT        = 1883;
int PUBLISH_EVERY       = 5L * 1000;
int MQTT_CONNECT_TIMEOUT= 10; 
