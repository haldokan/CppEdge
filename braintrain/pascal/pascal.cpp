#include <iostream>
#include <fstream>
#include <utility>
#include <vector>
#include "headers/EnglishDictionary.h"

using namespace std;

struct School {
    string name;
    int capacity = 0;

    // instead of passing name by ref (const &name), Clang-Tidy suggested passing by value and 'move'
    School(string name, int capacity) : name{move(name)}, capacity{capacity} {}

    bool operator==(const School &other) const {
        return name == other.name;
    }
};

bool operator<(const School &first, const School &second) {
    return first.capacity < second.capacity;
}

ostream &operator<<(ostream &os, const School &b) {
    return os << "{\"" << b.name << "\", " << b.capacity << "}";
}

// Dictionary is pass polymorphically to this func
void static_class_create_callee(Dictionary& dict, string word) {
    string w = std::move(word); // the move is suggested by Clang-Tidy
    cout << w << " :" << dict.lookup(w) << endl;
}

void static_class_create() {
    cout << "static_class_create" << endl;
    // unlike Java we cannot use Dictionary as the type of the variable
    EnglishDictionary dict_french = EnglishDictionary::create(Translated::French);
    cout << "health: " << dict_french.lookup("health") << endl;

    EnglishDictionary dict_german = EnglishDictionary::create(Translated::German);
    cout << "hope: " << dict_german.lookup("hope") << endl;

    static_class_create_callee(dict_french, "money");
    static_class_create_callee(dict_german, "health");
}

vector<int> instream_example() {
    cout << "instream_example" << endl;
    return vector<int>(2);
}

int main() {
    instream_example();
    static_class_create();

    return 0;
}