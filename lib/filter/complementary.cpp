#include <Arduino.h>
#include "complementary.h"

Complementary::Complementary(float alpha) {
    this->alpha = constrainAlpha(alpha);
}

void Complementary::update(sensors_event_t *evt, orientation_t *orientation) {
    float dt = (evt->timestamp - prevEvt.timestamp) / 1000.0f;
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
