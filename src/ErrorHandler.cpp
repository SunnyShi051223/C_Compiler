#include "../include/ErrorHandler.h"
#include <iostream>

using std::string;
using std::vector;
using std::cerr;

void ErrorHandler::reportError(int line, int col, const string& msg) {
    errors.push_back({line, col, msg});
}

bool ErrorHandler::hasErrors() const {
    return !errors.empty();
}

void ErrorHandler::printErrors() const {
    for (const auto& err : errors) {
        cerr << "Lexical Error at Line " << err.line
                  << ", Col " << err.column << ": " << err.message << "\n";
    }
}

const vector<CompileError>& ErrorHandler::getErrors() const {
    return errors;
}
