#include <iostream>
#include <string>
#include <utility>
#include <vector>
#include <map>
#include <unordered_map>

using namespace std;

struct Entry {
    string name;
    int number;
    // we have to define the equality operator if we want to use Entry as a hash key (similar to java requiring equals & hash)
    bool operator== (const Entry& other) const {
        return name == other.name && number == other.number;
    }
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

// vector and string are not checked for ranges when accessed using subscript for performance reasons (B.S. says there is 10% cost with can varies widely)
void vector_is_range_unchecked() {
    cout << "vector_is_range_unchecked" << endl;
    vector<Entry> v = {{"Jane", 42}, {"John", 45}};
    cout << v[7] << endl; // this prints {"", 0}

    // we can use at() which checks for range
    try {
        cout << v.at(7) << endl;
    } catch(out_of_range& e) {
        cout << "out_of_range: " << e.what() << endl;
    }
    // or we can rely on iterators
    for (auto &entry : v) {
        cout << entry << endl;
    }
}

void create_memory_for_new_obj(const Entry e) { // e has to be passed by value for the memory alloc to work (unlike B.S. example where it is passed by ref)
    cout << "create_memory_for_new_obj" << endl;

    Entry *ep; // ptr to e
    new(ep) Entry {e};
    ep++; // scary pointer math: seems we need this here bcz we start with unassigned ep
    cout << "name: [" << *ep << "]" << endl;

    Entry e2 = {"Susan", 45};
    new(ep) Entry{e2}; // this now advances the ptr
    cout << "name: [" << *ep << "]" << endl;

    Entry e3 = {"Julie", 19};
    new(ep) Entry{e3}; // this now advances the ptr
    cout << "name: [" << *ep << "]" << endl;
}

// the map data-structure is ordered and implemented as balanced binary search tree (red/black) similar to TreeMap in Java
void ordered_map() {
    cout << "ordered_map" << endl;
    map<string, int> map = {{"Susan", 30}, {"Jane", 42}, {"John", 45}};
    cout << map["Jane"] << endl;
    cout << map.at("John") << endl;

    // this is insane api design! If key is not found it inserts it with a default value (in this case 0 for int). With this gem getters modify their object
    cout << map["Santa"] << endl;
    // more insanity! This throws out_of_range since Paul in not in the map
    try {
        cout << map.at("Paul") << endl;
    } catch (out_of_range& e) {
        cout << "out_of_range: " << e.what() << endl;
    }
    // B.S. suggest using find and insert if the above behavior is not desired! (in C++20 contains is added)
    // look at ths ugly api that enables checking if the map contains a key
    cout << (map.find("Susan") == map.end()) << endl; // prints 0
    cout << (map.find("Tom") == map.end()) << endl; // prints 1

    // returns itr.first/second to the key and value and the insertion status
    const auto [itr, success] = map.insert({"Sheela"s, 3});
    cout << itr->first << " - " << itr->second << " - " << success << endl;

    //insert value with the same key again: success is 0 here because the entry was not inserted (no overriding values)
    const auto [itr2, success2] = map.insert({"Sheela"s, 39});
    cout << itr2->first << " - " << itr2->second << " - " << success2 << endl;

    // have to erase the existing key in order to override it
    map.erase("Sheela");
    const auto [itr3, success3] = map.insert({"Sheela"s, 39});
    cout << itr3->first << " - " << itr3->second << " - " << success3 << endl;

    cout << "in-order traverse" << endl;
    for (auto &entry_itr : map) {
        cout << entry_itr.first << " - " << entry_itr.second << endl;
    }

    // the auto type in the for loop above is actually pair<const string, int>; not the const in the pair (the map insures that callers cannot modify the key)
    cout << "in-order2 traverse" << endl;
    for (pair<const string, int> &pair1 : map) {
        cout << pair1.first << " - " << pair1.second << endl;
    }
}

namespace std { // inject the Entry hash func in std namespace
//we define Entry hash as a specialization of the standard-library hash:
    template<> // note that struct Entry has to implement operator== for hash to work (for obvious reasons)
    struct hash<Entry> {
        using argument_type = Entry;
        using result_type = size_t;

        size_t operator()(const Entry &e) const {
            // note that hash is an object func: class that can be called as a func (note how we init {} then call with its operator() params)
            return hash<string>{} (e.name) ^ hash<int>{} (e.number);
        }
    };
}

// the pedestrian way to define Entry hash (has to be passed as an argument to the map, set, etc.)
struct Entry_Hash {
    size_t operator()(const Entry &e) const {
        // note that hash is an object func: class that can be called as a func
        return hash<string>()(e.name) ^ hash<int>()(e.number);
    }
};

//unordered map: std lib provides hash funcs for built-in types
void hash_table() {
    cout << "hash_table" << endl;
    unordered_map<string, int> map = {{"Susan", 30}, {"Jane", 42}, {"John", 45}};
    cout << map["Jane"] << endl;
    cout << map.at("John") << endl;

    // with the hash for Entry defined in struct hash<Entry> we can now add it to the unordered map
    unordered_map<Entry, string> map2; // note how the hash func does not need passing
    const Entry jane = {"Jane"s, 3};
    map2.insert({jane, "girl-Jane"s});
    cout << map2[jane] << endl;

    // we can pass a hash func as well if we choose not to define it as specialization of the std-lib hash
    // commented-out because the unordered_map<Entry, string> cannot be defined with 2 hash funcs
//    unordered_map<Entry, string, Entry_Hash> map2; // pass him
//    const Entry bob = {"Bob"s, 7};
//    map2.insert({bob, "boy-Bob"s});
//    cout << map2[bob] << endl;
}

// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

class engine {
public:
    engine(string make, int horse_power) : m_make(move(make)), m_horse_power(horse_power) {}
    void upgrade(string make, int horse_power) {
        m_make = std::move(make);
        m_horse_power = horse_power;
    }

    [[nodiscard]] const string &get_make() const {
        return m_make;
    }

    [[nodiscard]] int get_horse_power() const {
        return m_horse_power;
    }

    string specs() {
        return "engine: " + m_make + ", " + to_string(m_horse_power);
    }
private:
    string m_make;
    int m_horse_power;
};

struct series {
    string name;
};

class car {
public:
    explicit car(const shared_ptr<engine> &engine, const series &series) : m_engine(engine), m_series{series} {}
    string specs() {
        return "car: " + m_engine->get_make() + ", " + m_series.name + ", " + to_string(m_engine->get_horse_power());
    }
private:
    const shared_ptr<engine> m_engine;
    // this now a pointer that hold the value of the passed ref; changes to the passed series anywhere will reflect here
    // I think this is not a good way to pass dependencies: raw pointers that would freakout if the resource they point to is deleted more than once
    // also note that if this class goes out of scope and this pointer is not destroyed we will have a dangling ptr. We also
    // can never be sure if it should be destroyed here or elsewhere.
    // so a better option would have been to pass 'series' by value and move it to assigned it to the member
    const series &m_series;
};

void car_factory() {
    shared_ptr<engine> engine1 = make_shared<engine>(engine {"bmw", 400});
    cout << engine1->specs() << endl;
    series series1 {"macan"};
    car car1 {engine1, series1};
    cout << "before upgrade: " << car1.specs() << endl;

    series1.name = "mercedes"; // updating the series will reflect in the car because we assign the member to the ref
    engine1->upgrade("porsche", 300); // now updating the engine thru the sptr will update it in the car as well
    cout << "after upgrade: " + car1.specs() << endl;
}

int main() {
    iterate_vector();
    copy_vs_move_to_vector_caller();
    Entry e = {"Tim", 24};
    create_memory_for_new_obj(e);
    vector_is_range_unchecked();
    ordered_map();
    hash_table();
    car_factory();

    return 0;
}
