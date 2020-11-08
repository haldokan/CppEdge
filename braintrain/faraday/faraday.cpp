#include <iostream>
#include <thread>
#include <string>
#include <cmath>
#include <functional>
#include <unordered_map>
#include <mutex>
#include <shared_mutex>
#include <queue>
#include <future>
#include <numeric>

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
    cout << "scoped_locks" << endl;
    // B.S: This scoped_lock will proceed only after acquiring all its mutexes arguments and will never block
    // (“go to sleep”) while holding a mutex.
    scoped_lock sl {m1, m2}; // scoped lock can take any number of mutex's: acquiring a lock is done in the cntr automatically by calling mutex.lock
    shared_data = "shared"; // now access shared data
    // lock is released automatically when this scope ends (internally the dstr calls mutex.release
}

shared_mutex sm; // note that the mutex has to be of this type
void read_write_locks() {
    cout << "read_write_locks" << endl;
    shared_lock slock {sm};
    slock.unlock();
    // all readers can read the resource
    unique_lock ulock {sm};
    // only one writer can update the resource at the same time
    this_thread::sleep_for(chrono::milliseconds{2});
}

void producer(queue<School> &qu, mutex &mtx, condition_variable &condition_var) {
    for (int i = 0; i != 5; i++) {
        scoped_lock sl{mtx};
        cout << "produce" << endl;
        qu.push(School{"Jersey High: " + to_string(i), 300 + i * 10});
        condition_var.notify_one();
    }
}

void consumer(queue<School> &qu, mutex &mtx, condition_variable &condition_var) {
    for (int i = 0; i != 5; i++) {
        unique_lock ul{mtx};
        // 2nd arg is a predicate the must be true for the awoken to access the protected resource
        condition_var.wait(ul, [&](){return !qu.empty();}); // wait expects unique_lock
        School school = qu.front();
        qu.pop(); // clunky... I would have expected pop to get me the elem like every other language (but not in c++)
        ul.unlock();
        cout << "consume: " << school << endl;
    }
}

void producer_consumer(bool run) {
    if (!run) {
        return; // consumer will block on the queue preventing the rest of the funcs in this file from running
    }
    cout << "producer_consumer" << endl;
    queue<School> qu;
    mutex mtx;
    condition_variable condition_var;

    thread pth {producer, ref(qu), ref(mtx), ref(condition_var)};
    thread cth {consumer, ref(qu), ref(mtx), ref(condition_var)};

    pth.join();
    cth.join();
}

double accum(const double* begin, const double* end) {
    return accumulate(begin, end, 0.0); // accumulate is in std::numerics
}

// more clunky apis ahead!
//B.S: Use a packaged_task and a future to express a request to an external service and wait for its response
void packaged_task_example() {
    cout << "packaged_task_example" << endl;
    using Task_Type = double(const double*, const double*); //alias the accum func types
    packaged_task<Task_Type> ptask1 {accum};
    packaged_task<Task_Type> ptask2 {accum};

    future<double> f1 {ptask1.get_future()}; // construct the future of the task
    future<double> f2 {ptask2.get_future()};

    vector<double> vec = {1.2, 3.5, 5.2, 3.1, 7.9, 1.3, 1.4, 3.4, 6.1};
    double *first = &vec[0];

    // B.S:The move() operations are needed because a packaged_task cannot be copied. The reason that a packaged_task cannot
    // be copied is that it is a resource handle: it owns its promise and is (indirectly) responsible for whatever resources its task may own.
    thread th1 {move(ptask1), first, first + vec.size()/2}; // you gotta love this pointer arithmetics!
    thread th2 {move(ptask2), first + vec.size()/2, first + vec.size()};

    // and finally after all the travesty we have above we can get the values from the futures
    cout << "vector sum: " << f1.get() + f2.get() << endl;

    th1.join();
    th2.join();
}

// if we can use this elegant async why on earth all the travesty of the function above?
// B.S: Use async() to launch simple tasks [why the devil is that?]
void async_example() {
    cout << "async_example" << endl;
    vector<double> vec = {1.2, 3.5, 5.2, 3.1, 7.9, 1.3, 1.4, 3.4, 6.1};
    double *first = &vec[0];

    future<double> f1 = async(accum, first, first + vec.size()/2);
    auto f2 = async(accum, first + vec.size()/2, first + vec.size());
    cout << "async vector sum: " << f1.get() + f2.get() << endl;
}

// note that argc will always be 1 or more; name of the executable is at index 0, and first custom arg is at index 1
int main(int argc, char* argv[]) {
    cout << "arg0: " << argv[0] <<  endl;
    cout << "arg1: " << argv[1] << endl;
    threads_basic();
    scoped_locks();
    read_write_locks();
    producer_consumer(false);
    packaged_task_example();
    async_example();

    return 0;
}
