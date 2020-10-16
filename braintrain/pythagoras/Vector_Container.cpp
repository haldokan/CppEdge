#include "headers/Container1.h"
#include "headers/Vector_Container.h"

Vector_Container::Vector_Container(initializer_list<double> init) : vec(init) {}

int Vector_Container::length() const {
    return vec.get_size();
}

double &Vector_Container::operator[] (int i) {
    return vec[i];
}
// Note that the member destructor (~Vector()) is implicitly invoked by its class’s destructor (~Vector_container()).
Vector_Container::~Vector_Container() {}
