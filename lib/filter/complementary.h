#pragma once
#include <Arduino.h>
#include <Adafruit_ADXL343.h>

typedef struct {
    float roll;
    float pitch;
} orientation_t;

class Complementary {
   private:
    float alpha;
    float pitch;
    float roll;

    sensors_event_t prevEvt;
    orientation_t prevOrientation;

    float constrainAlpha(float alpha);

   public:
    Complementary(float alpha);
    void setAlpha(float alpha);
    void update(sensors_event_t *evt, orientation_t *orientation);
};