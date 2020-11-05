
#ifndef BRAINTRAIN_DICTIONARY_H
#define BRAINTRAIN_DICTIONARY_H

#include <string>

// pure virtual class: DO NOT put member fields here nor provide a constructor: will cause misleading compilation errors
enum class Translated {
    French, German
};

class Dictionary {
public:
    virtual std::string lookup(std::string) const = 0;
    virtual ~Dictionary() = default;
};


#endif //BRAINTRAIN_DICTIONARY_H
