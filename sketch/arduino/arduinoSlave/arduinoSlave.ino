// Comumication to NODEMCU
#include <SoftwareSerial.h>

SoftwareSerial ArduinoSerial(2, 3); //(Rx Tx) ESP -> Arduino: Tx -> 2, Rx -> 3.
char data;

// Configuration of humidity (COH)
#include "DHT.h"

#define DHTPIN 5
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);


void setup() {

  Serial.begin(115200);
  ArduinoSerial.begin(9600); //bd from master (ESP)
  pinMode(4, OUTPUT);


  //COH
  dht.begin();

}

void loop()
{
  checkSerialCom();
  readHumiditySensor();
  readTemperatureSensor();

}


void readHumiditySensor() {

  float h = dht.readHumidity();

  if (isnan(h)) {
    Serial.println(F("Failed to read Humidity from DHT sensor!"));
    return;
  }



  Serial.print(F("Humidity: "));
  Serial.println(h);


  ArduinoSerial.print(F("Humidity: "));
  ArduinoSerial.print(h);
  ArduinoSerial.println("%");

  delay(500);

}


void readTemperatureSensor() {

  float t = dht.readTemperature();

  if (isnan(t)) {
    Serial.println(F("Failed to read Temperature from DHT sensor!"));
    return;

  }

  Serial.print(F("Temperature: "));
  Serial.print(t);
  Serial.println(F("°C "));

  ArduinoSerial.print(F("Temperature: "));
  ArduinoSerial.print(t);
  ArduinoSerial.println(F("°C "));

  delay(500);

}


void checkSerialCom() {
  String response;
  if (ArduinoSerial.available() > 0) {

    while (ArduinoSerial.available() > 0) {
      data = ArduinoSerial.read();
      response += data;
    }

    Serial.println(response);

    if (response == "on") {
      digitalWrite(4, LOW);
      ArduinoSerial.println("Turned on");
    }

    if (response == "off") {
      digitalWrite(4, HIGH);
      ArduinoSerial.println("Turned off");
    }
  }

  delay(500);
}
