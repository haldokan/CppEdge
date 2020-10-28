#include <iostream>
#include <string>
#include <vector>
#include <list>
#include <set>
#include <map>
#include <iterator>
#include <forward_list>
#include <fstream>

using namespace std;

struct Planet {
    string name;
    int diameter = 0;

    bool operator==(const Planet &other) const {
        return name == other.name;
    }
};

struct greater_than {
    int val;
    explicit greater_than(int val) : val(val) {}
    bool operator()(const pair<string, int> &pair1) const { return pair1.second > val; }
};

// per B.S we can make container algos nicer by abstracting ranges
namespace estd { // extended std
    using namespace std;

    template <typename T>
    void sort(T &t) {
        sort(t.begin(), t.end());
    }

    template <typename T, typename P>
    auto find_if(T &t, P pred) {
        return find_if(t.begin(), t.end(), pred); // not that we pass the predicate by value because it is an object func to be called on - won't compile if we pass it by ref
    }
}
// B.S defines this operator outside the struct conforming to his advise of defining operations not essential to a data
// structure outside the data-structure so as no to increase its size and to avoid forcing relinking code that uses the data
// structure because of these changes.
bool operator<(const Planet &first, const Planet &second) {
    return first.diameter < second.diameter;
}

ostream& operator<<(ostream& os, const Planet& e) {
    return os << "{\"" << e.name << "\", " << e.diameter << "}";
}

void sort_vector(vector<Planet> &planets) {
    cout << "sort_vector" << endl;
    sort(planets.begin(), planets.end()); // sort is done on diameter
    for (Planet &planet : planets) {
        cout << planet << endl;
    }
}

void find_item_in_vector(vector<Planet> &planets) {
    cout << "find_item_in_vector" << endl;
    auto itr = find(planets.begin(), planets.end(), Planet {"Mars"}); // seems diameter is default if we don't pass it
    if (itr != planets.end()) {
        cout << itr->name << " - " << itr->diameter << endl;
    }
}

// note that marking input as const will prevent adding the char (ptr) to the vector which enables clients to modify the string
vector<string::iterator> find_all_chars_in_string(string &input, const char chr) {
    vector<string::iterator> res;
    for (auto p = input.begin(); p != input.end(); p++) {
        if (*p == chr) {
            res.push_back(p);
        }
    }
    return res;
}

void find_all_char_in_string_caller() {
    cout << "find_all_char_in_string_caller" << endl;
    string scorpion {"scorpion"};
    vector<string::iterator> res = find_all_chars_in_string(scorpion, 'o');
    for (auto valPtr : res) {
        cout << *valPtr << "-";
        // since we have the pointer to string char we can modify it
        *valPtr = 'O';
    }
    cout << endl;
    // print the modified string
    cout << scorpion << endl;
}

list<Planet> copy_vector_to_other_containers(vector<Planet> &planets) {
    list<Planet> planet_list1;
    //The call back_inserter(planet_list1) constructs an iterator for res that adds elements at the end of a container, extending
    // the container to make room for them. This saves us from first having to allocate a fixed amount of space and then filling it.
    unique_copy(planets.begin(), planets.end(), back_inserter(planet_list1)); // don't copy *adjacent* equal elements: name of this func is misleading

    // plain copy works with back_inserter as well
    list<Planet> planet_list2;
    copy(planets.begin(), planets.end(), back_inserter(planet_list2));
    for (auto &planet : planet_list2) {
        cout << planet;
    }
    cout << endl;

    vector<Planet> planet_vec; // back_inserter works for vector (I think it works for all compatible containers)
    copy(planets.begin(), planets.end(), back_inserter(planet_vec));
    for (auto &planet : planet_vec) {
        cout << planet;
    }
    cout << endl;

    return planet_list1; // list has a move cntr so we can return by value
}

template <typename T, typename V>
vector<typename T::iterator> find_all_items_in_container_version1(T &input, const V v) {
    vector<typename T::iterator> res;
    for (auto p = input.begin(); p != input.end(); p++) {
        if (*p == v) {
            res.push_back(p);
        }
    }
    return res; // we can return vector by value bcz is impl a move cntr
}

// introduce type alias to avoid using typename in vector declaration
template <typename N>
using Iterator = typename N::iterator;
template <typename T, typename V>
vector<Iterator<T>> find_all_items_in_container_version2(T &input, const V v) {
    vector<Iterator<T>> res;
    for (auto p = input.begin(); p != input.end(); p++) {
        if (*p == v) {
            res.push_back(p);
        }
    }
    return res; // we can return vector by value bcz it impl a move cntr & assignment
}

void find_all_items_in_container_caller() {
    cout << "find_all_items_in_container_caller" << endl;
    string scorpion = {"scorpion"};

    vector<string::iterator> res = find_all_items_in_container_version1(scorpion, 'o');
    for (auto valPtr : res) {
        cout << *valPtr << "-";
    }
    cout << endl;

    // we can do that with any container type
    vector<string> scorpion2 = {"sc", "or", "pi", "n", "or", "bee"};
    // I can se auto instead of this long type but it is good to know how the stuff works
    vector<vector<string>::iterator> res2 = find_all_items_in_container_version1(scorpion2, "or");
    cout << *res2[0] << endl; // seems iterators can be accessed by subscript
    cout << *res2[1] << endl;

    // singly linked list (unlike 'list' which is a doubly linked)
    forward_list<string> scorpion3 = {"sc", "or", "pi", "n", "or", "pi"};
    vector<forward_list<string>::iterator> res3 = find_all_items_in_container_version2(scorpion3, "pi"); // call version2
    cout << *res3[0] << endl; // seems iterators can be accessed by subscript
    cout << *res3[1] << endl;
}

void copy_vector_to_other_containers_caller() {
    cout << "copy_vector_to_other_containers_caller" << endl;
    vector<Planet> planets = {
            {"Jupiter", 70},
            {"Mars",    15},
            {"Mars",    17},
            {"Earth",   20},
            {"Jupiter",   30}
    };
    auto planet_list = copy_vector_to_other_containers(planets);
    for (auto &planet : planet_list) { // note that Mars is listed only once (adjacent) but Jupiter listed twice
        cout << planet << endl;
    }
}

// example of how we can use unique_ptr to transfer ownership for container elements
vector<unique_ptr<Planet>> get_planets() {
    vector<unique_ptr<Planet>> planets;
    planets.push_back(make_unique<Planet>(Planet {"Mars", 17}));
    planets.push_back(make_unique<Planet>(Planet{"Earth", 7}));
    return planets; //contents are moved (not copied) even tho we are returning the vector by value
}

void get_planets_caller() {
    cout << "get_planets_caller" << endl;
    vector<unique_ptr<Planet>> planets = get_planets();
    for (auto &planet : planets) {
        cout << *planet << endl;
    }
}

void stream_iterators() {
    cout << "stream_iterators" << endl;
    ostream_iterator<Planet> ostream {cout}; // similar to OutputStream out = new FileOutputStream("file")
    *ostream = Planet {"Mercury", 7}; // works also by assigning to 'ostream' (not sure of the difference)
    ostream++;
    *ostream = Planet {"Earth", 2};
    cout << endl;
}

int read_from_write_to_file() {
    cout << "read_from_write_to_file" << endl;
    // we can read the soruce and target files form cin but that would halt for user input:
    // cin >> source >> target;
    // instead we specify them in in the code
    string source {"/Users/haldokanji/misc/cpp-test-file.txt"}; // does not understand '~' to refer to home directory

    ifstream in_stream {source};
    if (!in_stream) { // if file is not found c++ fails silently (sweet!) so we have to do the check explicitly
        cout << "could not open file: " << source;
        return EXIT_FAILURE; // we have also EXIT_SUCCESS
    }
    // I could have inlined these 2 iterators in the 'set' initialization below
    istream_iterator<string> in_stream_itr { in_stream };
    istream_iterator<string> in_stream_itr_sentinel {}; // this is c++ clunky way of defining an iterator (ptr) to mark end of input

    // set is ordered and unique (we have also unordered_set)
    set<string> file_data_buffer {in_stream_itr, in_stream_itr_sentinel};
    for (auto &itr : file_data_buffer) {
        cout << itr << " - ";
    }
    cout << endl;

    string target {"/Users/haldokanji/misc/cpp-test-file.out"};
    ofstream out_stream {target};
    ostream_iterator<string> out_stream_itr {out_stream, "\n"}; // the 2nd param is the eol char
    copy(file_data_buffer.begin(), file_data_buffer.end(), out_stream_itr);

    //ifstream.eof() returns 1 (true) if eof is reached (same for ofstream)
    return !in_stream.eof() || !out_stream; // out_stream.eof() returns 0 which means the eof was not reached. Wonder why B.S. used !out_stream here
}

// map is equivalent to Java TreeMap. We also have unordered_map which is more like HashMap.
void predicates_with_lambda(map<string, int> planets) {
    int x = 16;
    // we can use auto instead of 'pair' in lambda
    // find_if returns iterator to the first element satisfying the condition or last if no such element is found: map<string,int>::iterator
    // it is encouraged to use auto for iterators (at least the clang-tidy points that out and Mr. B.S does that consistently)
    auto found = find_if(planets.begin(), planets.end(), [&x] (const pair<const string, int> &entry) { return entry.second > x; });
    if (found != planets.end()) {
        cout << found->first << " - " << found->second << endl;
    } else {
        cout << "not found" << endl;
    }
}

void predicates_with_obj_func(map<string, int> planets) {
    int x = 16;
    // we can use auto instead of 'pair' in lambda
    // find_if returns iterator to the first element satisfying the condition or last if no such element is found: map<string,int>::iterator
    // it is encouraged to use auto for iterators (at least the clang-tidy points that out and Mr. B.S does that consistently)
    auto found = find_if(planets.begin(), planets.end(), greater_than {x});
    if (found != planets.end()) {
        cout << found->first << " - " << found->second << endl;
    } else {
        cout << "not found" << endl;
    }
}

void predicates_with_obj_func_wrapped(map<string, int> planets) {
    int x = 16;
    // we can use auto instead of 'pair' in lambda
    // find_if returns iterator to the first element satisfying the condition or last if no such element is found: map<string,int>::iterator
    // it is encouraged to use auto for iterators (at least the clang-tidy points that out and Mr. B.S does that consistently)
    estd::sort(planets);
    // we cann the estd::find_if
    auto found = estd::find_if(planets, greater_than {x});
    if (found != planets.end()) {
        cout << found->first << " - " << found->second << endl;
    } else {
        cout << "not found" << endl;
    }
}

void predicates_caller() {
    cout << "predicates_caller" << endl;
    predicates_with_lambda(map<string, int> {{"Jupiter", 70}, {"Mars",    15}, {"Earth",   17}});
    predicates_with_obj_func(map<string, int> {{"Jupiter", 70}, {"Mars",    15}, {"Earth",   17}});
    // todo: does not compile - fix
    predicates_with_obj_func_wrapped(map<string, int> {{"Jupiter", 70}, {"Mars",    15}, {"Earth",   17}});
}

int main() {
    vector<Planet> planets = {
            {"Jupiter", 70},
            {"Mars",    15},
            {"Earth",   17}
    };

    sort_vector(planets);
    copy_vector_to_other_containers_caller();
    find_item_in_vector(planets);
    find_all_char_in_string_caller();
    find_all_items_in_container_caller();
    get_planets_caller();
    stream_iterators();
    cout << read_from_write_to_file() << endl;
    predicates_caller();

    return 0;
}
