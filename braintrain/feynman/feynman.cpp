#include <iostream>
#include <string>
#include <utility>
#include <vector>

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
    return unique_ptr<City>(new_york);
}

unique_ptr<City> get_city2() {
    cout << "get_city2" << endl;
    City boston = City{"Boston", 4};
    // unique_ptr has move semantics so the pointer to new_york is moved to the caller
    return make_unique<City>(boston); // create a unique pointer to City and 'move' it to the caller
}

int main() {
    cout << *get_city1() << endl;
    cout << *get_city2() << endl;

    return 0;
}
