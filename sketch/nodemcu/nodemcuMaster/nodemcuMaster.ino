#include <ArduinoJson.h> //6.10.0
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266HTTPClient.h>
#include <SoftwareSerial.h>

ESP8266WiFiMulti WiFiMulti;
SoftwareSerial MCUSerial(D2, D3); //Rx, Tx
String host = "https://hydro-udc.herokuapp.com",
       endPoint = "";
boolean cardRead = false;
char data;
HTTPClient hydroApi;
String fingerPrint = "08 3b 71 72 02 43 6e ca ed 42 86 93 ba 7e df 81 c4 bc 62 30";
DynamicJsonDocument json(1024);

void setup()
{
  WiFi.mode(WIFI_STA);

  WiFiMulti.addAP("Fuentech", "!Guapeton*");
  WiFiMulti.addAP("ROCKELIN", "Fuentes2018TR");

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
    endPoint = "/status";

    if (hydroApi.begin(host + endPoint, fingerPrint))
    {
      int statusCode = hydroApi.GET();

      if (statusCode > 0)
      {
        // file found at server
        if (statusCode == HTTP_CODE_OK || statusCode == HTTP_CODE_MOVED_PERMANENTLY)
        {
          String response = hydroApi.getString();

          Serial.println("From API: " + response);

          createJson();
          delay(250);
          serializeJson(json, MCUSerial);
          Serial.flush();

          readFromArduino();
          cleanConnection();
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
      delay(1000);
      cleanConnection();
    }
  }
  else
  {
    cleanConnection();
  }
}

void ledBlink(int pin, int seconds)
{
  digitalWrite(pin, LOW);
  delay(1000 * seconds);
  digitalWrite(pin, HIGH);
}

void cleanConnection()
{
  hydroApi.end();
  clearJson();
}

void readFromArduino()
{
  clearJson();

  if (MCUSerial.available() > 0)
  {
    flushAll();
    String dataFromArduino = MCUSerial.readString();

    deserializeJson(json, dataFromArduino);
    Serial.println(dataFromArduino);
  }
  delay(250);
}

void createJson()
{
  JsonObject pump = json.createNestedObject("pump");
  pump["status"] = false;
}

void clearJson()
{
  json = JsonVariant();
}

void flushAll()
{
  while (MCUSerial.available())
    MCUSerial.read();
}
