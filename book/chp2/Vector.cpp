#include <iostream>
#include "Vector.h"

using namespace std;
using namespace N;

Vector::Vector(int _len): Vector(_len, Category::small) {};

Vector::Vector(int _len, Category _category) {
    if (_len < 0) {
        throw length_error("negative vector length: " + to_string(_len));
    }
    length = _len;
    items = new double[_len];
    category = _category;
}

int Vector::get_size() {
    return length;
}

double& Vector::operator[](int index) {
    if (index < 0 || index >= get_size()) {
        throw out_of_range("index: [" + to_string(index) + "] is out of range: [0, " + to_string(index) + "]");
    }
    return items[index];
}

Category Vector::get_category() noexcept {
    return category;
}
