#include <ArduinoJson.h>
#include <SoftwareSerial.h>

SoftwareSerial ArduinoSerial(2, 3); //(Rx Tx) ESP -> Arduino: Tx -> 2, Rx -> 3.
char data;
DynamicJsonDocument json(1024);

// Configuration of humidity (COH)
#include "DHT.h"

#define DHTPIN 5
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

void setup()
{

  Serial.begin(115200);
  ArduinoSerial.begin(9600); //bd from master (ESP)
  pinMode(4, OUTPUT);

  //COH
  dht.begin();
}

void loop()
{
  json["temperature"] = readTemperatureSensor();
  json["humidity"] = readHumiditySensor();

  delay(250);

  serializeJson(json, ArduinoSerial);

  readFromNodemcu();
  clearJson();
}

DynamicJsonDocument readHumiditySensor()
{
  DynamicJsonDocument humidityObj(JSON_OBJECT_SIZE(1) + JSON_OBJECT_SIZE(3) + 97);

  float humidity = dht.readHumidity();
  humidityObj["type"] = "dht11";

  if (isnan(humidity))
  {
    Serial.println(F("Failed to read Humidity from DHT sensor!"));
    humidityObj["status"] = false;
    humidityObj["value"] = 0;

    return humidityObj;
  }

  humidityObj["status"] = true;
  humidityObj["value"] = humidity;

  return humidityObj;
}

DynamicJsonDocument readTemperatureSensor()
{
  DynamicJsonDocument temperatureObj(JSON_OBJECT_SIZE(1) + JSON_OBJECT_SIZE(3) + 97);
  float temperature = dht.readTemperature();
  temperatureObj["type"] = "dht11";

  if (isnan(temperature))
  {
    Serial.println(F("Failed to read Temperature from DHT sensor!"));
    temperatureObj["status"] = false;
    temperatureObj["value"] = 0;
    return temperatureObj;
  }

  temperatureObj["status"] = true;
  temperatureObj["value"] = temperature;

  return temperatureObj;
}

void readFromNodemcu()
{
  clearJson();

  if (ArduinoSerial.available() > 0)
  {
    String dataFromNodemcu = ArduinoSerial.readString();

    deserializeJson(json, dataFromNodemcu);
    Serial.println(dataFromNodemcu);
  }
}

void clearJson()
{
  json = JsonVariant();
}

void flushAll()
{
  while (ArduinoSerial.available())
    ArduinoSerial.read();
}
