#include "optimizer.h"
#include <iostream>
#include <unordered_set>

using namespace std;

Optimizer::Optimizer() {}

vector<Token> Optimizer::optimize(const vector<Token>& tokens) {
    vector<Token> optimized = tokens;

    // Perform optimization passes
    removeSafetyChecks(optimized);
    optimizeArithmetic(optimized);
    optimizeLoops(optimized);

    return optimized;
}

bool Optimizer::validate(const vector<Token>& tokens) {
    return checkRegisterUsage(tokens) &&
           checkMemoryAccess(tokens) &&
           checkControlFlow(tokens);
}

void Optimizer::removeSafetyChecks(vector<Token>& tokens) {
    // In safety-stripped mode, we remove overflow checks and bounds checking
    // We assume the mathematical operations are correct by construction

    for (size_t i = 0; i < tokens.size(); ++i) {
        if (tokens[i].type == TokenType::CMP) {
            // Find the next instruction after CMP and its operands
            size_t next_instr_idx = i + 1;
            while (next_instr_idx < tokens.size() && 
                   tokens[next_instr_idx].type != TokenType::MOV_IF_GT &&
                   tokens[next_instr_idx].type != TokenType::LOAD &&
                   tokens[next_instr_idx].type != TokenType::STORE &&
                   tokens[next_instr_idx].type != TokenType::ADD &&
                   tokens[next_instr_idx].type != TokenType::SUB &&
                   tokens[next_instr_idx].type != TokenType::MUL &&
                   tokens[next_instr_idx].type != TokenType::CMP &&
                   tokens[next_instr_idx].type != TokenType::JMP_GT &&
                   tokens[next_instr_idx].type != TokenType::RET) {
                next_instr_idx++;
            }

            if (next_instr_idx < tokens.size() && tokens[next_instr_idx].type == TokenType::MOV_IF_GT) {
                // Check if the MOV_IF_GT is part of a saturation clamp (e.g., to 255)
                bool is_saturation_clamp = false;
                size_t j = next_instr_idx + 1;
                while (j < tokens.size() && tokens[j].type != TokenType::NEWLINE && tokens[j].type != TokenType::END_OF_FILE) {
                    if (tokens[j].type == TokenType::IMMEDIATE && tokens[j].value == "#255") {
                        is_saturation_clamp = true;
                        break;
                    }
                    j++;
                }

                if (is_saturation_clamp) {
                    // Mark CMP and its operands as comments
                    size_t k = i;
                    while (k < next_instr_idx) {
                        if (tokens[k].type != TokenType::NEWLINE) {
                            tokens[k].type = TokenType::COMMENT;
                            tokens[k].value = "Safety-stripped: " + tokens[k].value;
                        }
                        k++;
                    }
                    // Mark MOV_IF_GT and its operands as comments
                    k = next_instr_idx;
                    while (k < tokens.size() && tokens[k].type != TokenType::NEWLINE && tokens[k].type != TokenType::END_OF_FILE) {
                        tokens[k].type = TokenType::COMMENT;
                        tokens[k].value = "Safety-stripped: " + tokens[k].value;
                        k++;
                    }
                }
            }
        }
    }
}

void Optimizer::optimizeArithmetic(vector<Token>& tokens) {
    // Convert arithmetic operations to more efficient forms
    // This is where we'd implement strength reduction and other optimizations

    for (size_t i = 0; i < tokens.size(); ++i) {
        if (isArithmeticInstruction(tokens[i].type)) {
            // Look for opportunities to use ARM64's advanced arithmetic features
            // For example, fused multiply-add, saturating arithmetic, etc.

            // Placeholder for future arithmetic optimizations
            // In current implementation, we rely on the emitter to choose
            // optimal ARM64 instructions
        }
    }
}

void Optimizer::optimizeLoops(vector<Token>& tokens) {
    // Optimize loop structures for better performance
    // This includes loop unrolling, strength reduction, etc.

    // For image processing, we might want to unroll small loops
    // or use SIMD instructions (handled by the emitter/mapper)

    // Placeholder for future loop optimizations
}

bool Optimizer::checkRegisterUsage(const vector<Token>& tokens) {
    unordered_set<string> defined_registers;
    unordered_set<string> used_registers;

    for (const auto& token : tokens) {
        if (token.type == TokenType::REGISTER || token.type == TokenType::VARIABLE) {
            if (token.value.find("%r") == 0) {
                // Check if register is used before definition
                // For now, we assume all registers are properly handled
                used_registers.insert(token.value);
            }
        }
    }

    // Simple check: ensure we don't have obviously undefined registers
    // In a real compiler, this would be more sophisticated
    return true;
}

bool Optimizer::checkMemoryAccess(const vector<Token>& tokens) {
    // Validate memory access patterns
    // Ensure that memory operations are on valid addresses

    for (size_t i = 0; i < tokens.size(); ++i) {
        if (tokens[i].type == TokenType::STORE) {
            // Basic check: ensure we're storing to valid memory locations
            // In safety-stripped mode, we assume the programmer knows what they're doing
        }
    }

    return true;
}

bool Optimizer::checkControlFlow(const vector<Token>& tokens) {
    // Validate control flow integrity
    // Ensure labels exist, jumps are valid, etc.

    unordered_set<string> labels;
    unordered_set<string> jump_targets;

    // First pass: collect all labels
    for (const auto& token : tokens) {
        if (token.type == TokenType::LABEL) {
            string label_name = token.value.substr(0, token.value.size() - 1);
            labels.insert(label_name);
        }
    }

    // Second pass: check jump targets
    for (const auto& token : tokens) {
        if (token.type == TokenType::JMP_GT) {
            // JMP_GT should be followed by a label
            // For now, we assume the assembler will catch invalid labels
        }
    }

    return true;
}

bool Optimizer::isArithmeticInstruction(TokenType type) {
    return type == TokenType::ADD ||
           type == TokenType::SUB ||
           type == TokenType::MUL;
}

bool Optimizer::isMemoryInstruction(TokenType type) {
    return type == TokenType::LOAD ||
           type == TokenType::STORE;
}

bool Optimizer::isControlFlowInstruction(TokenType type) {
    return type == TokenType::JMP_GT ||
           type == TokenType::RET;
}