//
// Created by haldokanji on 11/4/20.
//

#ifndef BRAINTRAIN_ENGLISHDICTIONARY_H
#define BRAINTRAIN_ENGLISHDICTIONARY_H

#include "Dictionary.h"
#include <string>
#include <unordered_map>

class EnglishDictionary : public Dictionary {
public:
    static EnglishDictionary create(Translated translated) { return EnglishDictionary {translated};}

    EnglishDictionary(Translated);

    ~EnglishDictionary() override;

    std::string lookup(std::string) const override;

private:
    Translated m_dialect;
    std::unordered_map<std::string, std::string> m_dict;
};


#endif //BRAINTRAIN_ENGLISHDICTIONARY_H
