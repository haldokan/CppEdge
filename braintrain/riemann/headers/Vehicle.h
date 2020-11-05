
#ifndef BRAINTRAIN_VEHICLE_H
#define BRAINTRAIN_VEHICLE_H

// pure virtual class: DO NOT put member fields here nor provide a constructor: will cause misleading compilation errors
class Vehicle {
public:
    [[nodiscard]] virtual int top_speed() const = 0;
    [[nodiscard]] virtual int capacity() const = 0;
    virtual ~Vehicle() = default;
};


#endif //BRAINTRAIN_VEHICLE_H
