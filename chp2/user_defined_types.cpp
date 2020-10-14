#include <iostream>
#include <vector>
#include "Vector.h"

//import Vector2; -- does not work

using namespace std;
using namespace N;

struct Vector01 {
    int size;
    double* items;
};

double sum(const Vector01& v) {
    double s = 0;
    for (int i = 0; i != v.size; i++) {
        s += v.items[i];
    }
    return s;
}

// note that adding const to the Vector01 param will prevent changing its size or items
void initVector(Vector01& v, int size) {
    v.size = size;
    v.items = new double[size];
}

double sum2(Vector& v) {
    double s = 0;
    for (int i = 0; i != v.get_size(); i++) {
        s += v[i];
    }
    v[0] = 100; // changes the value in the vector declared in the calling code
    return s;
}

double sum3(Vector v) {
    double s = 0;
    for (int i = 0; i != v.get_size(); i++) {
        s += v[i];
    }
    v[0] = 200; // changes the value in the vector declared in the calling code
    return s;
}

void pbv_pbr(vector<int> v1, vector<int> &v2) {
    v1[0] = 33;
    v2[0] = 77;
}

void default_arg(int v1, int v2 = 10) {
    cout << v1 << "-" << v2 << endl;
}

int main() {
    Vector01 v;
    initVector(v, 3);
    v.items[0] = 2.1;
    v.items[1] = 3.1;
    v.items[2] = 7.1;

    cout << v.size << " - " << v.items[0] << endl;
    cout << "sum = " << sum(v) << endl;

    Vector vec = Vector(3);
    vec[0] = 1.5;
    vec[1] = 2.1;
    vec[2] = 3.2;
    // cout cannot print enum val! What gives?
//    cout << "vec.category = " << vec.get_category() << endl;
    Category cat = vec.get_category();
    Category next_cat = ++cat;
//    cout << next_cat;
//
    // pass by ref
    cout << "pbr::vec.size = " << vec.get_size() <<  " - sum = " << sum2(vec) << endl;
    cout << "pbr::v[0] = " << vec[0] << endl;

    // I think this is passing the pointer - what is the diff? Go figure!
    cout << "pbn::vec.size = " << vec.get_size() <<  " - sum = " << sum3(vec) << endl;
    cout << "pbn::v[0] = " << vec[0] << endl;

    try {
        Vector vec2 = Vector(3);
        vec2[0] = 1;
        vec2[2] = 5;
        vec2[3] = 10;
        vec2[166] = 3;
        // not how the exception is passed to the catch stmt by ref to avoid copying
    } catch (out_of_range& e) {
        cerr << "caught it: " << e.what() << endl;
    } catch (bad_alloc) {
        cerr << "could not allocate memory" << endl;
        terminate();
    }

    vector<int> v1 = {1, 2, 3};
    vector<int> v2 = {7, 5, 9};

    pbv_pbr(v1, v2);
    cout << v1[0] << "-" << v2[0] << endl;

    default_arg(2, 7);
    default_arg(2);

    return 0;
}
