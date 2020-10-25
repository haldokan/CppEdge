#include <iostream>
#include <complex>
#include <memory>
#include <vector>
#include "headers/TVector.h"
#include "headers/Vehicle.h"
#include "headers/Truck.h"
#include "headers/Sedan.h"

using namespace std;

TVector<string> pass_typed_vector(TVector<string>& v) {
    TVector<string> v2(2);
    v2[0] = v[0];
    v2[1] = v[1];
    return v2; // returned by value which trigger a move (our TVector implements move cntr & assign)
}

void work_with_custom_typed_vector() {
    cout << "work_with_custom_typed_vector" << endl;
    TVector<string> v = {"this", "is", "templated", "custom", "vector"};
    // our vector impl the begin and end ops
    cout << "print vector:" << endl;
    for (auto& val : v) {
        cout << val << ", ";
    }
    cout << endl;
    TVector<string> v2 = pass_typed_vector(v);
    cout << "v2[0] = " << v2[0] << " - v2[1] = " << v2[1] << endl;
}

void deduce_template_args() {
    cout << "deduce_template_args" << endl;
    pair<string, int> p1 = {"US", 1}; // explicitly specify pair arg types
    cout << "first = " << p1.first << ", second = " << p1.second << endl;

    pair p2 = make_pair("CA", 2); // using make_
    cout << "first = " << p2.first << ", second = " << p2.second << endl;

    pair p3 = {"UK", 3}; // since c++17 template args deduction can be done based on cntr args
    cout << "first = " << p3.first << ", second = " << p3.second << endl;

    // we can do that with our custom vector
    auto v = new TVector {7, 2, 3}; // TVector of type int
//    cout << "v[0] = " <<  v[0] << endl; for some stupid reason v[0] does not seem a value we can print
}

//A function template can be a member function, but not a virtual member
template <typename T, typename V>
V func_template1(T& seq, V v) { //2nd param specifies initial val
    cout << "func_template1" << endl;
    for (auto elem : seq) {
        v += elem;
    }
    return v;
}

void func_template1_caller() {
    TVector<int> v = {1, 2, 3};
    cout << func_template1(v, 3) << endl; // note how the func template types are deduced from the func args

    // we can use a different type
    vector<complex<double>> v2 = { complex{0.1 , 0.3}, complex{0.7, 1.2}};
    cout << func_template1(v2, complex{0.0, 0.0}) << endl;
    // use auto (the template args are deduced form the contr args0
    auto v3 = { complex{0.1 , 0.4}, complex{0.8, 1.2}};
    cout << func_template1(v3, complex{0.0, 0.0}) << endl;
}

// for a simple function object like this, inlining is simple, so a call of function objects is far more efficient than
// an indirect function call. They are called policy objects as well
template <typename T>
class function_object_gt {
private:
    const T t;
public:
    function_object_gt(T arg): t {arg} {}
    bool operator() (const T val) const { return val > t;} // func call operator()
};

template <typename sequence, typename predicate>
int count_using_func_obj(sequence &s, predicate p) {
    int count = 0;
    for (const auto &elem : s) {
        if (p(elem)) {
            count++;
        }
    }
    return count;
}

void count_using_func_obj_caller() {
    cout << "count_using_func_obj_caller" << endl;
    TVector<int> v = {1, 2, 3, 4};
//    function_object_gt<int> gt (2); // predicate - note that both type of initialization (2) or {2} work (the class init mess)
    function_object_gt<int> gt {2}; // predicate
    cout << count_using_func_obj(v, gt) << endl;
}

template <typename sequence, typename lambda>
int count_using_lambda(sequence &s, lambda lmda) {
    int count = 0;
    for (const auto &elem : s) {
        if (lmda(elem)) {
            count++;
        }
    }
    return count;
}

void lambda_expressions() {
    cout << "lambda_expressions" << endl;
    TVector<int> v = {1, 2, 3, 4};
    int x = 2;
    // the rather ugly syntax in the 2nd arg is the definition of the gt lambda
    cout << "lambda-ref: " << count_using_lambda(v, [&x](int elem) {return elem > x;})  << endl;// [&x] pass x by reference
//    cout << "lambda-copy: " << count_using_lambda(v, [=x](int elem) {return elem > x;})  << endl;// [=x] pass a copy of x => compile error

    cout << "lambda-all-ref: " << count_using_lambda(v, [&](int elem) {return elem > x;})  << endl;// [&x] pass all by reference
    cout << "lambda-all-copy: " << count_using_lambda(v, [=](int elem) {return elem > x;})  << endl;// [&x] pass all by copy
}

template <typename sequence, typename lambda>
void generic_lambda_callee(sequence &s, lambda lmda) {
    for (auto &elem : s) {
        cout << "generic_lambda_callee: " << lmda(s) << endl;
    }
}

template <typename S>
void generic_lambda(vector<S>& v) {// this garbage must be a vector of pointers but how to pass pointers to generics?
    // not that the compiler will catch calls on elem that are not valid.
    generic_lambda_callee(v, [](auto &elem) { return elem->imag();});
}

// value template args are permitted and it allows creating containers statically w/o accessing the heap
template <typename T, int N>
struct template_struct {
    using value_type = T; // alias type T to value_type (used in all std::collections)
    constexpr int size() { return N; }
    // T[N]; //and this gives a compilation error: Decomposition declaration not permitted in this context
};

template<typename firstT, typename secondT>
struct my_pair {
    firstT first;
    secondT second;
};

template<typename T>
my_pair<string, T> my_string_pair(string val, T second) {
    cout << "my_string_pair" << endl;
    // here's the aliasing
    using string_pair = my_pair<string, T>; // sort of similar to defining a func in a closure in javascript or python
    return string_pair {val, second};
}

void vehicle_specs(vector<shared_ptr<Vehicle>> &vehicles) {
    for (shared_ptr<Vehicle> vehicle : vehicles) {
        cout << vehicle->top_speed() << " - " << vehicle->capacity() << endl;
    }
}

void vehicle_specs_caller() {
    cout << "vehicle_specs_caller" << endl;
    shared_ptr<Vehicle> truck = make_unique<Truck>(100, 6); // note that I can make unique_ptr and assign to shared_ptr (not sure of rationale)
    auto sedan = make_shared<Sedan>(120, 4); // using auto here just to show that the compiler can figure out the variable type

    vector<shared_ptr<Vehicle>> vehicles;
    // this does not work if we use unique_ptr bcz vector.push_back(unique_ptr) will try to make a copy of the unique pointer (not referenced value)
    // which is disallowed bcz, well, they are unique!
    vehicles.push_back(truck);
    vehicles.push_back(sedan);

    vehicle_specs(vehicles);
}

void using_lambda_as_initializer(int selector) {
    cout << "using_lambda_as_initializer" << endl;
    vector<int> default_vec{1, 2, 3};
    int default_size = 2;

    vector<int> v = [&] { // everything inside lambda is captured by ref including selector, default_vec and default_size
        switch (selector) {
            case 1:
                return default_vec;
            case 2:
                return vector<int>(default_size);
        }
    }();
    cout << v[0] << endl;
}

int main() {
    work_with_custom_typed_vector();
    deduce_template_args();
    func_template1_caller();
    count_using_func_obj_caller();
    lambda_expressions();
    my_pair<string, int> p = my_string_pair("first", 3);
    cout << p.first << " - " << p.second << endl;
    vehicle_specs_caller();
    using_lambda_as_initializer(1);
//    generic_lambda(v);

    return 0;
}
