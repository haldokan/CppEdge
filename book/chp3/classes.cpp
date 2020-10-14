#include <iostream>
#include "headers/Complex.h"

using namespace std;

void const_call_rules() {
    cout << "const_call_rules" << endl;
    Complex c1; // initialized since it has a default cntr
    cout << c1.real() << "-" << c1.imag() << endl;

    const Complex c2 {2.3 , 7.9}; // const obj
    double rl2 = c2.real(); // const object calling const func - OK
//    double img2 = c2.imag(); -- compiler rejects this: const object calling non-const func

    Complex c3 {1.1, 2.9}; // non-const obj
    double rl3 = c3.real(); // non-const obj calling const func - OK
    double img3 = c3.imag(); // non-const obj calling non-const func - OK
}

void add_complex_nums() {
    cout << "add_complex_nums" << endl;
    Complex c1{1, 2};
    Complex c2 { 7, 1};

    c1 += c2;
    cout << c1.real() << "-" << c1.imag() << endl;
    cout << c2.real() << "-" << c2.imag() << endl;
}

int main() {
    const_call_rules();
    add_complex_nums();

    return 0;
}