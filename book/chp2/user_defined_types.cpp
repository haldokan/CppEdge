#include <iostream>
#include <vector>
#include <map>
#include<complex>
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
void init_vector(Vector01& v, int size) {
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

void pass_by_val_or_ref(vector<int> v1, vector<int> &v2) {
    v1[0] = 33;
    v2[0] = 77;
}

void default_arg(int v1, int v2 = 10) {
    cout << "default arg: " << v1 << "-" << v2 << endl;
}

void structured_binding_struct() {
    // can do structured binding on struct values
    Vector01 vec3;
    init_vector(vec3, 3);
    vec3.items[0] = 7.2;

    auto [val1, val2] = vec3; // all vals *must* be listed... auto[val1] alone will not work (unlike javascript destructuring)
    cout << "structured-binding-struct:" << val1 << "-" << val2[0] << endl;
}

void structured_binding_maps() {
    map<int, string> map1;
    map1[0] = "zero";
    map1[1] = "one";
    map1[2] = "two";

    cout << "structured-binding-map:" << endl;
    for (auto[key, val] : map1) {
        cout << key << " - " << val << endl;
    }
}

void structured_binding_classes() {
    complex<double> cnum = { 2.3, 1.4 };
    cout << cnum.real() << "-" << cnum.imag() << endl;
    /*A complex has two data members, but its interface consists of access functions, such as real() and imag().
     * Mapping a complex<double> to two local variables, such as re and im is feasible and efficient, but the technique
     * for doing so is beyond the scope of this book.*/
//    auto [re, im] = cnum + 7.1;
};

void init_container_with_init_list() {
    cout << "init_container_with_init_list" << endl;
    Vector v = { 1.2, 3.2, 7.1 }; // the compiler creates an object of type initializer_list and feed the Vector cntr
    cout << v[1] << endl;
}

void handling_exceptions() {
    cout << "handling_exceptions" << endl;
    try {
        Vector vec2 = Vector(4);
        vec2[0] = 1;
        vec2[2] = 5;
        vec2[3] = 10;
        vec2[166] = 3;
        // not how the exception is passed to the catch stmt by ref to avoid copying
    } catch (out_of_range& e) {
        cerr << "caught it: " << e.what() << endl;
    } catch (bad_alloc& e) {
        cerr << "could not allocate memory" << endl;
        terminate();
    }
}

void work_with_struts() {
    cout << "work_with_struts" << endl;
    Vector01 v;
    init_vector(v, 3);
    v.items[0] = 2.1;
    v.items[1] = 3.1;
    v.items[2] = 7.1;

    cout << v.size << " - " << v.items[0] << endl;
    cout << "sum = " << sum(v) << endl;
}

void work_with_classes() {
    cout << "work_with_classes" << endl;
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

    // This is a turd-pie: we pass vec to sum3 by value
    // But even tho we pass by value the double array in Vector is allocated on the heap and is common for the
    // argument we pass to sum3 and and param value. When sum3 is removed from the stack (finishes) the vec destructor is
    // called and the double array is freed. after that when this func is removed from the stack the vec destructor is called
    // again to free the (already freed) double array on the heap. This gives this out of context exception that reminds me
    // of my C days at school:
    // cpp2(7982,0x11b6e6dc0) malloc: *** error for object 0x7fe24fc05850: pointer being freed was not allocated

//    cout << "pbn::vec.size = " << vec.get_size() <<  " - sum = " << sum3(vec) << endl;
//    cout << "pbn::v[0] = " << vec[0] << endl;
}

int main() {
    vector<int> v1 = {1, 2, 3};
    vector<int> v2 = {7, 5, 9};
    pass_by_val_or_ref(v1, v2);
    cout << v1[0] << "-" << v2[0] << endl;

    work_with_classes();
    work_with_struts();
    default_arg(2, 7);
    default_arg(2);
    handling_exceptions();
    structured_binding_struct();
    structured_binding_maps();
    structured_binding_classes();
    init_container_with_init_list();

    return 0;
}
