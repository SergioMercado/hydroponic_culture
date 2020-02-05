#include <ArduinoJson.h> //6.10.0
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266HTTPClient.h>
#include <SoftwareSerial.h>

ESP8266WiFiMulti WiFiMulti;
SoftwareSerial MCUSerial(D2, D3); //Rx, Tx
HTTPClient hydroApi;
DynamicJsonDocument inputJson(JSON_OBJECT_SIZE(3) + 3 * JSON_OBJECT_SIZE(4) + 140);
String host = "https://hydro-udc.herokuapp.com",
       endPoint = "",
       agentCode = "mcu-h1",
       from = "mcu",
       agentId = "1",
       type = "pump",
       fingerPrint = "08 3b 71 72 02 43 6e ca ed 42 86 93 ba 7e df 81 c4 bc 62 30";

void setup()
{
  WiFi.mode(WIFI_STA);

  WiFiMulti.addAP("Fuentech", "!Guapeton*");
  WiFiMulti.addAP("ROCKELIN", "Fuentes2018TR");
  WiFiMulti.addAP("RED NITRO 5", "MasterCode");

  pinMode(D4, OUTPUT);

  Serial.begin(115200);
  MCUSerial.begin(9600);

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
    getActuatorValue();
    String data =  readFromArduino();

    if (data != "error") {
      updateSensorValues(data);
    }

    cleanConnection();
  }
  else
  {
    cleanConnection();
  }
  //delay(150);
}

void cleanConnection()
{
  hydroApi.end();
  clearInputJson();
}

String readFromArduino()
{
  clearInputJson();

  if (MCUSerial.available() > 0)
  {
    flushAll();
    String dataFromArduino = MCUSerial.readString();
    Serial.println("From arduino: " + dataFromArduino);

    return dataFromArduino;
  }
  return "error";
}

void clearInputJson() {
  inputJson = JsonVariant();
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
        MCUSerial.println(response);
        Serial.flush();
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

void flushAll()
{
  while (MCUSerial.available())
    MCUSerial.read();
}
