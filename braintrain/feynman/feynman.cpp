#include <iostream>
#include <string>
#include <utility>
#include <vector>
#include <set>
#include <fstream>

using namespace std;

struct City {
    string name;
    int population = 0;
    // instead of passing name by ref (const &name), Clang-Tidy suggested passing by value and 'move'
    City(string name, int population) : name{move(name)}, population{population} {}

    bool operator==(const City &other) const {
        return name == other.name;
    }
};

// B.S defines this operator outside the struct conforming to his advise of defining operations not essential to a data
// structure outside the data-structure so as no to increase its size and to avoid forcing relinking code that uses the data
// structure because of these changes.
bool operator<(const City &first, const City &second) {
    return first.population < second.population;
}

ostream& operator<<(ostream& os, const City& e) {
    return os << "{\"" << e.name << "\", " << e.population << "}";
}


unique_ptr<City> get_city1() {
    cout << "get_city1" << endl;
    City *new_york = new City{"New York", 4};
    // unique_ptr has move semantics so the pointer to new_york is moved to the caller
    return unique_ptr<City>(new_york); // note that unique_ptr takes a pointer as an arg to its cntr
}

unique_ptr<City> get_city2() {
    cout << "get_city2" << endl;
    City boston = City{"Boston", 4};
    // unique_ptr has move semantics so the pointer to new_york is moved to the caller
    return make_unique<City>(boston); // create a unique pointer to City and 'move' it to the caller
}


void shared_ptr_example_calle1(const shared_ptr<ifstream> &file_handle) {
    cout << "shared_ptr_example_calle1" << endl;
    istream_iterator<string> itr { *file_handle }; // stream itr needs the fstream pointer from the unique ptr
    istream_iterator<string> sentinel = {};

    vector<string> vec {itr, sentinel};
    for (string &val : vec) {
        cout << val << " - ";
    }
    cout << endl;
}

void shared_ptr_example_calle2(const shared_ptr<ifstream> &file_handle) {
    cout << "shared_ptr_example_calle2" << endl;
    file_handle->close(); // we close the handle then open it so we go back to the start of the file - there must be a seek api
    file_handle->open("/Users/haldokanji/misc/cpp-test-file.txt");
    istream_iterator<string> itr { *file_handle }; // stream itr needs the fstream pointer from the unique ptr
    istream_iterator<string> sentinel = {};

    // with set we eliminate dups
    set<string> set1 {itr, sentinel}; // using set here instead of vector: note the inconsistency in the for loop b/w vector and set (we cannot use string type because it is an iterator type)
    for (auto &val_itr : set1) {
        cout << val_itr << " - ";
    }
    cout << endl;
}

int shared_ptr_example() {
    cout << "shared_ptr_example" << endl;
    string source {"/Users/haldokanji/misc/cpp-test-file.txt"}; // does not understand '~' to refer to home directory
    // pass a pointer ('new' returns a ptr to heap memory address) to the shared_ptr cntr
    // shared_ptr<ifstream> in_stream {new ifstream {source}};

    // but much preferably use make_shared: (B.S: Using make_shared() is not just more convenient than separately making
    // an object using new and then passing it to a shared_ptr, it is also notably more efficient because it does not
    // need a separate allocation for the use count that is essential in the implementation of a shared_ptr.)
    shared_ptr<ifstream> in_stream = make_shared<ifstream>(ifstream {source});
    if (!*in_stream) {
        cout << "could not open file: " << source << endl;
        return EXIT_FAILURE;
    }
    shared_ptr_example_calle1(in_stream);
    shared_ptr_example_calle2(in_stream);
    return 0;
}

//std::move() doesn’t move anything. Instead, it casts its argument to an rvalue reference, thereby saying that its argument
// will not be used again and therefore may be moved. Like other casts, it’s error-prone and best avoided. It exists to
// serve a few essential cases (like swapping 2 values)
void move_example() {
    cout << "move_example" << endl;
    unique_ptr<string> p = make_unique<string>("sun");
    auto p1 = move(p); // p now point to nullptr
    cout << "p = " << p << endl; // this prints 0x0 (may be the hex value of nullptr)
//    this will trigger a segmentation fault for trying to access freed memory.
//    cout << *p
//    it can be assigned to again (could practise to assign move-from object to a safe value in case it is used accidentally later
    p = make_unique<string>("moon");
    cout << "p = " << *p << endl;
}

void move_to_vector() {
    cout << "move_to_vector" << endl;
    auto s1 = "earth";
    auto s2 = "mars";

    vector<string> v;
    v.push_back(s1); // s1 is *copied* because the param type for push_back is const &string.
    v.push_back(move(s2)); // s2 is moved and now s2 points to nullptr (can be re-assigned but will cause seg. fault if accessed; s[0])
    // B.S. take on using move to vectors is this:
    //This sometimes (only sometimes) makes the push_back() of s2 cheaper. The problem is that a moved-from object is
    //left behind. If we use s2 again, we have a problem

    // the interesting thing here is the Clang-Tidy suggest using emplace_back instead of push_back in this case but only when we use auto
    // emplace_back appends a new element to the end of the container... it constructs the element in-place at the location
    // provided by the container: check the cppreference for a nice example: https://en.cppreference.com/w/cpp/container/list/emplace_back
    auto s3 = "pluto"; // auto here is equivalent to const char* which means we cannot do: s3[0] = '8' for example
    string s4 = "venus"; // which string it is possible to do s3[0] = '8' for example (c++ is a hodge-bodge of inconsistencies)
    v.emplace_back(s3);
    v.push_back(s4);// possible to emplace it as well but Clang-Tidy is happy with it as is

    for (auto &val : v) {
        cout << val << " - ";
    }
    cout << endl;
}

template <typename T, typename ...Args> // the ... Args is a type for args - talk about beauty!
unique_ptr<T> make_unique2(Args&&... args) { // the && just means it is an rvalue
    return unique_ptr<T>(new T(forward<Args>(args)...)); // note how the ... moved to the right (another ugly syntactic hack)
}

//B.S. The standard-library forward() differs from the simpler std::move() by correctly handling subtleties to do with lvalue
// and rvalue [love that 'correctly'!]. Use std::forward() exclusively for forwarding and don’t forward() something twice;
// once you have forwarded an object, it’s not yours to use anymore.
void forward_example() {
    City city  {"Jersey", 1};
    unique_ptr<City> city_ptr = make_unique2<City>(city);
    cout << *city_ptr << endl;
}

int main() {
    cout << *get_city1() << endl;
    cout << *get_city2() << endl;
    shared_ptr_example();
    move_example();
    move_to_vector();
    forward_example();

    return 0;
}
