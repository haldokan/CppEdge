#include <iostream>
#include "headers/Complex3.h"

using namespace std;

Complex3::Complex3() {} // rl and img use the default member initializers (check header)

Complex3::Complex3(double rl, double img) : rl(rl), img(img) {}

// note where const is placed to actually be counted (seems it means different things at different locations - c++ till this point is living to its reputation of obfuscation)
// the const specifier on the function returning the real part indicates that this function does not modify the object for which they are called
double Complex3::real() const { return rl; }

double Complex3::imag() const { return img; } // not marked const for testing from driver

Complex3& Complex3::operator+=(Complex3 c) {
    rl += c.real();
    img += c.imag();
    return *this;
}
// B.S. says that this could be defined elsewhere but I got compile errors about having 3 args for the binary operator
// so defined here it does not work with c1 + c2. I am missing some more twists in this sad api.
Complex3 operator+ (Complex3 c1, Complex3 c2) { // passed by value so we can change in the func w/o affecting caller
   return  c1 += c2;
}
