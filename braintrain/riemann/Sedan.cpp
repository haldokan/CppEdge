#include "headers/Sedan.h"
#include "headers/Vehicle.h"

Sedan::Sedan(int top_speed, int capacity) : m_top_speed(top_speed), m_capacity(capacity) {}

int Sedan::top_speed() const {
    return m_top_speed;
}

int Sedan::capacity() const {
    return m_capacity;
}

Sedan::~Sedan() {

}
