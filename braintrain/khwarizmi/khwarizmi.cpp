#include <iostream>
#include "headers/Complex3.h"

using namespace std;

void struct_copy_assignment() {
    cout << "struct_copy_assignment" << endl;
    struct Item {
        string product;
        int id;
    };

   Item item1 = {"cellphone", 723};
   Item item2 = item1; // this is copy assignment (item1 data is *copied* to item2) and here's the proof:

   item1.product = "table";
   cout << item1.product << " - " << item2.product << endl; // product changes only for item1

   item2.id = 111;
   cout << item1.id << " - " << item2.id << endl; // id changes only for item2
}

void add_complex_nums() {
    cout << "add_complex_nums" << endl;
    Complex3 c1{1, 2};
    Complex3 c2 {7, 1};

    c1 += c2;
    cout << c1.real() << "-" << c1.imag() << endl;
    cout << c2.real() << "-" << c2.imag() << endl;

    Complex3 c3 {};
    c3 += c1;
    cout << c3.real() << "-" << c3.imag() << endl;
}

int main() {
    struct_copy_assignment();
    add_complex_nums();

    return 0;
}
