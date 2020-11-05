#include <iostream>
#include <thread>
#include <vector>
#include <cmath>
#include <cerrno>
#include <numeric>
#include <random>
#include <valarray>

using namespace std;

struct Book {
    string title;
    int pages = 0;

    // instead of passing name by ref (const &name), Clang-Tidy suggested passing by value and 'move'
    Book(string title, int pages) : title{move(title)}, pages{pages} {}

    bool operator==(const Book &other) const {
        return title == other.title;
    }

    [[nodiscard]] string summary() const { return title + "/" + to_string(pages); } // not much of a summary
};

bool operator<(const Book &first, const Book &second) {
    return first.pages < second.pages;
}

ostream &operator<<(ostream &os, const Book &b) {
    return os << "{\"" << b.title << "\", " << b.pages << "}";
}

// the obfuscation, confusion, and ugliness of a tired api are all present on the cpp-reference page regarding allocators:
//https://en.cppreference.com/w/cpp/memory/shared_ptr/allocate_shared
void allocator_example() {
    // please mercy-shoot me but don't make me go thru this!
    cout << "allocator_example" << endl;
}

//B.S: An elegant and efficient extension to <chrono>, supporting longer time intervals (e.g., years and months),
// calendars, and time zones, is being added to the standard for C++20.
void time_example() {
    using namespace chrono;
    auto t1 = high_resolution_clock::now(); // now is in name space high_resolution_clock
    this_thread::sleep_for(50ms + 29us);
    auto t2 = high_resolution_clock::now();

    auto duration = duration_cast<milliseconds>(t2 - t1).count();
    cout << "duration_msecs: " << duration << endl;
}

void func_adaptation_with_lambda() {
    cout << "func_adaptation_with_lambda" << endl;
    vector<Book*> books = initializer_list<Book *>{new Book("The Flood", 300), new Book("The Fire", 410)};
    // the last arg of for_each is 'function'
    for_each(books.begin(), books.end(), [](Book *b) {cout << b->summary() << endl;});
}

string adaptee(int val) {
    return to_string(val) + " books";
}

void func_adaptation_with_function() {
    cout << "func_adaptation_with_function" << endl;
    function<string(int)> f1 {adaptee};
    cout << f1(3) << endl;

    // we can also assign lamda to function
    vector<Book*> books = initializer_list<Book *>{new Book("The Chief", 200), new Book("The Tribe", 210)};
    // the last arg of for_each is 'function'
    function<void(Book*)> fn {[](Book *b) {cout << b->summary() << endl;}}; // this func can be passed as callback, etc.
    for_each(books.begin(), books.end(), fn);
}

// this is the old way to do it (*prefer lambdas*)
//Given a member function, the function adaptor mem_fn(mf) produces a function object that can be called as a nonmember
// function. For example
void func_adaptation_with_mem_fn() {
    cout << "func_adaptation_with_mem_fn" << endl;
    vector<Book*> books = initializer_list<Book *>{new Book("The River", 100), new Book("The Blaze", 510)};
    // the last arg of for_each is 'function'
    for_each(books.begin(), books.end(), mem_fn(&Book::summary));
}

void math_ops() {
    cout << "math_ops" << endl;
    cout << abs(-2.3) << endl;
    cout << ceil(2.6) << endl;
    cout << ceil(-2.6) << endl;
    cout << floor(2.6) << endl;
    cout << floor(-2.6) << endl;
    cout << sqrt(37) << endl;
    cout << exp(2) << endl; // base e
    cout << log(9) << endl; // base e
    cout << log10(100) << endl; // base 10
    cout << pow(3,2) << endl; // 3^2
    cout << max(2, 7) << endl;
}

void errno_setting() {
    cout << "errno_setting" << endl;
    errno = 0; // from lib std::cerrno
    cout << sqrt(-2) << endl; // this returns nan (no exceptions)
    // the sqrt(-2) will set the error num in errno
    cout << "errno = " << errno << endl;
    if (errno == EDOM) { // Error-Domain -- does NOT work: the errno is not set and its value remain 0
        cout << "called sqrt on negative number" << endl;
    }

    errno = 0; // reset errno
    double val = pow(numeric_limits<double>::max(),2); // max-double^2 will overflow double
    cout << val << endl; // this prints 'inf'
    if (errno == ERANGE) { // Error-range: again, does NOT work: errno is not set
        cerr << "result of pow() too large to represent as a double";
    }
}

void accumulate_example() {
    cout << "accumulate_example" << endl;
    vector<int> v = {1, 4, 2, 3};
    int val = accumulate(v.begin(), v.end(), 15); // sum the vector elems and add 15
    cout << val << endl;
    // accumulate using lambda instead of +
    int val2 = accumulate(v.begin(), v.end(), 15, [](int v1, int v2) { return v1 - v2;});
    cout << val2 << endl;
}
// B.S. suggests (for obvious reason) encapsulating this travesty in a class that implements
// int operator()() { return dist(engine); }
void random_nums_example() {
    cout << "random_nums_example" << endl;
    // we need to specify and engine that we pass to a distribution and for some awkward reason we have to use 'using'
    using default_engine = default_random_engine;
    using uniform_dist = uniform_int_distribution<>; // we can also use uniform_real_distribution
    // using normal_distribution; // bell curve dist
    // using exponential_distribution; // exp dist

    unsigned seed = numeric_limits<unsigned>::max();
    cout << "seed: " << seed << endl;
    default_engine engine {seed}; // optionally we pass current time as a seed
    uniform_dist dist {11, 37}; // random b/w 1 & 10
    auto rand_func = [&](){return dist(engine);}; // yes we need a lambda to to get a darn random number
    // now we can finally get our rand numbers
    for (int i = 0; i < 20; i++) {
        cout << rand_func() << ",";
    }
    cout << endl;
}

// valarray is used for numerical computations
void valarray_example() {
    cout << "valarray_example" << endl;
    valarray<int> a1 = {1, -2, 3};
    valarray<int> a2 = { 4, 5, 6};

    valarray<int> a3 = a1*2 + a2;
    for (auto val : a3) {
        cout << val << ",";
    }
    cout << endl;

    a1 += -1;
    valarray<int> a4 = abs(a1);
    for (auto val : a4) {
        cout << val << ",";
    }
}

int main() {
    allocator_example();
    time_example();
    func_adaptation_with_lambda();
    func_adaptation_with_mem_fn();
    func_adaptation_with_function();
    math_ops();
    errno_setting();
    accumulate_example();
    random_nums_example();
    valarray_example();

    return 0;
}
