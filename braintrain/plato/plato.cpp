#include <iostream>
#include <string>
#include <utility>
#include <vector>

using namespace std;

struct Entry {
    string name;
    int number;
};

// implement the operator()<<: sort of similar to Java's toString() but less elegant - I wonder if there is IDE support for that
ostream& operator<<(ostream& os, const Entry& e) {
    return os << "{\"" << e.name << "\", " << e.number << "}";
}

// the parameter passed by ref &v1 simply means: don't 'copy' the entry referred to the param
void copy_vs_move_to_vector(Entry &v1, Entry &&v2) {
    vector<Entry> v;
    v.push_back(v1); // copy v1
    v.push_back(v2); // move v2

    // pushing to a vector by ref copies the value pointed to by the reference: v[0] update will not be reflected in caller - I think we have to use shared_ptr for that
    v[0].name = "[" + v[0].name + "]";
    cout << v[0] << endl;
    cout << v[1] << endl;
}

void copy_vs_move_to_vector_caller() {
    cout << "copy_vs_move_to_vector_caller" << endl;
    Entry e1 = {"Jane", 42};
    copy_vs_move_to_vector(e1, {"John", 45}); // note that we cannot define e2 and pass it (not Rvalue)

    cout << e1 << endl;
}

void iterate_vector() {
    cout << "iterate_vector" << endl;
     vector<Entry> v = {{"Jane", 42}, {"John", 45}};
    for (const auto &val : v) {
        cout << val << endl;
    }
}

void create_memory_for_new_obj(const Entry e) { // e has to be passed by value for the memory alloc to work (unlike B.S. example where it is passed by ref)
    cout << "create_memory_for_new_obj" << endl;
    cout << e << endl;
    Entry *ep; // ptr to e
    new (ep) Entry {e}; // init pointer to e: does not work - exit code 10 (some sort of segmentation error)
    ep++; // scary pointer math

    cout << "name: [" << ep->name << "]" << endl;
    cout << "number: [" << ep->number << "]" << endl;
}

int main() {
    iterate_vector();
    copy_vs_move_to_vector_caller();
    Entry e = {"Tim", 24};
    create_memory_for_new_obj(e);

    return 0;
}
