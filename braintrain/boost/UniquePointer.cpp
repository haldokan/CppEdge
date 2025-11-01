#include <iostream>
#include <memory> // For std::unique_ptr
#include <vector>
#include <string>

class Resource {
public:
    std::string name;
    explicit Resource(std::string n) : name(std::move(n)) {
        std::cout << "Resource " << name << " created." << std::endl;
    }
    ~Resource() {
        std::cout << "Resource " << name << " destroyed." << std::endl;
    }
    void doSomething() const {
        std::cout << "Resource " << name << " is doing something." << std::endl;
    }
};

// Factory function returning a unique_ptr
std::unique_ptr<Resource> createResource(const std::string& name) {
    return std::make_unique<Resource>(name); // Recommended way to create unique_ptr
}

void processResource(std::unique_ptr<Resource> res) { // Takes ownership via move
    std::cout << "  Inside processResource for " << res->name << std::endl;
    res->doSomething();
    // When 'res' goes out of scope here, the Resource object will be destroyed
} // Resource "MovedInProcess" destroyed here

int main() {
    std::cout << "--- Unique_ptr Example ---" << std::endl;

    // 1. Basic unique_ptr creation
    std::unique_ptr<Resource> uPtr1 = std::make_unique<Resource>("MainResource");
    uPtr1->doSomething();

    // 2. Cannot copy unique_ptr (compile-time error)
    // std::unique_ptr<Resource> uPtr2 = uPtr1; // ERROR: call to deleted constructor

    // 3. Can move unique_ptr (transfers ownership)
    std::cout << "\nMoving uPtr1 to uPtr2..." << std::endl;
    std::unique_ptr<Resource> uPtr2 = std::move(uPtr1); // Ownership transferred
    if (uPtr1) { // uPtr1 is now empty (nullptr)
        std::cout << "uPtr1 still points to something (shouldn't)." << std::endl;
    } else {
        std::cout << "uPtr1 is now empty." << std::endl;
    }
    uPtr2->doSomething(); // uPtr2 now owns "MainResource"

    // 4. Using unique_ptr from a factory function
    std::cout << "\nCreating resource via factory..." << std::endl;
    std::unique_ptr<Resource> factoryRes = createResource("FactoryResource");
    factoryRes->doSomething();

    // 5. Passing unique_ptr by value (transfers ownership)
    std::cout << "\nPassing unique_ptr by value to function..." << std::endl;
    processResource(std::move(factoryRes)); // Ownership moves into processResource
    if (!factoryRes) {
        std::cout << "factoryRes is now empty after moving to function." << std::endl;
    }

    // 6. unique_ptr for managing arrays
    std::cout << "\nUsing unique_ptr to manage an array..." << std::endl;
    std::unique_ptr<int[]> intArray = std::make_unique<int[]>(5);
    intArray[0] = 10;
    std::cout << "intArray[0]: " << intArray[0] << std::endl;
    // No explicit delete[] needed

    std::cout << "\nEnd of main function." << std::endl;
    // "MainResource" (owned by uPtr2) and intArray will be destroyed here.
    return 0;
} // Resource "MainResource" destroyed here
  // intArray elements destroyed here