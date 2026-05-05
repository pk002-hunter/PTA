#include "utils.h"
#include "lexer.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>

using namespace std;

namespace utils {

string readFile(const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        throw runtime_error("Failed to open file: " + filename);
    }

    stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

bool writeFile(const string& filename, const string& content) {
    ofstream file(filename);
    if (!file.is_open()) {
        return false;
    }

    file << content;
    return true;
}

vector<string> splitString(const string& str, char delimiter) {
    vector<string> tokens;
    stringstream ss(str);
    string token;

    while (getline(ss, token, delimiter)) {
        if (!token.empty()) {
            tokens.push_back(token);
        }
    }

    return tokens;
}

string trimString(const string& str) {
    size_t start = str.find_first_not_of(" \t\n\r\f\v");
    if (start == string::npos) {
        return "";
    }

    size_t end = str.find_last_not_of(" \t\n\r\f\v");
    return str.substr(start, end - start + 1);
}

void printTokens(const vector<Token>& tokens) {
    for (const auto& token : tokens) {
        cout << "Line " << token.line << ":" << token.column << " ";

        switch (token.type) {
            case TokenType::LOAD: cout << "LOAD"; break;
            case TokenType::STORE: cout << "STORE"; break;
            case TokenType::ADD: cout << "ADD"; break;
            case TokenType::SUB: cout << "SUB"; break;
            case TokenType::MUL: cout << "MUL"; break;
            case TokenType::CMP: cout << "CMP"; break;
            case TokenType::MOV_IF_GT: cout << "MOV_IF_GT"; break;
            case TokenType::JMP_GT: cout << "JMP_GT"; break;
            case TokenType::RET: cout << "RET"; break;
            case TokenType::REGISTER: cout << "REGISTER"; break;
            case TokenType::MEMORY_REF: cout << "MEMORY_REF"; break;
            case TokenType::IMMEDIATE: cout << "IMMEDIATE"; break;
            case TokenType::VARIABLE: cout << "VARIABLE"; break;
            case TokenType::LABEL: cout << "LABEL"; break;
            case TokenType::COMMENT: cout << "COMMENT"; break;
            case TokenType::NEWLINE: cout << "NEWLINE"; break;
            case TokenType::END_OF_FILE: cout << "END_OF_FILE"; break;
            default: cout << "UNKNOWN"; break;
        }

        cout << " - '" << token.value << "'" << endl;
    }
}

} // namespace utils