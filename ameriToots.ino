#include <WiFi.h>
#include <WiFiClient.h>
#include <WiFiServer.h>
#include <WiFiUdp.h>

char ssid[]="Mysterious Rice Jawn";
extern char passwd[];
int status=WL_IDLE_STATUS;
WiFiServer server(80);

const int pingPin=2;

void setup() {
  Serial.begin(9600);
  while (!Serial);
  if (WiFi.status()==WL_NO_SHIELD) {
    while (true);
  }
  while (status!=WL_CONNECTED) {
    Serial.print("Attempting to connect to network ");
    Serial.println(ssid);
    status=WiFi.begin(ssid, passwd);
    delay(10000);
  }
  Serial.print("Connected. IP address ");
  Serial.println(WiFi.localIP());
  server.begin();
  Serial.println("HTTP server listening on port 80");
}

void loop() {
  WiFiClient client=server.available();
  if (client) {
    Serial.println("New client");
    String currentLine="";
    while (client.connected()) {
      if (client.available()) {
        char c=client.read();
        Serial.write(c);
        if (c=='\n') {
          if (currentLine.length()==0) {
            long vol=execPing(pingPin);
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type: text/plain");
            client.println();

            client.println(vol);
            client.println();

            Serial.println("\"GET / HTTP/1.1\" 200");
            break;
          }
          else {
            currentLine="";
          }
        }
        else if (c!='\r') {
          currentLine+=c;
        }
      }
    }
    delay(1);
    client.stop();
  }
}

long execPing(const int pin) {
  long duration, vol;
  float cm;

  // The PING))) is triggered by a HIGH pulse of 2 or more microseconds.
  // Give a short LOW pulse beforehand to ensure a clean HIGH pulse:
  pinMode(pin, OUTPUT);
  digitalWrite(pin, LOW);
  delayMicroseconds(2);
  digitalWrite(pin, HIGH);
  delayMicroseconds(5);
  digitalWrite(pin, LOW);

  // The same pin is used to read the signal from the PING))): a HIGH
  // pulse whose duration is the time (in microseconds) from the sending
  // of the ping to the reception of its echo off of an object.
  pinMode(pin, INPUT);
  duration=pulseIn(pin, HIGH);

  // convert the time into a distance
  cm=microsecondsToCentimeters(duration);

  // then convert to volume according to bottle markings
  vol=(18-(cm-7))*100+200;

  Serial.print(vol);
  Serial.println("ml");

  delay(100);
  return vol;
}

float microsecondsToCentimeters(long microseconds) {
  // The speed of sound is 340 m/s or 29 microseconds per centimeter.
  // The ping travels out and back, so to find the distance of the
  // object we take half of the distance travelled.
  return microseconds / 29 / 2;
}
