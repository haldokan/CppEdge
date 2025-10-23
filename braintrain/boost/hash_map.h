#ifndef CPPEDGE_TRAIN_H
#define CPPEDGE_TRAIN_H

#include <string>           // For std::string
#include <unordered_map>    // For std::unordered_map
#include <vector>           // To hold carriage information (optional, but good for example)
#include <iostream>         // For printing in example methods

// Define a struct to represent a single train carriage/wagon
struct Carriage {
    std::string id;
    int capacity;
    std::string type; // e.g., "Passenger", "Cargo", "Restaurant"

    // Constructor for convenience (using const std::string& instead of string_view)
    Carriage(const std::string& id_val, int cap, const std::string& type_val)
        : id(id_val), capacity(cap), type(type_val) {}

    // Method to print carriage info
    void print() const {
        std::cout << "  Carriage ID: " << id
                  << ", Type: " << type
                  << ", Capacity: " << capacity << std::endl;
    }
};

class Train {
public:
    // Constructor (using const std::string& instead of string_view)
    explicit Train(const std::string& train_id);

    // Hash Map Operations:

    // 1. Insert/Update a carriage
    // Returns true if inserted, false if updated
    bool addOrUpdateCarriage(const Carriage& carriage);

    // 2. Lookup a carriage by ID (using const std::string& instead of string_view)
    // Returns a pointer to the carriage if found, nullptr otherwise.
    Carriage* getCarriage(const std::string& carriage_id);
    const Carriage* getCarriage(const std::string& carriage_id) const; // Overload for const access

    // 3. Remove a carriage (using const std::string& instead of string_view)
    // Returns true if removed, false if not found
    bool removeCarriage(const std::string& carriage_id);

    // 4. Traversal - Print all carriages
    void printAllCarriages() const;

    // Get number of carriages
    size_t getNumCarriages() const;

private:
    std::string d_trainId;
    // Hash map to store carriages, keyed by their ID
    // Key: Carriage ID (std::string)
    // Value: Carriage object (struct Carriage)
    std::unordered_map<std::string, Carriage> d_carriages;
};

#endif // CPPEDGE_TRAIN_H