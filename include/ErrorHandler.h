#pragma once

#include <string>
#include <vector>

// --- 编译错误信息 ---
struct CompileError {
    int line;
    int column;
    std::string message;
};

// --- 错误处理器 ---
class ErrorHandler {
public:
    void reportError(int line, int col, const std::string& msg);
    bool hasErrors() const;
    void printErrors() const;
    const std::vector<CompileError>& getErrors() const;

private:
    std::vector<CompileError> errors;
};
