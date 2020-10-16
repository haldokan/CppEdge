#include <iostream>
#include "headers/Container_Factory.h"
#include "headers/Vector_Container.h"

using namespace std;

unique_ptr<Container1> Container_Factory::createContainer(initializer_list<double> init, int type) {
    // here we can potentially have a switch that return different implementations of Container 1 based on type (better be enum, etc.)
    switch (type) {
        case 1:
            return unique_ptr<Container1>(new Vector_Container(init));
        default:
            throw invalid_argument(to_string(type));

    }

}
