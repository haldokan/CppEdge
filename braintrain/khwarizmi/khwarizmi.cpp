#include <iostream>
#include "headers/Complex3.h"
#include "headers/Vector3.h"
#include <vector>

using namespace std;

void add_complex_nums() {
    cout << "==>>add_complex_nums" << endl;
    Complex3 c1{1, 2};
    Complex3 c2 {7, 1};

    c1 += c2; // copy-cntr
    cout << c1.real() << "-" << c1.imag() << endl;
    cout << c2.real() << "-" << c2.imag() << endl;

    Complex3 c3 {}; // default-cntr
    c3 += c1; // copy-cntr
    cout << c3.real() << "-" << c3.imag() << endl;
}

void struct_copy_assignment() {
    cout << "==>>struct_copy_assignment" << endl;
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

void class_copy_constructor() {
    cout << "==>>class_copy_constructor" << endl;
    Complex3 c1 = Complex3{1, 2};
    // init using the copy-cntr: called copy-initialization
    Complex3 c3 {c1};
    // copy-assignment (uses the copy cntr)
    Complex3 c4 = c1;

    Complex3 c5; // default-initialization
    // copy assignment but the copy-cntr is not called because c5 already constructed using the default cntr
    c5 = c1;
}

void container_copy_constructor() {
    cout << "container_copy_constructor" << endl;
    Vector3 v1 {1, 3, 4}; // the initializer_list contr
    Vector3 v2 {v1}; // copy-cntr
    cout << "should be equal: " << v1[1] << " - " << v2 [1] << endl;

    // because v2 was created from v1 using the copy cntr it must have its own data
    v1[1] = 7;
    cout << "should be different: " << v1[1] << " - " << v2 [1] << endl;
}

void container_copy_assignment() {
    cout << "container_copy_assignment" << endl;
    Vector3 v1 {1, 3, 4}; // the initializer_list contr
    Vector3 v3 {6, 8, 9}; // the initializer_list contr

    Vector3 v2 = v1; // this does NOT call the copy assign operator=, it rather calls the copy-cntr since v2 is initialized with v1 value
    cout << "should be equal: " << v1[1] << " - " << v2 [1] << endl;

    // because v2 was created from v1 using the copy cntr it must have its own data
    v2[1] = 7;
    cout << "should be different: " << v1[1] << " - " << v2 [1] << endl;

    v2 = v3; // this calls the copy assign operator=
}

// required the impl of a move cntr and move assignment
Vector3 container_move_constructor() {
    cout << "==>>container_move_constructor" << endl;
    Vector3 c1 {1, 2, 3};
    Vector3 c3 = {7, 9, 4};

    // c1 resources are moved to c2 and c1 is now ready for the destructor to run when c2 goes out of this func scope:
    // destructor deletes the resources pointed to by the c2 handle
    cout << "client::before calling move" << endl;
    Vector3 c2 = move(c1); // this calls the move cntr with c1 as arg (it is called move-assignment tho)
    // c2 destructor deletes c2 resources after this func return
    cout << "should be 3: c2[2] = " << c2[2] << endl;

    // we must NOT use c1 after we moved it because the move cntr assigned its resources to nullptr (examine Vector move cntr)
    cout << "moved handle len = " << c1.get_size() << endl;
    // calling subscript on the moved container items kills the program due to 'segmentation fault 11':
    // Process finished with exit code 11
//    cout << "moved handle items[0] = " << c1[0] << endl;

    cout << "client::before assigning c4 to c3" << endl;
    Vector3 c4 = c3; // this calls the copy-cntr (right-hand side of assignment is lvalue)
    //the return destroys the c4 handle and *moves* its resources it references to the caller scope (bcz Vector3 impl the move cntr)
    cout << "client::returning" << endl;
    return c4;
}

void container_move_constructor_caller() {
    Vector3 v = container_move_constructor();
    cout << "len = " << v.get_size() << ", v[0] = " << v[0] << endl;
}

int main() {
    add_complex_nums();
    struct_copy_assignment();
    class_copy_constructor();
    container_copy_constructor();
    container_copy_assignment();
    container_move_constructor_caller();

    return 0;
}
