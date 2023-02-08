#include <Arduino.h>
#include <SPI.h>

#define ACCEL_CS 5
#define ACCEL_SPI_CLOCK 5000000
uint32_t n = 5000000;

void setup() { Serial.begin(115200); }

void loop() {
    Serial.println(n);
    delay(1000);
}