#pragma once

#include "Grammar.h"
#include "Token.h"
#include "ErrorHandler.h"
#include "SemanticAnalyzer.h"
#include "AST.h"
#include <string>
#include <vector>
#include <fstream>
#include <memory>

// --- 符号栈元素 ---
struct StackSymbol {
    std::string symbol;
    bool isTerminal;
    int line;
    int treeNodeId;
    std::string addr;   // 语义值：变量名/临时变量/常量
    std::string type;   // 类型信息
    int quadCount;      // 该符号归约时的四元式数量（用于代码重排）
    std::shared_ptr<ASTNode> astNode;  // AST 节点
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

    // 获取 AST 根节点
    std::shared_ptr<ASTNode> getAST() const { return astRoot_; }

private:
    const Grammar& grammar_;
    ErrorHandler& errorHandler_;
    SemanticAnalyzer& sem_;

    std::vector<StackSymbol> stack_;
    std::vector<TreeNode> treeNodes_;
    int nextNodeId_;

    // AST 根节点
    std::shared_ptr<ASTNode> astRoot_;

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

    // AST 构建：归约时同步构建
    void buildASTForProduction(const Production& prod, int start);
    std::shared_ptr<ASTNode> tokenToASTLeaf(const Token& token) const;
};
