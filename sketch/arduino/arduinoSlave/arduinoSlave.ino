/*
  1) si el nivel del agua baja apagarbomba y notificar json con el nivel
  2) si el nivel de la humedad es mayor a y menor a, notificar
  3)
*/

#include <ArduinoJson.h>
#include <SoftwareSerial.h>
#include "DHT.h"

#define DHTPIN 5
#define DHTTYPE DHT11

SoftwareSerial ArduinoSerial(2, 3); //(Rx Tx) ESP -> Arduino: Tx -> 2, Rx -> 3.
DynamicJsonDocument outputJson(JSON_ARRAY_SIZE(3) + 3 * JSON_OBJECT_SIZE(4) + 348);
DynamicJsonDocument inputJson(JSON_OBJECT_SIZE(1) + JSON_OBJECT_SIZE(2) + 70);
DHT dht(DHTPIN, DHTTYPE);
const int Trigger = 6;   //Pin digital 6 para el Trigger del sensor
const int Echo = 7;   //Pin digital 7 para el Echo del sensor
long minRange = 1;
long maxRange = 100;
const int pumpPin = 4;
int floaterpin = 8; // definimos el pin


void setup()
{
  Serial.begin(115200);
  ArduinoSerial.begin(9600);
  dht.begin();
  pinMode(floaterpin, INPUT_PULLUP);
  pinMode(pumpPin, OUTPUT);
  digitalWrite(pumpPin, HIGH);
  pinMode(Trigger, OUTPUT); //pin como salida
  pinMode(Echo, INPUT);  //pin como entrada
  digitalWrite(Trigger, LOW);//Inicializamos el pin con 0

}

void loop()
{
  readFromNodemcu();
  clearOutputJson();

  outputJson.add(readTemperatureSensor());
  outputJson.add(readHumiditySensor());
  outputJson.add(readUltraSoundSensor());
  outputJson.add(readFloaterLevelSensor());
  delay(150);
  //distanceToPercent();

  serializeJson(outputJson, ArduinoSerial);
  clearOutputJson();
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

  return distanceObj;
}

long getDistanceUlt() {
  long timeToRetornEco; //timepo que demora en llegar el eco
  long distanceOnCm; //distancia en centimetros

  digitalWrite(Trigger, HIGH);
  delayMicroseconds(10);          //Enviamos un pulso de 10us
  digitalWrite(Trigger, LOW);

  timeToRetornEco = pulseIn(Echo, HIGH); //obtenemos el ancho del pulso
  distanceOnCm = timeToRetornEco / 59;

  delay(100);
  Serial.print("distance on cm: ");
  Serial.println(distanceOnCm);
  return distanceOnCm;

}

float distanceToPercent() {
  float percentNew = 0;
  long distance = getDistanceUlt();
  percentNew = ((distance * 100) / maxRange);
  Serial.println(String(percentNew) + "% " + String(distance));
  return percentNew;
}

void readFromNodemcu()
{
  if (ArduinoSerial.available() > 0)
  {
    String dataFromNodemcu = ArduinoSerial.readString();
    Serial.println("From MCU: " + dataFromNodemcu);
    DeserializationError error = deserializeJson(inputJson, dataFromNodemcu);

    if (!error) {
      takeAction();
    }
    else {
      clearInputJson();

      return;
    }
  }

  clearInputJson();
  delay(250);
}

void turnONOffPump(bool value) {
  digitalWrite(pumpPin, value);

}

void takeAction() {
  bool value = inputJson["pump"]["value"];
  //digitalWrite(pumpPin, !value);
  turnONOffPump(!value);
}

void clearOutputJson()
{
  outputJson = JsonVariant();
}

void clearInputJson() {
  inputJson = JsonVariant();
}

void flushAll()
{
  while (ArduinoSerial.available())
    ArduinoSerial.read();
}
