#include <ESP8266HTTPClient.h>
#include <ESP8266WiFi.h>
#include "DHT.h"

//#define DHTTYPE DHT11   // DHT 11
#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321

//Define InfluxDB 
 #define INFLUXDB_HOST "192.168.178.45"
 #define INFLUXDB_PORT "8086"
 #define INFLUXDB_DATABASE "temperaturedb"
 //if used with authentication
 #define INFLUXDB_USER "sensor"
 #define INFLUXDB_PASS "sensor"

// Replace with your network details
const char* ssid = "SSID";
const char* password = "PASSWORD";

// DHT Sensor
const int DHTPin = 5;
// Initialize DHT sensor.
DHT dht(DHTPin, DHTTYPE);

HTTPClient http;

void setup() {
  // Initializing serial port for debugging purposes
  Serial.begin(115200);
  delay(10);

  dht.begin();
  
  // Connecting to WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");

  
}

void loop() {

  float t = dht.readTemperature();
  float h = dht.readHumidity();

  Serial.print(t);
  Serial.print("*C \t");
  Serial.print(h);
  Serial.println("% Humidity");

  http.begin("http://192.168.178.45:8086/write?db=temperaturedb");
  String data = String("weather,location=bedroom temperature=" + String(t,1) + ",humidity=" + String(h,0));
  int httpCode = http.POST(data);   //Send the request
  String payload = http.getString();

  Serial.println(httpCode);
  Serial.println(payload);

  http.end();

  delay(120000);
}
