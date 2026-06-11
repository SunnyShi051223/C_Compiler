#pragma once

#include "Token.h"
#include "ErrorHandler.h"
#include "SymbolTable.h"
#include <string>
#include <vector>
#include <unordered_set>

// --- 词法分析器 ---
class Lexer {
public:
    Lexer(const std::string& source, ErrorHandler& errorHandler, SymbolTable& symbolTable);

    // 读取下一个 Token
    Token nextToken();

    // 一次性扫描所有 Token（含 END_OF_FILE）
    std::vector<Token> tokenize();

    bool hasErrors() const;
    void printErrors() const;

private:
    std::string source_;
    size_t pos_;
    int line_;
    int column_;

    ErrorHandler& errorHandler_;
    SymbolTable& symbolTable_;

    std::unordered_set<std::string> keywords_;

    // 初始化关键字集合
    void initKeywords();

    // 状态机辅助方法
    void skipWhitespaceAndComments();
    Token readNumber();
    Token readCharLiteral();
    Token readIdentifierOrKeyword();
    Token readOperator();
    Token readDelimiter();

    // 工具方法
    char currentChar() const;
    char peekChar(size_t offset = 1) const;
    void advance();
    bool isAtEnd() const;
    bool isDigit(char c) const;
    bool isAlpha(char c) const;
    bool isAlphaNumeric(char c) const;
};
