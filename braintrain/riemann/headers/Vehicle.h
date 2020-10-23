
#ifndef BRAINTRAIN_VEHICLE_H
#define BRAINTRAIN_VEHICLE_H

// pure virtual class: DO NOT put member fields here no provide a constructor: will cause misleading compilation errors
class Vehicle {
public:
    virtual int top_speed() const = 0;
    virtual int capacity() const = 0;
    virtual ~Vehicle() {}
};


#endif //BRAINTRAIN_VEHICLE_H
