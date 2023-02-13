#include <Arduino.h>
#include <Adafruit_ADXL343.h>
#include <Adafruit_GPS.h>
#include <Adafruit_Sensor.h>
#include "complementary.h"

// GPS
Adafruit_GPS gps(&Serial1);

// IMU
#define IMU_CS (5)

Adafruit_ADXL343 imu(IMU_CS, &SPI, 1);

void printOrientation(orientation_t *orientation);

void setup() {
    Serial.begin(9600); 
    imu.setDataRate(ADXL343_DATARATE_200_HZ);
    imu.setRange(ADXL343_RANGE_2_G);
    imu.begin();
}

void loop() {
    sensors_event_t evt;
    imu.getEvent(&evt);
}

void printOrientation(orientation_t *orientation) {
    Serial.print("roll: "); Serial.print('\t'); Serial.print(orientation->roll); Serial.print('\t');
    Serial.print("pitch: "); Serial.print('\t'); Serial.print(orientation->pitch); Serial.print('\n');
}