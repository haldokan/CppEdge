#include <iostream>
#include "headers/Complex3.h"
#include "headers/Vector3.h"
#include <vector>

using namespace std;

void add_complex_nums() {
    cout << "==>>add_complex_nums" << endl;
    Complex3 c1{1, 2};
    Complex3 c2 {7, 1};

    c1 += c2; // copy-cntr
    cout << c1.real() << "-" << c1.imag() << endl;
    cout << c2.real() << "-" << c2.imag() << endl;

    Complex3 c3 {}; // default-cntr
    c3 += c1; // copy-cntr
    cout << c3.real() << "-" << c3.imag() << endl;
}

void struct_copy_assignment() {
    cout << "==>>struct_copy_assignment" << endl;
    struct Item {
        string product;
        int id;
    };

    Item item1 = {"cellphone", 723};
    Item item2 = item1; // this is copy assignment (item1 data is *copied* to item2) and here's the proof:

    item1.product = "table";
    cout << item1.product << " - " << item2.product << endl; // product changes only for item1

    item2.id = 111;
    cout << item1.id << " - " << item2.id << endl; // id changes only for item2
}

void class_copy_constructor() {
    cout << "==>>class_copy_constructor" << endl;
    Complex3 c1 = Complex3{1, 2};
    // init using the copy-cntr: called copy-initialization
    Complex3 c3 {c1};
    // copy-assignment (uses the copy cntr)
    Complex3 c4 = c1;

    Complex3 c5; // default-initialization
    // copy assignment but the copy-cntr is not called because c5 already constructed using the default cntr
    c5 = c1;
}

void container_copy_constructor() {
    cout << "container_copy_constructor" << endl;
    Vector3 v1 {1, 3, 4}; // the initializer_list contr
    Vector3 v2 {v1}; // copy-cntr
    cout << "should be equal: " << v1[1] << " - " << v2 [1] << endl;

    // because v2 was created from v1 using the copy cntr it must have its own data
    v1[1] = 7;
    cout << "should be different: " << v1[1] << " - " << v2 [1] << endl;
}

void container_copy_assignment() {
    cout << "container_copy_assignment" << endl;
    Vector3 v1 {1, 3, 4}; // the initializer_list contr
    Vector3 v3 {6, 8, 9}; // the initializer_list contr

    Vector3 v2 = v1; // this does NOT call the copy assign operator=, it rather calls the copy-cntr since v2 is initialized with v1 value
    cout << "should be equal: " << v1[1] << " - " << v2 [1] << endl;

    // because v2 was created from v1 using the copy cntr it must have its own data
    v2[1] = 7;
    cout << "should be different: " << v1[1] << " - " << v2 [1] << endl;

    v2 = v3; // this calls the copy assign operator=
}

int main() {
    add_complex_nums();
    struct_copy_assignment();
    class_copy_constructor();
    container_copy_constructor();
    container_copy_assignment();

    return 0;
}
