#include "include/Lexer.h"
#include "include/Grammar.h"
#include "include/PrecedenceParser.h"
#include "include/SemanticAnalyzer.h"
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

void writeTokens(const string& filename, const vector<Token>& tokens) {
    ofstream file(filename);
    if (!file.is_open()) return;
    file << "Line\tCol\tType\t\tValue\n";
    file << "--------------------------------\n";
    for (const auto& token : tokens) {
        string typeStr = tokenTypeToString(token.type);
        file << token.line << "\t" << token.column << "\t";
        if (typeStr.size() < 8) file << typeStr << "\t\t";
        else file << typeStr << "\t";
        file << (token.value.empty() ? "(EOF)" : token.value) << "\n";
    }
}

int main(int argc, char* argv[]) {
    string filename = (argc >= 2) ? argv[1] : "test.txt";
    string source = readFile(filename);
    if (source.empty()) return 1;

    // ===== 阶段一：词法分析 =====
    ErrorHandler errorHandler;
    SymbolTable symbolTable;
    Lexer lexer(source, errorHandler, symbolTable);

    vector<Token> tokens = lexer.tokenize();
    writeTokens("output/Lexer/tokens.txt", tokens);
    cout << "[Lexer] Tokens -> output/Lexer/tokens.txt" << endl;

    if (errorHandler.hasErrors()) {
        cout << "[Lexer] Errors:" << endl;
        errorHandler.printErrors();
    }

    // ===== 阶段二：语法分析 + 语义分析 =====
    Grammar grammar;
    grammar.dumpProductions("output/Parser/productions.txt");
    grammar.dumpFirstVT("output/Parser/firstvt.txt");
    grammar.dumpLastVT("output/Parser/lastvt.txt");
    grammar.dumpPrecedenceTable("output/Parser/precedence_table.txt");
    cout << "[Parser] Grammar -> output/Parser/" << endl;

    SemanticAnalyzer semAnalyzer;
    PrecedenceParser parser(grammar, errorHandler, semAnalyzer);
    bool success = parser.parse(tokens);

    parser.dumpAll();
    cout << "[Parser] Trace/Tree -> output/Parser/" << endl;

    // ===== 阶段三：目标代码生成 =====
    semAnalyzer.dumpQuads("output/Codegen/quads.txt");
    semAnalyzer.dumpTarget("output/Codegen/Main.vm");
    cout << "[Codegen] Quads   -> output/Codegen/quads.txt" << endl;
    cout << "[Codegen] Target  -> output/Codegen/Main.vm" << endl;

    // ===== 结果 =====
    if (success) {
        cout << "\n[Result] PASSED. No syntax errors." << endl;
    } else {
        cout << "\n[Result] FAILED. Errors:" << endl;
        errorHandler.printErrors();
    }

    return 0;
}
