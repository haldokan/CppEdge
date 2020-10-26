#include <iostream>
#include <string>
#include <vector>
#include <list>

using namespace std;

struct Planet {
    string name;
    int diameter;

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

void sort_vector() {
    cout << "sort_vector" << endl;
    vector<Planet> planets = {
            {"Jupiter", 70},
            {"Mars",    15},
            {"Earth",   17}
    };
    sort(planets.begin(), planets.end()); // sort is done on diameter
    for (Planet &planet : planets) {
        cout << planet << endl;
    }
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
    sort_vector();
    copy_vector_to_other_containers_caller();

    return 0;
}
