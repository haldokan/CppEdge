#include <iostream>
#include <string>
#include <utility>
#include <vector>
#include <set>
#include <fstream>
#include <span>
#include <array>
#include <bitset>
#include <map>
#include <algorithm>
#include <variant>
#include <any>

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

ostream &operator<<(ostream &os, const City &e) {
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
    istream_iterator<string> itr{*file_handle}; // stream itr needs the fstream pointer from the unique ptr
    istream_iterator<string> sentinel = {};

    vector<string> vec{itr, sentinel};
    for (string &val : vec) {
        cout << val << " - ";
    }
    cout << endl;
}

void shared_ptr_example_calle2(const shared_ptr<ifstream> &file_handle) {
    cout << "shared_ptr_example_calle2" << endl;
    file_handle->close(); // we close the handle then open it so we go back to the start of the file - there must be a seek api
    file_handle->open("/Users/haldokanji/misc/cpp-test-file.txt");
    istream_iterator<string> itr{*file_handle}; // stream itr needs the fstream pointer from the unique ptr
    istream_iterator<string> sentinel = {};

    // with set we eliminate dups
    set<string> set1{itr,
                     sentinel}; // using set here instead of vector: note the inconsistency in the for loop b/w vector and set (we cannot use string type because it is an iterator type)
    for (auto &val_itr : set1) {
        cout << val_itr << " - ";
    }
    cout << endl;
}

int shared_ptr_example() {
    cout << "shared_ptr_example" << endl;
    string source{"/Users/haldokanji/misc/cpp-test-file.txt"}; // does not understand '~' to refer to home directory
    // pass a pointer ('new' returns a ptr to heap memory address) to the shared_ptr cntr
    // shared_ptr<ifstream> in_stream {new ifstream {source}};

    // but much preferably use make_shared: (B.S: Using make_shared() is not just more convenient than separately making
    // an object using new and then passing it to a shared_ptr, it is also notably more efficient because it does not
    // need a separate allocation for the use count that is essential in the implementation of a shared_ptr.)
    shared_ptr<ifstream> in_stream = make_shared<ifstream>(ifstream{source});
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
    v.push_back(
            move(s2)); // s2 is moved and now s2 points to nullptr (can be re-assigned but will cause seg. fault if accessed; s[0])
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

template<typename T, typename ...Args>
// the ... Args is a type for args - talk about beauty!
unique_ptr<T> make_unique2(Args &&... args) { // the && just means it is an rvalue
    return unique_ptr<T>(
            new T(forward<Args>(args)...)); // note how the ... moved to the right (another ugly syntactic hack)
}

//B.S. The standard-library forward() differs from the simpler std::move() by correctly handling subtleties to do with lvalue
// and rvalue [love that 'correctly'!]. Use std::forward() exclusively for forwarding and don’t forward() something twice;
// once you have forwarded an object, it’s not yours to use anymore.
void forward_example() {
    City city{"Jersey", 1};
    unique_ptr<City> city_ptr = make_unique2<City>(city);
    cout << *city_ptr << endl;
}

void span_example_callee(span<string> span1) { // note how we pass span by value (ref will not work)
    span1[2] = "kudos"; // changes made to the span are reflected in the arr on the caller side
    for (auto &elem : span1) {
        cout << "[" << elem << "] - ";
    }
    cout << endl;
    // ranges are miraculously are NOT checked with spans! B.S note that it range checks can be suppressed for performance (I guess they are by default)
    cout << "some random value: " << span1[span1.size() + 1] << endl;
}

// I think this is c++20
void span_example() {
    cout << "span_example" << endl;
    array<string, 10> arr; // can use string arr[10]; but this seems to be a minefield: implicitly converts its elems to pointers

    arr[0] = "thumbs";
    arr[1] = "up";
    span_example_callee(arr); // this call creates span {arr, 10}
    cout << arr[2] << endl; // this prints "kudos"

    // initializing a spand required a build-in arr: array<T, N> doesn't cut it - c++ at its finest!
    string arr1[10];
    span<string> span1{arr1, 10}; // works for array but not vectors or array... go figure
    cout << span1.size() << endl;
}

//array does not follow the “handle to elements” model of STL containers. Instead, an array directly contains its elements
// The number of elements in the initializer must be equal to or less than the number of elements specified for the array.
void array_example() {
    cout << "array_example" << endl;
// array<City, 2> cities; this does not compile bcz City has no dflt cntr
    array<string, 2> arr1; // this works bcz string has dflt cntr which assign it to empty

    // array can be constructed by providing and init list - will give a misleading error message if less than array.size
    // elems are provided and the elem does not impl dflt cntr. Nice message tho for excess elems
    array<City, 2> cities = {City{"nyc", 5}, City{"jersey", 3}};
    for (auto &city : cities) {
        cout << city << " - ";
    }
    cout << endl;
    // this will get a pointer to the first elem in array and can be passed to C-style interfaces (*arr, size)
    City *cs = cities.data();

    // returns a ptr to the first matching elem
    auto found = find(cities.begin(), cities.end(), City{"nyc", 5});
    cout << *found << endl;
    // pnt-- or ptr++ moves back and forth based on the size of the elems it points to (based on the types, not the actual vals)
    cout << "size of City: " << sizeof(*found) << endl; // prints 32: 24 for string (handle?) and 8 for int
}

void bitset_example() {
    cout << "bitset_example" << endl;
    // can be init with a string of 1s & 0s
    bitset<5> bs1{"10101"}; // chars in the string exceeding bitset size are ignored starting at from that right
    cout << bs1 << endl;

    // can be init with a binary num separated with digit separators (for clarity)
    bitset<10> bs2{0b10'1000'0001};
    cout << bs2 << endl;

//    bitset<10> bs3 = bs1&bs2; does not compile bcz the 2 bitsets have diff sizes
    bitset<10> bs3 = {0b001'01};
    bitset<10> bs4 = bs2 & bs3; // can also do |, <<, >>, complement ~, etc.
    cout << bs4 << endl;

    // we can construct bitset by assigning to int (positive or negative)
    bitset<8 * sizeof(int)> bs5 = 798;
    cout << bs5 << endl;

    string bs_str = bs5.to_string();
    cout << bs_str << endl;
    cout << bs5.to_ullong() << endl; // to unsigned long-long-int: 64
    cout << bs5.to_ulong() << endl; // to unsigned long-int: 32 or 64 depending on the platform
}

//equal_range: returns a range containing all elements equivalent to value in the range [first, last).
//The range [first, last) must be at least partially ordered with respect to value,
void pair_example() {
    cout << "pair_example" << endl;
    // vector does not have to be strictly ordered: only partitioned wrt the search value in equal_range
    vector<City> cities{{"new jersey", 2},
                        {"nevada",     5},
                        {"arizona",    5},
                        {"new york",   6}};

    for (auto &entry : cities) {
        cout << entry << " - ";
    }
    cout << endl;
    int population = 5; // search for a range where population is greater that 2;
    // heterogeneous comparison using struct that defines the < b/w City and an int
    struct compare_pop {
        bool operator()(const City &city, int pop) const { return city.population < pop; }

        bool operator()(int pop, const City &city) const { return pop < city.population; }
    };
    auto[first, second] = equal_range(cities.begin(), cities.end(), population, compare_pop{});
    for (auto p = first; p != second; ++p) {
        cout << "city(struct): " << *p << endl;
    }
    // we can instead use lambda (this time for homogenous comparison b/w City & City) and get the same results
    auto[first2, second2] = equal_range(cities.begin(), cities.end(), City{"", 5},
                                        [](const City &city1, const City &city2) {
                                            return city1.population < city2.population;
                                        });
    for (auto p = first2; p != second2; ++p) {
        cout << "city(lambda): " << *p << endl;
    }
}

void pair_assign_and_compare() {
    cout << "pair_assign_and_compare" << endl;
    pair<string, City> pair1 {"ny", {"new-york", 4}};
    pair<string, City> pair2 = pair1;
    cout << pair2.first << endl;

    pair<string, City> pair3 {"ny", {"new-york", 4}};
    cout << (pair1 == pair3) << endl;

    pair<string, City> pair4 { "az", {"arizona", 3}};
    cout << (pair1 > pair4) << endl; // prints 1: it compares pair.first and only compares pair.second if first is equal

    pair<string, City> pair5 { "az", {"arizona-or-whatever", 2}};
    cout << (pair4 > pair5) << endl; // prints 1 since first is equal and second has larger population for pair4
}

// I look at this ugly api and wonder how it ever passed muster with whoever admit features to c++
void tuple_example() {
    //B.S: when code doesn’t need to be generic, a simple struct with named members often leads to more maintainable code.
    cout << "tuple_example" << endl;
    tuple<City, string, int> tuple1 {City {"nyc", 4}, "soho", 9};
    cout << get<0>(tuple1) << endl;
    cout << get<string>(tuple1) << endl; // only if there is only on string val in tuple
    // set value in tuple
    get<1>(tuple1) = "midtown"; // using get to update value... someone shoot me now please!
    cout << get<1>(tuple1) << endl;
}

// one possible use case is returning a value from a func or an error
variant<string, int> variant_example() {
    variant<string, int> v1 {"nyc"};
    if (holds_alternative<string>(v1)) {
        cout << get<string>(v1) << endl;
        return "hello nyc"; //mapped to the string part of the variant
    }
    return 733; // mapped to the int part of variant
}

// far cry from the elegance of java or guava Optional
optional<string> optional_example(int i) {
    optional<string> opt = {"boston"};
    cout << *opt << endl; // not the (*): an optional is a pointer to its type
    optional<int> opt2 = {}; // this is the optional that points to nullptr

    if (i >= 0) {
        return "positive"; // will be wrapped in optional;
    }
    return {}; // optional to nullptr; trying to access undefined optional is undefined: wont throw
}

void optional_example_caller() {
    optional<string> opt = optional_example(2);
    if (opt) {
        cout << "tested " << *opt << endl; // again note the (*): optional is a ptr to its type
    } else {
        cout << "nullptr" << endl;
    }
}
// any can hold an object of any type
any any_example(int i) {
    if (i < 0) {
        return "negative"s; // the literal (s) is needed for the caller to identify the string type
    }
    return 731;
}

void any_example_caller() {
    auto a1 = any_example(-1);
    const string &s = any_cast<string>(a1); // how to check for the type of this sucker before calling?
    cout << s << endl;

    any a2 = any_example(1);
    const int &i = any_cast<int>(a2);
    cout << i << endl;
}

int main() {
    cout << *get_city1() << endl;
    cout << *get_city2() << endl;
    shared_ptr_example();
    move_example();
    move_to_vector();
    forward_example();
    span_example();
    array_example();
    bitset_example();
    pair_example();
    pair_assign_and_compare();
    tuple_example();
    cout << get<string>(variant_example()) << endl; // since int is not set, we get an error of we do get<int>: std::bad_variant_access
    optional_example_caller();
    any_example_caller();

    return 0;
}
