#include <iostream>
#include "Vector.h"

using namespace std;
using namespace N;

Vector::Vector(int _len): Vector(_len, Category::small) {};

// note that there are two syntactic styles of constructors: this one and the one below
Vector::Vector(int _len, Category _category) {
    if (_len < 0) {
        throw length_error("negative vector length: " + to_string(_len));
    }
    length = _len;
    items = new double[_len];
    category = _category;
}

// you gotta love this static-cast crap: arrays subscripts are unsigned int so they need the case to int here
Vector::Vector(initializer_list<double> init_list): length {static_cast<int>(init_list.size())}, items { new double [init_list.size()]} {
    copy(init_list.begin(), init_list.end(), items);
}

int Vector::get_size() const {
    return length;
}

double& Vector::operator[](int index) {
    if (index < 0 || index >= get_size()) {
        throw out_of_range("index: [" + to_string(index) + "] is out of range: [0, " + to_string(index) + "]");
    }
    return items[index];
}

const Category Vector::get_category() noexcept {
    return category;
}

// if a func creates a Vector and the func executes and goes out of scope this destructor is called to free heap memory
Vector::~Vector() {
    cout << "destructor is called" << endl;
    delete [] items; // plain delete deletes a single object, delete[] deletes the array
}
