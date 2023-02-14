#include <Arduino.h>
#include "complementary.h"

Complementary::Complementary(float alpha) {
    this->alpha = constrainAlpha(alpha);
}

void Complementary::update(sensors_event_t *evt, orientation_t *orientation) {
    float dt = (evt->timestamp - prevEvt.timestamp) / 1000.0f;
    // accel data
    float aPitch = atan2(-evt->acceleration.x, sqrt( pow(evt->acceleration.y, 2)) + pow(evt->acceleration.z, 2) );
    float aRoll = atan2(evt->acceleration.y, sqrt( pow(evt->acceleration.x, 2) + pow(evt->acceleration.z, 2)) );
    // filter
    pitch = (alpha) * (pitch + evt->gyro.y * dt) + (1 - alpha) * aPitch;
    roll = (alpha) * (roll + evt->gyro.x * dt) + (1 - alpha) * aRoll;
    orientation->pitch = pitch;
    orientation->roll = roll;
}

void Complementary::setAlpha(float alpha) {
    this->alpha = constrainAlpha(alpha);
}

float Complementary::constrainAlpha(float alpha) {
    if (alpha < 0.0f) {
        alpha = 0.0f;
    } else if (alpha > 1.0f) {
        alpha = 1.0f;
    }
    return alpha;
}
