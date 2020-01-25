#include <SoftwareSerial.h>

SoftwareSerial ArduinoSerial(2, 3); //(Rx Tx) ESP -> Arduino: Tx -> 2, Rx -> 3.
char data;

void setup() {
  Serial.begin(115200);
  ArduinoSerial.begin(9600); //bd from master (ESP)
  pinMode(4, OUTPUT);
}

void loop()
{
  checkSerialCom();
  delay(50);
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
      ArduinoSerial.print("Turned on");
    }

    if (response == "off") {
      digitalWrite(4, HIGH);
      ArduinoSerial.print("Turned off");
    }
  }
}