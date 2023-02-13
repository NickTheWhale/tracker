#include "complementary.h"
#include <Arduino.h>

Complementary::Complementary(float alpha) {
    this->alpha = constrainAlpha(alpha);
}

void Complementary::update(vec3_t *accel, vec3_t *gyro, orientation_t *orientation) {
    uint32_t dt = (millis() - prevOrientation.t) / 1000;

    float aroll = atan(accel->y / sqrt(accel->x * accel->x + accel->z * accel->z));
    float apitch = atan(-accel->x / sqrt(accel->y * accel->y + accel->z * accel->z));

    prevOrientation.roll = alpha*(prevOrientation.roll + gyro->x * dt) + (1 - alpha)*aroll;
    prevOrientation.pitch = alpha*(prevOrientation.pitch + gyro->y * dt) + (1 - alpha)*apitch;
    prevOrientation.t = millis();
}

void Complementary::setAlpha(float alpha) {
    this->alpha = constrainAlpha(alpha);
}

