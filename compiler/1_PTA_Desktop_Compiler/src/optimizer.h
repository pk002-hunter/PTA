#ifndef OPTIMIZER_H
#define OPTIMIZER_H

#include <vector>
#include "lexer.h"

class Optimizer {
public:
    Optimizer();

    // Perform safety-stripping optimization
    // Removes runtime checks while ensuring mathematical correctness
    std::vector<Token> optimize(const std::vector<Token>& tokens);

    // Validate the token stream for logical correctness
    bool validate(const std::vector<Token>& tokens);

private:
    // Optimization passes
    void removeSafetyChecks(std::vector<Token>& tokens);
    void optimizeArithmetic(std::vector<Token>& tokens);
    void optimizeLoops(std::vector<Token>& tokens);

    // Validation checks
    bool checkRegisterUsage(const std::vector<Token>& tokens);
    bool checkMemoryAccess(const std::vector<Token>& tokens);
    bool checkControlFlow(const std::vector<Token>& tokens);

    // Helper functions
    bool isArithmeticInstruction(TokenType type);
    bool isMemoryInstruction(TokenType type);
    bool isControlFlowInstruction(TokenType type);
};

#endif // OPTIMIZER_H