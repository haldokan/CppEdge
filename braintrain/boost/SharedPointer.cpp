#include <iostream>
#include <memory> // For std::shared_ptr
#include <vector>
#include <string>

class Document {
public:
    std::string title;

    explicit Document(std::string t) : title(std::move(t)) {
        std::cout << "Document '" << title << "' created." << std::endl;
    }
    ~Document() {
        std::cout << "Document '" << title << "' destroyed." << std::endl;
    }

    void printTitle() const {
        std::cout << "  Document Title: " << title << std::endl;
    }
};

void viewDocument(std::shared_ptr<Document> doc) { // Shares ownership (copies)
    std::cout << "  Inside viewDocument for '" << doc->title << "'." << std::endl;
    doc->printTitle();
    std::cout << "  Reference count in viewDocument: " << doc.use_count() << std::endl;
} // doc goes out of scope, reference count decremented

int main() {
    std::cout << "--- Shared_ptr Example ---" << std::endl;

    // 1. Basic shared_ptr creation
    // Use std::make_shared for efficiency (single allocation for object and control block)
    std::shared_ptr<Document> sPtr1 = std::make_shared<Document>("Report_Q4");
    sPtr1->printTitle();
    std::cout << "sPtr1 ref count: " << sPtr1.use_count() << std::endl; // 1

    // 2. Copying shared_ptr (increases ref count)
    std::cout << "\nCopying sPtr1 to sPtr2..." << std::endl;
    std::shared_ptr<Document> sPtr2 = sPtr1; // sPtr2 now also points to "Report_Q4"
    sPtr2->printTitle();
    std::cout << "sPtr1 ref count: " << sPtr1.use_count() << std::endl; // 2
    std::cout << "sPtr2 ref count: " << sPtr2.use_count() << std::endl; // 2

    // 3. Passing shared_ptr by value (increases ref count)
    std::cout << "\nPassing shared_ptr by value to function..." << std::endl;
    viewDocument(sPtr1); // sPtr1 is copied, ref count temporarily becomes 3, then drops back to 2
    std::cout << "sPtr1 ref count after function call: " << sPtr1.use_count() << std::endl; // 2

    // 4. Resetting a shared_ptr (decrements ref count)
    std::cout << "\nResetting sPtr2..." << std::endl;
    sPtr2.reset(); // sPtr2 now empty
    std::cout << "sPtr1 ref count: " << sPtr1.use_count() << std::endl; // 1
    if (!sPtr2) {
        std::cout << "sPtr2 is now empty." << std::endl;
    }

    // 5. Creating a shared_ptr from a unique_ptr
    std::cout << "\nConverting unique_ptr to shared_ptr..." << std::endl;
    std::unique_ptr<Document> tempUnique = std::make_unique<Document>("TempDoc");
    std::shared_ptr<Document> sPtr3 = std::move(tempUnique); // Transfers ownership
    sPtr3->printTitle();
    std::cout << "sPtr3 ref count: " << sPtr3.use_count() << std::endl; // 1

    std::cout << "\nEnd of main function." << std::endl;
    // When sPtr1 and sPtr3 go out of scope, their ref counts drop to 0,
    // and the "Report_Q4" and "TempDoc" objects will be destroyed.
    return 0;
} // Document "Report_Q4" destroyed here
  // Document "TempDoc" destroyed here