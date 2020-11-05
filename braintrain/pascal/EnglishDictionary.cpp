#include "headers/EnglishDictionary.h"
#include <string>
#include <unordered_map>
#include <iostream>

EnglishDictionary::EnglishDictionary(Translated translated): m_translated {translated} {
    if (m_translated == Translated::French) {
        m_dict = std::unordered_map<std::string, std::string> {
            {"hope", "espoir"},
            {"money", "argent"},
            {"health", "sante"},
        };
    } else if (m_translated == Translated::German) {
        m_dict = std::unordered_map<std::string, std::string> {
                {"hope", "Hoffe"},
                {"money", "Geld"},
                {"health", "Gesundheit"},
        };
    }
}

std::string EnglishDictionary::lookup(std::string word) const {
    // note that we cannot do: m_dict[word] because [] is overloaded for both subscript and update while the function is marked as const
    return m_dict.at(word);
}

EnglishDictionary::~EnglishDictionary() = default; // equivalent to {}
