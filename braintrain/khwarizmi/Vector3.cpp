#include "headers/Vector3.h"
#include <iostream>

using namespace std;

Vector3::Vector3(int len) {
    cout << "Vector3-conversion-cntr is called" << endl;
    if (len < 0) {
        throw length_error("negative vector length: " + to_string(len));
    }
    length = len;
    items = new double[len];
}

Vector3::Vector3(initializer_list<double> init): length {static_cast<int>(init.size())}, items{new double[length]} {
    cout << "Vector3-initializer-list-cntr is called" << endl;
    copy(init.begin(), init.end(), items);
}

Vector3::Vector3(const Vector3 & v): length {v.length}, items {new double[v.length]} {
    cout << "Vector3-copy-cntr is called" << endl;
    for (int i = 0; i != length; ++i) {
        items[i] = v.items[i];
    }
}

Vector3::Vector3(Vector3 &&v) noexcept: length {v.length}, items {v.items} { // move cntr
    cout << "Vector3-move-cntr is called" << endl;
    v.length = 0;
    v.items = nullptr;
}

Vector3 &Vector3::operator=(Vector3 &&v) noexcept { // move-assignment
    cout << "Vector3-move-assign is called" << endl;
    delete [] items;
    length = v.length;
    items = v.items;

    v.length = 0;
    v.items = nullptr;
    return *this;
}

// handle self-assign
Vector3 &Vector3::operator=(const Vector3 &v) { // copy-assignment
    cout << "Vector3-copy-assign is called" << endl;
    double* old = items;

    items = new double[v.length];
    length = v.length;
    for (int i = 0; i < v.length; i++) {
        items[i] = v.items[i];
    }
    delete [] old;
    return *this;
}

Vector3::~Vector3() {
    cout << "Vector3-destructor is called" << endl;
    delete [] items; // plain delete deletes a single object, delete[] deletes the array
}

int Vector3::get_size() const {
    return length;
}

double &Vector3::operator[](int i) {
    return items[i];
}

const double &Vector3::operator[](int i) const {
    return items[i];
}
