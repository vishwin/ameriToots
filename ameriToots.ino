const int pingPin=2;

void setup() {
  Serial.begin(9600);
}

void loop() {
  execPing(pingPin);
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
