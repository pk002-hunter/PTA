#include <iostream>
#include <string>
#include "lexer.h"
#include "optimizer.h"
#include "mapper.h"
#include "emitter.h"
#include "utils.h"

using namespace std;

int main(int argc, char* argv[]) {
    cout << "=== PTA Desktop Compiler ===" << endl;
    cout << "Safety-Stripped ARM64 Code Generation" << endl;
    cout << "Optimized for Snapdragon 8 Gen 4" << endl << endl;

    if (argc != 2) {
        cout << "Usage: pta_compiler <input.tasm>" << endl;
        cout << "Defaulting to demo_filter.tasm" << endl;
    }

    string inputFile = (argc == 2) ? argv[1] : "../scripts/demo_filter.tasm";
    string outputFile = "../2_PTA_Android_App/app/src/main/cpp/generated_logic.s";

    try {
        // Stage 1: Read and lex the input file
        cout << "[1/4] Reading and lexing " << inputFile << "..." << endl;
        string source = utils::readFile(inputFile);
        Lexer lexer(source);
        vector<Token> tokens = lexer.tokenize();

        cout << "   ✓ Tokens generated: " << tokens.size() << endl;

        // Stage 2: Optimize (safety-stripping)
        cout << "[2/4] Performing safety-stripping optimization..." << endl;
        Optimizer optimizer;
        vector<Token> optimized_tokens = optimizer.optimize(tokens);
        cout << "   ✓ Optimization complete" << endl;

        // Stage 3: Map to ARM64
        cout << "[3/4] Mapping to ARM64 registers..." << endl;
        ARM64Config config;
        config.use_simd = true;
        config.register_count = 16; // w0-w15 available

        Mapper mapper(config);
        cout << "   ✓ Register mapping configured" << endl;

        // Stage 4: Emit assembly
        cout << "[4/4] Generating ARM64 assembly..." << endl;
        Emitter emitter(outputFile);

        if (emitter.emitAssembly(optimized_tokens, mapper)) {
            emitter.close();
            cout << "   ✓ Assembly generated: " << outputFile << endl;
        } else {
            cerr << "   ✗ Assembly generation failed!" << endl;
            return 1;
        }

        cout << endl << "=== Compilation successful! ===" << endl;
        cout << "Generated safety-stripped ARM64 assembly ready for Android NDK." << endl;

    } catch (const exception& e) {
        cerr << "Error: " << e.what() << endl;
        return 1;
    }

    return 0;
}