#include "headers/Truck.h"

//Vector_Container::Vector_Container(initializer_list<double> init) : vec(init) {}
Truck::Truck(int top_speed, int capacity) : m_top_speed(top_speed), m_capacity(capacity) {}

int Truck::top_speed() const {
    return m_top_speed;
}

int Truck::capacity() const {
    return m_capacity;
}

Truck::~Truck() {

}
