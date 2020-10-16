#ifndef CPP_CONTAINER_FACTORY_H
#define CPP_CONTAINER_FACTORY_H

#include <iostream>
#include "Container1.h"

using namespace std;

class Container_Factory {
public:
    // we could return naked pointer or ref to Container1 but that's discouraged because it leads to resource leaks if the
    // container is not deleted thru the pointer (one pitfall of c++)
    // according to B.S. Objects 'owned' by unique_ptr need no destructors as the compiler generates one for them that does what is needed
    static unique_ptr<Container1> createContainer(initializer_list<double>, int);
};

#endif //CPP_CONTAINER_FACTORY_H
