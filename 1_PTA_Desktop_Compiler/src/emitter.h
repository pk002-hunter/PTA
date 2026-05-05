#ifndef EMITTER_H
#define EMITTER_H

#include <string>
#include <vector>
#include <fstream>
#include "lexer.h"
#include "mapper.h"

class Emitter {
public:
    Emitter(const std::string& outputFilename);

    // Generate ARM64 assembly file
    bool emitAssembly(const std::vector<Token>& tokens, Mapper& mapper);

    // Write assembly header (function prologue)
    void writeHeader();

    // Write assembly footer (function epilogue)
    void writeFooter();

    // Write instruction with proper formatting
    void writeInstruction(const std::string& instruction);

    // Write label
    void writeLabel(const std::string& label);

    // Write comment
    void writeComment(const std::string& comment);

    // Close the output file
    void close();

private:
    std::ofstream output_file_;
    std::string filename_;
    int indent_level_;

    // Helper to get current indentation
    std::string getIndent() const;
};

#endif // EMITTER_H