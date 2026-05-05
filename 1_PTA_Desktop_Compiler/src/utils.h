#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <vector>
#include "lexer.h"

namespace utils {

// File utility functions
std::string readFile(const std::string& filename);
bool writeFile(const std::string& filename, const std::string& content);

// String utility functions
std::vector<std::string> splitString(const std::string& str, char delimiter);
std::string trimString(const std::string& str);

// Debug utility functions
void printTokens(const std::vector<Token>& tokens);

} // namespace utils

#endif // UTILS_H