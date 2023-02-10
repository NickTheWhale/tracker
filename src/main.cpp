#include <Arduino.h>
#include <Adafruit_GPS.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_ADXL343.h>

// GPS
Adafruit_GPS gps(&Serial1);

// IMU
#define IMU_CS (5)

Adafruit_ADXL343 imu(IMU_CS, &SPI, 1);

struct position_t {
    float x;
    float y;
    float z;
    float d;
    uint32_t t;
};

position_t position;

void updatePosition(position_t *p, sensors_event_t *evt);
void printPosition(position_t *p);

void setup() {
    Serial.begin(9600);
    delay(2000);
    
    imu.begin();
    imu.setDataRate(ADXL343_DATARATE_100_HZ);
    imu.setRange(ADXL343_RANGE_2_G);

    delay(5000);
    sensors_event_t evt;
    imu.getEvent(&evt);
    updatePosition(&position, &evt);
}

void loop() { 
    sensors_event_t evt;
    imu.getEvent(&evt);
    updatePosition(&position, &evt);
    printPosition(&position);
    delay(50);
}

void updatePosition(position_t *p, sensors_event_t *evt) {
    uint32_t dt = evt->timestamp - p->t;
    p->t = evt->timestamp;
    
    int16_t total = (int16_t)sqrt( (evt->acceleration.x * evt->acceleration.x) 
    + (evt->acceleration.x * evt->acceleration.x) 
    + (evt->acceleration.x * evt->acceleration.x));

    p->x += evt->acceleration.x * dt * dt / 1000000.0f;
    p->y += evt->acceleration.y * dt * dt / 1000000.0f;
    p->z += evt->acceleration.z * dt * dt / 1000000.0f;
    p->d += total * dt * dt / 1000000.0f;
}

void printPosition(position_t *p) {
    Serial.print("x:"); Serial.print('\t'); Serial.print(p->x); Serial.print('\t');
    Serial.print("y:"); Serial.print('\t'); Serial.print(p->y); Serial.print('\t');
    Serial.print("z:"); Serial.print('\t'); Serial.print(p->z); Serial.print('\t');
    Serial.print("d:"); Serial.print('\t'); Serial.print(p->d); Serial.print('\n');
}

