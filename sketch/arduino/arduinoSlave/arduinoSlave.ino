/*
1) si el nivel del agua baja apagarbomba y notificar json con el nivel
2) si el nivel de la humedad es mayor a y menor a, notificar
3)
*/

#include <ArduinoJson.h>
#include <SoftwareSerial.h>

SoftwareSerial ArduinoSerial(2, 3); //(Rx Tx) ESP -> Arduino: Tx -> 2, Rx -> 3.
char data;
DynamicJsonDocument json(1024);
//Configuration of UltraSound
const int Trigger = 6;   //Pin digital 2 para el Trigger del sensor
const int Echo = 7;   //Pin digital 3 para el Echo del sensor
long minRange = 1; 
long maxRange = 100;


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
  //json["waterLevel"] = readUltraSoundSensor();
  delay(250);
  distanceToPercent();
  serializeJson(json, ArduinoSerial);
  
  readFromNodemcu();
  clearJson();
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

  return humidityObj;
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

  return temperatureObj;
}

DynamicJsonDocument readUltraSoundSensor()
{
  DynamicJsonDocument distanceObj(JSON_OBJECT_SIZE(1)+JSON_OBJECT_SIZE(3)+97);
  long distance = getDistanceUlt();
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

  return distanceObj;
}

long getDistanceUlt(){
  long timeToRetornEco; //timepo que demora en llegar el eco
  long distanceOnCm; //distancia en centimetros

  digitalWrite(Trigger, HIGH);
  delayMicroseconds(10);          //Enviamos un pulso de 10us
  digitalWrite(Trigger, LOW);
  
  timeToRetornEco = pulseIn(Echo, HIGH); //obtenemos el ancho del pulso
  distanceOnCm = timeToRetornEco/59;
  Serial.println(distanceOnCm);  
  return distanceOnCm;
}

float distanceToPercent(){
  float percentNew = 0;
  long distance=getDistanceUlt();
  percentNew = ((distance * 100) /maxRange);
  Serial.println(percentNew);
  Serial.println(String(percentNew)+"% "+String(distance));
  return percentNew;
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
