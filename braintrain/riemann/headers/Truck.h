#ifndef BRAINTRAIN_TRUCK_H
#define BRAINTRAIN_TRUCK_H

#include "Vehicle.h"

class Truck : public Vehicle {
public:
    Truck(int, int);

    ~Truck() override;

    int top_speed() const override;

    int capacity() const override;

protected:
    int m_top_speed;
    int m_capacity;
};


#endif //BRAINTRAIN_TRUCK_H
