#include "lexer.h"
#include <iostream>
#include <cctype>
#include <unordered_map>

using namespace std;

// Instruction mapping
static const unordered_map<string, TokenType> INSTRUCTION_MAP = {
    {"LOAD", TokenType::LOAD},
    {"STORE", TokenType::STORE},
    {"ADD", TokenType::ADD},
    {"SUB", TokenType::SUB},
    {"MUL", TokenType::MUL},
    {"CMP", TokenType::CMP},
    {"MOV_IF_GT", TokenType::MOV_IF_GT},
    {"JMP_GT", TokenType::JMP_GT},
    {"RET", TokenType::RET}
};

Lexer::Lexer(const string& source)
    : source_(source), position_(0), line_(1), column_(1) {}

vector<Token> Lexer::tokenize() {
    vector<Token> tokens;

    while (position_ < source_.size()) {
        skipWhitespace();
        if (position_ >= source_.size()) break;

        char current = peek();

        // Handle comments and immediates (both use '#')
        if (current == '#') {
            if (position_ + 1 < source_.size() && isdigit(source_[position_ + 1])) {
                tokens.push_back(scanImmediate());
            } else {
                skipComment();
            }
            continue;
        }

        // Handle labels (end with colon)
        bool is_label = false;
        size_t temp_pos = position_;
        while (temp_pos < source_.size() && (isalnum(source_[temp_pos]) || source_[temp_pos] == '_')) {
            temp_pos++;
        }
        if (temp_pos < source_.size() && source_[temp_pos] == ':') {
            is_label = true;
        }

        if (is_label) {
            tokens.push_back(scanLabel());
            continue;
        }

        // Handle instructions and identifiers
        if (isalpha(current) || current == '_') {
            Token token = scanInstruction();
            if (token.type != TokenType::END_OF_FILE) {
                tokens.push_back(token);
            }
            continue;
        }

        // Handle registers (%r1, %r2, etc.)
        if (current == '%' && position_ + 1 < source_.size() &&
            source_[position_ + 1] == 'r') {
            tokens.push_back(scanRegister());
            continue;
        }

        // Handle variables (%width, %height, etc.)
        if (current == '%' && position_ + 1 < source_.size() &&
            isalpha(source_[position_ + 1])) {
            tokens.push_back(scanVariable());
            continue;
        }

        // Handle memory references [%r3]
        if (current == '[') {
            tokens.push_back(scanMemoryRef());
            continue;
        }

        // Handle newlines
        if (current == '\n') {
            tokens.push_back(Token(TokenType::NEWLINE, "\n", line_, column_));
            advance();
            line_++;
            column_ = 1;
            continue;
        }

        // Skip unknown characters (for robustness)
        advance();
    }

    tokens.push_back(Token(TokenType::END_OF_FILE, "", line_, column_));
    return tokens;
}

char Lexer::peek() const {
    return source_[position_];
}

char Lexer::advance() {
    char current = source_[position_++];
    column_++;
    return current;
}

void Lexer::skipWhitespace() {
    while (position_ < source_.size() && isspace(peek()) && peek() != '\n') {
        advance();
    }
}

void Lexer::skipComment() {
    while (position_ < source_.size() && peek() != '\n') {
        advance();
    }
}

Token Lexer::scanInstruction() {
    size_t start = position_;
    while (position_ < source_.size() && (isalnum(peek()) || peek() == '_')) {
        advance();
    }

    string identifier = source_.substr(start, position_ - start);

    auto it = INSTRUCTION_MAP.find(identifier);
    if (it != INSTRUCTION_MAP.end()) {
        return Token(it->second, identifier, line_, column_ - identifier.size());
    }

    // If not an instruction, treat as regular identifier
    return Token(TokenType::IDENTIFIER, identifier, line_, column_ - identifier.size());
}

Token Lexer::scanRegister() {
    advance(); // Skip '%'
    if (peek() == 'r') advance(); // Skip 'r'
    
    size_t start = position_;

    while (position_ < source_.size() && isdigit(peek())) {
        advance();
    }

    string reg = "%r" + source_.substr(start, position_ - start);
    return Token(TokenType::REGISTER, reg, line_, column_ - reg.size());
}

Token Lexer::scanVariable() {
    advance(); // Skip '%'
    size_t start = position_;

    while (position_ < source_.size() && (isalnum(peek()) || peek() == '_')) {
        advance();
    }

    string var = "%" + source_.substr(start, position_ - start);
    return Token(TokenType::VARIABLE, var, line_, column_ - var.size());
}

Token Lexer::scanImmediate() {
    advance(); // Skip '#'
    size_t start = position_;

    while (position_ < source_.size() && isdigit(peek())) {
        advance();
    }

    string imm = "#" + source_.substr(start, position_ - start);
    return Token(TokenType::IMMEDIATE, imm, line_, column_ - imm.size());
}

Token Lexer::scanMemoryRef() {
    advance(); // Skip '['
    skipWhitespace();

    // Expect a register after '['
    if (peek() != '%') {
        // Error case, skip to closing bracket
        while (position_ < source_.size() && peek() != ']') {
            advance();
        }
        if (position_ < source_.size()) advance(); // Skip ']'
        return Token(TokenType::MEMORY_REF, "[invalid]", line_, column_);
    }

    Token regToken = scanRegister();
    skipWhitespace();

    if (position_ < source_.size() && peek() == ']') {
        advance(); // Skip ']'
        return Token(TokenType::MEMORY_REF, "[" + regToken.value + "]", line_, column_ - regToken.value.size() - 2);
    }

    return Token(TokenType::MEMORY_REF, "[invalid]", line_, column_);
}

Token Lexer::scanLabel() {
    size_t start = position_;
    while (position_ < source_.size() && peek() != ':') {
        advance();
    }

    string label = source_.substr(start, position_ - start);
    advance(); // Skip ':'

    return Token(TokenType::LABEL, label + ":", line_, column_ - label.size() - 1);
}