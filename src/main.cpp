#include <Arduino.h>
#include <Adafruit_GPS.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_ADXL343.h>

#define SERIAL_DEBUG

// GPS
Adafruit_GPS gps(&Serial1);

// IMU
#define IMU_CS (5)

Adafruit_ADXL343 imu(IMU_CS, &SPI, 1);

typedef struct {
    float x;
    float y;
    float z;
    float d;
    uint32_t t;
} position_t;

typedef struct {
    int16_t x;
    int16_t y;
    int16_t z;
} offset_t;

typedef enum {
    DEBUG,
    INFO,
    WARN,
    ERROR
} debug_level_t;

position_t g_position;
offset_t g_offsets;

void updatePosition(position_t *p, sensors_event_t *evt, offset_t *offsets = nullptr);
void printPosition(position_t *p);
void calculateOffsets(Adafruit_ADXL343 *imu, offset_t *offset, uint32_t averagingTimeMs);
void debugMsg(const char* msg, debug_level_t level = DEBUG);

void setup() {
    Serial.begin(9600);
    
    delay(2500);
    
    imu.begin();
    imu.setDataRate(ADXL343_DATARATE_100_HZ);
    imu.setRange(ADXL343_RANGE_2_G);

    calculateOffsets(&imu, &g_offsets, 5000);

    sensors_event_t evt;
    imu.getEvent(&evt);
    updatePosition(&g_position, &evt, &g_offsets);
}

void loop() { 
    sensors_event_t evt;
    imu.getEvent(&evt);
    updatePosition(&g_position, &evt, &g_offsets);
    printPosition(&g_position);
    delay(25);
}

void updatePosition(position_t *p, sensors_event_t *evt, offset_t *offsets) {
    uint32_t dt = evt->timestamp - p->t;
    p->t = evt->timestamp;
    
    if (offsets != nullptr) {
        evt->acceleration.x - abs(offsets->x);
        evt->acceleration.y - abs(offsets->y);
        evt->acceleration.z - abs(offsets->z);
    }

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

void calculateOffsets(Adafruit_ADXL343 *imu, offset_t *offset, uint32_t averagingTimeMs) {
    #if defined(SERIAL_DEBUG)
    debugMsg("Calculating IMU offsets");
    #endif // SERIAL_DEBUG
    
    const int SMOOTHING_FACTOR = 8;    
    sensors_event_t evt;
    sensors_event_t prevEvt;
    imu->getEvent(&evt);

    uint32_t startTime = millis();
    while (startTime + averagingTimeMs > millis()) {

        evt.acceleration.x += (prevEvt.acceleration.x - evt.acceleration.x) / SMOOTHING_FACTOR;
        evt.acceleration.y += (prevEvt.acceleration.y - evt.acceleration.y) / SMOOTHING_FACTOR;
        evt.acceleration.z += (prevEvt.acceleration.z - evt.acceleration.z) / SMOOTHING_FACTOR;

        imu->getEvent(&prevEvt);
        delay(10);

        #if defined(SERIAL_DEBUG)
        Serial.print("x:"); Serial.print('\t'); Serial.print(evt.acceleration.x); Serial.print('\t');
        Serial.print("y:"); Serial.print('\t'); Serial.print(evt.acceleration.y); Serial.print('\t');
        Serial.print("z:"); Serial.print('\t'); Serial.print(evt.acceleration.z); Serial.print('\n');
        #endif // SERIAL_DEBUG
    }

    offset->x = evt.acceleration.x;
    offset->y = evt.acceleration.y;
    offset->z = evt.acceleration.z;

    #if defined(SERIAL_DEBUG)
    debugMsg("Calculated offsets: ");
    Serial.print("x:"); Serial.print('\t'); Serial.print(evt.acceleration.x); Serial.print('\t');
    Serial.print("y:"); Serial.print('\t'); Serial.print(evt.acceleration.y); Serial.print('\t');
    Serial.print("z:"); Serial.print('\t'); Serial.print(evt.acceleration.z); Serial.print('\n');
    #endif // SERIAL_DEBUG

}

void debugMsg(const char* msg, debug_level_t level) {
    char* debugLevel = nullptr;
    switch (level) {
        case DEBUG:
            debugLevel = (char*)"DEBUG";
        case INFO:
            debugLevel = (char*)"INFO";
        case WARN:
            debugLevel = (char*)"WARN";
        case ERROR:
            debugLevel = (char*)"ERROR";
        default:
            debugLevel = (char*)"INFO";
    }
    Serial.print("["); Serial.print(debugLevel); Serial.print("]"); Serial.print('\t');
    Serial.print("["); Serial.print(millis()); Serial.print("]"); Serial.print('\t');
    Serial.print(msg);
}

