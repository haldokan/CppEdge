#ifndef CPP_VECTOR_CONTAINER_H
#define CPP_VECTOR_CONTAINER_H

#include "Container1.h"
#include "Vector_Container.h"

using namespace std;

class Vector_Container : public Container1 {
private:
    N::Vector vec; // N is the name space defined in Vector.h
public:
    Vector_Container(initializer_list<double>);

    int length() const override;

    double &operator[](int i) override;

    ~Vector_Container() override;
};

#endif //CPP_VECTOR_CONTAINER_H
