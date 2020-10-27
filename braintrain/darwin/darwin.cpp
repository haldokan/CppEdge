#include <iostream>
#include <string>
#include <vector>
#include <list>
#include <iterator>
#include <forward_list>

using namespace std;

struct Planet {
    string name;
    int diameter = 0;

    bool operator==(const Planet &other) const {
        return name == other.name;
    }
};

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

//  // note that marking input as const will prevent adding the char (ptr) to the vector which enables clients to modify the string
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
vector<typename T::iterator> find_all_items_in_container(T &input, const V v) {
    vector<typename T::iterator> res;
    for (auto p = input.begin(); p != input.end(); p++) {
        if (*p == v) {
            res.push_back(p);
        }
    }
    return res;
}

void find_all_items_in_container_caller() {
    cout << "find_all_items_in_container_caller" << endl;
    string scorpion = {"scorpion"};

    vector<string::iterator> res = find_all_items_in_container(scorpion, 'o');
    for (auto valPtr : res) {
        cout << *valPtr << "-";
    }
    cout << endl;

    // we can do that with any container type
    vector<string> scorpion2 = {"sc", "or", "pi", "n", "or", "bee"};
    // I can se auto instead of this long type but it is good to know how the stuff works
    vector<vector<string>::iterator> res2 = find_all_items_in_container(scorpion2, "or");
    cout << *res2[0] << endl; // seems iterators can be accessed by subscript
    cout << *res2[1] << endl;

    // singly linked list (unlike 'list' which is a doubly linked list)
    forward_list<string> scorpion3 = {"sc", "or", "pi", "n", "or", "pi"};
    vector<forward_list<string>::iterator> res3 = find_all_items_in_container(scorpion3, "pi");
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

    return 0;
}
