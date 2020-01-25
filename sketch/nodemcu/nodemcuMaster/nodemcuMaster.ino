#include <ArduinoJson.h> //6.10.0
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266HTTPClient.h>
#include <SoftwareSerial.h>

// #define D0 16
// #define D1 5
// #define D2 4
// #define D3 0
// #define D4 2

ESP8266WiFiMulti WiFiMulti;
SoftwareSerial MCUSerial(D2, D3); //Rx, Tx
String host = "https://hydro-udc.herokuapp.com",
       endPoint = "";
boolean cardRead = false;
char data;
HTTPClient hydroApi;
String fingerPrint = "08 3b 71 72 02 43 6e ca ed 42 86 93 ba 7e df 81 c4 bc 62 30";
//const size_t capacity = JSON_ARRAY_SIZE(16) + 3 * JSON_OBJECT_SIZE(6) + 5 * JSON_OBJECT_SIZE(7) + 8 * JSON_OBJECT_SIZE(9) + 5784;
//DynamicJsonDocument doc(capacity);

void setup()
{
    WiFi.mode(WIFI_STA);

    WiFiMulti.addAP("Fuentech", "!Guapeton*");

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
        //Serial.println("\n[Connecting to %s ... " + host);

        //Serial.println("[HTTP] begin...");
        endPoint = "/status";

        if (hydroApi.begin(host + endPoint, fingerPrint))
        {
            int statusCode = hydroApi.GET();

            if (statusCode > 0)
            {
                Serial.printf("statusCode: %d\n", statusCode);

                // file found at server
                if (statusCode == HTTP_CODE_OK || statusCode == HTTP_CODE_MOVED_PERMANENTLY)
                {
                    String response = hydroApi.getString();
                    // DeserializationError error = deserializeJson(doc, responseData);

                    // if (error)
                    //     return;

                    // for (JsonObject user : doc.as<JsonArray>())
                    // {
                    //     Serial.println(user["name"].as<String>());
                    // }

                    //serializeJsonPretty(doc, Serial);
                    Serial.println("From API: " + response);
                    MCUSerial.print(response);
                    readFromArduino();
                    delay(50);
                    //          switch (response) {
                    //            case "on":
                    //              digitalWrite(D3, LOW);
                    //              break;
                    //            case: "off":
                    //              digitalWrite(D3, HIGH);
                    //              break;
                    //            default:
                    //              break;
                    //          }

                    //            if(response == "on")
                    //              digitalWrite(D3, LOW);
                    //            if(response == "off")
                    //              digitalWrite(D3, HIGH);
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
        delay(50);
    }
    else
    {
        cleanConnection();
    }

    delay(50);
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
    Serial.println("Disconnected\n");
}

void readFromArduino()
{
    String response;
    if (MCUSerial.available() > 0)
    {
        while (MCUSerial.available() > 0)
        {
            data = MCUSerial.read();
            response += data;
        }

        Serial.println("From Arduino: " + response);
    }
}