#include <iostream>
#include <iostream>
#include <complex>
#include <vector>
#include "headers/TVector.h"

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
    function_object_gt<int> gt = {2}; // predicate
    cout << count_using_func_obj(v, gt) << endl;
}

void lambda_expressions() {

}

// value template args are permitted and it allows creating containers statically w/o accessing the heap
template <typename T, int N>
struct template_struct {
    using value_type = T; // not sure what this beauty does here
    constexpr int size() { return N; }
    // T[N]; //and this gives a compilation error: Decomposition declaration not permitted in this context
};

int main() {
    work_with_custom_typed_vector();
    deduce_template_args();
    func_template1_caller();
    count_using_func_obj_caller();

    return 0;
}
