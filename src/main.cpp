#include <Arduino.h>
#include <Adafruit_ADXL343.h>
#include <Adafruit_GPS.h>
#include <Adafruit_Sensor.h>

// GPS
Adafruit_GPS gps(&Serial1);

// IMU
#define IMU_CS (5)
Adafruit_ADXL343 imu(IMU_CS, &SPI, 1);

// LoRa
#define LORA_CS (4)
#define LORA_RST (8)
#define LORA_DIO0 (7)

#define SPREADING_FACTOR (12)
#define BANDWIDTH (125E3)
#define FREQUENCY (915E6)
#define CODING_RATE_DENOMINATOR (5)
#define PREAMBLE_LENGTH (8)
#define syncWord (0x12)
#define TX_POWER (20)
#define CRC (true)

// Prototypes
void printAccel(sensors_event_t *evt);

void setup() {
    Serial.begin(9600); 
    imu.setDataRate(ADXL343_DATARATE_1600_HZ);
    imu.setRange(ADXL343_RANGE_4_G);
    imu.begin();
}

unsigned long prevTime = micros();

void loop() {
    sensors_event_t evt;
    imu.getEvent(&evt);
    if (millis() % 1000 < 5) {
        Serial.print("t:"); Serial.print(micros() - prevTime); Serial.print('\t');
        printAccel(&evt);
    }
    prevTime = micros();
}

void printAccel(sensors_event_t *evt) {
    Serial.print("x:"); Serial.print('\t'); Serial.print(evt->acceleration.x); Serial.print('\t');
    Serial.print("y:"); Serial.print('\t'); Serial.print(evt->acceleration.y); Serial.print('\t');
    Serial.print("z:"); Serial.print('\t'); Serial.print(evt->acceleration.z); Serial.print('\n');
}