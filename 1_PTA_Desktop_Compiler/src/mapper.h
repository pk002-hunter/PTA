#ifndef MAPPER_H
#define MAPPER_H

#include <string>
#include <vector>
#include <unordered_map>
#include "lexer.h"

// ARM64 register mapping configuration
struct ARM64Config {
    bool use_simd;  // Whether to use SIMD/NEON instructions
    int register_count; // Number of available registers
};

class Mapper {
public:
    Mapper(const ARM64Config& config);

    // Map PTA virtual registers to ARM64 physical registers
    std::string mapRegister(const std::string& ptaRegister);

    // Map PTA instruction to ARM64 instruction
    std::string mapInstruction(TokenType instruction,
                              const std::vector<std::string>& operands);

    // Get ARM64 register type (w for 32-bit, x for 64-bit)
    char getRegisterType(const std::string& ptaRegister);

    // Check if a register is a pointer type
    bool isPointerRegister(const std::string& ptaRegister);

    // Get next available ARM64 register
    std::string getNextRegister();

    // Reset register allocation
    void resetRegisterAllocation();

private:
    ARM64Config config_;

    // Virtual to physical register mapping
    std::unordered_map<std::string, std::string> register_map_;

    // Current register allocation counter
    int current_register_index_;

    // Special variable mappings
    std::unordered_map<std::string, std::string> variable_map_;

    // Initialize default mappings
    void initializeMappings();
};

#endif // MAPPER_H