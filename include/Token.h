#pragma once

#include <string>

// --- Token 类型枚举 ---
enum class TokenType {
    KEYWORD,        // 关键字: int, char, struct, if, else, while, return, read, write, main
    IDENTIFIER,     // 标识符: 变量名、结构体名
    NUMBER,         // 整型常数: 42, 1024
    CHAR_LITERAL,   // 字符常数: 'A', '\n'
    OPERATOR,       // 运算符: +, -, *, /, >, <, ==, =, .
    DELIMITER,      // 界符: (, ), {, }, ;, [, ]
    UNKNOWN,        // 未知/非法字符
    END_OF_FILE     // 文件结束
};

// --- Token 结构体 ---
struct Token {
    TokenType type;
    std::string value;
    int line;
    int column;
};

// --- 辅助函数声明 ---
std::string tokenTypeToString(TokenType type);
