#include <ArduinoJson.h> //6.10.0
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266HTTPClient.h>
#include "DHT.h"

#define DHTTYPE DHT11
ESP8266WiFiMulti WiFiMulti;
DHT dht(D1, DHTTYPE);
HTTPClient hydroApi;
DynamicJsonDocument outputJson(JSON_OBJECT_SIZE(3) + 3 * JSON_OBJECT_SIZE(4) + 348);
DynamicJsonDocument inputJson(JSON_OBJECT_SIZE(1) + JSON_OBJECT_SIZE(2) + 70);
String host = "https://hydro-udc.herokuapp.com",
       endPoint = "",
       agentCode = "mcu-h1",
       from = "mcu",
       agentId = "1",
       type = "pump",
       fingerPrint = "08 3b 71 72 02 43 6e ca ed 42 86 93 ba 7e df 81 c4 bc 62 30";

const int Trigger = D2;   //Pin digital 6 para el Trigger del sensor
const int Echo = D3;   //Pin digital 7 para el Echo del sensor
long minRange = 1;
long maxRange = 100;
const int pumpPin = 4;
int floaterpin = 8; // definimos el pin

void setup()
{
  WiFi.mode(WIFI_STA);

  WiFiMulti.addAP("Fuentech", "!Guapeton*");
  WiFiMulti.addAP("ROCKELIN", "Fuentes2018TR");
  WiFiMulti.addAP("RED NITRO 5", "MasterCode");

  pinMode(D4, OUTPUT);
  pinMode(D2, OUTPUT);
  pinMode(D3, INPUT);
  pinMode(D0, OUTPUT);

  digitalWrite(D0, HIGH);
  Serial.begin(115200);

  while (WiFiMulti.run() != WL_CONNECTED)
  {
    digitalWrite(D4, HIGH);
    delay(500);
    Serial.print(".");
  }
  digitalWrite(D4, LOW);
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(WiFi.SSID().c_str());
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

void loop()
{
  if (WiFiMulti.run() == WL_CONNECTED)
  {
    String data = "";
    getActuatorValue();
    outputJson.add(readTemperatureSensor());
    outputJson.add(readHumiditySensor());
    outputJson.add(readUltraSoundSensor());

    serializeJson(outputJson, data);

    updateSensorValues(data);
    cleanConnection();
  }
  else
  {
    cleanConnection();
  }

  delay(500);
}

void cleanConnection()
{
  hydroApi.end();
  clearInputJson();
}

void clearInputJson() {
  inputJson = JsonVariant();
  outputJson = JsonVariant();
}

void getActuatorValue() {
  endPoint = "/actuator/" + agentId + "?agentCode=" + agentCode + "&from=" + from + "&type=" + type;

  if (hydroApi.begin(host + endPoint, fingerPrint))
  {
    int statusCode = hydroApi.GET();

    if (statusCode > 0)
    {
      if (statusCode == HTTP_CODE_OK || statusCode == HTTP_CODE_MOVED_PERMANENTLY)
      {
        String response = hydroApi.getString();

        Serial.println("From API: " + response);
        DeserializationError error = deserializeJson(inputJson, response);

        if (!error) {
          takeAction();
        }
        else {
          clearInputJson();

          return;
        }
      }
    }
    else
    {
      Serial.printf("[HTTP] GET failed, error: %s\n", hydroApi.errorToString(statusCode).c_str());
    }
  }
  else
  {
    Serial.printf("[HTTP] Unable to connect\n");
    delay(500);
    cleanConnection();
  }
}

void updateSensorValues(String sensorData) {
  Serial.println("Update data: ->" + sensorData);
  endPoint = "/sensor/" + agentId + "?agentCode=" + agentCode;

  if (hydroApi.begin(host + endPoint, fingerPrint)) {
    hydroApi.addHeader("Content-Type", "application/json");
    hydroApi.PUT(sensorData);

    /*
      if (statusCode > 0)
      {
      if (statusCode == HTTP_CODE_OK || statusCode == HTTP_CODE_MOVED_PERMANENTLY)
      {
        String response = hydroApi.getString();
        Serial.println("From API: " + response);
      }
      }
      else
      {
      Serial.printf("[HTTP] GET failed, error: %s\n", hydroApi.errorToString(statusCode).c_str());
      }
    */
  }
  else
  {
    Serial.printf("[HTTP] Unable to connect\n");
    delay(1000);
    cleanConnection();
  }
}

DynamicJsonDocument readHumiditySensor()
{
  DynamicJsonDocument humidityObj(JSON_OBJECT_SIZE(1) + JSON_OBJECT_SIZE(3) + 97);

  float humidity = dht.readHumidity();
  humidityObj["type"] = "dht11";
  humidityObj["code"] = "hs1-h1";
  if (isnan(humidity))
  {
    Serial.println(F("Failed to read Humidity from DHT sensor!"));
    humidityObj["status"] = false;
    humidityObj["value"] = 0;

    return humidityObj;
  }

  humidityObj["status"] = true;
  humidityObj["value"] = humidity;

  serializeJsonPretty(humidityObj, Serial);
  return humidityObj;
}

DynamicJsonDocument readFloaterLevelSensor()
{
  DynamicJsonDocument floaterObj(JSON_OBJECT_SIZE(1) + JSON_OBJECT_SIZE(3) + 97);
  int floater = digitalRead(floaterpin);
  String fullOrNot;

  if (floater == HIGH) {
    fullOrNot = "low";
    //turnONOffPump(LOW);
    Serial.println(F("The tank is not full please fill it!"));
  }
  else {
    fullOrNot = "full";
  }

  floaterObj["type"] = "wt-40";
  floaterObj["code"] = "fs1-h1";
  if (isnan(floater))
  {
    Serial.println(F("Failed to read Distance from Floater Level sensor!"));
    floaterObj["status"] = false;
    floaterObj["value"] = 0;
    return floaterObj;
  }

  floaterObj["status"] = true;
  floaterObj["value"] = fullOrNot;

  serializeJsonPretty(floaterObj, Serial);
  return floaterObj;

}

DynamicJsonDocument readUltraSoundSensor()
{
  DynamicJsonDocument distanceObj(JSON_OBJECT_SIZE(1) + JSON_OBJECT_SIZE(3) + 97);
  long distance = distanceToPercent();
  distanceObj["type"] = "hc-sr04";
  distanceObj["code"] = "us1-h1";
  if (isnan(distance))
  {
    Serial.println(F("Failed to read Distance from UltraSound sensor!"));
    distanceObj["status"] = false;
    distanceObj["value"] = 0;
    return distanceObj;
  }

  distanceObj["status"] = true;
  distanceObj["value"] = distance;

  serializeJsonPretty(distanceObj, Serial);
  return distanceObj;
}

DynamicJsonDocument readTemperatureSensor()
{
  DynamicJsonDocument temperatureObj(JSON_OBJECT_SIZE(1) + JSON_OBJECT_SIZE(3) + 97);
  float temperature = dht.readTemperature();
  temperatureObj["type"] = "dht11";
  temperatureObj["code"] = "ts1-h1";
  if (isnan(temperature))
  {
    Serial.println(F("Failed to read Temperature from DHT sensor!"));
    temperatureObj["status"] = false;
    temperatureObj["value"] = 0;
    return temperatureObj;
  }

  temperatureObj["status"] = true;
  temperatureObj["value"] = temperature;

  serializeJsonPretty(temperatureObj, Serial);
  return temperatureObj;
}

long getDistanceUlt() {
  long timeToRetornEco; //timepo que demora en llegar el eco
  long distanceOnCm; //distancia en centimetros

  digitalWrite(D2, HIGH);
  delayMicroseconds(10);          //Enviamos un pulso de 10us
  digitalWrite(D2, LOW);

  timeToRetornEco = pulseIn(D3, HIGH); //obtenemos el ancho del pulso
  distanceOnCm = timeToRetornEco / 59;

  delay(100);
  Serial.print("distance on cm: ");
  Serial.println(distanceOnCm);
  return distanceOnCm;

}

void turnONOffPump(bool value) {
  Serial.println("Pump: "+String(value));
  digitalWrite(D0, value);
}

void takeAction() {
  bool value = inputJson["pump"]["value"];
  turnONOffPump(!value);
}


float distanceToPercent() {
  float percentNew = 0;
  long distance = getDistanceUlt();
  percentNew = ((distance * 100) / maxRange);
  Serial.println(String(percentNew) + "% " + String(distance));
  return percentNew;
}
