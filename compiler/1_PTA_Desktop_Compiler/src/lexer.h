#ifndef LEXER_H
#define LEXER_H

#include <string>
#include <vector>
#include <regex>

// Token types for the PTA language
enum class TokenType {
    // Instructions
    LOAD,
    STORE,
    ADD,
    SUB,
    MUL,
    CMP,
    MOV_IF_GT,
    JMP_GT,
    RET,

    // Operands
    REGISTER,      // %r1, %r2, etc.
    MEMORY_REF,    // [%r3]
    IMMEDIATE,     // #50, #255
    VARIABLE,      // %width, %height, %pixel_in, %pixel_out
    LABEL,         // LOOP_START:

    // Control flow
    LOOP_START,

    // Miscellaneous
    IDENTIFIER,    // Unknown identifiers
    COMMENT,       // # comment
    NEWLINE,
    END_OF_FILE
};

// Token structure
struct Token {
    TokenType type;
    std::string value;
    int line;
    int column;

    Token(TokenType t, const std::string& v, int l, int c)
        : type(t), value(v), line(l), column(c) {}
};

class Lexer {
public:
    Lexer(const std::string& source);

    // Main lexing function
    std::vector<Token> tokenize();

private:
    std::string source_;
    size_t position_;
    int line_;
    int column_;

    // Helper functions
    char peek() const;
    char advance();
    void skipWhitespace();
    void skipComment();

    // Token recognition functions
    Token scanInstruction();
    Token scanRegister();
    Token scanImmediate();
    Token scanMemoryRef();
    Token scanVariable();
    Token scanLabel();
    Token scanIdentifier();
};

#endif // LEXER_H