#ifndef BRAINTRAIN_SEDAN_H
#define BRAINTRAIN_SEDAN_H

#include "Vehicle.h"

class Sedan : public Vehicle {
public:
    Sedan(int, int);

    ~Sedan() override;

    int top_speed() const override;

    int capacity() const override;

protected:
    int m_top_speed;
    int m_capacity;
};


#endif //BRAINTRAIN_SEDAN_H
