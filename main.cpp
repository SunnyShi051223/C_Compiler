#include "include/Lexer.h"
#include <iostream>
#include <fstream>
#include <sstream>

using std::string;
using std::vector;
using std::cout;
using std::cerr;
using std::endl;
using std::ifstream;
using std::ofstream;
using std::stringstream;

// 从文件读取源代码
string readFile(const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Error: Cannot open file '" << filename << "'\n";
        return "";
    }
    stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

// 将 Token 序列写入文件
void writeTokens(const string& filename, const vector<Token>& tokens) {
    ofstream file(filename);
    if (!file.is_open()) {
        cerr << "Error: Cannot write to file '" << filename << "'\n";
        return;
    }
    file << "Line\tCol\tType\t\tValue\n";
    file << "--------------------------------\n";
    for (const auto& token : tokens) {
        string typeStr = tokenTypeToString(token.type);
        file << token.line << "\t"
             << token.column << "\t";
        if (typeStr.size() < 8)
            file << typeStr << "\t\t";
        else
            file << typeStr << "\t";
        file << (token.value.empty() ? "(EOF)" : token.value) << "\n";
    }
}

int main(int argc, char* argv[]) {
    // 默认读取 test.txt，也可通过命令行参数指定文件
    string filename = (argc >= 2) ? argv[1] : "test.txt";
    string source = readFile(filename);
    if (source.empty()) return 1;

    // 初始化各组件
    ErrorHandler errorHandler;
    SymbolTable symbolTable;
    Lexer lexer(source, errorHandler, symbolTable);

    // 词法分析
    vector<Token> tokens = lexer.tokenize();

    // 保存 Token 序列到文件
    writeTokens("tokens.txt", tokens);
    cout << "Tokens saved to tokens.txt" << endl;

    // 输出错误信息
    if (errorHandler.hasErrors()) {
        errorHandler.printErrors();
    }

    return 0;
}
