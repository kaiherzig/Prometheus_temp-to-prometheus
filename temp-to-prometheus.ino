#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <WiFiClient.h>
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>

#define SEALEVELPRESSURE_HPA (1013.25)

const char *ssid = "<<FILLME>>";
const char *password = "<<FILLME>>";
unsigned long delayTime;

Adafruit_BME280 bme;

ESP8266WebServer server(80);
String GenerateMetrics()
{
  String message = "";

  message += "# HELP bme280_measuring_temperature Current sensor temperature in celsius.\n";
  message += "# TYPE bme280_measuring_temperature gauge\n";
  message += "bme280_measuring_temperature ";
  message += bme.readTemperature();
  message += "\n";

  message += "# HELP bme280_measuring_humidity Current sensor humidity in persent.\n";
  message += "# TYPE bme280_measuring_humidity gauge\n";
  message += "bme280_measuring_humidity ";
  message += bme.readHumidity();
  message += "\n";

  message += "# HELP bme280_measuring_pressure Current sensor Pressure in mmHg.\n";
  message += "# TYPE bme280_measuring_pressure gauge\n";
  message += "bme280_measuring_pressure ";
  message += bme.readPressure() * 0.007500617;
  message += "\n";

  return message;
}

void HandleRoot()
{
  server.send(200, "text/plain", GenerateMetrics());
}

void setup()
{
  Serial.begin(115200);
  delay(10);
  delayTime = 1000;
  Serial.println(F("BME280 test"));

  bool status;
  status = bme.begin(0x76);

  // Connect to WiFi network
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  server.on("/", HandleRoot);
  server.on("/metrics", HandleRoot);

  // Start the server
  server.begin();
  Serial.println("Server started");

  // Print the IP address
  Serial.println(WiFi.localIP());
}
void loop()
{

  server.handleClient();
  delay(delayTime);
}
