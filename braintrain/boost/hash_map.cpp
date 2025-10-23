#include "hash_map.h"
#include <utility> // For std::move and std::make_pair
#include <algorithm> // For std::find, though not used in map operations here
#include <iostream>         // For printing in example methods

// Implementation for TaskInfo
std::ostream& operator<<(std::ostream& os, const Carriage& carriage) {
    os << "Id: " << carriage.id << ", Capacity: " << carriage.capacity << ", Type" << carriage.type;
    return os; // Important: return the ostream reference to allow chaining
}

// Constructor implementation
Train::Train(const std::string& train_id) // Changed to const std::string&
    : d_trainId(train_id) {
    std::cout << "Train '" << d_trainId << "' created." << std::endl;
}

// 1. Insert/Update a carriage (pre-C++17 approach)
bool Train::addOrUpdateCarriage(const Carriage& carriage) {
    // Attempt to insert the carriage.
    // insert() returns a std::pair<iterator, bool>.
    // The bool is true if insertion took place (new element), false if key already existed.
    auto result = d_carriages.insert(std::make_pair(carriage.id, carriage));

    bool inserted = result.second; // 'second' is the boolean indicating if it was inserted

    if (inserted) {
        std::cout << "  Carriage '" << carriage.id << "' added to train '" << d_trainId << "'." << std::endl;
    } else {
        // Key already existed, so we need to update the existing value.
        // result.first is an iterator to the existing element.
        result.first->second = carriage; // Update the Carriage object
        std::cout << "  Carriage '" << carriage.id << "' updated in train '" << d_trainId << "'." << std::endl;
    }
    return inserted;
}

// 2. Lookup a carriage by ID (non-const)
Carriage* Train::getCarriage(const std::string& carriage_id) { // Changed to const std::string&
    auto it = d_carriages.find(carriage_id);
    if (it != d_carriages.end()) {
        return &(it->second); // Return pointer to the Carriage object
    }
    return nullptr; // Not found
}

// 2. Lookup a carriage by ID (const)
const Carriage* Train::getCarriage(const std::string& carriage_id) const { // Changed to const std::string&
    auto it = d_carriages.find(carriage_id);
    if (it != d_carriages.end()) {
        return &(it->second); // Return const pointer to the Carriage object
    }
    return nullptr; // Not found
}

// 3. Remove a carriage
bool Train::removeCarriage(const std::string& carriage_id) { // Changed to const std::string&
    // erase returns number of elements removed (0 or 1 for unique keys)
    size_t removed_count = d_carriages.erase(carriage_id);
    if (removed_count > 0) {
        std::cout << "  Carriage '" << carriage_id << "' removed from train '" << d_trainId << "'." << std::endl;
        return true;
    }
    std::cout << "  Carriage '" << carriage_id << "' not found in train '" << d_trainId << "' for removal." << std::endl;
    return false;
}

// 4. Traversal - Print all carriages
void Train::printAllCarriages() const {
    if (d_carriages.empty()) {
        std::cout << "Train '" << d_trainId << "' has no carriages." << std::endl;
        return;
    }
    std::cout << "Carriages for Train '" << d_trainId << "':" << std::endl;
    // Iterate through the unordered_map using a range-based for loop (C++11+)
    // This is valid in C++11 and C++14.
    for (const auto& pair : d_carriages) {
        // 'pair' is a std::pair<const std::string, Carriage>
        // pair.second.print(); // Use the Carriage's print method
        std::cout << pair.second << std::endl;
    }
}

// Get number of carriages
size_t Train::getNumCarriages() const {
    return d_carriages.size();
}


// --- Main function for demonstration (optional, typically in a test file or separate main.cpp) ---
// Note: main function logic remains the same as it doesn't use C++17 specific features
// in its calls to Train methods.
int main() {
    Train expressTrain("T123");

    // 1. Insert Carriages
    Carriage c1("C001", 80, "Passenger");
    Carriage c2("C002", 50, "Restaurant");
    Carriage c3("C003", 120, "Cargo");
    Carriage c4("C004", 75, "Passenger");

    expressTrain.addOrUpdateCarriage(c1);
    expressTrain.addOrUpdateCarriage(c2);
    expressTrain.addOrUpdateCarriage(c3);

    std::cout << "\n--- After initial additions ---" << std::endl;
    expressTrain.printAllCarriages();
    std::cout << "Total carriages: " << expressTrain.getNumCarriages() << std::endl;

    // 2. Lookup Carriages
    std::cout << "\n--- Looking up carriages ---" << std::endl;
    const Carriage* foundCarriage = expressTrain.getCarriage("C002");
    if (foundCarriage) {
        std::cout << "Found carriage C002:" << foundCarriage << std::endl;
    } else {
        std::cout << "Carriage C002 not found." << std::endl;
    }

    Carriage* modifiableCarriage = expressTrain.getCarriage("C001");
    if (modifiableCarriage) {
        std::cout << "Modifying capacity of C001 from " << modifiableCarriage->capacity << " to 90." << std::endl;
        modifiableCarriage->capacity = 90; // Modify through the non-const pointer
    }

    const Carriage* notFoundCarriage = expressTrain.getCarriage("C999");
    if (notFoundCarriage) {
        std::cout << "Found carriage C999 (unexpected):" << notFoundCarriage << std::endl;
    } else {
        std::cout << "Carriage C999 not found (as expected)." << std::endl;
    }

    // 1. Update a Carriage
    std::cout << "\n--- Updating C003 ---" << std::endl;
    Carriage c3_updated("C003", 150, "Heavy Cargo"); // Same ID, different details
    expressTrain.addOrUpdateCarriage(c3_updated);

    std::cout << "\n--- After update and modification ---" << std::endl;
    expressTrain.printAllCarriages();

    // 3. Remove Carriages
    std::cout << "\n--- Removing C002 ---" << std::endl;
    expressTrain.removeCarriage("C002");

    std::cout << "\n--- Attempting to remove C999 ---" << std::endl;
    expressTrain.removeCarriage("C999");

    std::cout << "\n--- After removals ---" << std::endl;
    expressTrain.printAllCarriages();
    std::cout << "Total carriages: " << expressTrain.getNumCarriages() << std::endl;

    // Adding a new carriage
    std::cout << "\n--- Adding C004 ---" << std::endl;
    expressTrain.addOrUpdateCarriage(c4);
    expressTrain.printAllCarriages();

    return 0;
}
