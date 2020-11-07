#include <iostream>
#include <thread>
#include <string>
#include <cmath>
#include <functional>
#include <unordered_map>
#include <mutex>
#include <shared_mutex>
#include <condition_variable>

using namespace std;

struct School {
    string name;
    int capacity = 0;

    // instead of passing name by ref (const &name), Clang-Tidy suggested passing by value and 'move'
    School(string name, int capacity) : name{move(name)}, capacity{capacity} {}

    bool operator==(const School &other) const {
        return name == other.name;
    }
    // defining operator()(args) makes an object callable like a func
    string operator()(const string& student) const { // note that calling this operator with an lvalue requires the const (will not compile w/o)
        if (student.starts_with("W")) { // this school has curious criteria about enrolling students
            return student + " is enrolled";
        } else {
            return student + " is not enrolled";
        }
    }
};

bool operator<(const School &first, const School &second) {
    return first.capacity < second.capacity;
}

ostream &operator<<(ostream &os, const School &b) {
    return os << "{\"" << b.name << "\", " << b.capacity << "}";
}

void pow_func(const unordered_map<int, int> &hash_map, unordered_map<int, int>* result) {
    for (auto &entry : hash_map) {
        (*result)[entry.first] = pow(entry.first, entry.second);
    }
}

void threads_basic() {
    cout << "threads_basic" << endl;
    School school {"Brooklyn High", 350};
    cout << school("William") << endl;
    cout << school("Thomas") << endl; // calling function object (defines operator()(args))

    unordered_map<int, int> num_map {{2, 5}, {3, 4}, {7, 3}};
    // task passed to thread has to be a function or a function object
    thread th1 {school, "Wendy"}; // pass args to the func obj (can pass any num of args)
    unordered_map<int, int> result;
    //B.S: The ref() is a type function from <functional> that unfortunately is needed to tell the variadic template to
    // treat num_map as a reference, rather than as an object. [another clunky hack that this language is full of]
    thread th2 {pow_func, ref(num_map), &result}; // pass args to func:
    // this a common way to return result via refs (B.S consider it as not "particularly elegant" which is quite generous since it is garbage)

    th1.join();
    th2.join();
    // make sure to do that after the joins
    for (auto entry : result) {
        cout << entry.first << ": " << entry.second << endl;
    }
    cout << endl;
}

mutex m1, m2;
string shared_data;
void scoped_locks() {
    // B.S: This scoped_lock will proceed only after acquiring all its mutexes arguments and will never block
    // (“go to sleep”) while holding a mutex.
    scoped_lock sl {m1, m2}; // scoped lock can take any number of mutex's: acquiring a lock is done in the cntr automatically by calling mutex.lock
    shared_data = "shared"; // now access shared data
    // lock is released automatically when this scope ends (internally the dstr calls mutex.release
}

shared_mutex sm; // note that the mutex has to be of this type
void read_write_locks() {
    shared_lock slock {sm};
    // all readers can read the resource
    unique_lock ulock {sm};
    // only one writer can update the resource at the same time
    this_thread::sleep_for(chrono::milliseconds{2});
}

void producer() {

}

void consumer() {

}

// note that argc will always be 1 or more; name of the executable is at index 0, and first custom arg is at index 1
int main(int argc, char* argv[]) {
    cout << "arg0: " << argv[0] <<  endl;
    cout << "arg1: " << argv[1] << endl;
    threads_basic();
    scoped_locks();
    read_write_locks();

    return 0;
}
