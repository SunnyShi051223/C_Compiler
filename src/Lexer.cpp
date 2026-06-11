#include "../include/Lexer.h"
#include <cctype>
#include <iostream>

using std::string;
using std::vector;
using std::unordered_set;

// --- Token 类型转字符串（辅助函数实现） ---
string tokenTypeToString(TokenType type) {
    switch (type) {
        case TokenType::KEYWORD:      return "KEYWORD";
        case TokenType::IDENTIFIER:   return "IDENTIFIER";
        case TokenType::NUMBER:       return "NUMBER";
        case TokenType::CHAR_LITERAL: return "CHAR_LITERAL";
        case TokenType::OPERATOR:     return "OPERATOR";
        case TokenType::DELIMITER:    return "DELIMITER";
        case TokenType::UNKNOWN:      return "UNKNOWN";
        case TokenType::END_OF_FILE:  return "END_OF_FILE";
        default:                      return "UNKNOWN";
    }
}

// --- Lexer 构造函数 ---
Lexer::Lexer(const string& source, ErrorHandler& errorHandler, SymbolTable& symbolTable)
    : source_(source), pos_(0), line_(1), column_(1),
      errorHandler_(errorHandler), symbolTable_(symbolTable) {
    initKeywords();
}

// --- 初始化关键字集合 ---
void Lexer::initKeywords() {
    keywords_ = {
        "int", "char", "double", "struct",
        "if", "else", "while", "return",
        "read", "write", "main"
    };
}

// --- 工具方法实现 ---
char Lexer::currentChar() const {
    if (pos_ >= source_.size()) return '\0';
    return source_[pos_];
}

char Lexer::peekChar(size_t offset) const {
    size_t peekPos = pos_ + offset;
    if (peekPos >= source_.size()) return '\0';
    return source_[peekPos];
}

void Lexer::advance() {
    if (pos_ < source_.size()) {
        if (source_[pos_] == '\n') {
            line_++;
            column_ = 1;
        } else {
            column_++;
        }
        pos_++;
    }
}

bool Lexer::isAtEnd() const {
    return pos_ >= source_.size();
}

bool Lexer::isDigit(char c) const {
    return c >= '0' && c <= '9';
}

bool Lexer::isAlpha(char c) const {
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_';
}

bool Lexer::isAlphaNumeric(char c) const {
    return isAlpha(c) || isDigit(c);
}

// --- 跳过空白符与注释 ---
void Lexer::skipWhitespaceAndComments() {
    while (!isAtEnd()) {
        char c = currentChar();

        // 跳过空白符
        if (c == ' ' || c == '\t' || c == '\n' || c == '\r') {
            advance();
            continue;
        }

        // 单行注释 //
        if (c == '/' && peekChar() == '/') {
            advance(); // 跳过 '/'
            advance(); // 跳过 '/'
            while (!isAtEnd() && currentChar() != '\n') {
                advance();
            }
            continue;
        }

        // 多行注释 /* ... */
        if (c == '/' && peekChar() == '*') {
            int startLine = line_;
            int startCol = column_;
            advance(); // 跳过 '/'
            advance(); // 跳过 '*'
            bool closed = false;
            while (!isAtEnd()) {
                if (currentChar() == '*' && peekChar() == '/') {
                    advance(); // 跳过 '*'
                    advance(); // 跳过 '/'
                    closed = true;
                    break;
                }
                advance();
            }
            if (!closed) {
                errorHandler_.reportError(startLine, startCol, "Unterminated multi-line comment");
            }
            continue;
        }

        // 不是空白也不是注释，退出
        break;
    }
}

// --- 读取整型常数 ---
Token Lexer::readNumber() {
    int startLine = line_;
    int startCol = column_;
    string value;

    while (!isAtEnd() && isDigit(currentChar())) {
        value += currentChar();
        advance();
    }

    // 检查标识符紧跟数字的情况（如 123abc），视为错误
    if (!isAtEnd() && isAlpha(currentChar())) {
        value += currentChar();
        advance();
        while (!isAtEnd() && isAlphaNumeric(currentChar())) {
            value += currentChar();
            advance();
        }
        errorHandler_.reportError(startLine, startCol,
            "Invalid token '" + value + "': identifier cannot start with a digit");
        return {TokenType::UNKNOWN, value, startLine, startCol};
    }

    return {TokenType::NUMBER, value, startLine, startCol};
}

// --- 读取字符常数 ---
Token Lexer::readCharLiteral() {
    int startLine = line_;
    int startCol = column_;
    advance(); // 跳过开头的单引号 '

    if (isAtEnd()) {
        errorHandler_.reportError(startLine, startCol, "Unterminated character literal");
        return {TokenType::UNKNOWN, "'", startLine, startCol};
    }

    char c = currentChar();
    string value;

    if (c == '\\') {
        // 转义字符
        advance();
        if (isAtEnd()) {
            errorHandler_.reportError(startLine, startCol, "Unterminated character literal");
            return {TokenType::UNKNOWN, "'\\", startLine, startCol};
        }
        char escapeChar = currentChar();
        switch (escapeChar) {
            case 'n':  value = "\\n"; break;
            case 't':  value = "\\t"; break;
            case '\\': value = "\\\\"; break;
            case '\'': value = "\\'"; break;
            case '0':  value = "\\0"; break;
            default:
                errorHandler_.reportError(line_, column_,
                    string("Unknown escape character '\\") + escapeChar + "'");
                value = string("\\") + escapeChar;
                break;
        }
        advance();
    } else {
        value = string(1, c);
        advance();
    }

    // 检查闭合的单引号
    if (isAtEnd() || currentChar() != '\'') {
        errorHandler_.reportError(startLine, startCol, "Unterminated character literal, expected '''");
        return {TokenType::UNKNOWN, "'" + value, startLine, startCol};
    }

    advance(); // 跳过结尾的单引号 '
    return {TokenType::CHAR_LITERAL, "'" + value + "'", startLine, startCol};
}

// --- 读取标识符或关键字 ---
Token Lexer::readIdentifierOrKeyword() {
    int startLine = line_;
    int startCol = column_;
    string value;

    while (!isAtEnd() && isAlphaNumeric(currentChar())) {
        value += currentChar();
        advance();
    }

    // 检查是否为关键字
    if (keywords_.find(value) != keywords_.end()) {
        return {TokenType::KEYWORD, value, startLine, startCol};
    }

    // 标识符，插入符号表
    symbolTable_.insert(value);
    return {TokenType::IDENTIFIER, value, startLine, startCol};
}

// --- 读取运算符 ---
Token Lexer::readOperator() {
    int startLine = line_;
    int startCol = column_;
    char c = currentChar();
    advance();

    // 处理 ==（双等号）
    if (c == '=' && !isAtEnd() && currentChar() == '=') {
        advance();
        return {TokenType::OPERATOR, "==", startLine, startCol};
    }

    return {TokenType::OPERATOR, string(1, c), startLine, startCol};
}

// --- 读取界符 ---
Token Lexer::readDelimiter() {
    int startLine = line_;
    int startCol = column_;
    char c = currentChar();
    advance();
    return {TokenType::DELIMITER, string(1, c), startLine, startCol};
}

// --- 读取下一个 Token ---
Token Lexer::nextToken() {
    skipWhitespaceAndComments();

    if (isAtEnd()) {
        return {TokenType::END_OF_FILE, "", line_, column_};
    }

    char c = currentChar();

    // 数字
    if (isDigit(c)) {
        return readNumber();
    }

    // 字符常数
    if (c == '\'') {
        return readCharLiteral();
    }

    // 标识符或关键字
    if (isAlpha(c)) {
        return readIdentifierOrKeyword();
    }

    // 运算符
    if (c == '+' || c == '-' || c == '*' || c == '/' ||
        c == '>' || c == '<' || c == '=' || c == '.') {
        return readOperator();
    }

    // 界符
    if (c == '(' || c == ')' || c == '{' || c == '}' ||
        c == ';' || c == '[' || c == ']') {
        return readDelimiter();
    }

    // 非法字符
    int errLine = line_;
    int errCol = column_;
    string badChar(1, c);
    advance();
    errorHandler_.reportError(errLine, errCol, "Illegal character '" + badChar + "'");
    return {TokenType::UNKNOWN, badChar, errLine, errCol};
}

// --- 扫描所有 Token ---
vector<Token> Lexer::tokenize() {
    vector<Token> tokens;
    while (true) {
        Token token = nextToken();
        tokens.push_back(token);
        if (token.type == TokenType::END_OF_FILE) {
            break;
        }
    }
    return tokens;
}

bool Lexer::hasErrors() const {
    return errorHandler_.hasErrors();
}

void Lexer::printErrors() const {
    errorHandler_.printErrors();
}
