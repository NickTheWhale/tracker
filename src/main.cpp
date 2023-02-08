#include <Arduino.h>

void setup() {
  Serial.begin(115200);
}

void loop() {
  Serial.println(micros());
  delay(1000);
}