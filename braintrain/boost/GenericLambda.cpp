#include <iostream>

int main() {
    // A generic lambda using 'auto' for parameters
    auto print_type = [](const auto& val)   {
        std::cout << "Value: " << val << ", Type: "
                  << typeid(val).name() << std::endl;

    };

    print_type(10);        // int
    print_type(3.14);      // double
    print_type("hello");   // const char*
    print_type(std::string("world")); // std::string

    return 0;
}