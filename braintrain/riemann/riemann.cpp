#include <iostream>
#include "headers/TVector.h"

using namespace std;

TVector<string> pass_typed_vector(TVector<string>& v) {
    TVector<string> v2(2);
    v2[0] = v[0];
    v2[1] = v[1];
    return v2; // returned by value which trigger a move (our TVector implements move cntr & assign)
}

void create_typed_vector() {
    cout << "create_typed_vector" << endl;
    TVector<string> v = {"this", "is", "templated", "custom", "vector"};
    // out vector impl the begin and end ops
    for (auto& val : v) {
        cout << val << ", ";
    }
    cout << endl;
    TVector<string> v2 = pass_typed_vector(v);
    cout << "v2[0] = " << v2[0] << " - v2[1] = " << v2[1] << endl;
}

int main() {
    create_typed_vector();

    return 0;
}
