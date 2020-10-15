#ifndef CPP_CONTAINER1_H
#define CPP_CONTAINER1_H

#include "../../chp2/Vector.h"

class Container1 {
public:
    // const indicate that this func will not change its object
    virtual int length() const = 0; // the = 0 indicate pure virtual (must be implemented in a derived class)
    virtual double &operator[](int) = 0; // return a ref to double so that we can access & change vals at subscript
    virtual ~Container1() {}; // will delegate to derived classes destructors
};

#endif //CPP_CONTAINER1_H
