#include <iostream>
#include <vector>
#include <variant>

using namespace std;

struct Id {
    string ssn;
    int age;
    variant<bool, int, double*>  id_variant; // can hold any type (c++17) - we can use tagged 'union' in c++14 and below
};

void print_val(Id& id) {
    cout << "Id ssn is: " << id.ssn << endl;
    cout << "Id age is: " << id.age << endl;
}

/**
 * To be constexpr, a function must be rather simple and cannot have side effects and can only use information passed to
 * it as arguments. In particular, it cannot modify non-local variables, but it can have loops and use its own local variables.
 */
constexpr double fct1(int counter, double val) {
    int c = 0;
    double res = 1;
    while (c < counter) {
        res *= val;
        c++;
    }
    return res;
}

// find val occurrences in arr assuming it ends with '0'
int count_c (const char* arr_ptr, char val) {
    if (arr_ptr == nullptr) {
        return 0;
    }
    int count = 0;
    for (; *arr_ptr != 0; arr_ptr++) {
        if (*arr_ptr == val) {
            count++;
        }
    }
    return count;
}

void assign1() {
    Id id1;
    id1.ssn = "ssn1";
    id1.age = 3;
    id1.id_variant = 4;

    if (holds_alternative<int>(id1.id_variant)) {
        cout << "do something based on: " << get<int>(id1.id_variant) << endl;
    }

    Id id2;
    id2.ssn = "ssn2";
    id2.age = 4;
    id2.id_variant = true;
    if (holds_alternative<bool>(id2.id_variant)) {
        // this get<bool>(variant) return 0 or 1
        cout << "do something based on: " << get<bool>(id2.id_variant) << endl;
    }

    id1 = id2;
    cout << id1.ssn << "-" << id2.ssn << endl;
    cout << id2.age << "-" << id2.age << endl;

    cout << "change id2\n";
    id2.ssn = "SSN2";
    id2.age = 100;
    // changes for both
    cout << id1.ssn << "-" << id2.ssn << endl;
    cout << id1.age << "-" << id2.age << endl;
}

// ptr assignment
void assign2() {
    int x = 3;
    int y = 7;

    int* xp = &x;
    int* yp = &y;

    xp = yp;
    cout << *xp << "-" << *yp << endl;

    *xp = 633;
    cout << *xp << "-" << *yp << endl;

    *yp = 611;
    cout << *xp << "-" << *yp << endl;
}

// ref assignment
void assign3() {
    int x = 3;
    int y = 7;

    int& xr = x;
    int& yr = y;

    xr = yr;
    cout << xr << "-" << yr << endl;

    xr = 633;
    cout << xr << "-" << yr << endl;

    yr = 611;
    cout << xr << "-" << yr << endl;
}

void init() {
    int x = 3;
    int &r { x }; // init: bind r to x; no copying happens
    cout << r << endl;

    r = 7;
    cout << r << endl;

    // int& r2 = 7; // error: assignment of ref b4 init
}

int main() {
    cout << sizeof(char) << endl;
    cout << 100'000'000 << endl;

    Id id;
    id.ssn = "ssn1";
    id.age = 43;

    print_val(id);

    vector<int> v {1, 2, 3, 4, 5};
    const int constInt = 3;
    constexpr int  constExprInt = 3;

    const double val1 = fct1(4, 3.2);
    constexpr double val2= fct1(5, 5.2);
    constexpr double val3= fct1(9, 7.2);

    cout << val1 << endl;
    cout << val2 << endl;
    cout << val3 << endl;

    int arr1[10]; // not initialized to 0 like in java
    arr1[3] = 71;
    int* p_int = &arr1[3]; // ptr to int
    int int_val = *p_int; // value pointed to by p_int
    cout << "int_val = " << int_val << endl;

    int arr2[] {7, 3, 1, 0, 10};
    // copy arr value to v then print it
    for (auto v : arr2) {
        cout << v << ", ";
    }
    cout << endl;

    // no copying here using references: & (using pointer: * does not work - go figure c++)
    for (int& v : arr2) {
        v++;
        cout << v << ", ";
    }
    cout << endl;
    /**
     * In a declaration, the unary suffix & means “reference to.” A reference is similar to a pointer, except that you
     * don’t need to use a prefix * to access the value referred to by the reference. Also, a reference cannot be made
     * to refer to a different object after its initialization.
     * References are particularly useful for specifying function arguments.
     */

    int* int_val2 = nullptr;

    char chars[] = {'a', 'b', 'x', 'd', 'x', '0'};
    cout << count_c(chars, 'x') << endl;

    assign1();
    cout << endl;
    assign2();
    cout << endl;
    assign3();
    cout << endl;
    init();
    cout << endl;

    return 0;
}
