#pragma once

#include "Grammar.h"
#include "Token.h"
#include "ErrorHandler.h"
#include "SemanticAnalyzer.h"
#include <string>
#include <vector>
#include <fstream>

// --- 符号栈元素 ---
struct StackSymbol {
    std::string symbol;
    bool isTerminal;
    int line;
    int treeNodeId;
    std::string addr;   // 语义值：变量名/临时变量/常量
    std::string type;   // 类型信息
};

// --- 语法树节点 ---
struct TreeNode {
    int id;
    std::string symbol;
    bool isTerminal;
    std::vector<int> children;
    int line;
};

// --- 语法分析器 ---
class PrecedenceParser {
public:
    PrecedenceParser(const Grammar& grammar, ErrorHandler& errorHandler,
                     SemanticAnalyzer& semAnalyzer);

    bool parse(const std::vector<Token>& tokens);

    void dumpTrace(const std::string& filename) const;
    void dumpParseTree(const std::string& filename) const;
    void dumpAll() const;

private:
    const Grammar& grammar_;
    ErrorHandler& errorHandler_;
    SemanticAnalyzer& sem_;

    std::vector<StackSymbol> stack_;
    std::vector<TreeNode> treeNodes_;
    int nextNodeId_;

    struct TraceEntry {
        std::string stackStr;
        std::string inputStr;
        std::string action;
    };
    std::vector<TraceEntry> trace_;

    void push(const StackSymbol& sym);
    int findTopTerminal() const;
    bool reduce();
    std::string tokenToTerminal(const Token& token) const;
    std::string stackToString() const;
    int createNode(const std::string& symbol, bool isTerminal, int line);

    // 语义动作：归约时执行
    void semanticAction(const Production& prod, int start);
};
