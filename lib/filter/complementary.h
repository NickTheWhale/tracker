#pragma once
#include <Arduino.h>

typedef struct {
    float x;
    float y;
    float z;
} vec3_t;

typedef struct {
    float roll;
    float pitch;
    uint32_t t;
} orientation_t;

class Complementary {
   private:
    float alpha;

    orientation_t prevOrientation;

    float constrainAlpha(float alpha) {
        if (alpha < 0.0f) {
            alpha = 0.0f;
        } else if (alpha > 1.0f) {
            alpha = 1.0f;
        }
        return alpha;
    }

   public:
    Complementary(float alpha);
    void setAlpha(float alpha);
    void update(vec3_t *accel, vec3_t *gyro, orientation_t *orientation);
};